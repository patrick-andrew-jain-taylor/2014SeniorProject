#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include "libcsv/csv.h"

struct counts {
	int fields;
	int rows;
	int frameCount;
	uint16_t frame[160][120];
};

void cb1 (void *s, size_t len, void *data) {
	((struct counts *)data)->frame
		[((struct counts *)data)->fields%160][((struct counts *)data)->rows%120] = atoi(s);
	printf("%u\t", 
		((struct counts *)data)->frame
			[((struct counts *)data)->fields%160][((struct counts *)data)->rows%120]);
	((struct counts *)data)->fields++;
	}
void cb2 (int c, void *data) {
	printf("\n");
	((struct counts *)data)->rows++;
	if (((struct counts *)data)->rows%120 == 0) printf("Frame %d\n", ((struct counts *)data)->frameCount++); 
	}


	
int main (int argc, char *argv[]) {
	FILE *fp;
	struct csv_parser p;
	char buf[1024];
	size_t bytes_read;
	uint16_t frame[160][120];
	struct counts c = {0, 0, 0};
	
	if (csv_init(&p, 0) != 0) exit(EXIT_FAILURE);
	fp = fopen(argv[1], "rb");
	if (!fp) exit(EXIT_FAILURE);
		
	while ((bytes_read=fread(buf, 1, 1024, fp)) > 0)
		if (csv_parse(&p, buf, bytes_read, cb1, cb2, &c) != bytes_read) {
			fprintf(stderr, "Error while parsing file: %s\n",
			csv_strerror(csv_error(&p)) );
			exit(EXIT_FAILURE);
		}
		
	csv_fini(&p, cb1, cb2, &c);
	
	fclose(fp);
	printf("%d fields, %d rows\n", c.fields, c.rows);
	
	csv_free(&p);
	exit(EXIT_SUCCESS);
}
