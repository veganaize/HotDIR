CL = cl.exe -nologo -J -WX -W4 -sdl -arch:IA32 -O2 -D_CRT_SECURE_NO_WARNINGS
LINK = link.exe -nologo -subsystem:console,"5.01"

GCC = gcc
CFLAGS = -std=c99 -O2 -fdiagnostics-color -mconsole -Wall -Wextra -pedantic-errors -Wfatal-errors -Wshadow -Wconversion -Wsign-conversion -Wno-missing-braces
LDFLAGS = -mconsole


all : tests hd.exe

test : tests
	hd-tests.exe

tests : hd-tests.exe

hd-tests.exe : hd-tests.obj hd.obj
	@$(LINK) hd-tests.obj hd.obj 2>NUL \
	|| ($(GCC) $(LDFLAGS) -o hd-tests.exe hd-tests.obj hd.obj; rm NUL)

hd-tests.obj : hd-tests.c hd.h
	@$(CL) -c hd-tests.c 2>NUL \
	|| ($(GCC) $(CFLAGS) -c hd-tests.c -o hd-tests.obj; rm NUL)

hd.exe : hd.obj main.obj
	@$(LINK) hd.obj main.obj 2>NUL \
	|| ($(GCC) $(LDFLAGS) -o hd.exe hd.obj main.obj; rm NUL)

hd.obj : hd.c hd.h
	@$(CL) -c hd.c 2>NUL \
	|| ($(GCC) $(CFLAGS) -c hd.c -o hd.obj; rm NUL)

main.obj : main.c hd.h
	@$(CL) -c main.c 2>NUL \
	|| ($(GCC) $(CFLAGS) -c main.c -o main.obj; rm NUL)

clean :
	@del hd-tests.obj hd-tests.exe hd.obj hd.exe main.obj 2>NUL \
	|| rm -f hd-tests.obj hd-tests.exe hd.obj hd.exe main.obj NUL
