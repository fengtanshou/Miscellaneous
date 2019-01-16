#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define LOG "log"
#define PRELOG "oldlog"
#define DEFAULT_MAXLINE 1000

int main(int argc, char *argv[])
{
	FILE * flog;
	int  opt, maxlogline, i, curline;
	char *line = NULL;
	ssize_t line_size, len;

	maxlogline = DEFAULT_MAXLINE;
	while ((opt = getopt(argc, argv, "l:")) != -1) {
		switch (opt) {
			case 'l':
				maxlogline = atoi(optarg);
				break;
			default: /* '?' */
				fprintf(stderr, "Usage: %s [-l maxlogline] \n",
						argv[0]);
				exit(EXIT_FAILURE);
		}
	}

	curline = 0;

	//turncate PRELOG to zero
	flog = fopen(PRELOG, "w+");
	fclose(flog);

	flog = fopen(LOG,"w+");
	while ((len = getline(&line, &line_size, stdin)) != -1) {
		curline++;
		if( curline > maxlogline) {
			curline = 1;
			fclose(flog);
			rename(LOG, PRELOG);
			flog = fopen(LOG,"w+");
		}
		fputs(line, flog);
	}

	free(line);
	fclose(flog);

	return 0;
}

