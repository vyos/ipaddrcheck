[NAME]
ipaddrcheck \- an IPv4 and IPv6 validator suitable for use in scripts

[DESCRIPTION]
ipaddrcheck is capable of checking various facts about IPv4 and IPv6 addresses,
from checking whether the address is valid to checking whether it is an IPv4
multicast address, host or network address and many more.

[OPTIONS]
If more than one option is given, they work as logical AND (i.e. if one of them
fails, overall check result is fail). Logical consistency of the options is
left up to the user. The program does not detect whether given options are
compatible, so a mutually exclusive combination like "--is-ipv4 --is-ipv6"
will be accepted and simply return fail for any argument.

[DEFINITIONS]
Broadcast address: the last address of an IPv4 subnet.

Interface address: address passes --is-valid-intf-address check if
prefix length is specified, it is not a broadcast address, not a multicast
address, not unspecified address, not from IPv4 THIS network, not a network address
and not the IPv4 limited broadcast address.

[EXAMPLES]
Check if the argument is a valid IPv4 host address:
ipaddrcheck --is-ipv4-host 192.0.2.78/24

Check if the argument is an address that can be assigned to a loopback interface:
ipaddrcheck --allow-loopback --is-valid-intf-address 127.0.0.7/8

[AUTHOR]
Written by SO3Group.
