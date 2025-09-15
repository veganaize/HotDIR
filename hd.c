/* HotDIR (clone) - Public Domain by veganaiZe
 *
 * Colorful file/folder listing at the command prompt.
 *
 * NO WARRANTY WHATSOEVER!
 *
 */
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "hd.h"

#define VERSION_STRING "0.6.4 (pre-release)"  /* <-- Increment just before release. */


HANDLE g_hConsole;
SHORT  g_console_width;
WORD   g_original_attributes;
char   g_search_drive   = 'C';     /* Pre-load with C: drive */
//char   g_search_string[MAX_PATH];
//char   g_search_path[MAX_PATH];

/* Console variables: */
CONSOLE_SCREEN_BUFFER_INFO
       g_screen_info_t;
WIN32_FIND_DATA
       g_file_data_t;
INT    g_console_height = 24;
//HANDLE g_search_handle;
SHORT  g_line_count = 3;  /* Preload w/ num lines in header */
DWORD  g_dwAttrib;

/* File variables */
char * g_file_ext       = NULL;    /* Current file's extension */
double  g_file_size      = -1.0;    /* Current file's size */
double  g_total_size     = 0.0;     /* Total of all listed file sizes */
double  g_total_consumed = 0.0;     /* Total actual/compressed disk usage */
int    g_file_counter   = 0;       /* Total listed file count */

/* Args to GetVolumeInformation() */
TCHAR  g_volume_name[MAX_PATH + 1] = { 0 };
char * g_root_path = "x:\\";


int build_initial_search_string(char * search_path, char * search_string)
{
    GetCurrentDirectory(MAX_PATH, search_string);
    strcpy(search_path, search_string);
    strcat(search_path, "\\*.*");
    return 0;
}


char *
create_horizontal_line(char * result, CONSOLE_SCREEN_BUFFER_INFO csbi)
{
    SHORT i;
    SHORT console_width = csbi.srWindow.Right + 1;
    const char horizontal_line_character[2] = { (char)196, 0 };

    /* Draw line in result string */
    for(i = 0; i < console_width; ++i) {
        //if (i == console_width / 2) {
        //    strcat(string, "%c", );
        //} else {
            strncat(result, horizontal_line_character, 2);
        //}
    }

    return result;
}


char *
compact_size_with_suffix(long long size_bytes, char * suffixed_size)
{
    if(size_bytes >= 1024LL)  /* KB */
        if((size_bytes /= 1024LL) >= 1024LL)  /* MB */
            if((size_bytes /= 1024LL) >= 1024LL)  /* GB */
                if((size_bytes /= 1024LL) >= 1024LL)  /* TB */
                    sprintf(suffixed_size, "%lld TB", size_bytes/1024LL);
                else sprintf(suffixed_size, "%lld GB", size_bytes);
            else sprintf(suffixed_size, "%lld MB", size_bytes);
        else sprintf(suffixed_size, "%lld KB", size_bytes);
    else sprintf(suffixed_size, "%lld B", size_bytes);

    return suffixed_size;
}


int display_footer()
{
    char line[8192] = { 0 };

    FG_AQUA();
    create_horizontal_line(line, g_screen_info_t);
    printf("%s", line);

    FG_LIGHT_AQUA();
    printf(" %6d", g_file_counter);
    FG_LIGHT_GREEN();
    printf(" files, totaling ");
    FG_LIGHT_AQUA();

    if(g_total_size > 1023)  /* KB */
        if((g_total_size /= 1024.0) > 1023)  /* MB */
            if((g_total_size /= 1024.0) > 1023)  /* GB */
                if((g_total_size /= 1024.0) > 1023)  /* TB */
                    printf("%.1f TB", g_total_size);
                else printf("%.1f GB", g_total_size);
            else printf("%.1f MB", g_total_size);
        else printf("%.1f KB", g_total_size);
    else printf("%d B", (int)g_total_size);

    FG_LIGHT_GREEN(); printf(", consuming ");
    FG_LIGHT_AQUA();  printf("%d", (int)g_total_consumed);
    FG_LIGHT_GREEN(); puts(" bytes of disk space.");
    FG_LIGHT_AQUA();  printf(" %d", 0);
    FG_LIGHT_GREEN(); printf(" bytes available on Drive ");
    FG_LIGHT_AQUA();  printf("%c:", g_search_drive);
    FG_LIGHT_GREEN(); printf(" \t\t Volume label: ");

    g_root_path[0] = g_search_drive;
//  GetVolumeInformation(g_root_path, g_volume_name, ARRAYSIZE(g_volume_name), &serial_number, &max_component_length, &filesystem_flags, filesystem_name, ARRAYSIZE(filesystem_name));
    GetVolumeInformation(g_root_path, g_volume_name, ARRAYSIZE(g_volume_name), NULL, NULL, NULL, NULL, 0);

    FG_LIGHT_RED(); printf("%s\n", g_volume_name);
    return 0;
}


