/*
 * iptest.c: iptest IPv4/IPv6 validator
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

#include "config.h"
#include "iptest.h"

static const struct option options[] =
{
    { "is-valid", required_argument, NULL, 'a' },
    { "is-ipv4", required_argument, NULL, 'b' },
    { "is-ipv4-host", required_argument, NULL, 'c'},
    { "is-ipv4-net", required_argument, NULL, 'd'},
    { "is-ipv6", required_argument, NULL, 'g' },
    { "is-ipv6-net", required_argument, NULL, 'G' },
    { NULL, no_argument, NULL, 0 }
};

void help(void)
{
    static char *message = \
"--is-valid STR    Check if STR is a valid IPv4 or IPv6 address\n\
                   or subnet\n\
--is-ipv4 STR Check if STR is a valid IPv4 address with mask\n";

    printf("%s", message);
}

int main(int argc, char* argv[])
{
    char *address_str = "";
    int action = 0;

    int option_index = 0;
    char c;
    int option_count = 0;

    while( (c = getopt_long(argc, argv, "abcd?", options, &option_index)) != -1 )
    {
         switch(c)
         {
             case 'a':
                 action = IS_VALID;
                 address_str = optarg;
                 break;
             case 'b':
                 action = IS_IPV4;
                 address_str = optarg;
                 break;
             case 'c':
                 action = IS_IPV4_HOST;
                 address_str = optarg;
                 break;
             case 'd':
                 action = IS_IPV4_NET;
                 address_str = optarg;
                 break;
             case 'I':
                 action = IS_IPV6;
                 address_str = optarg;
                 break;
             case '?':
             default:
                fprintf(stderr, "Invalid option\n");
                break;
         }
         option_count++;
    }

    if( option_count != 1 )
    {
        fprintf(stderr, "Wrong options number, exactly one option expected!\n");
        help();
        return(1);
    }

    CIDR *address;
    address = cidr_from_str(address_str);
    int result = EXIT_FAILURE;

    switch(action)
    {
        case IS_VALID:
            result = is_valid_address(address);
            break;
        case IS_IPV4:
            result = is_ipv4(address);
            break;
        case IS_IPV4_HOST:
            result = is_ipv4_host(address);
            break;
        case IS_IPV4_NET:
            result = is_ipv4_net(address);
            break;
    }

    return(result);
}


