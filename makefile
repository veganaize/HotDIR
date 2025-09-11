CC = cl.exe -nologo
CFLAGS = -WX -W4 -sdl -arch:IA32 -O2 -D_CRT_SECURE_NO_WARNINGS
LINK = link.exe -nologo
LDFLAGS = -subsystem:console,"5.01"

all : tests hd.exe

test : tests
	hd-tests.exe

tests : hd-tests.exe

hd-tests.exe : hd-tests.obj hd.obj
	$(LINK) $(LDFLAGS) hd-tests.obj hd.obj

hd-tests.obj : hd-tests.c hd.h
	$(CC) $(CFLAGS) -c hd-tests.c

hd.exe : hd.obj main.obj
	$(LINK) $(LDFLAGS) hd.obj main.obj

hd.obj : hd.c hd.h
	$(CC) $(CFLAGS) -c hd.c

main.obj : main.c hd.h
	$(CC) $(CFLAGS) -c main.c

clean :
	del hd-tests.obj hd-tests.exe hd.obj hd.exe main.obj
