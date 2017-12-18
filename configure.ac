#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([tomegus], [0.0.1], [go.wigust@gmail.com])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AM_INIT_AUTOMAKE
dnl AC_CONFIG_MACRO_DIRS([src/m4])

dnl Check for SDL
SDL_VERSION=2.0.0
AM_PATH_SDL2($SDL_VERSION, :,
AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!]))
AM_CFLAGS="$AM_CFLAGS $SDL_CFLAGS"
AM_LDFLAGS="$AM_LDFLAGS $SDL_LIBS"

# Check for Guile
PKG_CHECK_MODULES([GUILE], [guile-2.2])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
