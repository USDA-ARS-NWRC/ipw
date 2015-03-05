Predefined macros (automatically defined by "cc" on various hosts):

hardware	OS		macro		notes
-----------	---------	-----------	---------------------
ATT PC 7300	System V	mc68k
				mc68k32

DEC VAX		4.{2,3}BSD	vax

IBM PC/AT	XENIX 3.0	M_BITFIELDS	???
				M_I286
				M_I8086
				M_I86
				M_I86LM		"large" memory model
				M_I86MM		"medium" memory model
				M_I86SM		"small" memory model
				M_SYS3
				M_WORDSWAP	???
				M_XENIX

Sun-2		Sun UNIX 2.x	mc68000
				sun

		Sun UNIX 3.x	mc68000
				mc68010
				sun

Sun-3		Sun UNIX 3.x	mc68000
				mc68020
				sun

(as you discover more of these, enter them here)

NB: all UNIX systems define "unix"

$Header: /usr/home/ipw/h/conf/RCS/README.cpp,v 1.1 89/10/18 18:05:57 frew Exp Locker: frew $
