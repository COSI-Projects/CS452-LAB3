#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

char *io_read_file(const char *file) {
	FILE *f;
	long flen;
	char *data;

	assert(file != NULL);
	data = NULL;

	f = fopen(file, "r");

	if (!f)
		return NULL;

	if (fseek(f, 0, SEEK_END))
		goto error;

	if (!(flen = ftell(f)))
		goto error;

	rewind(f);

	if (!(data = malloc(flen))) {
		fputs("Insufficient memory!\n", stderr);
		return NULL;
	}

	if (!fread(data, flen, 1, f))
		goto error;

	return data;

error:
	perror(file);
	if (data)
		free(data);
	fclose(f);
	return NULL;
}
