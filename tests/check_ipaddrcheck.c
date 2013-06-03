/*
 * check_ipaddrcheck.c: ipaddrcheck unit tests
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

#include <check.h>
#include "../src/ipaddrcheck_functions.h"

START_TEST (test_is_valid_address)
{
    char* good_v4_address_str = "192.0.2.1";
    CIDR* good_v4_address = cidr_from_str(good_v4_address_str);
    ck_assert_int_eq(is_valid_address(good_v4_address), RESULT_SUCCESS);
    cidr_free(good_v4_address);

    char* good_v6_address_str = "2001:db8:dead::1/56";
    CIDR* good_v6_address = cidr_from_str(good_v6_address_str);
    ck_assert_int_eq(is_valid_address(good_v6_address), RESULT_SUCCESS);
    cidr_free(good_v6_address);

    char* bad_address_str = "192.0.299.563";
    CIDR* bad_address = cidr_from_str(bad_address_str);
    ck_assert_int_eq(is_valid_address(bad_address), RESULT_FAILURE);
    cidr_free(bad_address);
}
END_TEST

START_TEST (test_is_ipv4_cidr)
{
    char* good_address_str_1 = "192.0.2.1/8";
    ck_assert_int_eq(is_ipv4_cidr(good_address_str_1), RESULT_SUCCESS);

    char* good_address_str_2 = "192.0.2.1/21";
    ck_assert_int_eq(is_ipv4_cidr(good_address_str_2), RESULT_SUCCESS);

    char* address_str_no_mask = "192.0.2.1";
    ck_assert_int_eq(is_ipv4_cidr(address_str_no_mask), RESULT_FAILURE);

    /* libcidr allows it, but we don't want to support it */
    char* address_str_decimal_mask = "192.0.2.1/255.255.255.0";
    ck_assert_int_eq(is_ipv4_cidr(address_str_decimal_mask), RESULT_FAILURE);
}
END_TEST

START_TEST (test_is_ipv4_single)
{
    char* good_address_str = "192.0.2.1";
    ck_assert_int_eq(is_ipv4_single(good_address_str), RESULT_SUCCESS);

    char* bad_address_str = "192.0.2.1/25";
    ck_assert_int_eq(is_ipv4_single(bad_address_str), RESULT_FAILURE);
}
END_TEST

START_TEST (test_is_ipv6_cidr)
{
    char* good_address_str = "2001:db8:abcd::/64";
    ck_assert_int_eq(is_ipv6_cidr(good_address_str), RESULT_SUCCESS);

    char* address_str_no_mask = "2001:db8::1";
    ck_assert_int_eq(is_ipv6_cidr(address_str_no_mask), RESULT_FAILURE);

    /* libcidr allows fully spellt hex masks, but we don't want to support it */
    char* address_str_decimal_mask = "::/0:0:0:0:0:0:0:0";
    ck_assert_int_eq(is_ipv6_cidr(address_str_decimal_mask), RESULT_FAILURE);
}
END_TEST

START_TEST (test_is_ipv6_single)
{
    char* good_address_str = "2001:db8::10";
    ck_assert_int_eq(is_ipv6_single(good_address_str), RESULT_SUCCESS);

    char* bad_address_str = "2001:db8::/32";
    ck_assert_int_eq(is_ipv6_single(bad_address_str), RESULT_FAILURE);
}
END_TEST

START_TEST (test_is_any_cidr)
{
    char* good_address_str_v4 = "192.0.2.1/21";
    ck_assert_int_eq(is_any_cidr(good_address_str_v4), RESULT_SUCCESS);

    char* address_str_no_mask_v4 = "192.0.2.1";
    ck_assert_int_eq(is_any_cidr(address_str_no_mask_v4), RESULT_FAILURE);

    char* good_address_str_v6 = "2001:db8::a/56";
    ck_assert_int_eq(is_any_cidr(good_address_str_v6), RESULT_SUCCESS);

    char* address_str_no_mask_v6 = "2001:db8:a:b::c";
    ck_assert_int_eq(is_any_cidr(address_str_no_mask_v6), RESULT_FAILURE);
}
END_TEST

START_TEST (test_is_any_single)
{
    char* good_address_str_v4 = "192.0.2.1";
    ck_assert_int_eq(is_any_single(good_address_str_v4), RESULT_SUCCESS);

    char* bad_address_str_v4 = "192.0.2.1/25";
    ck_assert_int_eq(is_any_single(bad_address_str_v4), RESULT_FAILURE);

    char* good_address_str_v6 = "2001:db8::10";
    ck_assert_int_eq(is_any_single(good_address_str_v6), RESULT_SUCCESS);

    char* bad_address_str_v6 = "2001:db8::/32";
    ck_assert_int_eq(is_any_single(bad_address_str_v6), RESULT_FAILURE);
}
END_TEST

