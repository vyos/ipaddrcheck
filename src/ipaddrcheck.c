/*
 * ipaddrcheck.c: an IPv4/IPv6 validator
 *
 * Copyright (C) 2013 Daniil Baturin
 * Copyright (C) 2018 VyOS maintainers and contributors
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 or later as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <errno.h>
#include "config.h"
#include "ipaddrcheck_functions.h"

/* Option codes */
#define IS_VALID              10
#define IS_IPV4               20
#define IS_IPV4_CIDR          30
#define IS_IPV4_SINGLE        40
#define IS_IPV4_HOST          50
#define IS_IPV4_NET           60
#define IS_IPV4_BROADCAST     70
#define IS_IPV4_UNICAST       80
#define IS_IPV4_MULTICAST     90
#define IS_IPV4_RFC1918       100
#define IS_IPV4_LOOPBACK      110
#define IS_IPV4_LINKLOCAL     120
#define IS_IPV6               130
#define IS_IPV6_CIDR          140
#define IS_IPV6_SINGLE        150
#define IS_IPV6_HOST          160
#define IS_IPV6_NET           170
#define IS_IPV6_UNICAST       180
#define IS_IPV6_MULTICAST     190
#define IS_IPV6_LINKLOCAL     200
#define IS_VALID_INTF_ADDR    220
#define IS_ANY_CIDR           230
#define IS_ANY_SINGLE         240
#define ALLOW_LOOPBACK        250
#define IS_ANY_HOST           260
#define IS_ANY_NET            270
#define NO_ACTION             500

static const struct option options[] =
{
    { "is-valid",              no_argument, NULL, 'a' },
    { "is-ipv4",               no_argument, NULL, 'c' },
    { "is-ipv4-cidr",          no_argument, NULL, 'd' },
    { "is-ipv4-single",        no_argument, NULL, 'e' },
    { "is-any-cidr",           no_argument, NULL, 'A' },
    { "is-any-single",         no_argument, NULL, 'B' },
    { "is-ipv4-host",          no_argument, NULL, 'f' },
    { "is-ipv4-net",           no_argument, NULL, 'g' },
    { "is-ipv4-broadcast",     no_argument, NULL, 'h' },
    { "is-ipv4-multicast",     no_argument, NULL, 'i' },
    { "is-ipv4-loopback",      no_argument, NULL, 'j' },
    { "is-ipv4-link-local",    no_argument, NULL, 'k' },
    { "is-ipv4-rfc1918",       no_argument, NULL, 'l' },
    { "is-ipv6",               no_argument, NULL, 'm' },
    { "is-ipv6-cidr",          no_argument, NULL, 'n' },
    { "is-ipv6-single",        no_argument, NULL, 'o' },
    { "is-ipv6-host",          no_argument, NULL, 'p' },
    { "is-ipv6-net",           no_argument, NULL, 'r' },
    { "is-ipv6-multicast",     no_argument, NULL, 's' },
    { "is-ipv6-link-local",    no_argument, NULL, 't' },
    { "is-valid-intf-address", no_argument, NULL, 'u' },
    { "allow-loopback",        no_argument, NULL, 'C' },
    { "is-any-host",           no_argument, NULL, 'D' },
    { "is-any-net",            no_argument, NULL, 'E' },
    { "version",               no_argument, NULL, 'z' },
    { "help",                  no_argument, NULL, '?' },
    { "verbose",               no_argument, NULL, 'V' },
    { NULL,                    no_argument, NULL, 0   }
};

/* Auxiliary functions */
static void print_help(const char* program_name);
static void print_version(void);

