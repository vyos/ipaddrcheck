/*
 * ipaddrcheck.h: macros and functions for iptest IPv4/IPv6 validator
 *
 * Maintainer: Daniil Baturin <daniil at baturin dot org>
 *
 * Copyright (C) 2013 SO3Group
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <pcre.h>
#include <libcidr.h>

#define INVALID_PROTO -1

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

#define RESULT_SUCCESS 1
#define RESULT_FAILURE 0

#define IPV4_MULTICAST "224.0.0.0/4"
#define IPV4_LOOPBACK  "127.0.0.0/8"
#define IPV4_LINKLOCAL "169.254.0.0/16"
#define IPV4_RFC1918_A "10.0.0.0/8"
#define IPV4_RFC1918_B "172.16.0.0/12"
#define IPV4_RFC1918_C "192.168.0.0/16"

/*
 * Address string functions
 *
 * Note that they perform format check only
 * and must not be used to deermine if it's
 * a valid address, only what type of address
 * format it is.
 *
 * The only reason they exist is that libcidr
 * is very liberal on its input format and
 * doesn't provide any information on what
 * the format was.
 */

/* Does it have mask? */
int has_mask(char* address_str)
{
    int result;
    char *hasslash = strchr(address_str, '/');

    if( hasslash != NULL )
    {
        result = RESULT_SUCCESS;
    }
    else
    {
        result = RESULT_FAILURE;
    }

    return(result);
}

/* Does it look like IPv4 CIDR (e.g. 192.0.2.1/24)? */
int is_ipv4_cidr(char* address_str)
{
    int result;

    int offsets[1];
    pcre *re;
    int rc;
    const char *error;
    int erroffset;

    re = pcre_compile("^\\d{1,3}.\\d{1,3}.\\d{1,3}.\\d{1,3}/\\d{1,2}$",
                      0, &error, &erroffset, NULL);
    rc = pcre_exec(re, NULL, address_str, strlen(address_str), 0, 0, offsets, 1);

    if( rc < 0 )
    {
        result = RESULT_FAILURE;
    }
    else
    {
        result = RESULT_SUCCESS;
    }

    return(result);
}


/*
 * Address checking functions that rely on libcidr
 */

/* Does it look like a valid address of any protocol? */
int is_valid_address(CIDR *address)
{
     int result;

     if( cidr_get_proto(address) != INVALID_PROTO )
     {
          result = RESULT_SUCCESS;
     }
     else
     {
          result = RESULT_FAILURE;
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
          result = RESULT_SUCCESS;
     }
     else
     {
          result = RESULT_FAILURE;
     }

     return(result);
}

/* Is it a correct IPv4 host (i.e. not network) address? */
int is_ipv4_host(CIDR *address)
{
    int result;

    if( (cidr_get_proto(address) == CIDR_IPV4) &&
        ((cidr_equals(address, cidr_addr_network(address)) < 0) ||
        (cidr_get_pflen(address) == 32)) )
    {
         result = RESULT_SUCCESS;
    }
    else
    {
         result = RESULT_FAILURE;
    }

    return(result);
}

/* Is it a correct IPv4 network address? */
int is_ipv4_net(CIDR *address)
{
    int result;

    if( (cidr_get_proto(address) == CIDR_IPV4) &&
        (cidr_equals(address, cidr_addr_network(address)) == 0) )
    {
         result = RESULT_SUCCESS;
    }
    else
    {
         result = RESULT_FAILURE;
    }

    return(result);
}

/* Is it an IPv4 broadcast address? */
int is_ipv4_broadcast(CIDR *address)
{
    int result;

    if( cidr_equals(address, cidr_addr_broadcast(address)) == 0 )
    {
        result = RESULT_SUCCESS;
    }
    else
    {
        result = RESULT_FAILURE;
    }

    return(result);
}

/* Is it an IPv4 multicast address? */
int is_ipv4_multicast(CIDR *address)
{
    int result;

    if( (cidr_get_proto(address) == CIDR_IPV4) &&
        (cidr_contains(cidr_from_str(IPV4_MULTICAST), address) == 0) )
    {
        result = RESULT_SUCCESS;
    }
    else
    {
        result = RESULT_FAILURE;
    }

    return(result);
}

/* Is it an IPv4 loopback address? */
int is_ipv4_loopback(CIDR *address)
{
    int result;

    if( (cidr_get_proto(address) == CIDR_IPV4) &&
        (cidr_contains(cidr_from_str(IPV4_LOOPBACK), address) == 0) )
    {
        result = RESULT_SUCCESS;
    }
    else
    {
        result = RESULT_FAILURE;
    }

    return(result);
}

/* Is it an IPv4 link-local address? */
int is_ipv4_link_local(CIDR *address)
{
    int result;

    if( (cidr_get_proto(address) == CIDR_IPV4) &&
        (cidr_contains(cidr_from_str(IPV4_LINKLOCAL), address) == 0) )
    {
        result = RESULT_SUCCESS;
    }
    else
    {
        result = RESULT_FAILURE;
    }

    return(result);
}

/* Is it an IPv4 RFC1918 address? */
int is_ipv4_rfc1918(CIDR *address)
{
    int result;

    if( (cidr_get_proto(address) == CIDR_IPV4) &&
        ( (cidr_contains(cidr_from_str(IPV4_RFC1918_A), address) == 0) ||
        (cidr_contains(cidr_from_str(IPV4_RFC1918_B), address) == 0) ||
        (cidr_contains(cidr_from_str(IPV4_RFC1918_C), address) == 0) ) )
    {
        result = RESULT_SUCCESS;
    }
    else
    {
        result = RESULT_FAILURE;
    }

    return(result);
}

/* is it a correct IPv6 host or subnet address,
   with or withour mask */
int is_ipv6(CIDR *address)
{
    return RESULT_FAILURE;
}

int is_ipv6_host(CIDR *address)
{
    return RESULT_FAILURE;
}

int is_ipv6_net(CIDR *address)
{
    return RESULT_FAILURE;
}
