/* config.h.in.  Maintained by hand. */

/* Template definitions for autoconf */
#define __EXTENSIONS__
#define _ALL_SOURCE
#define _GNU_SOURCE
#define _POSIX_SOURCE
#define _POSIX_1_SOURCE
#define _POSIX_PTHREAD_SEMANTICS
#define _TANDEM_SOURCE
#define _MINIX

/* Define NO_MULTIBYTE_SUPPORT to not compile in support for multibyte
   characters, even if the OS supports them. */
#define NO_MULTIBYTE_SUPPORT

#define _FILE_OFFSET_BITS

/* Define as the return type of signal handlers (int or void).  */
#define RETSIGTYPE

#define VOID_SIGHANDLER

/* Characteristics of the compiler. */
#define sig_atomic_t

#define size_t

#define ssize_t

#define const

#define volatile

#define PROTOTYPES
#define __PROTOTYPES

#define __CHAR_UNSIGNED__

/* Define if the `S_IS*' macros in <sys/stat.h> do not work properly.  */
#define STAT_MACROS_BROKEN

/* Define if you have the chown function. */
#define HAVE_CHOWN

/* Define if you have the fcntl function. */
#define HAVE_FCNTL

/* Define if you have the fnmatch function. */
#define HAVE_FNMATCH

/* Define if you have the getpwent function. */
#define HAVE_GETPWENT

/* Define if you have the getpwnam function. */
#define HAVE_GETPWNAM

/* Define if you have the getpwuid function. */
#define HAVE_GETPWUID

/* Define if you have the isascii function. */
#define HAVE_ISASCII

/* Define if you have the iswctype function.  */
#define HAVE_ISWCTYPE

/* Define if you have the iswlower function.  */
#define HAVE_ISWLOWER

/* Define if you have the iswupper function.  */
#define HAVE_ISWUPPER

/* Define if you have the isxdigit function. */
#define HAVE_ISXDIGIT

/* Define if you have the kill function. */
#define HAVE_KILL

/* Define if you have the lstat function. */
#define HAVE_LSTAT

/* Define if you have the mbrlen function. */
#define HAVE_MBRLEN

/* Define if you have the mbrtowc function. */
#define HAVE_MBRTOWC

/* Define if you have the mbsrtowcs function. */
#define HAVE_MBSRTOWCS

/* Define if you have the memmove function. */
#define HAVE_MEMMOVE

/* Define if you have the pselect function.  */
#define HAVE_PSELECT

/* Define if you have the putenv function.  */
#define HAVE_PUTENV

/* Define if you have the readlink function.  */
#define HAVE_READLINK

/* Define if you have the select function.  */
#define HAVE_SELECT

/* Define if you have the setenv function.  */
#define HAVE_SETENV

/* Define if you have the setlocale function. */
#define HAVE_SETLOCALE

/* Define if you have the strcasecmp function.  */
#define HAVE_STRCASECMP

/* Define if you have the strcoll function.  */
#define HAVE_STRCOLL

#define STRCOLL_BROKEN

/* Define if you have the strpbrk function.  */
#define HAVE_STRPBRK

/* Define if you have the tcgetattr function.  */
#define HAVE_TCGETATTR

/* Define if you have the towlower function.  */
#define HAVE_TOWLOWER

/* Define if you have the towupper function.  */
#define HAVE_TOWUPPER

/* Define if you have the vsnprintf function.  */
#define HAVE_VSNPRINTF

/* Define if you have the wcrtomb function.  */
#define HAVE_WCRTOMB 

/* Define if you have the wcscoll function.  */
#define HAVE_WCSCOLL

/* Define if you have the wctype function.  */
#define HAVE_WCTYPE

/* Define if you have the wcwidth function.  */
#define HAVE_WCWIDTH

/* and whether it works */
#define WCWIDTH_BROKEN

#define STDC_HEADERS

/* Define if you have the <dirent.h> header file.  */
#define HAVE_DIRENT_H

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H

/* Define if you have the <langinfo.h> header file.  */
#define HAVE_LANGINFO_H

/* Define if you have the <libaudit.h> header file.  */
#define HAVE_LIBAUDIT_H

/* Define if you have the <limits.h> header file.  */
#define HAVE_LIMITS_H

