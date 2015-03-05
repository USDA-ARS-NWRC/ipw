CATEGORY := image
DESCRIPTION := high-level image access functions

FUNCS := new_image open_image close_image image_name nlines nsamples nbands \
	 mk_fplinebuf get_fpline put_fpline bihdrs geohdrs not_a_tty

FUNCS := open_image nlines nsamples nbands history annotation \
	 access image_name \
	 new_pbuf read_pbuf \
	 has_lqh lqh_min lqh_max read_fpbuf new_fpbuf \
	 get_geoh \
	 new_image set_history set_annotation write_pbuf \
	 set_lqh write_fpbuf \
	 set_geoh \
	 close_image not_a_tty

PRIVATE_SRCS := _free_img_cb.c _new_img_cb.c _read_hdrs.c _write_hdrs.c

include $(IPW)/make/funcCategory
