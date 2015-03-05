dnl  Process this file with autoconf to produce a configure script.
dnl
AC_INIT
AC_CONFIG_HEADER(conf.h)
dnl
dnl  Determine the type of system we're on, and then setup the
dnl  header file "host.h" as a link to the system-specific header file
AC_CANONICAL_HOST

echo "creating host.h"

rm -f host.h
while read pattern file
  do
    case "${host_os}" in
	$pattern)	ln -s hosts/$file host.h
			break
			;;
    esac
  done < hosts/List
ls host.h > /dev/null 2>&1 || ln -s hosts/default host.h

dnl
dnl checks for programs
AC_PATH_PROG(SH_PATH, sh)
AC_PATH_PROGS(AWK_PATH, mawk gawk nawk awk)
AC_PATH_PROG(SED_PATH, sed)
AC_PATH_PROG(PERL_PATH, perl)
AC_PROG_CC
AC_PATH_PROG(GRAPH_PATH, graph)
dnl

dnl  If GCC, then check if version 4 or newer, so we can use -iquote
gcc_has_iquote=
if test "$GCC" = "yes" ; then
    AC_EGREP_CPP([yes], [#if __GNUC__ >= 4
			 yes;
			 #endif],
		 [gcc_has_iquote=yes],[gcc_has_iquote=no])
fi
AC_SUBST(gcc_has_iquote)

dnl
dnl checks for libraries
dnl
dnl checks for header files
dnl
dnl checks for typedefs
AC_CHECK_TYPE(uchar_t, unsigned char)
AC_CHECK_TYPE(ushort_t, unsigned short)
AC_CHECK_TYPE(ulong_t, unsigned long)
dnl
dnl checks for structures
dnl
dnl checks for compiler characteristics
AC_C_BIGENDIAN
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(short)
dnl
dnl checks for library functions
dnl
dnl checks for system services
dnl
AC_OUTPUT(conf.mk conf.ipw)
