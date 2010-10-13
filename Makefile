
all: objtest

objtest: objtest.c bevelcube.c crater4.c crater4.h
	gcc -o objtest objtest.c bevelcube.c crater4.c -lglut

circletess: circletess.c crater4.c crater4.h
	gcc -o circletess circletess.c crater4.c -lglut -lm

shaderholes: shaderholes.c 
	gcc -o shaderholes shaderholes.c  -lglut -lm

clip: clip.c
	gcc -o clip clip.c -lglut -lm
