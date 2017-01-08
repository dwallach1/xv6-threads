#ifndef _TYPES_H_
#define _TYPES_H_

// Type definitions

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;
typedef uint cond_t;
typedef struct _lock_t {
	uint locked; //is the lock held? 0 for no, 1 for yes
} lock_t;

#ifndef NULL
#define NULL (0)
#endif

#endif //_TYPES_H_
