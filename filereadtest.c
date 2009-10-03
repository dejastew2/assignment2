#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	int reset_n_value;
	int n_value;
	int i;
	
	reset_n_value = 0;
	n_value = 10;
	
	for (i = 1; i < argc; i ++) {
	
		char* file_name;
		
		file_name = (char*)malloc(strlen(argv[i] + 1));
		strcpy(file_name, argv[i]);
		
		if ((i == 1) && (file_name[0] == '-')) {
			if (file_name[1] == 'n') {
				reset_n_value = 1;
			} else {
				printf("Invalid parameter (-n expected)\n");
				exit(1);
			}
		} else if((i == 2) && (reset_n_value == 1)) {
			int new_n;
			new_n = atoi(file_name);
			if (new_n > 0) {
				n_value = new_n;
			} else {
				printf("Error reading -n option. Must be a positive int!\n");
				exit(1);
			}
		} else {
			printf("Argument %d: %s\n", i, argv[i]);
		}
	}
	
	printf("N value is currently: %d\n", n_value);
	
	return 0;
}