int display_header(char * search_path)
{
    char line[8192] = { 0 };

    FG_BRIGHT_WHITE();
    puts("\nHD");
    FG_AQUA();
    printf("Path: %s\n", search_path);

    /** Draw horizontal line across screen */
    create_horizontal_line(line, g_screen_info_t);
    printf("%s", line);

    return 0;
}


int display_help()
{
    int i;

    FG_BRIGHT_WHITE(); puts("\nHD " VERSION_STRING);
    FG_AQUA(); puts("Public domain by veganaiZe");

    /* Draw ------------- */
    for (i = 0; i < g_console_width; i++) putchar(196);

    FG_PURPLE(); printf("\nClone of ");
    FG_YELLOW(); printf("HotDIR ");
    FG_PURPLE(); puts("by Tony Overfield and Robert Woeger");
    FG_AQUA();   puts("\nUsage:");
    FG_WHITE();  puts("\tHD [options] [drive:\\][path][search-string]");
    FG_AQUA();   puts("\nOptions:");
    FG_WHITE();  printf("\t/C ");
    FG_AQUA();   puts("- Clear Screen");
    FG_WHITE();  printf("\t/# ");
    FG_AQUA();   puts("- Number of Columns (2,4,6) (Default: 1)");
    FG_WHITE();  printf("\t/L ");
    FG_AQUA();   puts("- Left to Right Ordering (Default: Top to Bottom)");
    FG_WHITE();  printf("\t/E ");
    FG_AQUA();   puts("- Sort by Extension");
    FG_WHITE();  printf("\t/D ");
    FG_AQUA();   puts("- Sort by Date");
    FG_WHITE();  printf("\t/S ");
    FG_AQUA();   puts("- Sort by Size");

    return 0;
}


int fixup_path(char * search_path)
{
    /* Contents of directory (w/o trailing backslash) */
    g_dwAttrib = GetFileAttributes((LPCTSTR) search_path);

    /* Valid directory ? */
    if (g_dwAttrib != INVALID_FILE_ATTRIBUTES
            && (g_dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
        /* Append backslash & wildcard pattern */
        strcat(search_path, "\\*.*");
    }

    return 0;
}


struct console_info *
get_console_info(struct console_info * p_console)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    g_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &g_screen_info_t);

    g_original_attributes = g_screen_info_t.wAttributes; /* Save console colors */
    p_console->colors = g_screen_info_t.wAttributes;
    g_console_width = g_screen_info_t.srWindow.Right;    /* Get console width */
    p_console->width = g_screen_info_t.srWindow.Right + 1;
    g_console_height = g_screen_info_t.srWindow.Bottom
                     - g_screen_info_t.srWindow.Top;  /* Get console height */
    p_console->height = g_screen_info_t.srWindow.Bottom
                        - g_screen_info_t.srWindow.Top;

    return p_console;
}


