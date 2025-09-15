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
    size_t console_width = (size_t) csbi.srWindow.Right + 1;
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
test__get_console_info__sets_attributes()
{
    /* Arrange */
    struct console_info console = { -1, -1, -1 };

    /* Act */
    get_console_info(&console);

    /* Assert */
    if (console.colors < 0 || console.width < 1 || console.height < 0) {
        printf("RESULT: %d, %d, %d\n",
               console.colors,
               console.width,
               console.height);
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
test__compact_size_with_suffix__is_kilobytes()
{
    /* Arrange */
    int size = 1048575;
    char result[16] = { 0 };
    const char * expected = "1023 KB";

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
test__compact_size_with_suffix__is_megabytes()
{
    /* Arrange */
    int size = 1048576;
    char result[16] = { 0 };
    const char * expected = "1 MB";

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
test__compact_size_with_suffix__is_not_megabytes()
{
    /* Arrange */
    int size = 1048575;
    char result[16] = { 0 };
    const char * expected = "1023 KB";

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
test__compact_size_with_suffix__is_gigabytes()
{
    /* Arrange */
    long long size = 1073741824;
    char result[16] = { 0 };
    const char * expected = "1 GB";

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
test__compact_size_with_suffix__is_not_gigabytes()
{
    /* Arrange */
    long long size = 1073741823;
    char result[16] = { 0 };
    const char * expected = "1023 MB";

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
test__compact_size_with_suffix__is_terabytes()
{
    /* Arrange */
    long long size = 1099511627776;
    char result[16] = { 0 };
    const char * expected = "1 TB";

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
test__compact_size_with_suffix__works_with_ntfs_volume_limit()
{
    /* Arrange */
    long long size = 281474976710656;
    char result[16] = { 0 };
    const char * expected = "256 TB";

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
    failed_count += test__compact_size_with_suffix__is_kilobytes();
    failed_count += test__compact_size_with_suffix__is_megabytes();
    failed_count += test__compact_size_with_suffix__is_not_megabytes();
    failed_count += test__compact_size_with_suffix__is_gigabytes();
    failed_count += test__compact_size_with_suffix__is_not_gigabytes();
    failed_count += test__compact_size_with_suffix__is_terabytes();
    failed_count += test__compact_size_with_suffix__works_with_ntfs_volume_limit();
    failed_count += test__create_horizontal_line__is_console_width();
    //failed_count += test__create_footer();
    failed_count += test__get_console_info__sets_attributes();

    printf("\n%d failing test%c\n", failed_count, failed_count != 1 ? 's' : '\0');

    return failed_count;
}
