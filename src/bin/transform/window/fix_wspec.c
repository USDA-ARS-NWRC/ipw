#include "ipw.h"

#include "pgm.h"

static int vfy_begin(WSPEC_T *wp, int nlines, int nsamps);
static int vfy_center(WSPEC_T *wp, int nlines, int nsamps);
static int vfy_end(WSPEC_T *wp, int nlines, int nsamps);
static int vfy_size(WSPEC_T *wp, int nlines, int nsamps);
static int vfy_order(char iopt, int i, char jopt, int j);
static int wont_fit(void);

/*
 * fix_wspec -- derive intrinsic begin {line,samp} + # {lines,samps} from
 *              whatever was supplied on command line
 */

int
fix_wspec(
	WSPEC_T        *wp,		/* -> command-line window spec	 */
	int             nlines,		/* # input image lines		 */
	int             nsamps)		/* # samples / input line	 */
{

	switch (wp->flags) {

	case GOT_BEGIN:
		if (vfy_begin(wp, nlines, nsamps) == ERROR) {
			return (ERROR);
		}

		wp->nlines = nlines - wp->bline;
		wp->nsamps = nsamps - wp->bsamp;

		break;

	case GOT_CENTER:
		if (vfy_center(wp, nlines, nsamps) == ERROR) {
			return (ERROR);
		}

		if (wp->cline < nlines / 2) {
			wp->nlines = 2 * wp->cline + 1;
			wp->bline = 0;
		}
		else {
			wp->nlines = (nlines - wp->cline) * 2 - 1;
			wp->bline = wp->cline - (wp->nlines / 2);
		}

		if (wp->csamp < nsamps / 2) {
			wp->nsamps = 2 * wp->csamp + 1;
			wp->bsamp = 0;
		}
		else {
			wp->nsamps = (nsamps - wp->csamp) * 2 - 1;
			wp->bsamp = wp->csamp - (wp->nsamps / 2);
		}

		break;

	case GOT_CENTER | GOT_BEGIN:
		if (vfy_begin(wp, nlines, nsamps) == ERROR ||
		    vfy_center(wp, nlines, nsamps) == ERROR ||
		    vfy_order('b', wp->bline, 'c', wp->cline) == ERROR ||
		    vfy_order('b', wp->bsamp, 'c', wp->csamp) == ERROR) {
			return (ERROR);
		}

		wp->nlines = (wp->cline - wp->bline) * 2 + 1;
		wp->nsamps = (wp->csamp - wp->bsamp) * 2 + 1;

		if (wp->bline + wp->nlines > nlines ||
		    wp->bsamp + wp->nsamps > nsamps) {
			return (wont_fit());
		}

		break;

	case GOT_END:
		if (vfy_end(wp, nlines, nsamps) == ERROR) {
			return (ERROR);
		}

		wp->nlines = wp->eline + 1;
		wp->nsamps = wp->esamp + 1;

		break;

	case GOT_END | GOT_BEGIN:
		if (vfy_begin(wp, nlines, nsamps) == ERROR ||
		    vfy_end(wp, nlines, nsamps) == ERROR ||
		    vfy_order('b', wp->bline, 'e', wp->eline) == ERROR ||
		    vfy_order('b', wp->bsamp, 'e', wp->esamp) == ERROR) {
			return (ERROR);
		}

		wp->nlines = wp->eline - wp->bline + 1;
		wp->nsamps = wp->esamp - wp->bsamp + 1;

		break;

	case GOT_END | GOT_CENTER:
		if (vfy_center(wp, nlines, nsamps) == ERROR ||
		    vfy_end(wp, nlines, nsamps) == ERROR ||
		    vfy_order('c', wp->cline, 'e', wp->eline) == ERROR ||
		    vfy_order('c', wp->csamp, 'e', wp->esamp) == ERROR) {
			return (ERROR);
		}

		wp->nlines = (wp->eline - wp->cline) * 2 + 1;
		wp->nsamps = (wp->esamp - wp->csamp) * 2 + 1;

		wp->bline = wp->cline - (wp->nlines / 2);
		wp->bsamp = wp->csamp - (wp->nsamps / 2);

		if (wp->bline < 0 || wp->bsamp < 0) {
			return (wont_fit());
		}

		break;

	case GOT_SIZE:
		if (vfy_size(wp, nlines, nsamps) == ERROR) {
			return (ERROR);
		}

		break;

	case GOT_SIZE | GOT_BEGIN:
		if (vfy_begin(wp, nlines, nsamps) == ERROR ||
		    vfy_size(wp, nlines, nsamps) == ERROR) {
			return (ERROR);
		}

		if (wp->bline + wp->nlines > nlines ||
		    wp->bsamp + wp->nsamps > nsamps) {
			return (wont_fit());
		}

		break;

	case GOT_SIZE | GOT_CENTER:
		if (vfy_center(wp, nlines, nsamps) == ERROR ||
		    vfy_size(wp, nlines, nsamps) == ERROR) {
			return (ERROR);
		}

		wp->bline = wp->cline - wp->nlines / 2;
		wp->bsamp = wp->csamp - wp->nsamps / 2;

		if (wp->bline < 0 ||
		    wp->bsamp < 0 ||
		    wp->bline + wp->nlines > nlines ||
		    wp->bsamp + wp->nsamps > nsamps) {
			return (wont_fit());
		}

		break;

	case GOT_SIZE | GOT_END:
		if (vfy_end(wp, nlines, nsamps) == ERROR ||
		    vfy_size(wp, nlines, nsamps) == ERROR) {
			return (ERROR);
		}

		wp->bline = wp->eline - wp->nlines + 1;
		wp->bsamp = wp->esamp - wp->nsamps + 1;

		if (wp->bline < 0 || wp->bsamp < 0) {
			return (wont_fit());
		}

		break;

	default:
		bug("grozzled window specification");
	}

	return (OK);
}

