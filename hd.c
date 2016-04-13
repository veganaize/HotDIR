#include <stdio.h>
#include <string.h>
#include <windows.h>

int main(int argc, char* argv[]) {

	HANDLE hConsole;
	HANDLE search_handle;
	CONSOLE_SCREEN_BUFFER_INFO screen_info_t;
	WIN32_FIND_DATA file_data_t;
	WORD  original_attributes;	
	SHORT console_width;
	char current_path[MAX_PATH];
	char* file_ext = NULL;
	int i;
	
	GetCurrentDirectory(MAX_PATH, current_path);
	
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &screen_info_t);

	original_attributes = screen_info_t.wAttributes;
	console_width = screen_info_t.srWindow.Right;

	SetConsoleTextAttribute(hConsole, 0x0F);	//White foreground
	puts("\nveganaiZe");
	
	SetConsoleTextAttribute(hConsole, 0x03);	//Aqua foreground
	printf("Path: %s\\*.*\n", current_path);
	
	for(i = 0; i < console_width; i++) putchar(196);
	putchar('\n');
	
	// get first file
	search_handle = FindFirstFile((LPCTSTR)"*", &file_data_t);
	if(INVALID_HANDLE_VALUE == search_handle) return -1;
	
	do {
		if(file_data_t.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			SetConsoleTextAttribute(hConsole, 0x0D);
		else {
			// get file extension
			file_ext = strrchr(file_data_t.cFileName, '.');
			if(  file_ext != NULL )
				// set color for file type
				if( strcmp(file_ext, ".exe") == 0 || strcmp(file_ext, ".msi") == 0 )
					SetConsoleTextAttribute(hConsole, 0x0B);
				else if(strcmp(file_ext, ".txt") == 0 || strcmp(file_ext, ".doc") == 0 || strcmp(file_ext, ".c") == 0)
					SetConsoleTextAttribute(hConsole, 0x0F);
				else if(strcmp(file_ext, ".bat") == 0 || strcmp(file_ext, ".cmd") == 0 || strcmp(file_ext, ".btm") == 0)
					SetConsoleTextAttribute(hConsole, 0x0C);
				else if(strcmp(file_ext, ".bas") == 0 || strcmp(file_ext, ".pas") == 0 || strcmp(file_ext, ".js") == 0 || strcmp(file_ext, ".tcl") == 0
									|| strcmp(file_ext, ".jse") == 0 || strcmp(file_ext, ".vbs") == 0 || strcmp(file_ext, ".vbe") == 0
									|| strcmp(file_ext, ".wsf") == 0 || strcmp(file_ext, ".php") == 0 || strcmp(file_ext, ".py") == 0
									|| strcmp(file_ext, ".pl") == 0 || strcmp(file_ext, ".rb") == 0 || strcmp(file_ext, ".xsl") == 0 )
					SetConsoleTextAttribute(hConsole, 0x02);
				else SetConsoleTextAttribute(hConsole, 0x08);	// Gray (for no ext)
		}

		printf("%s\n", file_data_t.cFileName);
	} while( FindNextFile(search_handle, &file_data_t) != 0 );
	
	FindClose(search_handle);
	
	SetConsoleTextAttribute(hConsole, 0x03);	//Aqua foreground
	for(i = 0; i < console_width; i++) putchar(196);
	putchar('\n');

	SetConsoleTextAttribute(hConsole, 0x0A);
	puts(" NN files totaling NN bytes consuming NN bytes of disk space.");  //FIXME
	puts(" NN bytes available on Drive C: \t\t Volume label:C_DRIVE");  //FIXME
	
	SetConsoleTextAttribute(hConsole, original_attributes);
	return 0;
}