int main(int argc, char* argv[])
{
    char *address_str = "";    /* IP address string obtained from arguments */
    int action = 0;            /* Action associated with given check option */
    int* actions;              /* Array of all given actions */
    int action_count = 0;      /* Actions array size */

    int option_index = 0;      /* Number of the current option for getopt call */
    int optc;                  /* Option character for getopt call */

    int allow_loopback = NO_LOOPBACK;    /* Allow IPv4 loopback in --is-valid-intf-address */

    int no_action = 0;   /* Indicates the option modifies program behaviour
                            but doesn't have its own action */

    int verbose = 0;

    const char* program_name = argv[0]; /* Program name for use in messages */


    /* Parse options, convert to action codes, store in array */

    /* Try to allocate memory for the actions array, abort if fail */
    actions = (int*)calloc(argc, sizeof(int));
    if( errno == ENOMEM )
    {
        fprintf(stderr, "Error: could not allocate memory!\n");
        return(RESULT_INT_ERROR);
    }

    while( (optc = getopt_long(argc, argv, "acdefghijklmnoprstuzABCDEV?", options, &option_index)) != -1 )
    {
         switch(optc)
         {
             case 'a':
                 action = IS_VALID;
                 break;
             case 'b':
                 break;
             case 'c':
                 action = IS_IPV4;
                 break;
             case 'd':
                 action = IS_IPV4_CIDR;
                 break;
             case 'e':
                 action = IS_IPV4_SINGLE;
                 break;
             case 'f':
                 action = IS_IPV4_HOST;
                 break;
             case 'g':
                 action = IS_IPV4_NET;
                 break;
             case 'h':
                 action = IS_IPV4_BROADCAST;
                 break;
             case 'i':
                 action = IS_IPV4_MULTICAST;
                 break;
             case 'j':
                 action = IS_IPV4_LOOPBACK;
                 break;
             case 'k':
                 action = IS_IPV4_LINKLOCAL;
                 break;
             case 'l':
                 action = IS_IPV4_RFC1918;
                 break;
             case 'm':
                 action = IS_IPV6;
                 break;
             case 'n':
                 action = IS_IPV6_CIDR;
                 break;
             case 'o':
                 action = IS_IPV6_SINGLE;
                 break;
             case 'p':
                 action = IS_IPV6_HOST;
                 break;
             case 'r':
                 action = IS_IPV6_NET;
                 break;
             case 's':
                 action = IS_IPV6_MULTICAST;
                 break;
             case 't':
                 action = IS_IPV6_LINKLOCAL;
                 break;
             case 'u':
                 action = IS_VALID_INTF_ADDR;
                 break;
             case 'A':
                 action = IS_ANY_CIDR;
                 break;
             case 'B':
                 action = IS_ANY_SINGLE;
                 break;
             case 'C':
                 allow_loopback = LOOPBACK_ALLOWED;
                 no_action = NO_ACTION;
                 break;
             case 'D':
                 action = IS_ANY_HOST;
                 break;
             case 'E':
                 action = IS_ANY_NET;
                 break;
             case 'V':
                 verbose = 1;
                 break;
             case '?':
                 print_help(program_name);
                 return(EXIT_SUCCESS);
             case 'z':
                 print_version();
                 return(EXIT_SUCCESS);
             default:
                fprintf(stderr, "Error: invalid option\n");
                print_help(program_name);
                return(RESULT_INT_ERROR);
         }

         if( no_action != NO_ACTION )
         {
             action_count = optind-2;
             actions[action_count] = action;
         }
         else
         {
             no_action = 0; /* Reset no_action */
         }
    }

    /* Exit if no option given */
    if( optind < 2 )
    {
        fprintf(stderr, "Error: at least one option expected!\n");
        print_help(program_name);
        return(RESULT_INT_ERROR);
    }

    /* Get non-option arguments */
    if( (argc - optind) == 1 )
    {
         address_str = argv[optind];
    }
    else
    {
         fprintf(stderr, "Error: wrong number of arguments, one argument required!\n");
         print_help(program_name);
         return(RESULT_INT_ERROR);
    }

    CIDR *address;
    address = cidr_from_str(address_str);
    int result = RESULT_SUCCESS;

    /* Check if the address is valid and well-formatted at all,
       if not there is no point in going further */
    if( !( (is_valid_address(address) == RESULT_SUCCESS) &&
        ((is_any_cidr(address_str) == RESULT_SUCCESS) || (is_any_single(address_str) == RESULT_SUCCESS)) ) )
    {
        if( verbose )
        {
            printf("Malformed address %s\n", address_str);
        }
        return(EXIT_FAILURE);
    }

    /* FIXUP: libcidr allows more than one double colon, but RFC 4291 does not! */
    if( duplicate_double_colons(address_str) ) {
        if( verbose )
        {
            printf("More than one \"::\" is not allowed in IPv6 addresses\n");
        }
        return(EXIT_FAILURE);
    }

    while( (action_count >= 0) && (result == RESULT_SUCCESS) )
    {
        switch(actions[action_count])
        {
            case IS_VALID:
                result = is_valid_address(address);
                break;
            case IS_IPV4:
                result = is_ipv4(address);
                break;
            case IS_IPV4_CIDR:
                result = is_ipv4_cidr(address_str);
                break;
            case IS_IPV4_SINGLE:
                result = is_ipv4_single(address_str);
                break;
            case IS_IPV4_HOST:
                /* Host vs. network address check only makes sense
                   if prefix length is given */
                if( !(cidr_get_proto(address) == CIDR_IPV4) )
                {
                    if( verbose )
                    {
                        printf("%s is not a valid IPv4 address\n", address_str);
                    }
                    result = RESULT_FAILURE;
                    break;
                }

                if( !is_ipv4_cidr(address_str) )
                {
                    if( verbose )
                    {
                        printf("Cannot check if %s is a valid host address: missing prefix length\n", address_str);
                    }
                    result = RESULT_FAILURE;
                }
                else
                {
                    result = is_ipv4_host(address);
                    if( (result == RESULT_FAILURE) && verbose )
                    {
                        if( ((cidr_equals(address, cidr_addr_network(address)) >= 0) &&
                             (cidr_get_pflen(address) != 32)) )
                        {
                            printf("%s is an IPv4 network address, not a host address\n", address_str);
                        }
                    }
                }
                break;
            case IS_IPV4_NET:
                /* Host vs. network address check only makes sense
                   if prefix length is given */
                if( !(cidr_get_proto(address) == CIDR_IPV4) ) {
                    if( verbose )
                    {
                        printf("%s is not a valid IPv4 address\n", address_str);
                    }
                    result = RESULT_FAILURE;
                    break;
                }

                if( !is_ipv4_cidr(address_str) )
                {
                    if( verbose )
                    {
                        printf("Cannot check if %s is a valid network address: missing prefix length\n", address_str);
                    }
                    result = RESULT_FAILURE;
                }
                else
                {
                    result = is_ipv4_net(address);
                    if( (result == RESULT_FAILURE) && verbose )
                    {
                        if( ((cidr_equals(address, cidr_addr_network(address)) < 0) &&
                             (cidr_get_pflen(address) != 32)) )
                        {
                            char* network_addr = cidr_to_str(cidr_addr_network(address), 0);
                            printf("%s is an IPv4 host address, not a network address. Did you mean %s?\n", address_str, network_addr);
                        }
                    }
                }
                break;
            case IS_IPV4_BROADCAST:
                /* Broadcast address check only makes sense
                   if prefix length is given */
                if( !is_ipv4_cidr(address_str) )
                {
                    if( verbose )
                    {
                        printf("Cannot check if %s is a broadcast address: missing prefix length\n", address_str);
                    }
                    result = RESULT_FAILURE;
                }
                else
                {
                    result = is_ipv4_broadcast(address);
                }
                break;
            case IS_IPV4_MULTICAST:
                result = is_ipv4_multicast(address);
                break;
            case IS_IPV4_LOOPBACK:
                result = is_ipv4_loopback(address);
                break;
            case IS_IPV4_LINKLOCAL:
                result = is_ipv4_link_local(address);
                break;
            case IS_IPV4_RFC1918:
                result = is_ipv4_rfc1918(address);
                break;
            case IS_IPV6:
                result = is_ipv6(address);
                break;
            case IS_IPV6_CIDR:
                result = is_ipv6_cidr(address_str);
                break;
            case IS_IPV6_SINGLE:
                result = is_ipv6_single(address_str);
                break;
            case IS_IPV6_HOST:
                /* Host vs. network address check only makes sense
                   if prefix length is given */
                if( !(cidr_get_proto(address) == CIDR_IPV6) ) {
                    if( verbose )
                    {
                        printf("%s is not a valid IPv6 address\n", address_str);
                    }
                    result = RESULT_FAILURE;
                    break;
                }

                if( !is_ipv6_cidr(address_str) )
                {
                    if( verbose )
                    {
                        printf("Cannot check if %s is a valid IPv6 host address: missing prefix length\n", address_str);
                    }
                    result = RESULT_FAILURE;
                }
                else
                {
                    result = is_ipv6_host(address);
                    if( (result == RESULT_FAILURE) && verbose )
                    {
                        if( ((cidr_equals(address, cidr_addr_network(address)) >= 0) && (cidr_get_pflen(address) != 128)) )
                        {
                            printf("%s is an IPv6 network address, not a host address\n", address_str);
                        }
                    }
                }
                break;
            case IS_IPV6_NET:
                /* Host vs. network address check only makes sense
                   if prefix length is given */
                if( !(cidr_get_proto(address) == CIDR_IPV6) ) {
                    if( verbose )
                    {
                        printf("%s is not a valid IPv6 address\n", address_str);
                    }
                    result = RESULT_FAILURE;
                    break;
                }

                if( !is_ipv6_cidr(address_str) )
                {
                    if( verbose )
                    {
                        printf("Cannot check if %s is a valid IPv6 network address: missing prefix length\n", address_str);
                    }
                    result = RESULT_FAILURE;
                }
                else
                {
                    result = is_ipv6_net(address);
                    if( (result == RESULT_FAILURE) && verbose )
                    {
                        if( ((cidr_equals(address, cidr_addr_network(address)) < 0) && (cidr_get_pflen(address) != 128)) ) {
                            char* network_addr = cidr_to_str(cidr_addr_network(address), 0);
                            printf("%s is an IPv6 host address, not a network address. Did you mean %s?\n", address_str, network_addr);
                        }
                    }
                }
                break;
            case IS_IPV6_MULTICAST:
                 result = is_ipv6_multicast(address);
                 break;
            case IS_IPV6_LINKLOCAL:
                 result = is_ipv6_link_local(address);
                 break;
            case IS_ANY_CIDR:
                 result = is_any_cidr(address_str);
                 break;
            case IS_ANY_SINGLE:
                 result = is_any_single(address_str);
                 break;
            case IS_VALID_INTF_ADDR:
                 result = is_valid_intf_address(address, address_str, allow_loopback);
                 break;
            case NO_ACTION:
                 break;
            case IS_ANY_HOST:
                /* Host vs. network address check only makes sense
                   if prefix length is given */
                 if( !is_any_cidr(address_str) )
                 {
                    if( verbose )
                    {
                        printf("Cannot check if %s is a valid host address: missing prefix length\n", address_str);
                    }
                    result = RESULT_FAILURE;
                 }
                 else
                 {
                     result = is_any_host(address);
                     if( (result == RESULT_FAILURE) && verbose ) {
                         if( ((cidr_equals(address, cidr_addr_network(address)) >= 0) &&
                              (cidr_get_pflen(address) != 32) &&
                              (cidr_get_pflen(address) != 128)) )
                         {
                             printf("%s is a network address, not a host address\n", address_str);
                         }
                     }
                 }
                 break;
            case IS_ANY_NET:
                /* Host vs. network address check only makes sense
                   if prefix length is given */
                 if( !is_any_cidr(address_str) )
                 {
                     if( verbose )
                     {
                         printf("Cannot check if %s is a valid network address: missing prefix length\n", address_str);
                     }
                    result = RESULT_FAILURE;
                 }
                 else
                 {
                     result = is_any_net(address);
                     if( (result == RESULT_FAILURE) && verbose )
                     {
                         if( ((cidr_equals(address, cidr_addr_network(address)) < 0) &&
                              (cidr_get_pflen(address) != 128) &&
                              (cidr_get_pflen(address) != 32)) )
                         {
                             char* network_addr = cidr_to_str(cidr_addr_network(address), 0);
                             printf("%s is a host address, not a network address. Did you mean %s?\n", address_str, network_addr);
                         }
                     }
                 }
                 break;
            default:
                 break;
        }
        action_count--;
    }

    /* Clean up */
    free(actions);
    cidr_free(address);

    if( result == RESULT_SUCCESS )
    {
        return(EXIT_SUCCESS);
    }
    else
    {
        return(EXIT_FAILURE);
    }
}

