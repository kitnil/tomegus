#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([tomegus], [0.0.1], [go.wigust@gmail.com])
AC_CONFIG_HEADERS([config.h])
AM_PROG_AR

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.
AM_INIT_AUTOMAKE([-Wall -Werror])

dnl AC_CONFIG_MACRO_DIRS([src/m4])

PKG_CHECK_MODULES([CHECK], [check >= 0.9.6])
AM_PROG_CC_C_O

dnl Check for SDL
SDL_VERSION=2.0.0
AM_PATH_SDL2($SDL_VERSION, :,
AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!]))
AM_CFLAGS="$AM_CFLAGS $SDL_CFLAGS"
AM_LDFLAGS="$AM_LDFLAGS $SDL_LIBS"

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])

AC_OUTPUT
