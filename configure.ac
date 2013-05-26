AC_INIT([iptest], [0.1], [daniil@baturin.org])

AC_PROG_CC

AC_CHECK_HEADER(pcre.h)
AC_CHECK_HEADER(libcidr.h)

AM_INIT_AUTOMAKE([gnu no-dist-gzip dist-bzip2 subdir-objects])
AC_PREFIX_DEFAULT([/usr])

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])
AC_CONFIG_HEADERS([src/config.h])

PKG_CHECK_MODULES([CHECK], [check >= 0.9.4])

AC_OUTPUT