/*
 * private support functions
 */

static int
vfy_begin(
	WSPEC_T        *wp,
	int             nlines,
	int             nsamps)
{
	if (wp->bline < 0 || wp->bline >= nlines) {
		usrerr("%d: bad begin line", wp->bline);
		return (ERROR);
	}

	if (wp->bsamp < 0 || wp->bsamp >= nsamps) {
		usrerr("%d: bad begin sample", wp->bsamp);
		return (ERROR);
	}

	return (OK);
}

static int
vfy_center(
	WSPEC_T        *wp,
	int             nlines,
	int             nsamps)
{
	if (wp->cline < 0 || wp->cline >= nlines) {
		usrerr("%d: bad center line", wp->cline);
		return (ERROR);
	}

	if (wp->csamp < 0 || wp->csamp >= nsamps) {
		usrerr("%d: bad center sample", wp->csamp);
		return (ERROR);
	}

	return (OK);
}

static int
vfy_end(
	WSPEC_T        *wp,
	int             nlines,
	int             nsamps)
{
	if (wp->eline < 0 || wp->eline >= nlines) {
		usrerr("%d: bad end line", wp->eline);
		return (ERROR);
	}

	if (wp->esamp < 0 || wp->esamp >= nsamps) {
		usrerr("%d: bad end sample", wp->esamp);
		return (ERROR);
	}

	return (OK);
}

static int
vfy_size(
	WSPEC_T        *wp,
	int             nlines,
	int             nsamps)
{
	if (wp->nlines < 1 || wp->nlines > nlines) {
		usrerr("%d: bad # lines", wp->nlines);
		return (ERROR);
	}

	if (wp->nsamps < 1 || wp->nsamps > nsamps) {
		usrerr("%d: bad # samps", wp->nsamps);
		return (ERROR);
	}

	return (OK);
}

static int
vfy_order(
	char	iopt,
	int	i,
	char	jopt,
	int	j)
{
	if (i > j) {
		usrerr("-%c %d > -%c %d", iopt, i, jopt, j);
		return (ERROR);
	}

	return (OK);
}

static int
wont_fit(void)
{
	usrerr("specified window exceeds boundaries of input image");
	return (ERROR);
}
