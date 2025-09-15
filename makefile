CL = cl.exe -nologo -WX -W4 -sdl -arch:IA32 -O2 -D_CRT_SECURE_NO_WARNINGS
LINK = link.exe -nologo -subsystem:console,"5.01"
GCC = gcc -std=c99 -O2 -fdiagnostics-color -mconsole -Wall -Wextra -pedantic-errors -Wfatal-errors -Wshadow -Wconversion -Wsign-conversion -Wno-missing-braces
LD = gcc -mconsole

all : tests hd.exe

test : tests
	hd-tests.exe

tests : hd-tests.exe

hd-tests.exe : hd-tests.obj hd.obj
	@$(LINK) hd-tests.obj hd.obj \
	2>/dev/null || $(LD) $(LDFLAGS) -o hd-tests.exe hd-tests.obj hd.obj

hd-tests.obj : hd-tests.c hd.h
	@$(CL) -c hd-tests.c \
	2>/dev/null || $(GCC) $(CFLAGS) -c hd-tests.c -o hd-tests.obj

hd.exe : hd.obj main.obj
	@$(LINK) hd.obj main.obj \
	2>/dev/null || $(LD) $(LDFLAGS) -o hd.exe hd.obj main.obj

hd.obj : hd.c hd.h
	@$(CL) -c hd.c \
	2>/dev/null || $(GCC) $(CFLAGS) -c hd.c -o hd.obj

main.obj : main.c hd.h
	@$(CL) -c main.c \
	2>/dev/null || $(GCC) $(CFLAGS) -c main.c -o main.obj

clean :
	del hd-tests.obj hd-tests.exe hd.obj hd.exe main.obj \
	2>/dev/null || rm -f hd-tests.obj hd-tests.exe hd.obj hd.exe main.obj
