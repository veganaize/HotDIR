/* HotDIR (clone) - Public Domain by veganaiZe
 *
 * Colorful file/folder listing at the command prompt.
 *
 * NO WARRANTY WHATSOEVER!
 *
 */
#include <stdio.h>
#include <string.h>
#include <windows.h>

#define VERSION_STRING "0.6.3"

#define GREEN()        SetConsoleTextAttribute(hConsole, 0x02)
#define AQUA()         SetConsoleTextAttribute(hConsole, 0x03)
#define RED()          SetConsoleTextAttribute(hConsole, 0x04)
#define PURPLE()       SetConsoleTextAttribute(hConsole, 0x05)
#define YELLOW()       SetConsoleTextAttribute(hConsole, 0x06)
#define WHITE()        SetConsoleTextAttribute(hConsole, 0x07)
#define GRAY()         SetConsoleTextAttribute(hConsole, 0x08)
#define LIGHT_GREEN()  SetConsoleTextAttribute(hConsole, 0x0A)
#define LIGHT_AQUA()   SetConsoleTextAttribute(hConsole, 0x0B)
#define LIGHT_RED()    SetConsoleTextAttribute(hConsole, 0x0C)
#define LIGHT_PURPLE() SetConsoleTextAttribute(hConsole, 0x0D)
#define LIGHT_YELLOW() SetConsoleTextAttribute(hConsole, 0x0E)
#define BRIGHT_WHITE() SetConsoleTextAttribute(hConsole, 0x0F)


