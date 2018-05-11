ipaddrcheck
===========

An IPv4 and IPv6 validation utility for use in scripts

Depends on libcidr by Matthew Fuller (http://www.over-yonder.net/~fullermd/projects/libcidr) and libpcre.

    Usage: ./ipaddrcheck <OPTIONS> [STRING]
    Options:
        --is-valid                 Check if STRING is a valid IPv4 or IPv6 address
                                   with or without prefix length
        --is-any-cidr              Check if STRING is a valid IPv4 or IPv6 address
                                   with prefix length
        --is-any-single            Check if STRING is a valid single IPv4 or IPv6 address
        --is-any-host              Check if STRING is a valid IPv4 or IPv6 host address
        --is-any-net               Check if STRING is a valid IPv4 or IPv6 network address
        --is-ipv4                  Check if STRING is a valid IPv4 address with mask 
        --is-ipv4-cidr             Check if STRING is a valid CIDR-formatted address 
        --is-ipv4-single           Check if STRING is a valid single address
                                   (i.e. with no mask)
        --is-ipv4-host             Check if STRING is a host address 
        --is-ipv4-net              Check if STRING is a network address 
        --is-ipv4-broadcast        Check if STRING is a broadcast address 
        --is-ipv4-multicast        Check if STRING is a multicast address 
        --is-ipv4-loopback         Check if STRING is a loopback address 
        --is-ipv4-link-local       Check if STRING is a link-local address 
        --is-ipv4-rfc1918          Check if STRING is a private (RFC1918) address 
        --is-ipv6                  Check if STRING is a valid IPv6 address 
        --is-ipv6-cidr             Check if STRING is a CIDR-formatted IPv6 address 
        --is-ipv6-single           Check if STRING is an IPv6 address with no mask 
        --is-ipv6-host             Check if STRING is an IPv6 host address 
        --is-ipv6-net              Check if STRING is an IPv6 network address 
        --is-ipv6-multicast        Check if STRING is an IPv6 multicast address 
        --is-ipv6-link-local       Check if STRING is an IPv6 link-local address 
        --is-valid-intf-address    Check if STRING is an IPv4 or IPv6 address that 
                                   can be assigned to a network interface 
        --allow-loopback           When used with --is-valid-intf-address,
                                   makes IPv4 loopback addresses pass the check
        --version                  Print version information and exit 
        --help                     Print help message and exit

    Exit codes:
        0    if check passed,
        1    if check failed,
        2    if a problem occured (wrong option, internal error etc.)

