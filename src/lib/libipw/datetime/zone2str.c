#include <string.h>

#include "ipw.h"

char *
zone2str(
	int zone,	/* minutes west of Greenwich */
	int isdst)	/* is Daylight Savings Time? */
{
  static char z[50];
  char *zp;
  int   hr, min;

  switch (zone) {
		/* New Zealand */
    case -720:	strcpy(z, "NZ%T");	break;
		/* Australia/NSW (Tasmania,Queensland) */
    case -600:	strcpy(z, "EST");	break;
		/* Australia/North */
    case -570:	strcpy(z, "CST");	break;
		/* Japan if not dst, Korean otherwise */
    case -540:  strcpy(z,  (isdst) ? "K%T" : "JST");	break;
		/* Australia/West if not dst, PRChina otherwise (could also be
		   CST:ROC, HKT:HongKong, SST:Singapore) */
    case -480:	strcpy(z,  (isdst) ? "C%T" : "WST");	break;
		/* Iran */
    case -210:	strcpy(z, "I%T");	break;
		/* Turkey (West Soviet Union) */
    case -180:	strcpy(z, "EET%");	break;
		/* East Europe */
    case -120:	strcpy(z, "EET%");	break;
		/* Middle Europe (Poland) */
		/* Also known as CET for Central European Time */
    case  -60:	strcpy(z, "MET%");	break;
		/* UTC time if not dst, otherwise British Summer Time */
		/* Ought to be WET% for Western Europe & Iceland */
    case    0:	strcpy(z,  (isdst) ? "BST" : "GMT");	break;
		/* Brazil/DeNoranha */
    case  120:	strcpy(z, "F%T");	break;
		/* Brazil/East */
    case  180:	strcpy(z, "E%T");	break;
		/* Canada/NewFoundland */
    case  210:	strcpy(z, "N%T");	break;
		/* Canada/Atlantic (W%T: Brazil/West, C%T: Chile/Continental) */
    case  240:	strcpy(z, "A%T");	break;
		/* US/Eastern (Canada/Eastern, Cuba, A%T: Brazil/Acre */
    case  300:	strcpy(z, "E%T");	break;
		/* US/Central (Canada/Central, Mexico/Central, E%T:Chile/EasterIsland */
    case  360:	strcpy(z, "C%T");	break;
		/* US/Mountain (Canada/Mountain, Mexico/BajaSur) */
    case  420:	strcpy(z, "M%T");	break;
		/* US/Pacific (Canada/Pacific, Mexico/BajaNorte) */
    case  480:	strcpy(z, "P%T");	break;
		/* US/Alaska (Y%T: Canada/Yukon) */
    case  540:	strcpy(z, "AK%T");	break;
		/* US/Hawaii if not dst, otherwise US/Aleutian */
    case  600:	strcpy(z,  (isdst) ? "HA%T" : "HST");	break;
		/* US/Samoa */
    case  660:	strcpy(z, "SST");	break;

    default:
			hr = zone / 60;
			min = ABS(zone - (hr * 60));
			hr = -hr;
			(void) sprintf(z, "GMT%+d:%02d", hr, min);
			break;
  }

  zp = strchr(z, '%');
  if (zp != NULL)
    *zp = (isdst) ? 'D' : 'S';

  return(z);
}


#ifdef TEST_MAIN

void main() {
  int i;

  for (i=-13*60; i < (14*60); i+= 30) {
    printf("%+4d, %d, Zone %20s\n", i, 0, zone2str(i,0) );
    printf("%+4d, %d, Zone %20s\n", i, 1, zone2str(i,1) );
  }
}
#endif
