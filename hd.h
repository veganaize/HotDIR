#ifndef HD_H
#define HD_H

#include <windows.h>

int restore_console();
int display_footer();
int build_initial_search_string(char *search_path, char *search_string);
int process_files(char *search_handle, char *search_path);
int fixup_path(char *search_path);
int display_header(char *search_path, SHORT console_width);
CONSOLE_SCREEN_BUFFER_INFO get_console_info();
int display_help();
int process_cmdline_args(int argc, char *argv[], char search_drive, char *search_path, char *search_string);
void determine_size_suffix(int size_bytes, char *string, size_t string_size);
create_footer(char *footer_string, short console_width, char *root_path, char search_drive);
void create_horizontal_line(char * string, CONSOLE_SCREEN_BUFFER_INFO csbi);


#endif  /* HD_H */
