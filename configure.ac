AC_INIT([ipaddrcheck], [1.0], [maintainers@vyos.net], [ipaddrcheck], [https://github.com/vyos/ipaddrcheck])
AC_COPYRIGHT([Copyright (c) 2018 VyOS maintainers and contributors.])

#AC_PROG_CC
AM_PROG_CC_C_O

AC_CHECK_HEADER([pcre.h], [], [AC_MSG_FAILURE([pcre.h is not found.])])
AC_CHECK_HEADER([libcidr.h], [], [AC_MSG_FAILURE([libcidr.h is not found.])])

AM_INIT_AUTOMAKE([gnu no-dist-gzip dist-bzip2 subdir-objects])
AC_PREFIX_DEFAULT([/usr])

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile man/Makefile])
AC_CONFIG_HEADERS([src/config.h])

PKG_CHECK_MODULES([CHECK], [check >= 0.9.4])

AC_OUTPUT
