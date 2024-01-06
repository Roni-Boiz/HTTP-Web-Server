/*
 * Response.c
 *
 *  Created on: Aug 19, 2023
 *      Author: Ronila
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include "Response.h"

char * render_static_file(char *file_path)
{
	FILE *file = fopen(file_path, "rb");

	if (file == NULL) {
		printf("Cannot open file %s or does not exist \n", file_path);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* temp = malloc(sizeof(char) * (fsize+1));
	char ch;
	int i = 0;
	while((ch = fgetc(file)) != EOF)
	{
		temp[i] = ch;
		i++;
	}
	fclose(file);
	return temp;
}