/* Define if you have the <locale.h> header file.  */
#define HAVE_LOCALE_H

/* Define if you have the <memory.h> header file.  */
#define HAVE_MEMORY_H

/* Define if you have the <ndir.h> header file.  */
#define HAVE_NDIR_H

/* Define if you have the <ncurses/termcap.h> header file.  */
#define HAVE_NCURSES_TERMCAP_H

/* Define if you have the <pwd.h> header file.  */
#define HAVE_PWD_H

/* Define if you have the <stdarg.h> header file.  */
#define HAVE_STDARG_H

/* Define if you have the <stdbool.h> header file.  */
#define HAVE_STDBOOL_H

/* Define if you have the <stdlib.h> header file.  */
#define HAVE_STDLIB_H

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H

/* Define if you have the <strings.h> header file.  */
#define HAVE_STRINGS_H

/* Define if you have the <sys/dir.h> header file.  */
#define HAVE_SYS_DIR_H

/* Define if you have the <sys/file.h> header file.  */
#define HAVE_SYS_FILE_H

/* Define if you have the <sys/ioctl.h> header file.  */
#define HAVE_SYS_IOCTL_H

/* Define if you have the <sys/ndir.h> header file.  */
#define HAVE_SYS_NDIR_H

/* Define if you have the <sys/pte.h> header file.  */
#define HAVE_SYS_PTE_H

/* Define if you have the <sys/ptem.h> header file.  */
#define HAVE_SYS_PTEM_H

/* Define if you have the <sys/select.h> header file.  */
#define HAVE_SYS_SELECT_H

/* Define if you have the <sys/stream.h> header file.  */
#define HAVE_SYS_STREAM_H

/* Define if you have the <termcap.h> header file.  */
#define HAVE_TERMCAP_H

/* Define if you have the <termio.h> header file.  */
#define HAVE_TERMIO_H

/* Define if you have the <termios.h> header file.  */
#define HAVE_TERMIOS_H

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H

/* Define if you have the <varargs.h> header file.  */
#define HAVE_VARARGS_H

/* Define if you have the <wchar.h> header file.  */
#define HAVE_WCHAR_H

/* Define if you have the <wctype.h> header file.  */
#define HAVE_WCTYPE_H

#define HAVE_MBSTATE_T

/* Define if you have wchar_t in <wctype.h>. */
#define HAVE_WCHAR_T 

/* Define if you have wctype_t in <wctype.h>. */
#define HAVE_WCTYPE_T

/* Define if you have wint_t in <wctype.h>. */  
#define HAVE_WINT_T  

/* Define if you have <langinfo.h> and nl_langinfo(CODESET). */
#define HAVE_LANGINFO_CODESET

/* Define if you have <linux/audit.h> and it defines AUDIT_USER_TTY */
#define HAVE_DECL_AUDIT_USER_TTY

/* Definitions pulled in from aclocal.m4. */
#define VOID_SIGHANDLER

#define GWINSZ_IN_SYS_IOCTL

#define STRUCT_WINSIZE_IN_SYS_IOCTL

#define STRUCT_WINSIZE_IN_TERMIOS

#define TIOCSTAT_IN_SYS_IOCTL

#define FIONREAD_IN_SYS_IOCTL

#define SPEED_T_IN_SYS_TYPES

#define HAVE_GETPW_DECLS

#define HAVE_STRUCT_DIRENT_D_INO

#define HAVE_STRUCT_DIRENT_D_FILENO

#define HAVE_STRUCT_DIRENT_D_NAMLEN

#define HAVE_BSD_SIGNALS

#define HAVE_POSIX_SIGNALS

#define HAVE_USG_SIGHOLD

#define MUST_REINSTALL_SIGHANDLERS

#define HAVE_POSIX_SIGSETJMP

#define CTYPE_NON_ASCII

/* modify settings or make new ones based on what autoconf tells us. */

/* Ultrix botches type-ahead when switching from canonical to
   non-canonical mode, at least through version 4.3 */
#if !defined (HAVE_TERMIOS_H) || !defined (HAVE_TCGETATTR) || defined (ultrix)
#  define TERMIOS_MISSING
#endif

/* VARARGS defines moved to rlstdc.h */