START_TEST (test_is_ipv4)
{
    char* good_address_str = "192.0.2.1";
    CIDR* good_address = cidr_from_str(good_address_str);
    ck_assert_int_eq(is_ipv4(good_address), RESULT_SUCCESS);
    cidr_free(good_address);

    char* bad_address_str = "2001:db8::1/64";
    CIDR* bad_address = cidr_from_str(bad_address_str);
    ck_assert_int_eq(is_ipv4(bad_address), RESULT_FAILURE);
    cidr_free(bad_address);

}
END_TEST

START_TEST (test_is_ipv4_host)
{
    char* good_address_str_no_mask = "192.0.2.1";
    CIDR* good_address = cidr_from_str(good_address_str_no_mask);
    ck_assert_int_eq(is_ipv4_host(good_address), RESULT_SUCCESS);
    cidr_free(good_address);

    char* good_address_str_cidr = "192.0.2.55/24";
    CIDR* good_address_cidr = cidr_from_str(good_address_str_cidr);
    ck_assert_int_eq(is_ipv4_host(good_address_cidr), RESULT_SUCCESS);
    cidr_free(good_address_cidr);

    char* bad_address_str = "192.0.2.0/24";
    CIDR* bad_address = cidr_from_str(bad_address_str);
    ck_assert_int_eq(is_ipv4_host(bad_address), RESULT_FAILURE);
    cidr_free(bad_address);
}
END_TEST

START_TEST (test_is_ipv4_net)
{
    char* good_address_str = "192.0.2.0/25";
    CIDR* good_address = cidr_from_str(good_address_str);
    ck_assert_int_eq(is_ipv4_net(good_address), RESULT_SUCCESS);
    cidr_free(good_address);

    char* bad_address_str = "192.0.2.55/24";
    CIDR* bad_address = cidr_from_str(bad_address_str);
    ck_assert_int_eq(is_ipv4_net(bad_address), RESULT_FAILURE);
    cidr_free(bad_address);
}
END_TEST

START_TEST (test_is_ipv4_broadcast)
{
    char* good_address_str = "192.0.2.255/24";
    CIDR* good_address = cidr_from_str(good_address_str);
    ck_assert_int_eq(is_ipv4_broadcast(good_address), RESULT_SUCCESS);
    cidr_free(good_address);

    char* bad_address_str = "192.0.2.55/24";
    CIDR* bad_address = cidr_from_str(bad_address_str);
    ck_assert_int_eq(is_ipv4_broadcast(bad_address), RESULT_FAILURE);
    cidr_free(bad_address);

    char* bad_address_str_ptp = "192.0.2.1/31";
    CIDR* bad_address_ptp = cidr_from_str(bad_address_str_ptp);
    ck_assert_int_eq(is_ipv4_broadcast(bad_address_ptp), RESULT_FAILURE);
    cidr_free(bad_address_ptp);

    char* bad_address_str_v6 = "2001:0db8:ffff:ffff:ffff:ffff:ffff:ffff/32";
    CIDR* bad_address_v6 = cidr_from_str(bad_address_str_v6);
    ck_assert_int_eq(is_ipv4_broadcast(bad_address_v6), RESULT_FAILURE);
    cidr_free(bad_address_v6);
}
END_TEST

START_TEST (test_is_ipv4_multicast)
{
    char* good_address_str = "224.0.0.5";
    CIDR* good_address = cidr_from_str(good_address_str);
    ck_assert_int_eq(is_ipv4_multicast(good_address), RESULT_SUCCESS);
    cidr_free(good_address);

    char* bad_address_str = "192.0.2.55";
    CIDR* bad_address = cidr_from_str(bad_address_str);
    ck_assert_int_eq(is_ipv4_multicast(bad_address), RESULT_FAILURE);
    cidr_free(bad_address);
}
END_TEST

START_TEST (test_is_ipv4_loopback)
{
    char* good_address_str = "127.0.0.90";
    CIDR* good_address = cidr_from_str(good_address_str);
    ck_assert_int_eq(is_ipv4_loopback(good_address), RESULT_SUCCESS);
    cidr_free(good_address);

    char* bad_address_str = "192.0.2.55";
    CIDR* bad_address = cidr_from_str(bad_address_str);
    ck_assert_int_eq(is_ipv4_loopback(bad_address), RESULT_FAILURE);
    cidr_free(bad_address);
}
END_TEST

START_TEST (test_is_ipv4_link_local)
{
    CIDR* address = NULL;

    char* good_address_str = "169.254.23.32";
    address = cidr_from_str(good_address_str);
    ck_assert_int_eq(is_ipv4_link_local(address), RESULT_SUCCESS);
    cidr_free(address);

    char* bad_address_str = "192.0.2.55";
    address = cidr_from_str(bad_address_str);
    ck_assert_int_eq(is_ipv4_link_local(address), RESULT_FAILURE);
    cidr_free(address);
}
END_TEST

