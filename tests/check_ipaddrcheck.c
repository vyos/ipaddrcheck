#include <check.h>
#include "../src/iptest.h"

START_TEST (test_has_mask)
{
    char* good_address_str_cidr = "192.0.2.1/25";
    ck_assert_int_eq(has_mask(good_address_str_cidr), RESULT_SUCCESS);

    char* good_address_str_decimal = "192.0.2.1/255.255.0.0";
    ck_assert_int_eq(has_mask(good_address_str_decimal), RESULT_SUCCESS);

    char* bad_address_str = "192.0.2.1";
    ck_assert_int_eq(has_mask(bad_address_str), RESULT_FAILURE);
}
END_TEST

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
    char* good_address_str = "192.0.2.1/21";
    ck_assert_int_eq(is_ipv4_cidr(good_address_str), RESULT_SUCCESS);

    char* address_str_no_mask = "192.0.2.1";
    ck_assert_int_eq(is_ipv4_cidr(address_str_no_mask), RESULT_FAILURE);

    char* address_str_decimal_mask = "192.0.2.1/255.255.255.0";
    ck_assert_int_eq(is_ipv4_cidr(address_str_decimal_mask), RESULT_FAILURE);
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


Suite *ipaddrcheck_suite(void)
{
    Suite *s = suite_create("ipaddrcheck");

    /* Core test case */
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_has_mask);
    tcase_add_test(tc_core, test_is_valid_address);
    tcase_add_test(tc_core, test_is_ipv4_cidr);
    tcase_add_test(tc_core, test_is_ipv4);
    tcase_add_test(tc_core, test_is_ipv4_host);
    tcase_add_test(tc_core, test_is_ipv4_net);
    tcase_add_test(tc_core, test_is_ipv4_broadcast);
    tcase_add_test(tc_core, test_is_ipv4_multicast);
    tcase_add_test(tc_core, test_is_ipv4_loopback);
    tcase_add_test(tc_core, test_is_ipv4_link_local);
    tcase_add_test(tc_core, test_is_ipv4_rfc1918);

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

