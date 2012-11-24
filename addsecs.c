/*******************************************************
 *  addsecs:
 *  add n seconds from current date or (optionally) from a given date,
 *  if no format is passed, assumed locale's appropriate date and time representation 
 * 
 *  History:
 *  23/11/2012: Flavio Siotto
 *   First Implementation
 *
*******************************************************/

#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define MAXBUFSIZE 80

#define VERSION "0.8"

static int isinteger(const char *s);
void printUsage(char *progr_name);

int main(int argc, char **argv)
{
  time_t now;
  struct tm now_tm, then_tm;
  char buffer[MAXBUFSIZE];
  char *format_s = NULL;
  char *start_s = NULL;
  int c;
  long secs;
  char *ret;

  while ( (argc-1)!=optind && ((c = getopt(argc, argv, ":f:s:vh")) != -1) ) {
	  switch (c) {
	  case 'f':
	    format_s = (char *)malloc(strlen(optarg));
      strncpy(format_s, optarg, strlen(optarg));
      break;
	  case 's':
	    start_s = (char *)malloc(strlen(optarg));
      strncpy(start_s, optarg, strlen(optarg));
      break;
    case 'h':
      printUsage(argv[0]);
      exit(0);
    case 'v':
      printf("%s: version "VERSION, argv[0]);
      exit(0);
	  case '?':
	  default:
      printUsage(argv[0]);
      exit(-1);
	  }
  }

  if ( ! isinteger(argv[argc-1]) ) {
    printUsage(argv[0]);
  	exit(-1);
  }

  secs = atol(argv[argc-1]);

	memset(&now_tm, 0, sizeof(struct tm));
	memset(&then_tm, 0, sizeof(struct tm));
	now_tm.tm_year = 1900;

  if ( format_s == NULL ) {
    format_s = (char *) malloc(strlen("%c"));
    strcpy(format_s, "%c", strlen("%c"));
  }

  if ( start_s == NULL ) {
    now = time(NULL);
    now_tm = *localtime(&now);
  } else {
    ret = strptime( start_s, format_s, &now_tm);
	  if ( ret == NULL || strlen(ret)!=0 ) {
	    printf("ret %s\n", ret);
      printUsage(argv[0]);
    	exit(-1);
		}
	}
  memcpy( &then_tm, &now_tm, sizeof(struct tm));
  then_tm.tm_sec += secs;
  mktime(&then_tm);

  strftime( buffer, MAXBUFSIZE, format_s, &then_tm);
  printf("%s\n",buffer);

  return 0;

}

/*
 * numeric - verify that string is an integer number
 */
static int isinteger(const char *s)
{
	/* skip any sign */
	if ( ((unsigned char)*s) == '-' || ((unsigned char)*s) == '+' )
		s++;

	while (*s) {
		if (!isdigit((unsigned char)*s)) {
			break;
		}
		s++;
	}
	return (*s == '\0');
}


void printUsage( char *progr_name ) {
  printf ("Usage: %s [-f <format>] [-s <start>] secs\n", progr_name);

/*"%a	Abbreviated weekday name *	Thu"\
"%A	Full weekday name * 	Thursday"\
"%b	Abbreviated month name *	Aug"\
"%B	Full month name *	August"\
"%c	Date and time representation *	Thu Aug 23 14:55:02 2001"\
"%d	Day of the month (01-31)	23"\
"%H	Hour in 24h format (00-23)	14"\
"%I	Hour in 12h format (01-12)	02"\
"%j	Day of the year (001-366)	235"\
"%m	Month as a decimal number (01-12)	08"\
"%M	Minute (00-59)	55"\
"%p	AM or PM designation	PM"\
"%S	Second (00-61)	02"\
"%U	Week number with the first Sunday as the first day of week one (00-53)	33"\
"%w	Weekday as a decimal number with Sunday as 0 (0-6)	4"\
"%W	Week number with the first Monday as the first day of week one (00-53)	34"\
"%x	Date representation *	08/23/01"\
"%X	Time representation *	14:55:02"\
"%y	Year, last two digits (00-99)	01"\
"%Y	Year	2001"\
"%Z	Timezone name or abbreviation	CDT"\
"%%	A % sign	%"\
*/

}
