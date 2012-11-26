/*******************************************************
*  Copyright 2012 Flavio Siotto
* 
*  addsecs is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

  while ( (argc-1)!=optind && ((c = getopt(argc, argv, ":f:s:vh")) != -1) ) {
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
	  printf("strptime error\n");
      printUsage(argv[0]);
      exit(-1);
    }
    free(start_s);
  }

  time_val.tm_sec += secs;

  if ( mktime(&time_val) == -1) {
    printf("mktime error\n");
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
