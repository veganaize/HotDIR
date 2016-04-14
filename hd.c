#include <stdio.h>
#include <string.h>
#include <windows.h>

//TODO: 
//	Implement a counter and determine if number of output lines
//	will surpass console's max display lines.

#define SORT_NAME	1
#define SORT_EXT	2
#define SORT_SIZE	4
#define SORT_DATE	8
#define ONE_COLUMN	16
#define TWO_COLUMN	32
#define FOUR_COLUMN	64
#define SIX_COLUMN	128


int main(int argc, char* argv[]) {

	HANDLE hConsole;
	HANDLE search_handle;
	CONSOLE_SCREEN_BUFFER_INFO screen_info_t;
	WIN32_FIND_DATA file_data_t;
	WORD  original_attributes;	
	SHORT console_width;
	//??? console_height;
	char search_string[MAX_PATH];
	char* file_ext = NULL;
	int i;
	
	GetCurrentDirectory(MAX_PATH, search_string);
	strcat(search_string, "\\*.*");
	
	/*
	 * Process command line arguments
	 *
	 */
	while(--argc > 0) {
		if( *(argv[argc]) == '/' ) {
			switch( (int)*(argv[argc]+1) ) {
				case 'h' : case 'H' : case '?':
					puts("\nHELP");
					return 0;
				case 'n' : case 'N' :
					puts("\nSORT_NAME");
					break;
				case 'e' : case 'E' :
					puts("\nSORT_EXT");
					break;
				case 'd' : case 'D' :
					puts("\nSORT_DATE");
					break;
				case 's' : case 'S' :
					puts("\nSORT_SIZE");
					break;
				case '1' :
					puts("\nONE_COLUMN");
					break;
				case '2' :
					puts("\nTWO_COLUMN");
					break;
				case '4' :
					puts("\nFOUR_COLUMN");
					break;
				case '6' :
					puts("\nSIX_COLUMN");
					break;					
			}  // switch
		} else {
			GetCurrentDirectory(MAX_PATH, search_string);
			strcat(search_string, "\\");
			strcat(search_string, argv[argc]);

		} // if

	}  // while

/*	printf("\nARGC:  %d\n", argc);
	for(i = 0; i < argc; i++) {
		printf(" %c\n", *(argv[i]+1));
	}
*/

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &screen_info_t);  //Get console info

	original_attributes = screen_info_t.wAttributes;  //Save console info
	console_width = screen_info_t.srWindow.Right;  //Get screen width

	SetConsoleTextAttribute(hConsole, 0x0F);	//White foreground label
	puts("\nveganaiZe");
	
	SetConsoleTextAttribute(hConsole, 0x03);	//Aqua foreground path
	printf("Path: %s\n", search_string);
	
	for(i = 0; i < console_width; i++) putchar(196);  //Draw -------------
	putchar('\n');
	
	//Get first file
	search_handle = FindFirstFile((LPCTSTR)search_string, &file_data_t);
	if(INVALID_HANDLE_VALUE == search_handle) {
		puts("\nFile or Folder Not Found!");
		return -1;
	} else if(ERROR_FILE_NOT_FOUND == (long)search_handle) {
		puts("\nNormal Error?");
		return -1;
	}
	
	do {	//If current file is a directory
		if(file_data_t.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			SetConsoleTextAttribute(hConsole, 0x0D);  //Light Purple
		else {
			//Get file extension
			file_ext = strrchr(file_data_t.cFileName, '.');
			if(  file_ext != NULL )
				//Set color for file type
				if( strcmp(file_ext, ".exe") == 0 || strcmp(file_ext, ".msi") == 0 )
					SetConsoleTextAttribute(hConsole, 0x0B);  //Light Aqua
				else if(strcmp(file_ext, ".txt") == 0 || strcmp(file_ext, ".doc") == 0 || strcmp(file_ext, ".c") == 0 || strcmp(file_ext, ".rtf") == 0)
					SetConsoleTextAttribute(hConsole, 0x0F);  //Bright White
				else if(strcmp(file_ext, ".bat") == 0 || strcmp(file_ext, ".cmd") == 0 || strcmp(file_ext, ".btm") == 0)
					SetConsoleTextAttribute(hConsole, 0x0C);  //Light Red
				else if(strcmp(file_ext, ".bas") == 0 || strcmp(file_ext, ".pas") == 0 || strcmp(file_ext, ".js") == 0 || strcmp(file_ext, ".tcl") == 0
									|| strcmp(file_ext, ".jse") == 0 || strcmp(file_ext, ".vbs") == 0 || strcmp(file_ext, ".vbe") == 0
									|| strcmp(file_ext, ".wsf") == 0 || strcmp(file_ext, ".php") == 0 || strcmp(file_ext, ".py") == 0
									|| strcmp(file_ext, ".pl") == 0 || strcmp(file_ext, ".rb") == 0 || strcmp(file_ext, ".xsl") == 0 )
					SetConsoleTextAttribute(hConsole, 0x02);  //Green (low intensity)
				else if(strcmp(file_ext, ".mp3") == 0 || strcmp(file_ext, ".mpg") == 0 || strcmp(file_ext, ".mpeg") == 0
									|| strcmp(file_ext, ".jpg") == 0 || strcmp(file_ext, ".jpeg") == 0 || strcmp(file_ext, ".gif") == 0
									|| strcmp(file_ext, ".png") == 0 || strcmp(file_ext, ".tif") == 0 || strcmp(file_ext, ".tiff") == 0
									|| strcmp(file_ext, ".psd") == 0 || strcmp(file_ext, ".xcf") == 0 || strcmp(file_ext, ".svg") == 0
									|| strcmp(file_ext, ".mp4") == 0 || strcmp(file_ext, ".mkv") == 0 || strcmp(file_ext, ".avi") == 0
									|| strcmp(file_ext, ".mov") == 0 || strcmp(file_ext, ".pcx") == 0 || strcmp(file_ext, ".wav") == 0
									|| strcmp(file_ext, ".aif") == 0 || strcmp(file_ext, ".aiff") == 0 || strcmp(file_ext, ".eps") == 0
									|| strcmp(file_ext, ".ico") == 0 || strcmp(file_ext, ".xpm") == 0 || strcmp(file_ext, ".jpe") == 0
									|| strcmp(file_ext, ".wmf") == 0 || strcmp(file_ext, ".pdf") == 0 || strcmp(file_ext, ".bmp") == 0
									|| strcmp(file_ext, ".tga") == 0 || strcmp(file_ext, ".xbm") == 0 || strcmp(file_ext, ".pnm") == 0
									|| strcmp(file_ext, ".ppm") == 0 || strcmp(file_ext, ".mng") == 0 || strcmp(file_ext, ".cur") == 0
									|| strcmp(file_ext, ".ani") == 0 || strcmp(file_ext, ".svgz") == 0 || strcmp(file_ext, ".ai") == 0
									)
					SetConsoleTextAttribute(hConsole, 0x0A);  //Light Green
				else if(strcmp(file_ext, ".7z") == 0 || strcmp(file_ext, ".zip") == 0 || strcmp(file_ext, ".gz") == 0
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
									|| strcmp(file_ext, ".par") == 0 || strcmp(file_ext, ".par2") == 0 || strcmp(file_ext, ".img") == 0 )
					SetConsoleTextAttribute(hConsole, 0x06);  //Yellow (low intensity)
				else SetConsoleTextAttribute(hConsole, 0x08);	// Gray (for no ext)
		}

		printf("%s\n", file_data_t.cFileName);  //Print file name
		
	} while( FindNextFile(search_handle, &file_data_t) != 0 );
	
	FindClose(search_handle);
	
	SetConsoleTextAttribute(hConsole, 0x03);	//Aqua foreground
	for(i = 0; i < console_width; i++) putchar(196);  //Draw ---------
	putchar('\n');

	SetConsoleTextAttribute(hConsole, 0x0A);
	puts(" NN files totaling NN bytes consuming NN bytes of disk space.");  //FIXME
	puts(" NN bytes available on Drive C: \t\t Volume label:C_DRIVE");  //FIXME
	
	SetConsoleTextAttribute(hConsole, original_attributes);  //Restore console
	return 0;
}
