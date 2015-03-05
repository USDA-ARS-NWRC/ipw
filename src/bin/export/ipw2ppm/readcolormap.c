/*
** NAME
**      readcolormap -- reads the colormap file and sets the CLUT
**
** DESCRIPTION
**
*/
#include "ipw.h"
#include "pgm.h"

void
readcolormap(void)
{
  char      s[80], fit[80], pixst[80], type;
  int       i, n;
  double    pixel = 0.0;
  double    pv;
  colors    c;

  parm.nM = 0;

  if (parm.cmfile != NULL) {
    parm.nD = 0;
    parm.nP = 0;
    parm.nM = 0;
    while ( fgets(s, 80, parm.cmfile) != NULL) {
      if (strcmp(s, "\n") == 0)
        continue;
      n = sscanf(s, "%c %s %d %d %d %s", &type, pixst, &c.r, &c.g, &c.b, fit);
      if (type != '#') {
        if (n < 5)
          error ("Colormap: %c <value> <red> <green> <blue>", type);
        if ( (n==6) && (strcmp(fit, "lin") != 0) )
          error ("Only linear [lin] fits are currently supported.");
        if ( (c.r < 0) || (c.g < 0) || (c.b < 0) ||
             (c.r > 255) || (c.g > 255) || (c.b > 255) )
          error ("rgb values must be between 0 and 255.");

        if (strcmp(pixst, "-") == 0)
          pixel = parm.minv;
        else if (strcmp(pixst, "-X") == 0)
          pixel = -parm.amaxv;
        else if (strcmp(pixst, "-F") == 0)
          pixel = parm.fpm[0];
        else if (strcmp(pixst, "+") == 0)
          pixel = parm.maxv;
        else if (strcmp(pixst, "+X") == 0)
          pixel = parm.amaxv;
        else if (strcmp(pixst, "+F") == 0)
          pixel = parm.fpm[ipow2(parm.nbits) - 1];
        else if (strcmp(pixst, "M") == 0) {
          parm.M.c = c;   parm.nM = 1;  type = 'M';
        } else
          (void) sscanf(pixst, "%lf", &pixel);
        if ( type == 'D' ) {
          parm.D[parm.nD].value = pixel;
          parm.D[parm.nD].c = c;
          parm.nD++;
        } else if ( type == 'P' ) {
          if ( (pixel < 0) || (pixel > ipow2(parm.nbits)) )
            error ("pixel value %f out of range.", pixel);
          parm.P[parm.nP].value = pixel;
          parm.P[parm.nP].c = c;
          parm.nP++;
        } else if (type != 'M' ) {
          error ("Unrecognized type %c", type);
        }
      }
    }
    (void) fclose(parm.cmfile);
  } else {
    if (parm.graymap) {
      parm.nD = 2;
      parm.D[0].value = parm.minv;
      parm.D[0].c = parm.gblack;

      parm.D[1].value = parm.maxv;
      parm.D[1].c = parm.gwhite;

      parm.nP = 0;
    } else {
      parm.nD = 3;
      parm.D[0].value = -parm.amaxv;
      parm.D[0].c = parm.gblue;

      parm.D[1].value = 0;
      parm.D[1].c = parm.gblack;

      parm.D[2].value = parm.amaxv;
      parm.D[2].c = parm.gred;

      parm.nP = 0;
    }
  }

  if ( !parm.nM ) {
    parm.nM = 1;
    parm.M.c = parm.gwhite;
  }

  if (parm.verbose) {
    for (i = 0; i < parm.nD; i++)
      fprintf(stderr, "data  value: %11.6f mapped to %3d %3d %3d\n",
              parm.D[i].value, parm.D[i].c.r, parm.D[i].c.g, parm.D[i].c.b);
    for (i = 0; i < parm.nP; i++)
      fprintf(stderr, "pixel value: %11.6f mapped to %3d %3d %3d\n",
              parm.P[i].value, parm.P[i].c.r, parm.P[i].c.g, parm.P[i].c.b);
    if (parm.imask != ERROR)
      fprintf(stderr, "                    mask mapped to %3d %3d %3d\n",
                      parm.M.c.r, parm.M.c.g, parm.M.c.b);
  }

  parm.CLUT  = (colors *) ecalloc(ipow2(parm.nbits), sizeof(colors));

  /*
   * If we are to print out a colorbar, we'll be looking up values that
   * don't exist in the file.  Otherwise, don't do an rgblookup on values
   * that don't exist.
   *
   * Graymaps are done seperately, bypassing the call to rgblookup.
   *
   * After setting up the CLUT by rgblookups, go through special pixel
   * color assignments and apply them to the CLUT.
   */

  if (parm.graymap) {
    pv = 255.0 / (parm.maxv - parm.minv);
    if (parm.cbar)
      for (i = 0; i < ipow2(parm.nbits); i++)
        parm.CLUT[i].r = (int) ( (parm.fpm[i] - parm.minv) * pv + 0.5 );
    else
      for (i = parm.minp; i <= parm.maxp; i++)
        if (parm.pbins[i])
          parm.CLUT[i].r = (int) ( (parm.fpm[i] - parm.minv) * pv + 0.5 );
  } else {
    if (parm.cbar)
      for (i = 0; i < ipow2(parm.nbits); i++)
        parm.CLUT[i] = rgblookup (i);
    else
      for (i = parm.minp; i <= parm.maxp; i++)
        if (parm.pbins[i])
          parm.CLUT[i] = rgblookup (i);

    for (i = 0; i < parm.nP; i++)
      parm.CLUT[(int) parm.P[i].value] = parm.P[i].c;
  }

}
