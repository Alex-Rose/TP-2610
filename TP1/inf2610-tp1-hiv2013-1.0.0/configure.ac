AC_INIT([INF2610-TP1-Hiv2013], 1.0.0)
AC_CONFIG_SRCDIR([src/processeur.cpp])
AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE([-Wall])
AM_INIT_AUTOMAKE([color-tests])
#AC_CONFIG_MACRO_DIR([m4])

LT_INIT

AC_CHECK_HEADERS(sys/types.h unistd.h fcntl.h strings.h pthread.h time.h errno.h stdarg.h limits.h signal.h stdlib.h)
AC_CHECK_HEADERS(inttypes.h math.h)
AC_CHECK_LIB(pthread, pthread_create)
AC_CHECK_LIB(m, pow)
AC_CHECK_LIB(stdc++, fclose)

# Fedora has no pkg-config for tbb
#PKG_CHECK_MODULES(TBB, [tbb])
AC_MSG_CHECKING(whether to enable debug mode)
debug_default="no"
AC_ARG_ENABLE(debug,
        AS_HELP_STRING([--enable-debug],[turn on debug mode [[default=no]]])
        , , enable_debug=$debug_default)
if test "$enable_debug" = "yes"; then
    AC_MSG_RESULT(yes)
    CFLAGS="-Wall -g -O0 -fno-inline"
    AC_DEFINE([DEBUG],[],[Debug])
else
    AC_MSG_RESULT(no)
    CFLAGS="-Wall -O3 -fomit-frame-pointer"
fi

AC_OPENMP

# be silent by default
AM_SILENT_RULES([yes])

AC_PROG_LIBTOOL
AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O
AC_CONFIG_FILES([Makefile
    src/Makefile
    tests/Makefile])
AC_OUTPUT