int process_cmdline_args(int argc,
                         char *argv[],
                         char search_drive,
                         char *search_path,
                         char *search_string)
{
    /** Process command line arguments */
    while (argc-- > 1) {
        if (*(argv[argc]) == '/') {
            switch ((int)*(argv[argc]+1)) {

                /* CHOICE: Display Help `/h` */
                case 'h' : case 'H' : case '?':
                    display_help();
                    restore_console();
                    /* Quit */
                    return 0;

                /* CHOICE: Clear Screen `/c` */
                case 'c' : case 'C' :
                    system("cls");
                    break;

                /* CHOICE: Sort Name `/n` */
                case 'n' : case 'N' :
                    puts("\nSORT_NAME -- not implemented (default)");
                    break;

                /* CHOICE: Sort Extension `/e` */
                case 'e' : case 'E' :
                    puts("\nSORT_EXT -- not implemented");
                    break;

                /* CHOICE: Sort Date `/d` */
                case 'd' : case 'D' :
                    puts("\nSORT_DATE -- not implemented");
                    break;

                /* CHOICE: Sort Size `/s` */
                case 's' : case 'S' :
                    puts("\nSORT_SIZE -- not implemented");
                    break;
            }  /* End switch */
        /** Process any drive, folder, and file arguments */
        } else {
            /* Drive indicator ? */
            if (strchr(argv[argc], ':') != NULL) {

               /* Get current drive letter */
               search_drive = (char)toupper(*(strchr(argv[argc], ':')-1));

               /* Drop drive letter */
               argv[argc] = argv[argc]+2;

            } else {
               /* Fallback to current drive letter */
               search_drive = *search_string;
            }

            /* Drop drive letter no matter what */
            strcpy(search_string, search_string+2);

            /* Arg has more than just drive letter ? */
            if (argv[argc][0]) {

                if (argv[argc][0] != '\\')
                    sprintf(search_path, "%c:%s\\%s", search_drive,
                            search_string, argv[argc])
                    ;
                else
                    sprintf(search_path, "%c:%s", search_drive, argv[argc])
                    ;

            } else {

                sprintf(search_path, "%c:\\", search_drive);

            }

            if (search_path[strlen(search_path)-1] == '\\')
                strcat(search_path, "*.*")
                ;

        }  /* End if */

    }  /* End while */

    return 0;
}


