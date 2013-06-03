/*
 * ipaddrcheck_functions.c: IPv4/IPv6 validation functions for ipaddrcheck
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "ipaddrcheck_functions.h"

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

/* Does it look like IPv4 CIDR (e.g. 192.0.2.1/24)? */
int is_ipv4_cidr(char* address_str)
{
    int result;

    int offsets[1];
    pcre *re;
    int rc;
    const char *error;
    int erroffset;

    re = pcre_compile("^((([1-9]\\d{0,2}|0)\\.){3}([1-9]\\d{0,2}|0)\\/([1-9]\\d*|0))$",
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

/* Is it a single dotted decimal address? */
int is_ipv4_single(char* address_str)
{
    int result;

    int offsets[1];
    pcre *re;
    int rc;
    const char *error;
    int erroffset;

    re = pcre_compile("^((([1-9]\\d{0,2}|0)\\.){3}([1-9]\\d{0,2}|0))$",
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

/* Is it an IPv6 address with prefix length? */
int is_ipv6_cidr(char* address_str)
{
    int result;

    int offsets[1];
    pcre *re;
    int rc;
    const char *error;
    int erroffset;

    re = pcre_compile("^((([0-9a-fA-F\\:])+)(\\/\\d{1,3}))$",
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

/* Is it a single IPv6 address? */
int is_ipv6_single(char* address_str)
{
    int result;

    int offsets[1];
    pcre *re;
    int rc;
    const char *error;
    int erroffset;

    re = pcre_compile("^(([0-9a-fA-F\\:])+)$",
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

/* Is it a CIDR-formatted IPv4 or IPv6 address? */
int is_any_cidr(char* address_str)
{
    int result;

    if( (is_ipv4_cidr(address_str) == RESULT_SUCCESS) ||
        (is_ipv6_cidr(address_str) == RESULT_SUCCESS) )
    {
        result = RESULT_SUCCESS;
    }
    else
    {
        result = RESULT_FAILURE;
    }

    return(result);
}

/* Is it a single IPv4 or IPv6 address? */
int is_any_single(char* address_str)
{
    int result;

    if( (is_ipv4_single(address_str) == RESULT_SUCCESS) ||
        (is_ipv6_single(address_str) == RESULT_SUCCESS) )
    {
        result = RESULT_SUCCESS;
    }
    else
    {
        result = RESULT_FAILURE;
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

    /* The very concept of broadcast address doesn't apply to
       IPv6 and point-to-point or /32 IPv4 */
    if( (cidr_get_proto(address) == CIDR_IPV4) &&
        (cidr_equals(address, cidr_addr_broadcast(address)) == 0 ) &&
        (cidr_get_pflen(address) < 31) )
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
     int result;

     if( cidr_get_proto(address) == CIDR_IPV6 )
     {
          result = RESULT_SUCCESS;
     }
     else
     {
          result = RESULT_FAILURE;
     }

     return(result);
}

/* Is it a correct IPv6 host address? */
int is_ipv6_host(CIDR *address)
{
    int result;

    if( (cidr_get_proto(address) == CIDR_IPV6) &&
        ((cidr_equals(address, cidr_addr_network(address)) < 0) ||
        (cidr_get_pflen(address) == 128)) )
    {
         result = RESULT_SUCCESS;
    }
    else
    {
         result = RESULT_FAILURE;
    }

    return(result);
}

/* Is it a correct IPv6 network address? */
int is_ipv6_net(CIDR *address)
{
    int result;

    if( (cidr_get_proto(address) == CIDR_IPV6) &&
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

/* Is it an IPv6 multicast address? */
int is_ipv6_multicast(CIDR *address)
{
    int result;

    if( (cidr_get_proto(address) == CIDR_IPV6) &&
        (cidr_contains(cidr_from_str(IPV6_MULTICAST), address) == 0) )
    {
        result = RESULT_SUCCESS;
    }
    else
    {
        result = RESULT_FAILURE;
    }

    return(result);
}

/* Is it an IPv6 link-local address? */
int is_ipv6_link_local(CIDR *address)
{
    int result;

    if( (cidr_get_proto(address) == CIDR_IPV6) &&
        (cidr_contains(cidr_from_str(IPV6_LINKLOCAL), address) == 0) )
    {
        result = RESULT_SUCCESS;
    }
    else
    {
        result = RESULT_FAILURE;
    }

    return(result);
}

/* Is it an address that can belong an interface? */
int is_valid_intf_address(CIDR *address, char* address_str, int allow_loopback)
{
    int result;

    if( (is_ipv4_broadcast(address) == RESULT_FAILURE) &&
        (is_ipv4_multicast(address) == RESULT_FAILURE) &&
        (is_ipv6_multicast(address) == RESULT_FAILURE) &&
        ((is_ipv4_loopback(address) == RESULT_FAILURE) || (allow_loopback == LOOPBACK_ALLOWED)) &&
        (cidr_equals(address, cidr_from_str(IPV6_LOOPBACK)) != 0) &&
        (cidr_equals(address, cidr_from_str(IPV4_UNSPECIFIED)) != 0) &&
        (cidr_contains(cidr_from_str(IPV4_THIS), address) != 0) &&
        (cidr_equals(address, cidr_from_str(IPV4_LIMITED_BROADCAST)) != 0) &&
        (is_any_cidr(address_str) == RESULT_SUCCESS) )
    {
        result = RESULT_SUCCESS;
    }
    else
    {
        result = RESULT_FAILURE;
    }

    return(result);
}

/* Is it an IPv4 or IPv6 host address? */
int is_any_host(CIDR *address)
{
    int result;

    if( (is_ipv4_host(address) == RESULT_SUCCESS) ||
        (is_ipv6_host(address) == RESULT_SUCCESS) )
    {
        result = RESULT_SUCCESS;
    }
    else
    {
        result = RESULT_FAILURE;
    }

    return(result);
}

/* Is it an IPv4 or IPv6 network address? */
int is_any_net(CIDR *address)
{
    int result;

    if( (is_ipv4_net(address) == RESULT_SUCCESS) ||
        (is_ipv6_net(address) == RESULT_SUCCESS) )
    {
        result = RESULT_SUCCESS;
    }
    else
    {
        result = RESULT_FAILURE;
    }

    return(result);
}


