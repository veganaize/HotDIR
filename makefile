CFLAGS =-WX -W4 -sdl -arch:IA32 -O2 -D_CRT_SECURE_NO_WARNINGS
LINK = link.exe
LDFLAGS = -subsystem:console,"5.01"

all : hd-test.exe hd.exe

test : hd-test.exe

hd-test.exe : hd-test.obj hd.obj
	$(LINK) $(LDFLAGS) $**

hd-test.obj : hd-test.c hd.h
	$(CC) $(CFLAGS) -c hd-test.c

hd.exe : hd.obj main.obj
	$(LINK) $(LDFLAGS) $**

hd.obj : hd.c hd.h
	$(CC) $(CFLAGS) -c hd.c

main.obj : main.c hd.h
	$(CC) $(CFLAGS) -c main.c

clean :
	del hd-test.obj hd-test.exe hd.obj hd.exe main.obj
