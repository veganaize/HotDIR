#ifndef HD_H
#define HD_H

#include <windows.h>

#define GREEN()        SetConsoleTextAttribute(g_hConsole, 0x02)
#define AQUA()         SetConsoleTextAttribute(g_hConsole, 0x03)
#define RED()          SetConsoleTextAttribute(g_hConsole, 0x04)
#define PURPLE()       SetConsoleTextAttribute(g_hConsole, 0x05)
#define YELLOW()       SetConsoleTextAttribute(g_hConsole, 0x06)
#define WHITE()        SetConsoleTextAttribute(g_hConsole, 0x07)
#define GRAY()         SetConsoleTextAttribute(g_hConsole, 0x08)
#define LIGHT_GREEN()  SetConsoleTextAttribute(g_hConsole, 0x0A)
#define LIGHT_AQUA()   SetConsoleTextAttribute(g_hConsole, 0x0B)
#define LIGHT_RED()    SetConsoleTextAttribute(g_hConsole, 0x0C)
#define LIGHT_PURPLE() SetConsoleTextAttribute(g_hConsole, 0x0D)
#define LIGHT_YELLOW() SetConsoleTextAttribute(g_hConsole, 0x0E)
#define BRIGHT_WHITE() SetConsoleTextAttribute(g_hConsole, 0x0F)


int build_initial_search_string(char *search_path, char *search_string);
//create_footer(char *footer_string, short console_width, char *root_path, char search_drive);
void create_horizontal_line(char * string, CONSOLE_SCREEN_BUFFER_INFO csbi);
void determine_size_suffix(int size_bytes, char *string, size_t string_size);
int display_footer();
int display_header(char *search_path, SHORT console_width);
int display_help();
int fixup_path(char *search_path);
CONSOLE_SCREEN_BUFFER_INFO get_console_info();
int process_cmdline_args(int argc, char *argv[], char search_drive, char *search_path, char *search_string);
int process_files(char *search_handle, char *search_path);
int restore_console();

#endif  /* HD_H */
