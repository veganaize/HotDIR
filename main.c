#include <windows.h>
#include "hd.h"


int main(int argc, char* argv[])
{
    char   search_path[MAX_PATH];
    char   search_string[MAX_PATH];

    CONSOLE_SCREEN_BUFFER_INFO screen_info = get_console_info();
    WORD original_console_colors = screen_info.wAttributes;
    SHORT console_width = screen_info.srWindow.Right + 1;
    SHORT console_height = screen_info.srWindow.Bottom - screen_info.srWindow.Top;


    build_initial_search_string(search_path, search_string);
    process_cmdline_args(argc,
                         argv,
                         'C',  /* search_drive */
                         search_path,
                         search_string);
    display_header(search_path, console_width);
    fixup_path(search_path);
    process_files(search_handle, search_path);

    FindClose(search_handle);

    display_footer();
    restore_console(original_console_colors);
    return 0;
}
