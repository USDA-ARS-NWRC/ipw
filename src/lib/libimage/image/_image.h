#ifndef	_IMAGE_H
#define	_IMAGE_H

/*
 * internal header file for IPW images
 */

extern	bool_t	  _close_output (IMAGE_T * image);
extern	void	  _free_img_cb  (IMAGE_T * icb);
extern	IMAGE_T * _new_img_cb   (void);
extern	bool_t	  _read_hdrs    (IMAGE_T * image);


#endif
