#include <stdio.h>
#include <stdlib.h>
#include "fb.h"

int main(int argc, char **argv)
{
	int width   = 564;
	int height  = 846;
	FILE *fp    = NULL;
	int nrd     = 0;
	size_t size = width * height * 3/2;
	unsigned char *raw_input	= NULL;

	fb_init();

	raw_input = (unsigned char *)malloc(size);
	if (raw_input == NULL)
		exit(1);

	fp = fopen("demo.yuv", "r");
	if (fp == NULL)
		exit(1);

	nrd = fread(raw_input, 1, size, fp);
	if (nrd != size)
		exit(1);

	fb_process(raw_input, width, height, 0);

	fb_exit();

	return 0;
}

