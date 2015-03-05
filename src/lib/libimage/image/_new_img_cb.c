/*
** NAME
**	_new_img_cb - create a new image control-block
**
** SYNOPSIS
**	#include "_image.h"
**
**	IMAGE_T *
**	_new_img_cb(void)
**
** DESCRIPTION
**	This routine allocates and initializes a new control-block
**	for an image.
**
** RETURN VALUE
**	non-NULL	pointer to new image control-block
**
**	NULL		insufficient memory to allocate control-block
**
** GLOBAL VARIABLES READ
**
** GLOBAL VARIABLES MODIFIED
*/

#include "ipw.h"
#include "image.h"
#include "_image.h"

IMAGE_T *
_new_img_cb(void)
{
	IMAGE_T	    *new_cb;	/* -> new image control-block */

	new_cb = (IMAGE_T *) ecalloc(1, sizeof(IMAGE_T));
	if (new_cb == NULL)
		return NULL;

	new_cb->name = NULL;
	new_cb->fd = -1;
	new_cb->flags = 0;

	new_cb->bih = NULL;
	new_cb->lqh = NULL;
	new_cb->geoh = NULL;

	new_cb->lqh_mins = NULL;
	new_cb->lqh_maxs = NULL;

	return (new_cb);
}
