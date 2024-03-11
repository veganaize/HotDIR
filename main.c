
int main(int argc, char* argv[])
{
    //SHORT  console_width;
    char   search_path[MAX_PATH];
    char   search_string[MAX_PATH];

    //struct mystruct_t {
    //    int i,
    //    long l,
    //} strA;

    original_attributes = screen_info_t.wAttributes; /* Save console info */
    console_width = screen_info_t.srWindow.Right;    /* Get console width */
    console_height = screen_info_t.srWindow.Bottom
                     - screen_info_t.srWindow.Top;


    build_initial_search_string(search_path, search_string);
    get_console_info();
    process_cmdline_args(argc,
                         argv,
                         search_drive,
                         search_path,
                         search_string);
    display_header(search_path, console_width);
    fixup_path(search_path);
    process_files(search_handle, search_path);

    FindClose(search_handle);

    display_footer();
    restore_console();
    return 0;
}
