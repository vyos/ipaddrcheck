/*
 * ipaddrcheck.c: an IPv4/IPv6 validator
 *
 * Maintainer: Daniil Baturin <daniil at baturin dot org>
 *
 * Copyright (C) 2013 SO3Group
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
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
#define HAS_MASK              210
#define IS_VALID_INTF_ADDR    220

static const struct option options[] =
{
    { "is-valid",              no_argument, NULL, 'a' },
    { "has-mask",              no_argument, NULL, 'b' },
    { "is-ipv4",               no_argument, NULL, 'c' },
    { "is-ipv4-cidr",          no_argument, NULL, 'd' },
    { "is-ipv4-single",        no_argument, NULL, 'e' },
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
    { "legacy",                no_argument, NULL, 'w' },
    { "version",               no_argument, NULL, 'z' },
    { NULL,                    no_argument, NULL, 0   }
};

/* Auxillary functions */
static void print_help(void);
static void print_version(void);

int main(int argc, char* argv[])
{
    char *address_str = "";    /* IP address string obtained from arguments */
    int action = 0;            /* Action associated with given check option */
    int* actions;              /* Array of all given actions */
    int action_count = 0;      /* Actions array size */

    int option_index = 0;      /* Number of the current option for getopt call */
    int optc;                  /* Option character for getopt call */

    /* Parse options, convert to action codes, store in array */

    /* Try to allocate memory for the actions array, abort if fail */
    actions = (int*)calloc(argc, sizeof(int));
    if( errno == ENOMEM )
    {
        fprintf(stderr, "Error: could not allocate memory!\n");
        return(EXIT_FAILURE);
    }

    while( (optc = getopt_long(argc, argv, "abcdefghijklmnoprstuz?", options, &option_index)) != -1 )
    {
         switch(optc)
         {
             case 'a':
                 action = IS_VALID;
                 break;
             case 'b':
                 action = HAS_MASK;
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
             case '?':
                 print_help();
                 return(0);
             case 'z':
                 print_version();
                 return(0);
             default:
                fprintf(stderr, "Error: invalid option\n");
                break;
         }

         action_count = optind-2;
         actions[action_count] = action;
    }

    /* Get non-option arguments */
    if( (argc - optind) == 1 )
    {
         address_str = argv[optind];
    }
    else
    {
         fprintf(stderr, "Error: wrong number of arguments, one argument required!\n");
         print_help();
         return(EXIT_FAILURE);
    }

    CIDR *address;
    address = cidr_from_str(address_str);
    int result = RESULT_SUCCESS;

    /* Check if the address is valid at all,
       if not there is no point in going further */
    if( is_valid_address(address) != RESULT_SUCCESS )
    {
        return(EXIT_FAILURE);
    }

    printf("action_count: %d\n", action_count);
    while( (action_count >= 0) && (result == RESULT_SUCCESS) )
    {
        switch(actions[action_count])
        {
            case IS_VALID:
                result = is_valid_address(address);
                break;
            case HAS_MASK:
                result = has_mask(address_str);
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
                result = is_ipv4_host(address);
                break;
            case IS_IPV4_NET:
                /* XXX: Should we fail this check for single addresses? */
                result = is_ipv4_net(address);
                break;
            case IS_IPV4_BROADCAST:
                result = is_ipv4_broadcast(address);
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
                break;
            case IS_IPV6_SINGLE:
                break;
            case IS_IPV6_HOST:
                result = is_ipv6_host(address);
                break;
            case IS_IPV6_NET:
                result = is_ipv6_net(address);
                break;
            case IS_IPV6_MULTICAST:
                 result = is_ipv6_multicast(address);
                 break;
            case IS_IPV6_LINKLOCAL:
                 result = is_ipv6_link_local(address);
                 break;
        }
        printf("action: %d\n", actions[action_count]);
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
void print_help(void)
{
    static const char *message = \
"--is-valid STR    Check if STR is a valid IPv4 or IPv6 address\n\
                   or subnet\n\
--is-ipv4 STR Check if STR is a valid IPv4 address with mask\n";

    printf("%s", message);
}

/*
 * Print version information, no other side effects
 */
void print_version(void)
{
    printf("%s %s\n", PACKAGE_NAME, PACKAGE_VERSION);
    printf("Copyright (C) SO3Group 2013.\n\
License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.\n");
}


