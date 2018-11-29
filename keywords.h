#ifndef __KEYWORD_H__
#define __KEYWORD_H__

#define PUBLIC 
#define PRIVATE static
#define STATIC 
#define VIRTUAL 

#define FOREACH(__type, __item, __array)    \
	__type __item = __array[0];             \
	for (int __i = 0; __i < sizeof(__array) / sizeof(__type); __item = __array[++__i])

#endif // __KEYWORD_H__
