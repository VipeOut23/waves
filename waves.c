/* crack */
/* sip */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <sysexits.h>

#include "sources.h"

#define HELPTEXT \
	"Waves.\t A little toy for plotting a sinus curve with text (stdin)\n"\
	"\n"\
	"Usage:\n"\
	"  waves [-t <text>] [-w <num>] [-W <num>] [-s <num>] [...]\n"\
	"\n"\
	"Options:\n"\
	"  -t --text\t\t Specify text to use in the curve instead of stdin\n"\
	"  -w --width\t\t Specify the width of a text \"chunk\"(default: 1)\n"\
	"  -W --output-width\t Specify the amplitude of the curve\n"\
	"\t\t\t (e.g. Terminal width) (default: 80\n"\
	"  -s --steps\t\t How many steps are made per curve cycle\n"\
	"\t\t\t (somewhat the reverse frequency) (default: 20)\n"\
	"  --sin-precision\t Specify, how many iterations of the taylor\n"\
	"\t\t\t series are used; Sane: 8-11(Default: 9)\n"\
	"  --sources\t\t Save the sources in ./waves-src.tgz\n"\
	"  -? -h --help\t\t Well, you're here now"

#define PI 3.1415 /* accurate enough */

static short *offsets;

static struct {
	int steps   ;
	int center  ;
	int text_len;
	int prec    ;
	char *text  ;
} settings;

float power(float base, int exponent)
{
	register float res = base;

	if(!exponent) return 1;

	if(exponent < 0)
		return 1/power(base, -exponent);

	while(--exponent)
		res *= base;

	return res;
}

long factorial(long n)
{
	register long res = n;

	if(!n) return 1;

	while(--n)
		res *= n;

	return res;
}

float taylor_sin(float rad, int prec)
{
	register int   a;
	register float sign, pr;
	register long  fac;
	register float res = 0;

	while(prec--) {
		a    = (2*prec) + 1;
		sign = power(-1, prec);
		pr   = power(rad, a);
		fac  = factorial(a);

		res += (sign*pr)/fac;
	}

	return res;
}

static inline void prepare()
{
	const int half_len = settings.text_len / 2;
	const int travel   = settings.center - half_len;
	const float step   = PI/settings.steps;

	int     off   = 0;
	float   pvt   = 0.0;

	offsets = malloc(sizeof(short)*2*settings.steps);

	/* calculate offsets */
	for(int i = 0; i < 2*settings.steps; ++i) {
		pvt += step;
		off = travel * taylor_sin(pvt, settings.prec);
		offsets[i] = off+settings.center+half_len;
	}
}

static inline void do_waves_text()
{
	int index = 0;
	int max   = settings.steps*2;

	for(;;) {
		if(++index >= max)
			index = 0;
		printf("%*s\n", offsets[index], settings.text);
	}
}

static inline void do_waves_stdin()
{
	int  index = 0;
	int  max   = settings.steps*2;
	char *buf  = calloc(1,settings.text_len+1);  // +1 for \0
	int  bindex = 0;

	for(char c = getc(stdin); c != EOF; c = getc(stdin)) {
		if(c != '\n' && c != '\r' && c != ' ')
			buf[bindex++] = c;
		/* flush */
		if(bindex >= settings.text_len) {
			if(++index >= max)
				index = 0;
			printf("%*s\n", offsets[index], buf);
			bindex = 0;
		}
	}

	/* print tail */
	if(bindex) {
		buf[bindex+1] = 0;
		if(++index >= max)
			index = 0;
		printf("%*s\n", offsets[index], buf);
	}

	free(buf);
}

static void cleanup()
{
	free(offsets);
	free(settings.text);
}

int main(int argc, char **argv)
{
	char *endptr;
	char c;
	int  parsed;

	int           longind    = 0;
	const char    *optstring = "t:w:W:s:h?";
	struct option longopts[] = {
				    {"text", required_argument, NULL, 't'},
				    {"width", required_argument, NULL, 'w'},
				    {"output-width", required_argument, NULL, 'W'},
				    {"steps", required_argument, NULL, 's'},
				    {"sin-precision", required_argument, NULL, 0},
				    {"sources", no_argument, NULL, 0},
				    {"help", no_argument, NULL, 'h'},
				    {NULL, no_argument, NULL, 0}
	};

	/* defaults */
	settings.text_len = 1;
	settings.center   = 40;
	settings.steps    = 20;
	settings.prec     = 9;

	while( (c = getopt_long(argc, argv, optstring, longopts, &longind)) != -1) {
		switch(c) {
		case 't':
			settings.text     = strdup(optarg);
			settings.text_len = strlen(optarg);

			break;
		case 'w':
			parsed = strtol(optarg, &endptr, 10);
			if(endptr == optarg) {
				fprintf(stderr, "%s is not a number.\n", optarg);
				return EX_USAGE;
			}
			settings.text_len = parsed;

			break;
		case 'W':
			parsed = strtol(optarg, &endptr, 10);
			if(endptr == optarg) {
				fprintf(stderr, "%s is not a number.\n", optarg);
				return EX_USAGE;
			}
			settings.center = parsed/2;

			break;
		case 's':
			parsed = strtol(optarg, &endptr, 10);
			if(endptr == optarg) {
				fprintf(stderr, "%s is not a number.\n", optarg);
				    return EX_USAGE;
			}
			settings.steps = parsed;

			break;
		case 'h': /* fallthrough */
		case '?':
			puts(HELPTEXT);
			return 0;
		case 0:
		    if(!strcmp("sin-precision", longopts[longind].name)) {
			    parsed = strtol(optarg, &endptr, 10);
			    if(endptr == optarg) {
				    fprintf(stderr, "%s is not a number.\n", optarg);
				    return EX_USAGE;
			    }
			    if(parsed > 11) {
				    fprintf(stderr, "Precision > 11 is not possible\n");
				    return EX_USAGE;
			    }
			    settings.prec = parsed;
		    }

		    if(!strcmp("sources", longopts[longind].name)) {
			    switch(sources_write("waves-src.tgz")) {
			    case 1: fprintf(stderr, "Sources not included...\n");
				    return EX_UNAVAILABLE;
			    case 2: fprintf(stderr, "Could not open \"./waves-src.tgz\"...\n");
				    return EX_IOERR;
			    case 3: fprintf(stderr, "Write error occoured...\n");
				    return EX_IOERR;
			    }
			    cleanup();
			    return 0;
		    }
		    break;
		}
	}

	prepare();
	if(settings.text)
		do_waves_text();
	else
		do_waves_stdin();
	cleanup();

	return 0;
}
