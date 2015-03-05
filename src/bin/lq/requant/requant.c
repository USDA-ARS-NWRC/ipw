/*
** NAME
**      requant -- read image and requantize
**
*/
#include "ipw.h"
#include "bih.h"
#include "gethdrs.h"
#include "lqh.h"
#include "fpio.h"
#include "pgm.h"

void
requant(
	int       fdi,
	bool_t    got_lqh,
	int       fdh,
	bool_t    got_lohi,
	fpixel_t *newmm,
	bool_t    got_bits,
	int       newbits,
	bool_t    got_verb,
	int       fdo)
{
  BIH_T    **i_bihpp;	/* -> input BIH array		 */
  BIH_T    **o_bihpp;	/* -> output BIH array		 */
  LQH_T    **i_lqhpp;	/* -> input LQH array		 */
  LQH_T    **h_lqhpp;	/* -> new LQH array		 */
  LQH_T    **o_lqhpp;	/* -> output LQH array		 */
  int        nlines;	/* # lines in image		 */
  int        nsamps;	/* # samples per line		 */
  int        nbands;	/* # bands per sample		 */
  int        band;	/* band index			 */
  pixel_t    ival[2];
  fpixel_t  *fpbuf;

  static GETHDR_T h_lqh = {LQH_HNAME, (ingest_t) lqhread};
  static GETHDR_T *request[] = {&h_lqh, 0};

	/* read BIH */

  i_bihpp = bihread(fdi);
  if (i_bihpp == NULL) {
    error("can't read basic image header");
  }
  nbands = hnbands(fdi);
  nsamps = hnsamps(fdi);
  nlines = hnlines(fdi);
  h_lqhpp = NULL;

	/* create output BIH */

  o_bihpp = bihdup(i_bihpp);
  if (o_bihpp == NULL) {
    error("can't create basic image header");
  }

	/* get past the BIH of the header file */
	
  if (got_lqh) {
    if (bihread(fdh) == NULL) {
      error("can't read dummy BIH of file with new LQH");
    }

    if (hnbands(fdh) != nbands) {
      error("input image and LQH file have different # bands");
    }

	/* read new LQH */

    gethdrs(fdh, request, NO_COPY, fdo);

    if ((h_lqhpp = (LQH_T **) hdr_addr(h_lqh)) == NULL) {
      error("new LQH not valid");
    }

	/* adjust nbits field of output image to correspond with new LQH */

    for (band = 0; band < nbands; ++band) {
      if (h_lqhpp[band] != NULL) {
        bih_nbits(o_bihpp[band]) = hnbits(fdh, band);
        bih_nbytes(o_bihpp[band]) = hnbytes(fdh, band);
      }
    }

  }

  if (got_bits) {
    for (band = 0; band < nbands; ++band) {
      bih_nbits(o_bihpp[band]) = newbits;
      bih_nbytes(o_bihpp[band]) = (newbits + CHAR_BIT - 1) / CHAR_BIT;
    }
  }

	/* write the output BIH */

  if (bihwrite(fdo, o_bihpp) == ERROR) {
    error("can't write output BIH");
  }

	/* read existing LQH of image, other headers get passed through */

  gethdrs(fdi, request, nbands, fdo);

  i_lqhpp = (LQH_T **) hdr_addr(h_lqh);
  if (i_lqhpp == NULL) {
    warn("Image has no LQH");
  }

  if ( (!got_lqh) && (i_lqhpp != NULL) ) {
    if (got_lohi) {
      h_lqhpp = (LQH_T **) hdralloc(nbands, sizeof(LQH_T *), fdo, LQH_HNAME);
      for (band = 0; band < nbands; ++band) {
        if (!got_bits)  newbits = bih_nbits(o_bihpp[band]);
        ival[0] = 0;
        ival[1] = ipow2(newbits) - 1;
        if (i_lqhpp[band] != NULL) {
          h_lqhpp[band] = lqhmake(newbits, 2, ival, newmm,
                                  lqh_units(i_lqhpp[band]),
                                  lqh_interp(i_lqhpp[band]));
        } else {
          h_lqhpp[band] = lqhmake(newbits, 2, ival, newmm,
                                  NULL, LQH_LIN_INTERP);
        }
      }
    } else {
      h_lqhpp = lqhdup(i_lqhpp, nbands);
      if (h_lqhpp == NULL) {
        error("can't duplicate input LQH");
      }
	/* correct each band for scale differences */
      for (band = 0; band < nbands; ++band) {
        if (lqhremap(h_lqhpp[band], bih_nbits(i_bihpp[band]),
                     bih_nbits(o_bihpp[band])) == ERROR) {
          error("error during LQH header remap of band %d", band);
        }
      }
    } /* not remapping lo-hi */
  }

  if (got_verb) {
    fprintf(stderr, "\nbefore:\n");
    for (band = 0; band < nbands; ++band)
      lqhprint(i_lqhpp[band]);
    fprintf(stderr, "\nafter:\n");
    for (band = 0; band < nbands; ++band)
      lqhprint(h_lqhpp[band]);
  }

	/* reconcile the old and new LQH to create the output LQH */

  if (i_lqhpp == NULL) {
    o_lqhpp = h_lqhpp;
  }
  else {
    o_lqhpp = lqhdup(i_lqhpp, nbands);
    if (o_lqhpp == NULL) {
    	error("can't duplicate input LQH");
    }

    for (band = 0; band < nbands; ++band) {
      if (h_lqhpp[band] != NULL) {
        if (o_lqhpp[band] != NULL) {
          LQH_T *lqhp = o_lqhpp[band];
  
          SAFE_FREE (lqhp->bkpt);
          SAFE_FREE (lqhp->map);
          SAFE_FREE (lqhp->units);
          SAFE_FREE (lqhp->interp);
          SAFE_FREE (lqhp->lininv);
          SAFE_FREE (lqhp);
      	}
      	o_lqhpp[band] = h_lqhpp[band];
      }
    }
  }

	/* Bale out and just copy image if there isn't a LQ header */
  if (o_lqhpp == NULL) {
    warn("No LQ header on output image -- copying image");

    if (boimage(fdo) == ERROR) {
      error("can't terminate header output");
    }
    if (imgcopy(fdi, fdo) == ERROR) {
      error("can't copy image data");
    }
    return;
  }

	/* write the new LQH */

  if (lqhwrite(fdo, o_lqhpp) == ERROR) {
    error("can't write output LQH");
  }

	/* done with header processing */

  if (boimage(fdo) == ERROR) {
    error("can't terminate header output");
  }

	/* now run through image */

  fpbuf = (fpixel_t *) ecalloc(nsamps * nbands, sizeof(fpixel_t));
  if (fpbuf == NULL) {
    error("can't allocate I/O buffer");
  }

  while (nlines-- > 0) {
    if (fpvread(fdi, fpbuf, nsamps) != nsamps) {
      error("read error, line %d", hnlines(fdi) - nlines + 1);
    }
    if (fpvwrite(fdo, fpbuf, nsamps) == ERROR) {
      error("write error, line %d", hnlines(fdo) - nlines + 1);
    }
  }

  SAFE_FREE(fpbuf);
  
}
