TESTS = check_ipaddrcheck integration_tests.sh

TESTS_ENVIRONMENT = top_srcdir=$(top_srcdir) PATH=.:$(top_srcdir)/src:$$PATH

check_PROGRAMS = check_ipaddrcheck
check_ipaddrcheck_SOURCES = check_ipaddrcheck.c ../src/ipaddrcheck_functions.c
check_ipaddrcheck_CFLAGS = @CHECK_CFLAGS@
check_ipaddrcheck_LDADD = -lcidr -lpcre @CHECK_LIBS@
