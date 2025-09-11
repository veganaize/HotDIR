#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "hd.h"


int
test__create_horizontal_line__is_console_width()
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
    create_horizontal_line(line, csbi);
    length = strlen(line);

    /* Assert */
    if (length != console_width) {
        puts("LENGTH OF LINE SHOULD MATCH CONSOLE WIDTH");
        return 1;
    }

    return 0;
}


int
test__get_console_info__has_no_zeros()
{
    /* Arrange */
    struct console_info console = { -1, -1, -1 };

    /* Act */
    get_console_info(&console);

    /* Assert */
    if (console.colors < 1 || console.width < 1 || console.height < 1) {
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
test__compact_size_with_suffix__is_bytes()
{
    /* Arrange */
    int size = 1023;
    char result[16] = { 0 };
    const char * expected = "1023 B";

    /* Act */
    compact_size_with_suffix(size, result);

    /* Assert */
    if (strcmp(result, expected) != 0) {
        printf("\nRESULT:   %s\nEXPECTED: %s\n", result, expected);
        return 1;
    }

    return 0;
}


int
main()
{
    int failed_count = 0;

    failed_count += test__compact_size_with_suffix__is_bytes();
    failed_count += test__create_horizontal_line__is_console_width();
    //failed_count += test__create_footer();
    failed_count += test__get_console_info__has_no_zeros();

    printf("\n%d failing test%c\n", failed_count, failed_count != 1 ? 's' : '\0');

    return failed_count;
}
