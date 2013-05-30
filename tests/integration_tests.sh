#!/bin/bash

. ./assert.sh

IPADDRCHECK=ipaddrcheck

# begin ipaddrcheck_integration

assert_raises "$IPADDRCHECK --is-valid 192.0.2.1"      0
assert_raises "$IPADDRCHECK --is-valid 192.0.2.1/24"   0
assert_raises "$IPADDRCHECK --is-valid 192.0.2.0/26"   0
assert_raises "$IPADDRCHECK --is-valid 2001:db8::1"    0
assert_raises "$IPADDRCHECK --is-valid 2001:db8::/56"  0
assert_raises "$IPADDRCHECK --is-valid 192.0.2.666"    1
assert_raises "$IPADDRCHECK --is-valid garbage"        1

assert_end ipaddrcheck_integration