START_TEST (test_is_ipv4_rfc1918)
{
    char* good_address_str_a = "10.0.0.1";
    CIDR* good_address_a = cidr_from_str(good_address_str_a);
    ck_assert_int_eq(is_ipv4_rfc1918(good_address_a), RESULT_SUCCESS);
    cidr_free(good_address_a);

    char* good_address_str_b = "172.16.25.100";
    CIDR* good_address_b = cidr_from_str(good_address_str_b);
    ck_assert_int_eq(is_ipv4_rfc1918(good_address_b), RESULT_SUCCESS);
    cidr_free(good_address_b);

    char* good_address_str_c = "192.168.1.67";
    CIDR* good_address_c = cidr_from_str(good_address_str_c);
    ck_assert_int_eq(is_ipv4_rfc1918(good_address_c), RESULT_SUCCESS);
    cidr_free(good_address_c);

    char* bad_address_str = "192.0.2.55";
    CIDR* bad_address = cidr_from_str(bad_address_str);
    ck_assert_int_eq(is_ipv4_link_local(bad_address), RESULT_FAILURE);
    cidr_free(bad_address);
}
END_TEST

START_TEST (test_is_ipv6)
{
    char* good_address_str = "2001:db8:1fe::49";
    CIDR* good_address = cidr_from_str(good_address_str);
    ck_assert_int_eq(is_ipv6(good_address), RESULT_SUCCESS);
    cidr_free(good_address);

    char* bad_address_str = "192.0.2.44";
    CIDR* bad_address = cidr_from_str(bad_address_str);
    ck_assert_int_eq(is_ipv6(bad_address), RESULT_FAILURE);
    cidr_free(bad_address);

}
END_TEST

START_TEST (test_is_ipv6_host)
{
    char* good_address_str_no_mask = "2001:db8:a::1";
    CIDR* good_address = cidr_from_str(good_address_str_no_mask);
    ck_assert_int_eq(is_ipv6_host(good_address), RESULT_SUCCESS);
    cidr_free(good_address);

    char* good_address_str_cidr = "2001:db8:b::100/64";
    CIDR* good_address_cidr = cidr_from_str(good_address_str_cidr);
    ck_assert_int_eq(is_ipv6_host(good_address_cidr), RESULT_SUCCESS);
    cidr_free(good_address_cidr);

    char* bad_address_str = "2001:db8:f::/48";
    CIDR* bad_address = cidr_from_str(bad_address_str);
    ck_assert_int_eq(is_ipv6_host(bad_address), RESULT_FAILURE);
    cidr_free(bad_address);
}
END_TEST

START_TEST (test_is_ipv6_net)
{
    char* good_address_str = "2001:db8::/32";
    CIDR* good_address = cidr_from_str(good_address_str);
    ck_assert_int_eq(is_ipv6_net(good_address), RESULT_SUCCESS);
    cidr_free(good_address);

    char* bad_address_str = "2001:db8:34::1/64";
    CIDR* bad_address = cidr_from_str(bad_address_str);
    ck_assert_int_eq(is_ipv6_net(bad_address), RESULT_FAILURE);
    cidr_free(bad_address);
}
END_TEST

START_TEST (test_is_ipv6_multicast)
{
    char* good_address_str = "ff02::6";
    CIDR* good_address = cidr_from_str(good_address_str);
    ck_assert_int_eq(is_ipv6_multicast(good_address), RESULT_SUCCESS);
    cidr_free(good_address);

    char* bad_address_str = "2001:db8::1";
    CIDR* bad_address = cidr_from_str(bad_address_str);
    ck_assert_int_eq(is_ipv6_multicast(bad_address), RESULT_FAILURE);
    cidr_free(bad_address);
}
END_TEST

START_TEST (test_is_ipv6_link_local)
{
    CIDR* address = NULL;

    char* good_address_str = "fe80::5ab0:35ff:fef2:9365";
    address = cidr_from_str(good_address_str);
    ck_assert_int_eq(is_ipv6_link_local(address), RESULT_SUCCESS);
    cidr_free(address);

    char* bad_address_str = "2001:db8::2";
    address = cidr_from_str(bad_address_str);
    ck_assert_int_eq(is_ipv6_link_local(address), RESULT_FAILURE);
    cidr_free(address);
}
END_TEST

