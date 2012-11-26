#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.57)
AC_INIT(Modigliani, 2.0 , ali@neishabouri.net)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([modigliani.lsm])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_LEX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

AC_LANG(C++)

# Checks for libraries.
AC_CHECK_LIB([lua],[main], [HAVE_LUA=1], [AC_MSG_ERROR([Lua is not installed.])])
AC_CHECK_LIB([m], [sqrt],[HAVE_LIBM=1], AC_MSG_ERROR([libm not found.]))
AC_CHECK_LIB([plplotd], [c_plwid],[HAVE_PLPLOT="yes"])
if test "x$host" = xi486-mingw32; then
	AC_CHECK_LIB([jsoncpp], [main],[HAVE_JSONCPP=1],[AC_MSG_ERROR([libjson missing])])
	AC_CHECK_LIB([boost_system-mt], [main],[HAVE_BOOSTSYSTEM=1],[AC_MSG_ERROR([libboost_system missing])])
	AC_CHECK_LIB([boost_filesystem-mt], [main],[HAVE_BOOSTFILESYSTEM=1],[AC_MSG_ERROR([libboost_filesystem missing])])
elif test 1; then
	AC_CHECK_LIB([json], [main],[HAVE_JSON=1],[AC_MSG_ERROR([libjson missing])])
	AC_CHECK_LIB([boost_filesystem], [main],[HAVE_BOOSTFILESYSTEM=1],[AC_MSG_ERROR([libboost_filesystem missing])])
	AC_CHECK_LIB([boost_system], [main],[HAVE_BOOSTSYSTEM=1],[AC_MSG_ERROR([libboost_system missing])])
fi

# Checks for header files.
AC_PATH_X
AC_FUNC_ALLOCA
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_REQUIRE_CPP
AC_CHECK_HEADER([tnt/tnt.h],[],[AC_MSG_ERROR([TNT is not installed.])])


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

# Are we cross compiling?
AM_CONDITIONAL([MINGW], [test "x$host" = "xi486-mingw32"])

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

dnl Make sure we register this option with Automake, so we know whether to
dnl descend into ctempl ate for more configuration or not
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
                 modigliani/Makefile
                 modigliani/auxsrc/Makefile
                 modigliani/modigliani_core/Makefile
                 modigliani/modigliani_base/Makefile])

AC_OUTPUT
