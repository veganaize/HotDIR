#include <windows.h>
#include "hd.h"


int main(int argc, char* argv[])
{
    char search_path[MAX_PATH] = { 0 };
    char search_string[MAX_PATH] = { 0 };
    HANDLE search_handle = 0;
    struct console_info console;
    WORD original_colors;

    get_console_info(&console);
    original_colors = console.colors;

    build_initial_search_string(search_path, search_string);
    process_cmdline_args(argc,
                         argv,
                         'C',  /* search_drive */
                         search_path,
                         search_string);
    display_header(search_path, console.width);
    fixup_path(search_path);
    process_files(search_handle, search_path);

    display_footer();
    restore_console(original_colors);
    return 0;
}