int main(int argc, char* argv[]) {

/* Console variables: */

    HANDLE hConsole;
    HANDLE search_handle;

    CONSOLE_SCREEN_BUFFER_INFO screen_info_t;
    WIN32_FIND_DATA file_data_t;

    WORD   original_attributes;
    SHORT  console_width;
    SHORT  line_count = 3;  /* Pre-loaded with number of lines in header */
    DWORD  dwAttrib;

    /* Args to GetVolumeInformation() */
    TCHAR volume_name[MAX_PATH + 1] = { 0 };
    char  *root_path = "x:\\";

/* Other variables: */

    char  search_string[MAX_PATH];
    char  search_path[MAX_PATH];
    char  search_drive   = 'C';     /* Pre-load with C: drive */
    char  *file_ext      = NULL;    /* Current file's extension */
    float file_size      = -1.0;    /* Current file's size */
    float total_size     = 0.0;     /* Total of all listed file sizes */
    float total_consumed = 0.0;     /* Total actual/compressed disk usage */
    int   file_counter   = 0;       /* Total listed file count */
    int   console_height = 24;
    int   i;                        /* General counter */

    GetCurrentDirectory(MAX_PATH, search_string);
    strcpy(search_path, search_string);
    strcat(search_path, "\\*.*");

    /** Get console info */
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &screen_info_t);

    original_attributes = screen_info_t.wAttributes; /* Save console info */
    console_width = screen_info_t.srWindow.Right;    /* Get console width */
    console_height = screen_info_t.srWindow.Bottom
                     - screen_info_t.srWindow.Top;  /* Get console height */

    /** Process command line arguments */
    while (argc-- > 1) {
        if (*(argv[argc]) == '/') {
            switch ((int)*(argv[argc]+1)) {

                /* CHOICE: Display Help `/h` */
                case 'h' : case 'H' : case '?':
                    BRIGHT_WHITE(); puts("\nHD " VERSION_STRING);

                    AQUA(); puts("Public domain by veganaiZe");

                    /* Draw ------------- */
                    for (i = 0; i < console_width; i++) putchar(196);

                    PURPLE(); printf("\nClone of ");
                    YELLOW(); printf("HotDIR ");
                    PURPLE(); puts("by Tony Overfield and Robert Woeger");
                    AQUA();   puts("\nUsage:");
                    WHITE();  puts("\tHD [options] [drive:\\][path][search-string]");
                    AQUA();   puts("\nOptions:");
                    WHITE();  printf("\t/C ");
                    AQUA();   puts("- Clear Screen");
                    WHITE();  printf("\t/# ");
                    AQUA();   puts("- Number of Columns (2,4,6) (Default: 1)");
                    WHITE();  printf("\t/L ");
                    AQUA();   puts("- Left to Right Ordering (Default: Top to Bottom)");
                    WHITE();  printf("\t/E ");
                    AQUA();   puts("- Sort by Extension");
                    WHITE();  printf("\t/D ");
                    AQUA();   puts("- Sort by Date");
                    WHITE();  printf("\t/S ");
                    AQUA();   puts("- Sort by Size");

                    /* Restore console */
                    SetConsoleTextAttribute(hConsole, original_attributes);
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
               search_drive = toupper(*(strchr(argv[argc], ':')-1));

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


    BRIGHT_WHITE(); puts("\nHD");
    AQUA(); printf("Path: %s\n", search_path);

    /** Draw horizontal line across screen */
    for (i = 0; i < console_width; i++) {
        /* Draw ------|------- */
        // i == console_width / 2 ? putchar(194) : putchar(196)

        /* Draw ----------------- */
        putchar(196);
    }

    putchar('\n');

    /* Contents of directory (w/o trailing backslash) */
    dwAttrib = GetFileAttributes((LPCTSTR) search_path);

    /* Valid directory ? */
    if (dwAttrib != INVALID_FILE_ATTRIBUTES
            && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
        /* Append backslash & wildcard pattern */
        strcat(search_path, "\\*.*");
    }

    /** Attempt to retrieve first file */
    if ((search_handle = FindFirstFile((LPCTSTR)search_path, &file_data_t))
            == INVALID_HANDLE_VALUE) {
        puts("\nNo file or folder found.");

        /* Restore console */
        SetConsoleTextAttribute(hConsole, original_attributes);
        return -1;

    } else if((long)search_handle == ERROR_FILE_NOT_FOUND) {
        puts("\nNo file or folder found.");

        /* Restore console */
        SetConsoleTextAttribute(hConsole, original_attributes);
        return -1;
    }

    do {
        /* File is directory ? */
        if (file_data_t.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            LIGHT_PURPLE();
        } else {
            file_counter++;

            /* Get file extension */
            file_ext = strrchr(file_data_t.cFileName, '.');

            /* Convert file extension to lowercase */
            if (file_ext != NULL) {
                for (i = 0; file_ext[i]; i++) {
                    file_ext[i] = tolower(file_ext[i]);
                }

                /* Set color based on file extension */
                if (strcmp(file_ext, ".exe") == 0 || strcmp(file_ext, ".msi") == 0 )
                    LIGHT_AQUA();
                else if(strcmp(file_ext, ".txt") == 0 || strcmp(file_ext, ".doc") == 0 || strcmp(file_ext, ".c") == 0
                            || strcmp(file_ext, ".rtf") == 0 || strcmp(file_ext, ".cc") == 0 || strcmp(file_ext, ".asm") == 0
                            || strcmp(file_ext, ".docx") == 0 || strcmp(file_ext, ".xml") == 0 || strcmp(file_ext, ".odt") == 0
                            || strcmp(file_ext, ".fodt") == 0 || strcmp(file_ext, ".ods") == 0 || strcmp(file_ext, ".fods") == 0
                            || strcmp(file_ext, ".odp") == 0 || strcmp(file_ext, ".fodp") == 0 || strcmp(file_ext, ".odg") == 0
                            || strcmp(file_ext, ".fodg") == 0 || strcmp(file_ext, ".odf") == 0 || strcmp(file_ext, ".pub") == 0
                            || strcmp(file_ext, ".ppt") == 0 || strcmp(file_ext, ".ott") == 0 || strcmp(file_ext, ".sxw") == 0
                            || strcmp(file_ext, ".stw") == 0 || strcmp(file_ext, ".docm") == 0 || strcmp(file_ext, ".dotx") == 0
                            || strcmp(file_ext, ".dotm") == 0 || strcmp(file_ext, ".dot") == 0 || strcmp(file_ext, ".wps") == 0
                            || strcmp(file_ext, ".wpd") == 0 || strcmp(file_ext, ".lwp") == 0 || strcmp(file_ext, ".htm") == 0
                            || strcmp(file_ext, ".html") == 0 || strcmp(file_ext, ".xhtml") == 0 || strcmp(file_ext, ".css") == 0
                            || strcmp(file_ext, ".abw") == 0 || strcmp(file_ext, ".zabw") == 0 || strcmp(file_ext, ".cwk") == 0
                            || strcmp(file_ext, ".pdb") == 0 || strcmp(file_ext, ".mw") == 0 || strcmp(file_ext, ".mcw") == 0
                            || strcmp(file_ext, ".ots") == 0 || strcmp(file_ext, ".sxc") == 0 || strcmp(file_ext, ".stc") == 0
                            || strcmp(file_ext, ".xls") == 0 || strcmp(file_ext, ".xlsx") == 0 || strcmp(file_ext, ".xlsm") == 0
                            || strcmp(file_ext, ".xlt") == 0 || strcmp(file_ext, ".xltx") == 0 || strcmp(file_ext, ".xltm") == 0
                            || strcmp(file_ext, ".pdf") == 0 || strcmp(file_ext, ".ps") == 0 || strcmp(file_ext, ".wdb") == 0
                            || strcmp(file_ext, ".xlc") == 0 || strcmp(file_ext, ".xlm") == 0 || strcmp(file_ext, ".xlw") == 0
                            || strcmp(file_ext, ".dif") == 0 || strcmp(file_ext, ".dbf") == 0 || strcmp(file_ext, ".wb2") == 0
                            || strcmp(file_ext, ".wk1") == 0 || strcmp(file_ext, ".wks") == 0 || strcmp(file_ext, ".123") == 0
                            || strcmp(file_ext, ".pps") == 0 || strcmp(file_ext, ".ouf") == 0 || strcmp(file_ext, ".uop") == 0
                            || strcmp(file_ext, ".sxi") == 0 || strcmp(file_ext, ".sti") == 0 || strcmp(file_ext, ".sxd") == 0
                            || strcmp(file_ext, ".potm") == 0 || strcmp(file_ext, ".potx") == 0 || strcmp(file_ext, ".pptx") == 0
                            || strcmp(file_ext, ".pptm") == 0 || strcmp(file_ext, ".ppsx") == 0 || strcmp(file_ext, ".key") == 0
                            || strcmp(file_ext, ".wpg") == 0 || strcmp(file_ext, ".dxf") == 0 || strcmp(file_ext, ".blend") == 0
                            || strcmp(file_ext, ".eps") == 0 || strcmp(file_ext, ".pm") == 0 || strcmp(file_ext, ".pm6") == 0
                            || strcmp(file_ext, ".pm65") == 0 || strcmp(file_ext, ".pmd") == 0 || strcmp(file_ext, ".log") == 0
                            || strcmp(file_ext, ".tex") == 0 || strcmp(file_ext, ".pages") == 0 || strcmp(file_ext, ".msg") == 0
                            || strcmp(file_ext, ".csv") == 0 || strcmp(file_ext, ".srt") == 0 || strcmp(file_ext, ".3ds") == 0
                            || strcmp(file_ext, ".3dm") == 0 || strcmp(file_ext, ".max") == 0 || strcmp(file_ext, ".indd") == 0
                            || strcmp(file_ext, ".pct") == 0 || strcmp(file_ext, ".xlr") == 0 || strcmp(file_ext, ".chm") == 0
                            || strcmp(file_ext, ".hlp") == 0 || strcmp(file_ext, ".jsp") == 0 || strcmp(file_ext, ".asp") == 0
                            || strcmp(file_ext, ".aspx") == 0 || strcmp(file_ext, ".csr") == 0 || strcmp(file_ext, ".rss") == 0
                            || strcmp(file_ext, ".h") == 0 || strcmp(file_ext, ".a") == 0 || strcmp(file_ext, ".cxx") == 0
                            || strcmp(file_ext, ".hxx") == 0 || strcmp(file_ext, ".xps") == 0 || strcmp(file_ext, ".oxps") == 0 )
                    BRIGHT_WHITE();
                else if(strcmp(file_ext, ".bat") == 0 || strcmp(file_ext, ".cmd") == 0 || strcmp(file_ext, ".btm") == 0)
                    LIGHT_RED();
                else if(strcmp(file_ext, ".com") == 0 || strcmp(file_ext, ".msc") == 0 )
                    LIGHT_GREEN();
                else if(strcmp(file_ext, ".bas") == 0 || strcmp(file_ext, ".pas") == 0 || strcmp(file_ext, ".js") == 0
                            || strcmp(file_ext, ".jse") == 0 || strcmp(file_ext, ".vbs") == 0 || strcmp(file_ext, ".vbe") == 0
                            || strcmp(file_ext, ".wsf") == 0 || strcmp(file_ext, ".php") == 0 || strcmp(file_ext, ".py") == 0
                            || strcmp(file_ext, ".pl") == 0 || strcmp(file_ext, ".rb") == 0 || strcmp(file_ext, ".xsl") == 0
                            || strcmp(file_ext, ".tcl") == 0 || strcmp(file_ext, ".wsh") == 0)
                    GREEN();
                else if(strcmp(file_ext, ".mp3") == 0 || strcmp(file_ext, ".mpg") == 0 || strcmp(file_ext, ".mpeg") == 0
                            || strcmp(file_ext, ".jpg") == 0 || strcmp(file_ext, ".jpeg") == 0 || strcmp(file_ext, ".gif") == 0
                            || strcmp(file_ext, ".png") == 0 || strcmp(file_ext, ".tif") == 0 || strcmp(file_ext, ".tiff") == 0
                            || strcmp(file_ext, ".psd") == 0 || strcmp(file_ext, ".xcf") == 0 || strcmp(file_ext, ".svg") == 0
                            || strcmp(file_ext, ".mp4") == 0 || strcmp(file_ext, ".mkv") == 0 || strcmp(file_ext, ".avi") == 0
                            || strcmp(file_ext, ".mov") == 0 || strcmp(file_ext, ".pcx") == 0 || strcmp(file_ext, ".wav") == 0
                            || strcmp(file_ext, ".aif") == 0 || strcmp(file_ext, ".aiff") == 0 || strcmp(file_ext, ".emf") == 0
                            || strcmp(file_ext, ".ico") == 0 || strcmp(file_ext, ".xpm") == 0 || strcmp(file_ext, ".jpe") == 0
                            || strcmp(file_ext, ".wmf") == 0 || strcmp(file_ext, ".lmb") == 0 || strcmp(file_ext, ".bmp") == 0
                            || strcmp(file_ext, ".tga") == 0 || strcmp(file_ext, ".xbm") == 0 || strcmp(file_ext, ".pnm") == 0
                            || strcmp(file_ext, ".pbm") == 0 || strcmp(file_ext, ".pgm") == 0 || strcmp(file_ext, ".ff") == 0
                            || strcmp(file_ext, ".ppm") == 0 || strcmp(file_ext, ".mng") == 0 || strcmp(file_ext, ".cur") == 0
                            || strcmp(file_ext, ".ani") == 0 || strcmp(file_ext, ".svgz") == 0 || strcmp(file_ext, ".ai") == 0
                            || strcmp(file_ext, ".flac") == 0 || strcmp(file_ext, ".ogg") == 0 || strcmp(file_ext, ".ogv") == 0
                            || strcmp(file_ext, ".oga") == 0 || strcmp(file_ext, ".asx") == 0 || strcmp(file_ext, ".wm") == 0
                            || strcmp(file_ext, ".wma") == 0 || strcmp(file_ext, ".wmx") == 0 || strcmp(file_ext, ".m3u") == 0
                            || strcmp(file_ext, ".aac") == 0 || strcmp(file_ext, ".asf") == 0 || strcmp(file_ext, ".wmv") == 0
                            || strcmp(file_ext, ".m2ts") == 0 || strcmp(file_ext, ".m2t") == 0 || strcmp(file_ext, ".qt") == 0
                            || strcmp(file_ext, ".wtv") == 0 || strcmp(file_ext, ".dvr-ms") == 0 || strcmp(file_ext, ".m4v") == 0
                            || strcmp(file_ext, ".mpe") == 0 || strcmp(file_ext, ".m1v") == 0 || strcmp(file_ext, ".mp2") == 0
                            || strcmp(file_ext, ".mpv2") == 0 || strcmp(file_ext, ".mod") == 0 || strcmp(file_ext, ".vob") == 0
                            || strcmp(file_ext, ".voc") == 0 || strcmp(file_ext, ".wdp") == 0 || strcmp(file_ext, ".raw") == 0
                            || strcmp(file_ext, ".hdp") == 0 || strcmp(file_ext, ".flv") == 0 || strcmp(file_ext, ".mid") == 0
                            || strcmp(file_ext, ".mpa") == 0 || strcmp(file_ext, ".m4a") == 0 || strcmp(file_ext, ".iff") == 0
                            || strcmp(file_ext, ".3gp") == 0 || strcmp(file_ext, ".3g2") == 0 || strcmp(file_ext, ".m4v") == 0
                            || strcmp(file_ext, ".rm") == 0 || strcmp(file_ext, ".ram") == 0 || strcmp(file_ext, ".swf") == 0
                            || strcmp(file_ext, ".vob") == 0 || strcmp(file_ext, ".pspimge") == 0 || strcmp(file_ext, ".thm") == 0
                            || strcmp(file_ext, ".yuv") == 0 || strcmp(file_ext, ".divx") == 0 || strcmp(file_ext, ".m4p") == 0
                            || strcmp(file_ext, ".mts") == 0 || strcmp(file_ext, ".pam") == 0)
                    LIGHT_YELLOW();
                else if( strcmp(file_ext, ".7z") == 0 || strcmp(file_ext, ".zip") == 0 || strcmp(file_ext, ".gz") == 0
                            || strcmp(file_ext, ".tar") == 0 || strcmp(file_ext, ".bz2") == 0 || strcmp(file_ext, ".rar") == 0
                            || strcmp(file_ext, ".arc") == 0 || strcmp(file_ext, ".devpak") == 0 || strcmp(file_ext, ".xz") == 0
                            || strcmp(file_ext, ".lzma") == 0 || strcmp(file_ext, ".iso") == 0 || strcmp(file_ext, ".lz") == 0
                            || strcmp(file_ext, ".lzo") == 0 || strcmp(file_ext, ".rz") == 0 || strcmp(file_ext, ".sz") == 0
                            || strcmp(file_ext, ".z") == 0 || strcmp(file_ext, ".Z") == 0 || strcmp(file_ext, ".arj") == 0
                            || strcmp(file_ext, ".b1") == 0 || strcmp(file_ext, ".cab") == 0 || strcmp(file_ext, ".cfs") == 0
                            || strcmp(file_ext, ".dmg") == 0 || strcmp(file_ext, ".ear") == 0 || strcmp(file_ext, ".jar") == 0
                            || strcmp(file_ext, ".lzh") == 0 || strcmp(file_ext, ".lha") == 0 || strcmp(file_ext, ".kgb") == 0
                            || strcmp(file_ext, ".lzx") == 0 || strcmp(file_ext, ".pea") == 0 || strcmp(file_ext, ".partimg") == 0
                            || strcmp(file_ext, ".pim") == 0 || strcmp(file_ext, ".sda") == 0 || strcmp(file_ext, ".sea") == 0
                            || strcmp(file_ext, ".sfx") == 0 || strcmp(file_ext, ".shk") == 0 || strcmp(file_ext, ".sit") == 0
                            || strcmp(file_ext, ".sitx") == 0 || strcmp(file_ext, ".sqx") == 0 || strcmp(file_ext, ".tgz") == 0
                            || strcmp(file_ext, ".tbz2") == 0 || strcmp(file_ext, ".tlz") == 0 || strcmp(file_ext, ".war") == 0
                            || strcmp(file_ext, ".har") == 0 || strcmp(file_ext, ".wim") == 0 || strcmp(file_ext, ".xp3") == 0
                            || strcmp(file_ext, ".yz1") == 0 || strcmp(file_ext, ".zipx") == 0 || strcmp(file_ext, ".zoo") == 0
                            || strcmp(file_ext, ".zpaq") == 0 || strcmp(file_ext, ".zz") == 0 ||strcmp(file_ext, ".ecc") == 0
                            || strcmp(file_ext, ".par") == 0 || strcmp(file_ext, ".par2") == 0 || strcmp(file_ext, ".img") == 0
                            || strcmp(file_ext, ".hqx") == 0 || strcmp(file_ext, ".hcx") == 0 || strcmp(file_ext, ".hex") == 0
                            || strcmp(file_ext, ".deb") == 0 || strcmp(file_ext, ".rpm") == 0 || strcmp(file_ext, ".mdf") == 0
                            || strcmp(file_ext, ".cue") == 0 || strcmp(file_ext, ".bin") == 0 || strcmp(file_ext, ".apk") == 0)
                    YELLOW();
                else GRAY();  // (everything else)
            else GRAY();  // (no extension)
        } /* First if/else */

        /* Pause if console screen is full */
        if(++line_count == console_height) {
            line_count = 0;
            GetConsoleScreenBufferInfo(hConsole, &screen_info_t);
            dwAttrib = screen_info_t.wAttributes;
            GRAY(); system("PAUSE");
            SetConsoleTextAttribute(hConsole, dwAttrib); // Restore Color
        }

        /* "DARK RED" for hidden files */
        if(file_data_t.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) {
            RED();
        }

        /* Display file name */
        printf("%-*s", console_width / 2 - 8, file_data_t.cFileName);
        //printf("%s\n", file_ext);

        /* Display <dir> for directories */
        if(file_data_t.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            printf("  <dir> ");

        /* Otherwise display file size */
        } else {
            file_size = (float)((file_data_t.nFileSizeHigh * (MAXDWORD+1))
                                    + file_data_t.nFileSizeLow);
            total_size += file_size;
            GRAY();

            if(file_size > 1023)  /* KB */
                if((file_size /= 1024.0) > 1023)  /* MB */
                    if((file_size /= 1024.0) > 1023)  /* BB */
                        if((file_size /= 1024.0) > 1023)  /* TB */
                            printf("% 5.1f TB", file_size);
                        else printf("% 5.1f GB", file_size);
                    else printf("% 5.1f MB", file_size);
                else printf("% 5.1f KB", file_size);
            else printf("% 5d B ", (int)file_size);

            //total_consumed += (float)((*lpFileSizeHigh * (MAXDWORD+1)) + GetCompressedFileSize(file_data_t.cFileName, lpFileSizeHigh));
        }

        AQUA(); printf("\263 \n");  // Print |
    } while( FindNextFile(search_handle, &file_data_t) != 0 );
    /* End do */

    FindClose(search_handle);

    AQUA();  /* Draw ----|----- */
    for(i = 0; i < console_width; i++) {
        i == console_width / 2 ? putchar(193) : putchar(196);
    }
    putchar('\n');

    LIGHT_AQUA();  printf(" %6d", file_counter);
    LIGHT_GREEN(); printf(" files, totaling ");
    LIGHT_AQUA();

    if(total_size > 1023)  /* KB */
        if((total_size /= 1024.0) > 1023)  /* MB */
            if((total_size /= 1024.0) > 1023)  /* GB */
                if((total_size /= 1024.0) > 1023)  /* TB */
                    printf("%.1f TB", total_size);
                else printf("%.1f GB", total_size);
            else printf("%.1f MB", total_size);
        else printf("%.1f KB", total_size);
    else printf("%d B", (int)total_size);

    LIGHT_GREEN(); printf(", consuming ");
    LIGHT_AQUA();  printf("%d", (int)total_consumed);
    LIGHT_GREEN(); puts(" bytes of disk space.");
    LIGHT_AQUA();  printf(" %d", 0);
    LIGHT_GREEN(); printf(" bytes available on Drive ");
    LIGHT_AQUA();  printf("%c:", search_drive);
    LIGHT_GREEN(); printf(" \t\t Volume label: ");

    root_path[0] = search_drive;
//  GetVolumeInformation(root_path, volume_name, ARRAYSIZE(volume_name), &serial_number, &max_component_length, &filesystem_flags, filesystem_name, ARRAYSIZE(filesystem_name));
    GetVolumeInformation(root_path, volume_name, ARRAYSIZE(volume_name), NULL, NULL, NULL, NULL, 0);

    LIGHT_RED(); printf("%s\n", volume_name);

    SetConsoleTextAttribute(hConsole, original_attributes);  //Restore console
    return 0;
}
