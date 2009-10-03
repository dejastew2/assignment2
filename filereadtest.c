#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {

	int i;
	for (i = 1; i < argc; i ++) {
	
		char* file_name;
		
		file_name = (char*)malloc(strlen(argv[i] + 1));
		strcpy(file_name, argv[i]);
		
		if ((i == 1) && (file_name[0] == '-')) {
			if (file_name[1] == 'n') {
				printf("-n parameter given!\n");
			} else {
				printf("Invalid parameter (-n expected)\n");
				exit(1);
			}
		} else {
			printf("Argument %d: %s\n", i, argv[i]);
		}
	}
	
	return 0;
}