int process_files(HANDLE search_handle, char * search_path)
{
    int i;

    /** Attempt to retrieve first file */
    search_handle = FindFirstFile((LPCTSTR) search_path, &g_file_data_t);
    if (search_handle == INVALID_HANDLE_VALUE) {
        puts("\nNo file or folder found.");

        restore_console();
        return -1;

    }
//    else if (search_handle == ERROR_FILE_NOT_FOUND) {
//        puts("\nNo file or folder found.");
//
//        restore_console();
//        return -1;
//    }

    do {
        /* File is directory ? */
        if (g_file_data_t.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            FG_LIGHT_PURPLE();
        } else {
            g_file_counter++;

            /* Get file extension */
            g_file_ext = strrchr(g_file_data_t.cFileName, '.');

            /* Convert file extension to lowercase */
            if (g_file_ext != NULL) {
                for (i = 0; g_file_ext[i]; i++) {
                    g_file_ext[i] = (char)tolower(g_file_ext[i]);
                }

                /* Set color based on file extension */
                if (strcmp(g_file_ext, ".exe") == 0 || strcmp(g_file_ext, ".msi") == 0 )
                    FG_LIGHT_AQUA();
                else if(strcmp(g_file_ext, ".txt") == 0 || strcmp(g_file_ext, ".doc") == 0 || strcmp(g_file_ext, ".c") == 0
                            || strcmp(g_file_ext, ".rtf") == 0 || strcmp(g_file_ext, ".cc") == 0 || strcmp(g_file_ext, ".asm") == 0
                            || strcmp(g_file_ext, ".docx") == 0 || strcmp(g_file_ext, ".xml") == 0 || strcmp(g_file_ext, ".odt") == 0
                            || strcmp(g_file_ext, ".fodt") == 0 || strcmp(g_file_ext, ".ods") == 0 || strcmp(g_file_ext, ".fods") == 0
                            || strcmp(g_file_ext, ".odp") == 0 || strcmp(g_file_ext, ".fodp") == 0 || strcmp(g_file_ext, ".odg") == 0
                            || strcmp(g_file_ext, ".fodg") == 0 || strcmp(g_file_ext, ".odf") == 0 || strcmp(g_file_ext, ".pub") == 0
                            || strcmp(g_file_ext, ".ppt") == 0 || strcmp(g_file_ext, ".ott") == 0 || strcmp(g_file_ext, ".sxw") == 0
                            || strcmp(g_file_ext, ".stw") == 0 || strcmp(g_file_ext, ".docm") == 0 || strcmp(g_file_ext, ".dotx") == 0
                            || strcmp(g_file_ext, ".dotm") == 0 || strcmp(g_file_ext, ".dot") == 0 || strcmp(g_file_ext, ".wps") == 0
                            || strcmp(g_file_ext, ".wpd") == 0 || strcmp(g_file_ext, ".lwp") == 0 || strcmp(g_file_ext, ".htm") == 0
                            || strcmp(g_file_ext, ".html") == 0 || strcmp(g_file_ext, ".xhtml") == 0 || strcmp(g_file_ext, ".css") == 0
                            || strcmp(g_file_ext, ".abw") == 0 || strcmp(g_file_ext, ".zabw") == 0 || strcmp(g_file_ext, ".cwk") == 0
                            || strcmp(g_file_ext, ".pdb") == 0 || strcmp(g_file_ext, ".mw") == 0 || strcmp(g_file_ext, ".mcw") == 0
                            || strcmp(g_file_ext, ".ots") == 0 || strcmp(g_file_ext, ".sxc") == 0 || strcmp(g_file_ext, ".stc") == 0
                            || strcmp(g_file_ext, ".xls") == 0 || strcmp(g_file_ext, ".xlsx") == 0 || strcmp(g_file_ext, ".xlsm") == 0
                            || strcmp(g_file_ext, ".xlt") == 0 || strcmp(g_file_ext, ".xltx") == 0 || strcmp(g_file_ext, ".xltm") == 0
                            || strcmp(g_file_ext, ".pdf") == 0 || strcmp(g_file_ext, ".ps") == 0 || strcmp(g_file_ext, ".wdb") == 0
                            || strcmp(g_file_ext, ".xlc") == 0 || strcmp(g_file_ext, ".xlm") == 0 || strcmp(g_file_ext, ".xlw") == 0
                            || strcmp(g_file_ext, ".dif") == 0 || strcmp(g_file_ext, ".dbf") == 0 || strcmp(g_file_ext, ".wb2") == 0
                            || strcmp(g_file_ext, ".wk1") == 0 || strcmp(g_file_ext, ".wks") == 0 || strcmp(g_file_ext, ".123") == 0
                            || strcmp(g_file_ext, ".pps") == 0 || strcmp(g_file_ext, ".ouf") == 0 || strcmp(g_file_ext, ".uop") == 0
                            || strcmp(g_file_ext, ".sxi") == 0 || strcmp(g_file_ext, ".sti") == 0 || strcmp(g_file_ext, ".sxd") == 0
                            || strcmp(g_file_ext, ".potm") == 0 || strcmp(g_file_ext, ".potx") == 0 || strcmp(g_file_ext, ".pptx") == 0
                            || strcmp(g_file_ext, ".pptm") == 0 || strcmp(g_file_ext, ".ppsx") == 0 || strcmp(g_file_ext, ".key") == 0
                            || strcmp(g_file_ext, ".wpg") == 0 || strcmp(g_file_ext, ".dxf") == 0 || strcmp(g_file_ext, ".blend") == 0
                            || strcmp(g_file_ext, ".eps") == 0 || strcmp(g_file_ext, ".pm") == 0 || strcmp(g_file_ext, ".pm6") == 0
                            || strcmp(g_file_ext, ".pm65") == 0 || strcmp(g_file_ext, ".pmd") == 0 || strcmp(g_file_ext, ".log") == 0
                            || strcmp(g_file_ext, ".tex") == 0 || strcmp(g_file_ext, ".pages") == 0 || strcmp(g_file_ext, ".msg") == 0
                            || strcmp(g_file_ext, ".csv") == 0 || strcmp(g_file_ext, ".srt") == 0 || strcmp(g_file_ext, ".3ds") == 0
                            || strcmp(g_file_ext, ".3dm") == 0 || strcmp(g_file_ext, ".max") == 0 || strcmp(g_file_ext, ".indd") == 0
                            || strcmp(g_file_ext, ".pct") == 0 || strcmp(g_file_ext, ".xlr") == 0 || strcmp(g_file_ext, ".chm") == 0
                            || strcmp(g_file_ext, ".hlp") == 0 || strcmp(g_file_ext, ".jsp") == 0 || strcmp(g_file_ext, ".asp") == 0
                            || strcmp(g_file_ext, ".aspx") == 0 || strcmp(g_file_ext, ".csr") == 0 || strcmp(g_file_ext, ".rss") == 0
                            || strcmp(g_file_ext, ".h") == 0 || strcmp(g_file_ext, ".a") == 0 || strcmp(g_file_ext, ".cxx") == 0
                            || strcmp(g_file_ext, ".hxx") == 0 || strcmp(g_file_ext, ".xps") == 0 || strcmp(g_file_ext, ".oxps") == 0 )
                    FG_BRIGHT_WHITE();
                else if(strcmp(g_file_ext, ".bat") == 0 || strcmp(g_file_ext, ".cmd") == 0 || strcmp(g_file_ext, ".btm") == 0)
                    FG_LIGHT_RED();
                else if(strcmp(g_file_ext, ".com") == 0 || strcmp(g_file_ext, ".msc") == 0 )
                    FG_LIGHT_GREEN();
                else if(strcmp(g_file_ext, ".bas") == 0 || strcmp(g_file_ext, ".pas") == 0 || strcmp(g_file_ext, ".js") == 0
                            || strcmp(g_file_ext, ".jse") == 0 || strcmp(g_file_ext, ".vbs") == 0 || strcmp(g_file_ext, ".vbe") == 0
                            || strcmp(g_file_ext, ".wsf") == 0 || strcmp(g_file_ext, ".php") == 0 || strcmp(g_file_ext, ".py") == 0
                            || strcmp(g_file_ext, ".pl") == 0 || strcmp(g_file_ext, ".rb") == 0 || strcmp(g_file_ext, ".xsl") == 0
                            || strcmp(g_file_ext, ".tcl") == 0 || strcmp(g_file_ext, ".wsh") == 0)
                    FG_GREEN();
                else if(strcmp(g_file_ext, ".mp3") == 0 || strcmp(g_file_ext, ".mpg") == 0 || strcmp(g_file_ext, ".mpeg") == 0
                            || strcmp(g_file_ext, ".jpg") == 0 || strcmp(g_file_ext, ".jpeg") == 0 || strcmp(g_file_ext, ".gif") == 0
                            || strcmp(g_file_ext, ".png") == 0 || strcmp(g_file_ext, ".tif") == 0 || strcmp(g_file_ext, ".tiff") == 0
                            || strcmp(g_file_ext, ".psd") == 0 || strcmp(g_file_ext, ".xcf") == 0 || strcmp(g_file_ext, ".svg") == 0
                            || strcmp(g_file_ext, ".mp4") == 0 || strcmp(g_file_ext, ".mkv") == 0 || strcmp(g_file_ext, ".avi") == 0
                            || strcmp(g_file_ext, ".mov") == 0 || strcmp(g_file_ext, ".pcx") == 0 || strcmp(g_file_ext, ".wav") == 0
                            || strcmp(g_file_ext, ".aif") == 0 || strcmp(g_file_ext, ".aiff") == 0 || strcmp(g_file_ext, ".emf") == 0
                            || strcmp(g_file_ext, ".ico") == 0 || strcmp(g_file_ext, ".xpm") == 0 || strcmp(g_file_ext, ".jpe") == 0
                            || strcmp(g_file_ext, ".wmf") == 0 || strcmp(g_file_ext, ".lmb") == 0 || strcmp(g_file_ext, ".bmp") == 0
                            || strcmp(g_file_ext, ".tga") == 0 || strcmp(g_file_ext, ".xbm") == 0 || strcmp(g_file_ext, ".pnm") == 0
                            || strcmp(g_file_ext, ".pbm") == 0 || strcmp(g_file_ext, ".pgm") == 0 || strcmp(g_file_ext, ".ff") == 0
                            || strcmp(g_file_ext, ".ppm") == 0 || strcmp(g_file_ext, ".mng") == 0 || strcmp(g_file_ext, ".cur") == 0
                            || strcmp(g_file_ext, ".ani") == 0 || strcmp(g_file_ext, ".svgz") == 0 || strcmp(g_file_ext, ".ai") == 0
                            || strcmp(g_file_ext, ".flac") == 0 || strcmp(g_file_ext, ".ogg") == 0 || strcmp(g_file_ext, ".ogv") == 0
                            || strcmp(g_file_ext, ".oga") == 0 || strcmp(g_file_ext, ".asx") == 0 || strcmp(g_file_ext, ".wm") == 0
                            || strcmp(g_file_ext, ".wma") == 0 || strcmp(g_file_ext, ".wmx") == 0 || strcmp(g_file_ext, ".m3u") == 0
                            || strcmp(g_file_ext, ".aac") == 0 || strcmp(g_file_ext, ".asf") == 0 || strcmp(g_file_ext, ".wmv") == 0
                            || strcmp(g_file_ext, ".m2ts") == 0 || strcmp(g_file_ext, ".m2t") == 0 || strcmp(g_file_ext, ".qt") == 0
                            || strcmp(g_file_ext, ".wtv") == 0 || strcmp(g_file_ext, ".dvr-ms") == 0 || strcmp(g_file_ext, ".m4v") == 0
                            || strcmp(g_file_ext, ".mpe") == 0 || strcmp(g_file_ext, ".m1v") == 0 || strcmp(g_file_ext, ".mp2") == 0
                            || strcmp(g_file_ext, ".mpv2") == 0 || strcmp(g_file_ext, ".mod") == 0 || strcmp(g_file_ext, ".vob") == 0
                            || strcmp(g_file_ext, ".voc") == 0 || strcmp(g_file_ext, ".wdp") == 0 || strcmp(g_file_ext, ".raw") == 0
                            || strcmp(g_file_ext, ".hdp") == 0 || strcmp(g_file_ext, ".flv") == 0 || strcmp(g_file_ext, ".mid") == 0
                            || strcmp(g_file_ext, ".mpa") == 0 || strcmp(g_file_ext, ".m4a") == 0 || strcmp(g_file_ext, ".iff") == 0
                            || strcmp(g_file_ext, ".3gp") == 0 || strcmp(g_file_ext, ".3g2") == 0 || strcmp(g_file_ext, ".m4v") == 0
                            || strcmp(g_file_ext, ".rm") == 0 || strcmp(g_file_ext, ".ram") == 0 || strcmp(g_file_ext, ".swf") == 0
                            || strcmp(g_file_ext, ".vob") == 0 || strcmp(g_file_ext, ".pspimge") == 0 || strcmp(g_file_ext, ".thm") == 0
                            || strcmp(g_file_ext, ".yuv") == 0 || strcmp(g_file_ext, ".divx") == 0 || strcmp(g_file_ext, ".m4p") == 0
                            || strcmp(g_file_ext, ".mts") == 0 || strcmp(g_file_ext, ".pam") == 0)
                    FG_LIGHT_YELLOW();
                else if( strcmp(g_file_ext, ".7z") == 0 || strcmp(g_file_ext, ".zip") == 0 || strcmp(g_file_ext, ".gz") == 0
                            || strcmp(g_file_ext, ".tar") == 0 || strcmp(g_file_ext, ".bz2") == 0 || strcmp(g_file_ext, ".rar") == 0
                            || strcmp(g_file_ext, ".arc") == 0 || strcmp(g_file_ext, ".devpak") == 0 || strcmp(g_file_ext, ".xz") == 0
                            || strcmp(g_file_ext, ".lzma") == 0 || strcmp(g_file_ext, ".iso") == 0 || strcmp(g_file_ext, ".lz") == 0
                            || strcmp(g_file_ext, ".lzo") == 0 || strcmp(g_file_ext, ".rz") == 0 || strcmp(g_file_ext, ".sz") == 0
                            || strcmp(g_file_ext, ".z") == 0 || strcmp(g_file_ext, ".Z") == 0 || strcmp(g_file_ext, ".arj") == 0
                            || strcmp(g_file_ext, ".b1") == 0 || strcmp(g_file_ext, ".cab") == 0 || strcmp(g_file_ext, ".cfs") == 0
                            || strcmp(g_file_ext, ".dmg") == 0 || strcmp(g_file_ext, ".ear") == 0 || strcmp(g_file_ext, ".jar") == 0
                            || strcmp(g_file_ext, ".lzh") == 0 || strcmp(g_file_ext, ".lha") == 0 || strcmp(g_file_ext, ".kgb") == 0
                            || strcmp(g_file_ext, ".lzx") == 0 || strcmp(g_file_ext, ".pea") == 0 || strcmp(g_file_ext, ".partimg") == 0
                            || strcmp(g_file_ext, ".pim") == 0 || strcmp(g_file_ext, ".sda") == 0 || strcmp(g_file_ext, ".sea") == 0
                            || strcmp(g_file_ext, ".sfx") == 0 || strcmp(g_file_ext, ".shk") == 0 || strcmp(g_file_ext, ".sit") == 0
                            || strcmp(g_file_ext, ".sitx") == 0 || strcmp(g_file_ext, ".sqx") == 0 || strcmp(g_file_ext, ".tgz") == 0
                            || strcmp(g_file_ext, ".tbz2") == 0 || strcmp(g_file_ext, ".tlz") == 0 || strcmp(g_file_ext, ".war") == 0
                            || strcmp(g_file_ext, ".har") == 0 || strcmp(g_file_ext, ".wim") == 0 || strcmp(g_file_ext, ".xp3") == 0
                            || strcmp(g_file_ext, ".yz1") == 0 || strcmp(g_file_ext, ".zipx") == 0 || strcmp(g_file_ext, ".zoo") == 0
                            || strcmp(g_file_ext, ".zpaq") == 0 || strcmp(g_file_ext, ".zz") == 0 ||strcmp(g_file_ext, ".ecc") == 0
                            || strcmp(g_file_ext, ".par") == 0 || strcmp(g_file_ext, ".par2") == 0 || strcmp(g_file_ext, ".img") == 0
                            || strcmp(g_file_ext, ".hqx") == 0 || strcmp(g_file_ext, ".hcx") == 0 || strcmp(g_file_ext, ".hex") == 0
                            || strcmp(g_file_ext, ".deb") == 0 || strcmp(g_file_ext, ".rpm") == 0 || strcmp(g_file_ext, ".mdf") == 0
                            || strcmp(g_file_ext, ".cue") == 0 || strcmp(g_file_ext, ".bin") == 0 || strcmp(g_file_ext, ".apk") == 0)
                    FG_YELLOW();
                else FG_GRAY();  // (everything else)
            } else FG_GRAY();  // (no extension)
        } /* First if/else */

        /* Pause if console screen is full */
        if(++g_line_count == g_console_height) {
            g_line_count = 0;
            GetConsoleScreenBufferInfo(g_hConsole, &g_screen_info_t);
            g_dwAttrib = g_screen_info_t.wAttributes;
            FG_GRAY(); system("PAUSE");
            SetConsoleTextAttribute(g_hConsole, (WORD)g_dwAttrib); // Restore Color
        }

        /* "DARK FG_RED" for hidden files */
        if(g_file_data_t.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) {
            FG_RED();
        }

        /* Display file name */
        printf("%-*s", g_console_width / 2 - 8, g_file_data_t.cFileName);
        //printf("%s\n", g_file_ext);

        /* Display <dir> for directories */
        if(g_file_data_t.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            printf("  <dir> ");

        /* Otherwise display file size */
        } else {
            g_file_size = (double)((g_file_data_t.nFileSizeHigh * (MAXDWORD+1))
                                    + g_file_data_t.nFileSizeLow);
            g_total_size += g_file_size;
            FG_GRAY();

            if(g_file_size > 1023)  /* KB */
                if((g_file_size /= 1024.0) > 1023)  /* MB */
                    if((g_file_size /= 1024.0) > 1023)  /* BB */
                        if((g_file_size /= 1024.0) > 1023)  /* TB */
                            printf("% 5.1f TB", g_file_size);
                        else printf("% 5.1f GB", g_file_size);
                    else printf("% 5.1f MB", g_file_size);
                else printf("% 5.1f KB", g_file_size);
            else printf("% 5d B ", (int)g_file_size);

            //g_total_consumed += (float)((*lpFileSizeHigh * (MAXDWORD+1)) + GetCompressedFileSize(g_file_data_t.cFileName, lpFileSizeHigh));
        }

        FG_AQUA(); printf("\263 \n");  // Print |
    } while( FindNextFile(search_handle, &g_file_data_t) != 0 );
    /* End do */

    FindClose(search_handle);
    return 0;
}


int restore_console()
{
    SetConsoleTextAttribute(g_hConsole, g_original_attributes);
    return 0;
}
