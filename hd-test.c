#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "hd.h"


int test__create_horizontal_line()
{
    /* Arrange */
    char string[8192] = { '\0' };
    CONSOLE_SCREEN_BUFFER_INFO csbi = {
            80, 40,       /* COORD */
            0, 0,         /* COORD */
            14,           /* WORD */
            0, 0, 79, 39  /* SMALL_RECT */
            //100, 100      /* COORD */
    };
    size_t string_length = 0;

    /* Act */
    create_horizontal_line(string, csbi);

    /* Assert */
    string_length = strlen(string);
    if (string_length != csbi.srWindow.Right + 2) {
        puts("LENGTH SHOULD MATCH");
        return 1;
    }

    return 0;
}


//int test__get_console_info()  // Windows specific
//{
//    /* Arrange */
//    CONSOLE_SCREEN_BUFFER_INFO csbi;
//
//    /* Act */
//    csbi = get_console_info();
//
//    /* Assert */
//    printf("%d\n", csbi.wAttributes);
//    printf("%d\n", csbi.srWindow.Right);
//    printf("%d\n", csbi.srWindow.Bottom - csbi.srWindow.Top);
//
//    return 0;
//}


//int test__create_footer()
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


int test__determine_size_suffix__returns_b()
{
    /* Arrange */
    int size = 1023;
    char string[16];

    /* Act */
    determine_size_suffix(size, string, sizeof string);

    /* Assert */
    if (strcmp(string, "B") != 0) {
        puts("SHOULD HAVE \"B\" SUFFIX");
        return 1;
    }

    return 0;
}


int main()
{
    test__determine_size_suffix__returns_b();
    test__create_horizontal_line();
    //test__create_footer();
    //test__get_console_info();
}


//int test__something()
//{
//    /* Arrange */
//
//    /* Act */
//
//    /* Assert */
//
//    return 0;
//}
