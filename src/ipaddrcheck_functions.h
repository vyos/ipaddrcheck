/*
 * ipaddrcheck_functions.h: macros and prototypes for ipaddrcheck
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <pcre.h>
#include <libcidr.h>

#define INVALID_PROTO -1

#define RESULT_SUCCESS 1
#define RESULT_FAILURE 0
#define RESULT_INT_ERROR 2

#define IPV4_MULTICAST "224.0.0.0/4"
#define IPV4_LOOPBACK  "127.0.0.0/8"
#define IPV4_LINKLOCAL "169.254.0.0/16"
#define IPV4_UNSPECIFIED "0.0.0.0/0"
#define IPV4_THIS        "0.0.0.0/8"
#define IPV4_RFC1918_A "10.0.0.0/8"
#define IPV4_RFC1918_B "172.16.0.0/12"
#define IPV4_RFC1918_C "192.168.0.0/16"
#define IPV4_LIMITED_BROADCAST "255.255.255.255/32"
#define IPV6_MULTICAST "ff00::/8"
#define IPV6_LINKLOCAL "fe80::/64"
#define IPV6_LOOPBACK  "::1/128"

#define NO_LOOPBACK      0
#define LOOPBACK_ALLOWED 1

int is_ipv4_cidr(char* address_str);
int is_ipv4_single(char* address_str);
int is_ipv6_cidr(char* address_str);
int is_ipv6_single(char* address_str);
int is_any_cidr(char* address_str);
int is_any_single(char* address_str);
int is_valid_address(CIDR *address);
int is_ipv4(CIDR *address);
int is_ipv4_host(CIDR *address);
int is_ipv4_net(CIDR *address);
int is_ipv4_broadcast(CIDR *address);
int is_ipv4_multicast(CIDR *address);
int is_ipv4_loopback(CIDR *address);
int is_ipv4_link_local(CIDR *address);
int is_ipv4_rfc1918(CIDR *address);
int is_ipv6(CIDR *address);
int is_ipv6_host(CIDR *address);
int is_ipv6_net(CIDR *address);
int is_ipv6_multicast(CIDR *address);
int is_ipv6_link_local(CIDR *address);
int is_valid_intf_address(CIDR *address, char* address_str, int allow_loopback);
int is_any_host(CIDR *address);
int is_any_net(CIDR *address);
