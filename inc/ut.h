#ifndef __UT_H__
#define __UT_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef UNIT_TEST
#define LOOP
#define MOCKABLE(method)    mockable##method
#else
#define LOOP                for(;;)
#define MOCKABLE(method)    method
#endif // UNIT_TEST

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __UT_H__
