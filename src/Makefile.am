lib_LTLIBRARIES = libgame.la
libgame_la_SOURCES = game.c game.h

bin_PROGRAMS = tomegus
tomegus_SOURCES = main.c pt_console.c game.c level.c fov.c
tomegus_LDADD = libgame.la
tomegus_CPPFLAGS = @SDL_CFLAGS@
tomegus_LDFLAGS = @SDL_LIBS@
ACLOCAL_AMFLAGS = --install -I m4
