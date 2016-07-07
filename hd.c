#include <stdio.h>
#include <string.h>
#include <windows.h>

//TODO:
//	Implement a counter and determine if number of output lines
//	will surpass console's max display lines.

#define ONE_COLUMN	1
#define TWO_COLUMN	2
#define FOUR_COLUMN	4
#define SIX_COLUMN	8
#define SORT_NAME	16
#define SORT_EXT	32
#define SORT_SIZE	64
#define SORT_DATE	128

int main(int argc, char* argv[]) {

	HANDLE hConsole;
	HANDLE search_handle;
	CONSOLE_SCREEN_BUFFER_INFO screen_info_t;
	PCONSOLE_CURSOR_INFO cursor_info_t;
	WIN32_FIND_DATA file_data_t;
	WORD  original_attributes;
	SHORT console_width;
	SHORT line_count = 3;
	DWORD dwAttrib;
	//??? console_height;
	char search_string[MAX_PATH];
	char search_path[MAX_PATH];
	char search_drive = 'C';
	char* file_ext = NULL;	// Current file's extension
	int sort_flag;			// Sort by ...
	float file_size;			// Current file's size
	int total_size = 0;		// Total of all listed file sizes
	int file_counter = 0;	// Total listed file count
	int console_height = 24;
	int i;					// General counter

	GetCurrentDirectory(MAX_PATH, search_string);
	strcpy(search_path, search_string);
	strcat(search_path, "\\*.*");

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &screen_info_t);  //Get console info

	original_attributes = screen_info_t.wAttributes;  //Save console info
	console_width = screen_info_t.srWindow.Right;  //Get console width
	console_height = screen_info_t.srWindow.Bottom - screen_info_t.srWindow.Top;  //Get console height
	
	/*
	 * Process command line arguments
	 *
	 */
	while(argc-- > 1) {
		if( *(argv[argc]) == '/' ) {
			switch( (int)*(argv[argc]+1) ) {
				case 'h' : case 'H' : case '?':  // --> HELP <--
					SetConsoleTextAttribute(hConsole, 0x0F);  //Bright White
					puts("\nHD 5.x ");
					SetConsoleTextAttribute(hConsole, 0x03);  //Low Aqua
					puts("Public domain by veganaiZe");
					for(i = 0; i < console_width; i++) putchar(196);  //Draw -------------
					putchar('\n');
					SetConsoleTextAttribute(hConsole, 0x05);  //Light Purple
					printf("Based on ");
					SetConsoleTextAttribute(hConsole, 0x06);
					printf("HotDIR ");
					SetConsoleTextAttribute(hConsole, 0x05);
					puts("by Tony Overfield and Robert Woeger");
					SetConsoleTextAttribute(hConsole, 0x03);  //Low Aqua
					puts("\nUsage:");
					SetConsoleTextAttribute(hConsole, 0x07);  //Low White
					puts("\tHD [options] [drive:\\][path][search-string]");
					SetConsoleTextAttribute(hConsole, 0x03);  //Low Aqua
					puts("\nOptions:");
					SetConsoleTextAttribute(hConsole, 0x07);  //Low White
					printf("\t/C ");
					SetConsoleTextAttribute(hConsole, 0x03);  //Low Aqua
					puts("- Clear Screen");
					SetConsoleTextAttribute(hConsole, 0x07);  //Low White
					printf("\t/# ");
					SetConsoleTextAttribute(hConsole, 0x03);  //Low Aqua
					puts("- Number of Columns (2,4,6) (Default: 1)");
					SetConsoleTextAttribute(hConsole, 0x07);  //Low White
					printf("\t/L ");
					SetConsoleTextAttribute(hConsole, 0x03);  //Low Aqua
					puts("- Left to Right Ordering (Default: Top to Bottom)");
					SetConsoleTextAttribute(hConsole, 0x07);  //Low White
					printf("\t/E ");
					SetConsoleTextAttribute(hConsole, 0x03);  //Low Aqua
					puts("- Sort by Extension");
					SetConsoleTextAttribute(hConsole, 0x07);  //Low White
					printf("\t/D ");
					SetConsoleTextAttribute(hConsole, 0x03);  //Low Aqua
					puts("- Sort by Date");
					SetConsoleTextAttribute(hConsole, 0x07);  //Low White
					printf("\t/S ");
					SetConsoleTextAttribute(hConsole, 0x03);  //Low Aqua
					puts("- Sort by Size");
					//Restore console
					SetConsoleTextAttribute(hConsole, original_attributes);
					//Quit
					return 0;
				case 'c' : case 'C' :  // --> CLEAR SCREEN <--
					system("cls");
					break;
				case 'n' : case 'N' :  // --> SORT NAME <--
					puts("\nSORT_NAME -- not implemented (default)");
					break;
				case 'e' : case 'E' :  // --> SORT EXTENSION <--
					puts("\nSORT_EXT -- not implemented");
					break;
				case 'd' : case 'D' :  // --> SORT DATE <--
					puts("\nSORT_DATE -- not implemented");
					break;
				case 's' : case 'S' :  // --> SORT SIZE <--
					puts("\nSORT_SIZE -- not implemented");
					break;
				case '1' :			// --> ONE COLUMN <--
					puts("\nONE_COLUMN -- not implemented (default)");
					break;
				case '2' :			// -->TWO COLUMN <--
					puts("\nTWO_COLUMN -- not implemented");
					break;
				case '4' :			// --> FOUR COLUMN <--
					puts("\nFOUR_COLUMN -- not implemented");
					break;
				case '6' :			// --> SIX COLUMN <--
					puts("\nSIX_COLUMN -- not implemented");
					break;
			}  // switch
		} else {

			if( strchr(argv[argc], ':') != NULL ) { //Look for drive indicator ":"
				search_drive = toupper( *( strchr(argv[argc], ':') - 1 ) ); //Get letter
				argv[argc] = argv[argc] + 2; //Drop drive letter
			} else search_drive = *search_string; //Fallback to current drive letter
			
			strcpy(search_string, search_string + 2); //Drop drive letter
			
			if( argv[argc][0] != NULL ) { // Argument contains more than just drive letter?
				if( argv[argc][0] != '\\' ) sprintf(search_path, "%c:%s\\%s", search_drive, search_string, argv[argc]);
				else sprintf(search_path, "%c:%s", search_drive, argv[argc]);
			} else sprintf(search_path, "%c:\\", search_drive);
			
			if( search_path[ strlen(search_path) - 1 ] == '\\') strcat(search_path, "*.*");
		}  // if	
	}  // while



	SetConsoleTextAttribute(hConsole, 0x0F);	//Bright White label
	puts("\nHD");

	SetConsoleTextAttribute(hConsole, 0x03);	//Aqua (low intensity) path
	printf("Path: %s\n", search_path);

	for(i = 0; i < console_width; i++) i == console_width / 2 ? putchar(194) : putchar(196);  //Draw ------|-------
	putchar('\n');

	//Properly display contents of directory (w/o trailing backslash)
	dwAttrib = GetFileAttributes( (LPCTSTR) search_path );
	//Add a trailing backslash and wildcard pattern if needed
	if( dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) )
		strcat(search_path, "\\*.*");

	//Get first file
	search_handle = FindFirstFile((LPCTSTR)search_path, &file_data_t);
	if(INVALID_HANDLE_VALUE == search_handle) {
		puts("\nNo file or folder found.");
		//Restore console
		SetConsoleTextAttribute(hConsole, original_attributes);
		return -1;
	} else if(ERROR_FILE_NOT_FOUND == (long)search_handle) {
		puts("\nNo file or folder found.");
		//Restore console
		SetConsoleTextAttribute(hConsole, original_attributes);
		return -1;
	}

	do {
		file_counter++;
		
		//If current file is a directory
		if(file_data_t.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			SetConsoleTextAttribute(hConsole, 0x0D);  // ^ Light Purple ^
		else {
			//Get (lower case) file extension
			file_ext = tolower(strrchr(file_data_t.cFileName, '.'));
			if(  file_ext != NULL )
				//Set color based on file extension
				if( strcmp(file_ext, ".exe") == 0 || strcmp(file_ext, ".msi") == 0 )
					SetConsoleTextAttribute(hConsole, 0x0B);  // ^ Light Aqua ^
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
					SetConsoleTextAttribute(hConsole, 0x0F);  // ^ Bright White ^
				else if(strcmp(file_ext, ".bat") == 0 || strcmp(file_ext, ".cmd") == 0 || strcmp(file_ext, ".btm") == 0)
					SetConsoleTextAttribute(hConsole, 0x0C);  // ^ Light Red ^
				else if(strcmp(file_ext, ".com") == 0 || strcmp(file_ext, ".msc") == 0 )  // <-- Light Green
					SetConsoleTextAttribute(hConsole, 0x0A);
				else if(strcmp(file_ext, ".bas") == 0 || strcmp(file_ext, ".pas") == 0 || strcmp(file_ext, ".js") == 0
									|| strcmp(file_ext, ".jse") == 0 || strcmp(file_ext, ".vbs") == 0 || strcmp(file_ext, ".vbe") == 0
									|| strcmp(file_ext, ".wsf") == 0 || strcmp(file_ext, ".php") == 0 || strcmp(file_ext, ".py") == 0
									|| strcmp(file_ext, ".pl") == 0 || strcmp(file_ext, ".rb") == 0 || strcmp(file_ext, ".xsl") == 0
									|| strcmp(file_ext, ".tcl") == 0 || strcmp(file_ext, ".wsh") == 0)
					SetConsoleTextAttribute(hConsole, 0x02);  // ^ Green (low intensity) ^
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
									|| strcmp(file_ext, ".rm") == 0 || strcmp(file_ext, ".ram") == 0 || strcmp(file_ext, ".swf") == 0\
									|| strcmp(file_ext, ".vob") == 0 || strcmp(file_ext, ".pspimge") == 0 || strcmp(file_ext, ".thm") == 0
									|| strcmp(file_ext, ".yuv") == 0 || strcmp(file_ext, ".divx") == 0 || strcmp(file_ext, ".m4p") == 0
									|| strcmp(file_ext, ".mts") == 0 || strcmp(file_ext, ".pam") == 0)
					SetConsoleTextAttribute(hConsole, 0x0E);  // ^ Light Yellow^
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
					SetConsoleTextAttribute(hConsole, 0x06);  // ^ Yellow (low intensity) ^
				else SetConsoleTextAttribute(hConsole, 0x08);	// Gray (everything else)
			else SetConsoleTextAttribute(hConsole, 0x08);	// Gray (no extension)
		} // first if/else

		//TODO:Check if console screen is full
		if(++line_count == console_height)
			system("PAUSE");
		
		//"DARK RED" for hidden files
		if(file_data_t.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) SetConsoleTextAttribute(hConsole, 0x04);

		printf("%-*s", console_width / 2 - 8, file_data_t.cFileName);  //Print file name
		//printf("%s\n", file_ext);

/////////////////////////////Print file's size or <dir>//////////////////////	
		if(file_data_t.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) printf("  <dir> ");	
		else {
			file_size = (float)((file_data_t.nFileSizeHigh * ( MAXDWORD+1)) + file_data_t.nFileSizeLow);
			total_size += (int)file_size;
			SetConsoleTextAttribute(hConsole, 0x08);  // Grey
			
			// KB                    MB                             GB                             TB
			if(file_size > 1023) if((file_size /= 1024.0) > 1023) if((file_size /= 1024.0) > 1023) if((file_size /= 1024.0) > 1023)
							printf("%5.1f TB", file_size);
						else printf("%5.1f GB", file_size);
					else printf("%5.1f MB", file_size);
				else printf("%5.1f KB", file_size);
			else printf("%5.1f B ", file_size);
		}

		SetConsoleTextAttribute(hConsole, 0x03);	//Aqua (low intensity)
		printf("\263 \n");  // Print |
		//while(screen_info_t.dwCursorPosition.X < console_width) putchar(' ');
		//puts("");
/////////////////////////////////////////////////////////////////////////////
	} while( FindNextFile(search_handle, &file_data_t) != 0 );

	FindClose(search_handle);

	SetConsoleTextAttribute(hConsole, 0x03);	//Aqua foreground
	for(i = 0; i < console_width; i++) i == console_width / 2 ? putchar(193) : putchar(196);  //Draw ----|-----
	putchar('\n');

	SetConsoleTextAttribute(hConsole, 0x0B);  //Light Aqua
	printf(" %6d", file_counter);
	SetConsoleTextAttribute(hConsole, 0x0A);  //Light Green
	printf(" files totaling ");
	SetConsoleTextAttribute(hConsole, 0x0B);  //Light Aqua
	// KB                    MB                             GB                             TB
	if(total_size > 1023) if((total_size /= 1024.0) > 1023) if((total_size /= 1024.0) > 1023) if((total_size /= 1024.0) > 1023)
					printf("%5.1f TB", total_size);
				else printf("%5.1f GB", total_size);
			else printf("%5.1f MB", total_size);
		else printf("%5.1f KB", total_size);
	else printf("%5.1f B ", total_size);

	SetConsoleTextAttribute(hConsole, 0x0A);  //Light Green
	printf(" bytes consuming ");
	SetConsoleTextAttribute(hConsole, 0x0B);  //Light Aqua
	printf("%d", file_counter);
	SetConsoleTextAttribute(hConsole, 0x0A);  //Light Green
	puts(" bytes of disk space.");
	SetConsoleTextAttribute(hConsole, 0x0B);  //Light Aqua
	printf(" %6d", file_counter);
	SetConsoleTextAttribute(hConsole, 0x0A);  //Light Green
	printf(" bytes available on Drive ");
	SetConsoleTextAttribute(hConsole, 0x0B);  //Light Aqua
	printf("C:");
	SetConsoleTextAttribute(hConsole, 0x0A);  //Light Green
	printf(" \t\t Volume label: ");
	SetConsoleTextAttribute(hConsole, 0x0C);  //Light Red
	printf("C_DRIVE\n");

	SetConsoleTextAttribute(hConsole, original_attributes);  //Restore console
	return 0;
}
