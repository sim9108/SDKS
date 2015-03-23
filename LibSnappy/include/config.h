#pragma once
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if the compiler supports __builtin_ctz and friends. */
#undef HAVE_BUILTIN_CTZ

/* Define to 1 if the compiler supports __builtin_expect. */
#undef HAVE_BUILTIN_EXPECT

/* Define to 1 if you have the <byteswap.h> header file. */
#undef HAVE_BYTESWAP_H

/* Define to 1 if you have the <dlfcn.h> header file. */
#undef HAVE_DLFCN_H

/* Use the gflags package for command-line parsing. */
#undef HAVE_GFLAGS

/* Defined when Google Test is available. */
#undef HAVE_GTEST

/* Define to 1 if you have the <inttypes.h> header file. */
#undef HAVE_INTTYPES_H

/* Define to 1 if you have the `fastlz' library (-lfastlz). */
#undef HAVE_LIBFASTLZ

/* Define to 1 if you have the `lzf' library (-llzf). */
#undef HAVE_LIBLZF

/* Define to 1 if you have the `lzo2' library (-llzo2). */
#undef HAVE_LIBLZO2

/* Define to 1 if you have the `quicklz' library (-lquicklz). */
#undef HAVE_LIBQUICKLZ

/* Define to 1 if you have the `z' library (-lz). */
#undef HAVE_LIBZ

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <stddef.h> header file. */
#define HAVE_STDDEF_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#undef HAVE_STRINGS_H 

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/byteswap.h> header file. */
#undef HAVE_SYS_BYTESWAP_H

/* Define to 1 if you have the <sys/endian.h> header file. */
#undef HAVE_SYS_ENDIAN_H

/* Define to 1 if you have the <sys/mman.h> header file. */
#undef HAVE_SYS_MMAN_H

/* Define to 1 if you have the <sys/resource.h> header file. */
#undef HAVE_SYS_RESOURCE_H

/* Define to 1 if you have the <sys/stat.h> header file. */
#undef HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/time.h> header file. */
#undef HAVE_SYS_TIME_H

/* Define to 1 if you have the <sys/types.h> header file. */
#undef HAVE_SYS_TYPES_H

/* Define to 1 if you have the <unistd.h> header file. */
#undef HAVE_UNISTD_H

/* Define to 1 if you have the <windows.h> header file. */
#define HAVE_WINDOWS_H 1

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#undef LT_OBJDIR

/* Name of package */
#define PACKAGE "Snappy"

/* Define to the address where bug reports for this package should be sent. */
#undef PACKAGE_BUGREPORT

/* Define to the full name of this package. */
#undef PACKAGE_NAME

/* Define to the full name and version of this package. */
#undef PACKAGE_STRING

/* Define to the one symbol short name of this package. */
#undef PACKAGE_TARNAME

/* Define to the version of this package. */
#undef PACKAGE_VERSION

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "1.1.2"

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel and VAX). */

#define __LITTLE_ENDIAN__ 
#if defined __BIG_ENDIAN__
# define WORDS_BIGENDIAN 1
#elif ! defined __LITTLE_ENDIAN__
# undef WORDS_BIGENDIAN
#endif

/* Define to `unsigned int' if <sys/types.h> does not define. */
#define size_t unsigned int

/* Define to `int' if <sys/types.h> does not define. */
#define ssize_t signed int

#if defined(SNAPPY__NO_DLL)
#define SNAPPY_API

#elif defined(_MSC_VER)
#ifdef SNAPPY_API_EXPORTS
#define	SNAPPY_API __declspec(dllexport)
#else
#define SNAPPY_API __declspec(dllimport)
#endif

#elif defined(SNAPPY__USE_VISIBILITY_ATTR)
#define SNAPPY_API __attribute__ ((visibility ("default")))

#else
#define SNAPPY_API

#endif
