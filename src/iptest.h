/*
 * iptest.h: macros and functions for iptest IPv4/IPv6 validator
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


#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <libcidr.h>

#define INVALID_PROTO -1

/* Option codes */
#define IS_VALID      10
#define IS_IPV4 20
#define IS_IPV4_HOST 30
#define IS_IPV4_NET 40
#define IS_IPV4_BROADCAST 50
#define IS_IPV4_UNICAST 60
#define IS_IPV4_MULTICAST 70
#define IS_IPV4_RFC1918 80
#define IS_IPV4_LOOPBACK 85
#define IS_IPV6 90
#define IS_IPV6_HOST 100
#define IS_IPV6_NET 110
#define IS_IPV6_UNICAST 120
#define IS_IPV6_MULTICAST 130
#define IS_IPV6_LINKLOCAL 140

/* Does it look like a valid address of any protocol? */
int is_valid_address(CIDR *address)
{
     int result;

     if( cidr_get_proto(address) != INVALID_PROTO )
     {
          result = EXIT_SUCCESS;
     }
     else
     {
          result = EXIT_FAILURE;
     }

     return(result);
}

/* Is it a correct IPv4 host or subnet address
   with or without net mask */
int is_ipv4(CIDR *address)
{
     int result;

     if( cidr_get_proto(address) == CIDR_IPV4 )
     {
          result = EXIT_SUCCESS;
     }
     else
     {
          result = EXIT_FAILURE;
     }

     return(result);
}

/* Is it a correct IPv4 host (i.e. not network) address? */
int is_ipv4_host(CIDR *address)
{
    int result;

    if( (cidr_get_proto(address) == CIDR_IPV4) &&
        cidr_equals(address, cidr_addr_network(address)) )
    {
         result = EXIT_SUCCESS;
    }
    else
    {
         result = EXIT_FAILURE;
    }

    return(result);
}

/* Is it a correct IPv4 network address? */
int is_ipv4_net(CIDR *address)
{
    /* TODO: Don't try to validate is mask is not present */
    int result;

    if( (cidr_get_proto(address) == CIDR_IPV4) &&
        (cidr_equals(address, cidr_addr_network(address)) == 0) )
    {
         result = EXIT_SUCCESS;
    }
    else
    {
         result = EXIT_FAILURE;
    }

    return(result);
}
