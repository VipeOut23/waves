#ifndef __SOURCES_H
#define __SOURCES_H
#include <stdio.h>

unsigned int SOURCETAR_LEN;
unsigned char *SOURCETAR_BYTES;

int sources_write(const char *fname)
{
	size_t written;
	FILE *fp;

	if(!SOURCETAR_BYTES || !SOURCETAR_LEN)
		return 1;

	fp = fopen(fname, "w");
	if(!fp)
		return 2;

	written = fwrite(SOURCETAR_BYTES, 1, SOURCETAR_LEN, fp);

	if(written < SOURCETAR_LEN) {
		fclose(fp);
		return 3;
	}

	fclose(fp);
	return 0;
}
#endif