START_TEST (test_is_valid_intf_address)
{
    char* good_address_str_v4 = "192.0.2.5/24";
    CIDR* good_address_v4 = cidr_from_str(good_address_str_v4);
    ck_assert_int_eq(is_valid_intf_address(good_address_v4, good_address_str_v4, NO_LOOPBACK), RESULT_SUCCESS);
    cidr_free(good_address_v4);

    char* good_address_str_v6 = "2001:db8:a:b::14/64";
    CIDR* good_address_v6 = cidr_from_str(good_address_str_v6);
    ck_assert_int_eq(is_valid_intf_address(good_address_v6, good_address_str_v6, NO_LOOPBACK), RESULT_SUCCESS);
    cidr_free(good_address_v6);
}
END_TEST

START_TEST (test_is_any_host)
{
    char* good_address_str_v4 = "192.0.2.1/25";
    CIDR* good_address_v4 = cidr_from_str(good_address_str_v4);
    ck_assert_int_eq(is_any_host(good_address_v4), RESULT_SUCCESS);
    cidr_free(good_address_v4);

    char* good_address_str_v6 = "2001:db8:aff::1/64";
    CIDR* good_address_v6 = cidr_from_str(good_address_str_v6);
    ck_assert_int_eq(is_any_host(good_address_v6), RESULT_SUCCESS);
    cidr_free(good_address_v6);

    char* bad_address_str_v4 = "192.0.2.0/24";
    CIDR* bad_address_v4 = cidr_from_str(bad_address_str_v4);
    ck_assert_int_eq(is_any_host(bad_address_v4), RESULT_FAILURE);
    cidr_free(bad_address_v4);

    char* bad_address_str_v6 = "2001:db8::/32";
    CIDR* bad_address_v6 = cidr_from_str(bad_address_str_v6);
    ck_assert_int_eq(is_any_host(bad_address_v6), RESULT_FAILURE);
    cidr_free(bad_address_v6);
}
END_TEST

START_TEST (test_is_any_net)
{
    char* good_address_str_v4 = "192.0.2.0/25";
    CIDR* good_address_v4 = cidr_from_str(good_address_str_v4);
    ck_assert_int_eq(is_any_net(good_address_v4), RESULT_SUCCESS);
    cidr_free(good_address_v4);

    char* good_address_str_v6 = "2001:db8:aff::/64";
    CIDR* good_address_v6 = cidr_from_str(good_address_str_v6);
    ck_assert_int_eq(is_any_net(good_address_v6), RESULT_SUCCESS);
    cidr_free(good_address_v6);

    char* bad_address_str_v4 = "192.0.2.33/24";
    CIDR* bad_address_v4 = cidr_from_str(bad_address_str_v4);
    ck_assert_int_eq(is_any_net(bad_address_v4), RESULT_FAILURE);
    cidr_free(bad_address_v4);

    char* bad_address_str_v6 = "2001:db8::1/32";
    CIDR* bad_address_v6 = cidr_from_str(bad_address_str_v6);
    ck_assert_int_eq(is_any_net(bad_address_v6), RESULT_FAILURE);
    cidr_free(bad_address_v6);
}
END_TEST


Suite *ipaddrcheck_suite(void)
{
    Suite *s = suite_create("ipaddrcheck");

    /* Core test case */
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_is_valid_address);
    tcase_add_test(tc_core, test_is_ipv4_cidr);
    tcase_add_test(tc_core, test_is_ipv4_single);
    tcase_add_test(tc_core, test_is_ipv6_cidr);
    tcase_add_test(tc_core, test_is_ipv6_single);
    tcase_add_test(tc_core, test_is_any_cidr);
    tcase_add_test(tc_core, test_is_any_single);
    tcase_add_test(tc_core, test_is_ipv4);
    tcase_add_test(tc_core, test_is_ipv4_host);
    tcase_add_test(tc_core, test_is_ipv4_net);
    tcase_add_test(tc_core, test_is_ipv4_broadcast);
    tcase_add_test(tc_core, test_is_ipv4_multicast);
    tcase_add_test(tc_core, test_is_ipv4_loopback);
    tcase_add_test(tc_core, test_is_ipv4_link_local);
    tcase_add_test(tc_core, test_is_ipv4_rfc1918);
    tcase_add_test(tc_core, test_is_ipv6);
    tcase_add_test(tc_core, test_is_ipv6_host);
    tcase_add_test(tc_core, test_is_ipv6_net);
    tcase_add_test(tc_core, test_is_ipv6_multicast);
    tcase_add_test(tc_core, test_is_ipv6_link_local);
    tcase_add_test(tc_core, test_is_valid_intf_address);
    tcase_add_test(tc_core, test_is_any_host);
    tcase_add_test(tc_core, test_is_any_net);

    suite_add_tcase(s, tc_core);

  return(s);
}

int main (void)
{
    int number_failed;
    Suite *s = ipaddrcheck_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free (sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

