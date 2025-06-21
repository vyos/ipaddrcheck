AM_CFLAGS = --pedantic -Wall -Werror -std=c99 -O2
AM_CPPFLAGS = $(PCRE2_CFLAGS)

ipaddrcheck_SOURCES = ipaddrcheck.c ipaddrcheck_functions.c
ipaddrcheck_LDADD = -lcidr $(PCRE2_LIBS)

bin_PROGRAMS = ipaddrcheck
