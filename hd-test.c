#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "hd.h"


int
test__create_horizontal_line()
{
    /* Arrange */
    CONSOLE_SCREEN_BUFFER_INFO csbi = {
            80, 40,        /* buffer columns, rows */
            0, 0,          /* cursor column, row */
            14,            /* color attributes */
            0, 0, 79, 39,  /* coords left, top, right, bottom */
            0, 0           /* max window size */
    };
    size_t console_width = csbi.srWindow.Right + 1;
    char line[8192] = { 0 };
    size_t length = 0;

    /* Act */
    length = strlen(create_horizontal_line(line, csbi));

    /* Assert */
    if (length != console_width) {
        puts("LENGTH OF LINE SHOULD MATCH CONSOLE WIDTH");
        return 1;
    }

    return 0;
}


int
test__get_console_info()  // Windows specific
{
    /* Arrange */
    struct console_info console = {
            -1,          /* colors */
            -1,          /* width */
            -1           /* height */
    };

    /* Act */
    get_console_info(&console);

    /* Assert */
    if (! (console.colors > 0
            || console.width > 0
            || console.height > 0)) {
        puts("CONSOLE ATTRIBUTES SHOULD BE SET");
        return 1;
    }

    return 0;
}


//int
//test__create_footer()
//{
//    /* Arrange */
//    char footer_string[640];
//    short console_width = 80;
//    char root_path[] = "x:\\";
//    char search_drive = 'C';
//
//    /* Act */
//    create_footer(footer_string, console_width, root_path, search_drive);
//
//    /* Assert */
//    if (strcmp(footer_string,
//            "     0 files, totaling , consuming 0 bytes of disk space.\n"
//            " 0 bytes available on Drive C:           Volume label: C_DRIVE\n"
//            ) != 0) {
//        puts("STRING SHOULD MATCH");
//        return 1;
//    }
//
//    return 0;
//}


int
test__compact_size_with_suffix__returns_b()
{
    /* Arrange */
    int size = 1023;
    char string[16];

    /* Act */
    compact_size_with_suffix(size, string);

    /* Assert */
    if (strcmp(string, "1023 B") != 0) {
        puts("SHOULD HAVE \"B\" SUFFIX");
        return 1;
    }

    return 0;
}


int
main()
{
    test__compact_size_with_suffix__returns_b();
    test__create_horizontal_line();
    //test__create_footer();
    test__get_console_info();
}
