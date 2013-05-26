TESTS = check_ipaddrcheck
check_PROGRAMS = check_ipaddrcheck
check_ipaddrcheck_SOURCES = check_ipaddrcheck.c $(top_builddir)/src/iptest.h
check_ipaddrcheck_CFLAGS = @CHECK_CFLAGS@
check_ipaddrcheck_LDADD = -lcidr -lpcre @CHECK_LIBS@
