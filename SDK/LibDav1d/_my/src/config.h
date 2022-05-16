#define CONFIG_8BPC 1
#define CONFIG_16BPC 1
#define HAVE_ASM 1
#define CONFIG_LOG 1

#define _TARGETTING_WINDOWS
#ifdef _TARGETTING_WINDOWS
#	define _WIN32_WINNT 0x0601
#	define UNICODE 1
#	define _UNICODE 1
#	define __USE_MINGW_ANSI_STDIO 1
#	define _CRT_DECLARE_NONSTDC_NAMES 1
#endif

/* #undef _FILE_OFFSET_BITS */

#define _FSEEKO_FUNCTION _fseeki64
#define _FTELLO_FUNCTION _ftelli64

#ifdef _FSEEKO_FUNCTION
#	define fseeko _FSEEKO_FUNCTION
#endif

#ifdef _FTELLO_FUNCTION
#	define ftello _FTELLO_FUNCTION
#endif

/* #undef HAVE_CLOCK_GETTIME */
/* #undef HAVE_DLSYM */
/* #undef HAVE_UNISTD_H */
#define HAVE_ALIGNED_MALLOC 1
/* #undef HAVE_POSIX_MEMALIGN */
/* #undef HAVE_MEMALIGN */
/* #undef HAVE_GETAUXVAL */
/* #undef HAVE_ELF_AUX_INFO */
#define ENDIANNESS_BIG 0
#define STACK_ALIGNMENT 16

#define ARCH_AARCH64 0
#define ARCH_ARM 0
#define PIC 1
#define ARCH_X86 1
#define ARCH_X86_32 0
#define ARCH_X86_64 1
#define ARCH_PPC64LE 0
#define UNDERSCORE_PREFIXED 0
#define PREFIX UNDERSCORE_PREFIXED
