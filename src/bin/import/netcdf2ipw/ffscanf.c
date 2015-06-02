
#include "ipw.h"
#include "pgm.h"

int ffscanf(
	FILE	*file,
	int	 num,
	float	*flar)
{
  int ntoread;
  int nread;

  ntoread = num;

  while (ntoread > 15) {
    if ( (nread = fscanf(file, "%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f",
                               flar, flar+1, flar+2, flar+3, flar+4, flar+5,
                               flar+6, flar+7, flar+8, flar+9, flar+10,
                               flar+11, flar+12, flar+13, flar+14,
                               flar+15) ) != 16)
      return (num - ntoread - nread);
    ntoread -= nread;
    flar += 16;
  }
  while (ntoread > 7) {
    if ( (nread = fscanf(file, "%f%f%f%f%f%f%f%f", flar, flar+1, flar+2,
                               flar+3, flar+4, flar+5, flar+6, flar+7) ) != 8)
      return (num - ntoread - nread);
    ntoread -= nread;
    flar += 8;
  }
  while (ntoread > 0) {
    if ( (nread = fscanf(file, "%f", flar) ) != 1)
      return (num - ntoread - nread);
    ntoread -= nread;
    flar++;
  }
  return(num);
}
  