/*
 * Print help, no other side effects
 */
void print_help(const char* program_name)
{
    printf("Usage: %s <OPTIONS> [STRING]\n", program_name);
    printf("\
Options:\n\
  --is-valid                 Check if STRING is a valid IPv4 or IPv6 address\n\
                               with or without prefix length\n\
  --is-any-cidr              Check if STRING is a valid IPv4 or IPv6 address\n\
                               with prefix length\n\
  --is-any-single            Check if STRING is a valid single IPv4 or IPv6 address\n\
  --is-any-host              Check if STRING is a valid IPv4 or IPv6 host address\n\
  --is-any-net               Check if STRING is a valid IPv4 or IPv6 network address\n\
  --is-ipv4                  Check if STRING is a valid IPv4 address with mask \n\
  --is-ipv4-cidr             Check if STRING is a valid CIDR-formatted address \n\
  --is-ipv4-single           Check if STRING is a valid single address\n\
                               (i.e. with no mask)\n\
  --is-ipv4-host             Check if STRING is a host address \n\
  --is-ipv4-net              Check if STRING is a network address \n\
  --is-ipv4-broadcast        Check if STRING is a broadcast address \n\
  --is-ipv4-multicast        Check if STRING is a multicast address \n\
  --is-ipv4-loopback         Check if STRING is a loopback address \n\
  --is-ipv4-link-local       Check if STRING is a link-local address \n\
  --is-ipv4-rfc1918          Check if STRING is a private (RFC1918) address \n\
  --is-ipv6                  Check if STRING is a valid IPv6 address \n\
  --is-ipv6-cidr             Check if STRING is a CIDR-formatted IPv6 address \n\
  --is-ipv6-single           Check if STRING is an IPv6 address with no mask \n\
  --is-ipv6-host             Check if STRING is an IPv6 host address \n\
  --is-ipv6-net              Check if STRING is an IPv6 network address \n\
  --is-ipv6-multicast        Check if STRING is an IPv6 multicast address \n\
  --is-ipv6-link-local       Check if STRING is an IPv6 link-local address \n\
  --is-valid-intf-address    Check if STRING is an IPv4 or IPv6 address that \n\
                               can be assigned to a network interface \n\
  --allow-loopback           When used with --is-valid-intf-address,\n\
                               makes IPv4 loopback addresses pass the check\n\
  --version                  Print version information and exit \n\
  --help                     Print help message and exit\n\
\n\
Exit codes:\n\
  0    if check passed,\n\
  1    if check failed,\n\
  2    if a problem occured (wrong option, internal error etc.)\n");
}

/*
 * Print version information, no other side effects
 */
void print_version(void)
{
    printf("%s %s\n\n", PACKAGE_NAME, PACKAGE_VERSION);
    printf("Copyright (C) VyOS maintainers and contributors 2018.\n\
License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.\n");
}
