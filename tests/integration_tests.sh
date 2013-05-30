#!/bin/bash
#
# integration_tests.sh: ipaddrcheck integration tests
#
# Maintainer: Daniil Baturin <daniil at baturin dot org>
#
# Copyright (C) 2013 SO3Group
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

. ./assert.sh

IPADDRCHECK=ipaddrcheck

# begin ipaddrcheck_integration

assert_raises "$IPADDRCHECK --is-valid 192.0.2.1"      0
assert_raises "$IPADDRCHECK --is-valid 192.0.2.1/24"   0
assert_raises "$IPADDRCHECK --is-valid 192.0.2.0/26"   0
assert_raises "$IPADDRCHECK --is-valid 2001:db8::1"    0
assert_raises "$IPADDRCHECK --is-valid 2001:db8::/56"  0
assert_raises "$IPADDRCHECK --is-valid 192.0.2.666"    0
assert_raises "$IPADDRCHECK --is-valid garbage"        1

assert_end ipaddrcheck_integration
