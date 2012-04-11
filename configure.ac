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

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lGL':
#AC_CHECK_LIB([GL], [main])
# FIXME: Replace `main' with a function in `-lGLU':
#AC_CHECK_LIB([GLU], [main])
# FIXME: Replace `main' with a function in `-lX11':
#AC_CHECK_LIB([X11], [main])
# FIXME: Replace `main' with a function in `-lXext':
#AC_CHECK_LIB([Xext], [main])
# FIXME: Replace `main' with a function in `-lm':
#AC_CHECK_LIB([m], [main])

# Checks for header files.
AC_PATH_X
AC_FUNC_ALLOCA
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([OS.h fcntl.h fenv.h float.h inttypes.h climits malloc.h memory.h nl_types.h cstddef cstdint cstdlib map cmath cstring sys/file.h sys/param.h ctime unistd.h values.h wchar.h wctype.h])

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
AM_CONDITIONAL(MINGW, test "$host" = i486-mingw32)

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_MEMCMP
if test "$MINGW" = yes; then
AC_FUNC_MALLOC
AC_FUNC_REALLOC
fi
AC_FUNC_SELECT_ARGTYPES
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_FUNC_STRFTIME
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([alarm clock_gettime fesetround floor ftime getcwd gettimeofday memchr memmove memset pathconf pow regcomp rint select setlocale sqrt strchr strcspn strerror strpbrk strrchr strspn strstr strtol strtoul uname])

dnl CXXFLAGS="-g -Wall -W -Wundef -Wshadow -ansi -pedantic -std=c++0x -Wno-vla"
CXXFLAGS="-Ofast -march=native -mtune=native -ffast-math -ansi -pedantic -Wshadow -Wno-vla -malign-double -std=c++0x"


# This adds the option of compiling without using the PLPlot library,
# which has proved troublesome for compilation on windows
AC_ARG_ENABLE(plplot,
  [ --disable-plplot   Disable compilation with PlPlot],
  [case "${enableval}" in
     yes | no ) WITH_PLPLOT="${enableval}" ;;
     *) AC_MSG_ERROR(bad value ${enableval} for --disable-plplot) ;;
   esac],
  [WITH_PLPLOT="yes"]
)

dnl Make sure we register this option with Automake, so we know whether to
dnl descend into ctemplate for more configuration or not
AM_CONDITIONAL([WITH_PLPLOT], [test "x$WITH_PLPLOT" = "xyes"])

# Define CTEMPLATE in config.h if we're going to compile against it
if test "x$WITH_PLPLOT" = "xyes"; then
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
                 modigliani/nt3dsrc/Makefile
                 modigliani/ntbpsrc/Makefile
                 modigliani/ntgsrc/Makefile
                 modigliani/ntsrc/Makefile
                 modigliani/tnt/Makefile])

AC_OUTPUT