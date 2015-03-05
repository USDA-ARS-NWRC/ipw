#include "ipw.h"
#include "bih.h"
#include "hdrio.h"
#include "pgm.h"

void
pnm2ipw(void)
{
	BIH_T	*bihp0;
	BIH_T	**bihpp =NULL;
	STRVEC_T *annot;


/* check for valid nbands */

	assert(parm.nbands == 1 || parm.nbands == 3);


/* create annotation string */

	annot = addsv((STRVEC_T *)NULL,
		"image converted to IPW by pnm2ipw");


/* create basic image header */

	bihp0 = bihmake(1,0,NULL,annot,NULL,parm.nlines,parm.nsamps,
		parm.nbands);

	if (bihp0 == NULL) {
		error("can't make BIH for band 0");
	}

	freesv(annot);

/* duplicate bih for other bands 1 and 2 if present */

	switch(parm.nbands){
	case 1:
		bihpp = &bihp0;
		break;
	case 3:
		bihpp = (BIH_T **) hdralloc(3,sizeof(BIH_T *),
				ERROR,BIH_HNAME);
		bihpp[0] = bihpp[1] = bihpp[2] = bihp0;
		break;
	}


/* write bih */

	if (bihwrite(parm.o_fd,bihpp) == ERROR){
		error("can't write BIH");
	}


/* write end-of-headers marker */

	if (boimage(parm.o_fd) == ERROR) {
		error("can't terminate header output");
	}

 /* copy raw pixel data (IPW storage and interleaving is same as PNM) */

	if (ucopy(parm.i_fd, parm.o_fd,
		parm.nlines*parm.nsamps*parm.nbands) == ERROR) {
		error("image data copy failed");
	}

	uclose(parm.o_fd);
}
