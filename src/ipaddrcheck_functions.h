#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <pcre.h>
#include <libcidr.h>

#define INVALID_PROTO -1

#define RESULT_SUCCESS 1
#define RESULT_FAILURE 0

#define IPV4_MULTICAST "224.0.0.0/4"
#define IPV4_LOOPBACK  "127.0.0.0/8"
#define IPV4_LINKLOCAL "169.254.0.0/16"
#define IPV4_RFC1918_A "10.0.0.0/8"
#define IPV4_RFC1918_B "172.16.0.0/12"
#define IPV4_RFC1918_C "192.168.0.0/16"
#define IPV6_MULTICAST "ff00::/8"
#define IPV6_LINKLOCAL "fe80::/64"

int has_mask(char* address_str);
int is_ipv4_cidr(char* address_str);
int is_ipv4_single(char* address_str);
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


