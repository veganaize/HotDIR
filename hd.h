#ifndef HD_H
#define HD_H

#include <windows.h>

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define AQUA 3
#define RED 4
#define PURPLE 5
#define YELLOW 6
#define WHITE 7
#define GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_AQUA 11
#define LIGHT_RED 12
#define LIGHT_PURPLE 13
#define LIGHT_YELLOW 14
#define BRIGHT_WHITE 15

#define FG_GREEN()        SetConsoleTextAttribute(g_hConsole, 0x02)
#define FG_AQUA()         SetConsoleTextAttribute(g_hConsole, 0x03)
#define FG_RED()          SetConsoleTextAttribute(g_hConsole, 0x04)
#define FG_PURPLE()       SetConsoleTextAttribute(g_hConsole, 0x05)
#define FG_YELLOW()       SetConsoleTextAttribute(g_hConsole, 0x06)
#define FG_WHITE()        SetConsoleTextAttribute(g_hConsole, 0x07)
#define FG_GRAY()         SetConsoleTextAttribute(g_hConsole, 0x08)
#define FG_LIGHT_GREEN()  SetConsoleTextAttribute(g_hConsole, 0x0A)
#define FG_LIGHT_AQUA()   SetConsoleTextAttribute(g_hConsole, 0x0B)
#define FG_LIGHT_RED()    SetConsoleTextAttribute(g_hConsole, 0x0C)
#define FG_LIGHT_PURPLE() SetConsoleTextAttribute(g_hConsole, 0x0D)
#define FG_LIGHT_YELLOW() SetConsoleTextAttribute(g_hConsole, 0x0E)
#define FG_BRIGHT_WHITE() SetConsoleTextAttribute(g_hConsole, 0x0F)


struct console_info {
        int colors;
        int width;
        int height;
};


int build_initial_search_string(char * search_path, char * search_string);
//create_footer(char *footer_string, short console_width, char *root_path, char search_drive);
char * create_horizontal_line(char *, CONSOLE_SCREEN_BUFFER_INFO);
char * compact_size_with_suffix(int, char *);
int display_footer();
int display_header(char * search_path);
int display_help();
int fixup_path(char * search_path);
struct console_info * get_console_info();
int process_cmdline_args(int argc, char * argv[], char search_drive, char * search_path, char * search_string);
int process_files(char * search_handle, char * search_path);
int restore_console();

#endif  /* HD_H */
