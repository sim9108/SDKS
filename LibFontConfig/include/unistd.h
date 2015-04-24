
/* access function */
#define	F_OK		0	/* test for existence of file */
#define	X_OK		0x01	/* test for execute or search permission */
#define	W_OK		0x02	/* test for write permission */
#define	R_OK		0x04	/* test for read permission */

#ifndef S_ISDIR
# define S_ISDIR(ST_MODE) (((ST_MODE)& _S_IFMT) == _S_IFDIR)
 #endif
#ifndef S_ISREG
 # define S_ISREG(ST_MODE) (((ST_MODE)& _S_IFMT) == _S_IFREG)
 #endif