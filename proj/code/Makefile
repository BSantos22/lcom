# Makefile for the proj

COMPILER_TYPE= gnu

CC=gcc

PROG=	proj
SRCS=	proj.c   game.c   timer.c   kbd.c	vbe.c   video_gr.c   xpm.c   sprite.c	mouse.c		vector.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-llm -ldriver -lsys	liblm.a -lm

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>