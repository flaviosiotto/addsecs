/*******************************************************
*  Copyright 2012 Flavio Siotto
*
*  addsecs is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  addsecs is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*  addsecs:
*  add n seconds from current date or (optionally) from a given date,
*  if no format is passed, assumed locale's appropriate date and time representation 
* 
*  History:
*  23/11/2012: Flavio Siotto
*   First Implementation
*
*******************************************************/

#define _XOPEN_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXBUFSIZE 80

#define VERSION "0.8"

static int isinteger(const char *s);
void printUsage(char *progr_name);

int main(int argc, char **argv)
{
  time_t now;
  struct tm time_val;
  char buffer[MAXBUFSIZE];
  char *format_s = NULL;
  char *start_s = NULL;
  char *ret_s = NULL;
  int c, len_opt;
  long secs;
  
  if (argc == 2 && *argv[1]=='-' && *(argv[1]+1) == 'h' ) {
    printUsage(argv[0]);
    exit(0);
  }

  if (argc == 2 && *argv[1]=='-' && *(argv[1]+1) == 'v' ) {
    printf("%s: version "VERSION"\n", argv[0]);
    exit(0);
  }
	

  while ( (argc-1)!=optind && ((c = getopt(argc, argv, "f:s:")) != -1) ) {
    switch (c) {
    case 'f':
      len_opt = strlen(optarg);
      format_s = (char *)malloc(len_opt+1);
      strncpy(format_s, optarg, len_opt);
      format_s[len_opt]='\0';
      break;
    case 's':
      len_opt = strlen(optarg);
      start_s = (char *)malloc(len_opt+1);
      strncpy(start_s, optarg, len_opt);
      start_s[len_opt]='\0';
      break;
	case '?':
	default:
      printUsage(argv[0]);
      exit(-1);
	}
  }

  if ( ! isinteger(argv[argc-1]) ) {
	fprintf(stderr, "invalid argument, %s is not an integer\n", argv[argc-1]);
    printUsage(argv[0]);
  	exit(-1);
  }

  secs = atol(argv[argc-1]);

  now = time(NULL);
  time_val = *localtime(&now);

  if ( format_s == NULL ) {
    format_s = (char *) malloc(3);
    strncpy(format_s, "%c", 2);
    format_s[2]='\0';
  }

  if ( start_s != NULL ) {
	ret_s = strptime( start_s, format_s, &time_val);
    if ( ret_s == NULL || *ret_s != '\0' ) {
	  fprintf(stderr, "strptime error\n");
      printUsage(argv[0]);
      exit(-1);
    }
    free(start_s);
  }

  time_val.tm_sec += secs;

  if ( mktime(&time_val) == -1) {
    fprintf(stderr, "mktime error\n");
    printUsage(argv[0]);
    exit(-1);
  }

  strftime( buffer, MAXBUFSIZE, format_s, &time_val);
  printf("%s\n", buffer);

  free(format_s);

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
  printf("\nPrint the current date adding seconds passed as argument.\n\n");
  printf ("Usage: %s [-f <format>] [-s <start>] secs\n", progr_name);
  printf ("       %s -v\n", progr_name);
  printf ("       %s -h\n", progr_name);
  printf("Options:\n"\
		"\t-f <format>\n"\
		"\t\tSymbol	Description\n"\
		"\t\t%%a	Abbreviated weekday name -> Thu\n"\
		"\t\t%%A	Full weekday name -> Thursday\n"\
		"\t\t%%b	Abbreviated month name -> Aug\n"\
		"\t\t%%B	Full month name -> August\n"\
		"\t\t%%c	Date and time representation -> Thu Aug 23 14:55:02 2001\n"\
		"\t\t%%d	Day of the month (01-31) -> 23\n"\
		"\t\t%%H	Hour in 24h format (00-23) -> 14\n"\
		"\t\t%%I	Hour in 12h format (01-12) -> 02\n"\
		"\t\t%%j	Day of the year (001-366) -> 235\n"\
		"\t\t%%m	Month as a decimal number (01-12) -> 08\n"\
		"\t\t%%M	Minute (00-59) -> 55\n"\
		"\t\t%%p	AM or PM designation -> PM\n"\
		"\t\t%%S	Second (00-61) -> 02\n"\
		"\t\t%%U	Week number with the first Sunday\n\t\t as the first day of week one (00-53) -> 33\n"\
		"\t\t%%w	Weekday as a decimal number\n\t\t with Sunday as 0 (0-6) -> 4\n"\
		"\t\t%%W	Week number with the first Monday\n\t\t as the first day of week one (00-53) -> 34\n"\
		"\t\t%%x	Date representation -> 08/23/01\n"\
		"\t\t%%X	Time representation -> 14:55:02\n"\
		"\t\t%%y	Year, last two digits (00-99) -> 01\n"\
		"\t\t%%Y	Year -> 2001\n"\
		"\t\t%%Z	Timezone name or abbreviation -> CDT\n"\
		"\t\t%%	A %% sign -> %%\n"\
		"\t-s <start>\n"\
		"\t\tInstead of current date, add seconds from the <start> value passed.\n"\
		"\t\tIf no format specified, assumed %%c.\n"\
		"\t-h\n"\
		"\t\tThis help message.\n"\
		"\t-v\n"\
		"\t\tPrint the version number.\n"\
		);

}
