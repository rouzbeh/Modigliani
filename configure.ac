#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.57)
AC_INIT(Modigliani, 2.0.10 , ali@neishabouri.net)
AM_INIT_AUTOMAKE([foreign])
LT_INIT
AC_CONFIG_SRCDIR([modigliani.lsm])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_LEX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

AC_LANG(C++)

# Checks for libraries.
AX_BOOST_BASE([1.55],, [AC_MSG_ERROR([Modigliani needs Boost, but it was not found in your system])])
AX_BOOST_RANDOM
AX_BOOST_FILESYSTEM
AX_BOOST_PROGRAM_OPTIONS
AX_BOOST_SYSTEM
PKG_CHECK_MODULES([LUA], [lua >= 5.2], [], [PKG_CHECK_MODULES([LUA],[lua5.2])])
PKG_CHECK_MODULES([plplotd],[plplotd],[HAVE_PLPLOT="yes"],[HAVE_PLPLOT="no"])

# Checks for header files.
AC_PATH_X
AC_FUNC_ALLOCA
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_REQUIRE_CPP

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM
AC_C_VOLATILE
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_MEMCMP
AC_FUNC_SELECT_ARGTYPES
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([alarm fesetround floor ftime getcwd gettimeofday memchr memmove memset pathconf pow regcomp rint select setlocale sqrt strchr strcspn strerror strpbrk strrchr strspn strstr strtol strtoul uname])

# Make sure we register this option with Automake, so we know whether to
# descend into ctempl ate for more configuration or not
AM_CONDITIONAL([WITH_PLPLOT], [test "x$HAVE_PLPLOT" = "xyes"])

# Define CTEMPLATE in config.h if we're going to compile against it
if test "x$HAVE_PLPLOT" = "xyes"; then
    AC_DEFINE([PLPLOT], [], ["build using PLPlot library"])
    AC_MSG_NOTICE([PLPlot will be used])
else
    AC_MSG_NOTICE([PLPlot will not be used])
fi

all_includes="$all_includes $USER_INCLUDES"
AC_SUBST(all_includes)

all_libraries="$all_libraries $USER_LDFLAGS"
AC_SUBST(all_libraries)

AC_CONFIG_FILES([Makefile
                 test/Makefile
                 src/Makefile
                 src/auxsrc/Makefile
                 src/modigliani/Makefile])

AC_OUTPUT
