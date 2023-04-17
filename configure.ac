#
# Gurobify: Gurobify provides an interface to Gurobi from GAP.
#
# This file is part of the build system of a GAP kernel extension.
# Requires GNU autoconf, GNU automake and GNU libtool.
#

dnl ##
dnl ## Setup autoconf
dnl ##
AC_PREREQ([2.68])
AC_INIT([Gurobify], m4_esyscmd([tr -d '\n' < VERSION]))
AC_CONFIG_SRCDIR([src/Gurobify.c])
AC_CONFIG_HEADER([src/pkgconfig.h:cnf/pkgconfig.h.in])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([cnf])
m4_include([m4/find_gap.m4])

dnl ##
dnl ## Get canonical host info
dnl ##
AC_CANONICAL_HOST

dnl ##
dnl ## Setup automake
dnl ##
AM_INIT_AUTOMAKE([1.11 -Wall foreign subdir-objects])
AM_SILENT_RULES([yes])
AM_PROG_AR

dnl ##
dnl ## Set the language
dnl ##
AC_PROG_CC
AC_LANG([C])

dnl ##
dnl ## Setup libtool (for building the GAP kernel extension)
dnl ##
LT_PREREQ([2.4.2])
LT_INIT([disable-static dlopen win32-dll])

dnl ##
dnl ## Locate the GAP root dir
dnl ##
FIND_GAP

dnl ##
dnl ## Detect pointer size to distinguish 32 and 64 bit builds
dnl ##
AC_CHECK_SIZEOF([void **])

AC_ARG_WITH([gurobi],
    [AC_HELP_STRING([--with-gurobi=<path>], [specify root of Gurobi installation])],
    [GUROBI="$with_gurobi"],
)
GUROBI_CPPFLAGS="-I$GUROBI/include"
GUROBI_LDPATH="$GUROBI/lib"
GUROBI_LDFLAGS="-L$GUROBI_LDPATH"
AC_CONFIG_COMMANDS([GUROBIVERSION], [ls $GUROBI_LDPATH/*.so | grep "libgurobi" | xargs basename | tr -d 'libgurobi.so' > GUROBIVERSION], [GUROBI_LDPATH=$GUROBI_LDPATH])

GUROBI_VERSION=m4_esyscmd([tr -d '\n' < GUROBIVERSION])

AC_SUBST(GUROBI_VERSION, $GUROBI_VERSION)
AC_SUBST(GUROBI_LDPATH)
AC_SUBST(GUROBI_LDFLAGS)
AC_SUBST(GUROBI_CPPFLAGS)

dnl ##
dnl ## Detect Windows resp. Cygwin
dnl ##
case $host_os in
  *cygwin* ) CYGWIN=yes;;
         * ) CYGWIN=no;;
esac
AM_CONDITIONAL([SYS_IS_CYGWIN], [test "$CYGWIN" = "yes"])

dnl ##
dnl ## Rebuild configure if VERSION file changes
dnl ##
AC_SUBST([CONFIG_STATUS_DEPENDENCIES], ['$(top_srcdir)/VERSION'])

dnl ##
dnl ## Output everything
dnl ##
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
