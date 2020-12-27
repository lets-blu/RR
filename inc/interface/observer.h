#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct IObserver;

typedef struct ISubject {
    void (*attach)(struct ISubject * subject, struct IObserver * observer);
    void (*detach)(struct ISubject * subject, struct IObserver * observer);
    void (*notify)(struct ISubject * subject);
} ISubject;

typedef struct IObserver {
    void (*update)(struct IObserver * observer, struct ISubject * subject);
} IObserver;

typedef void (*attach_observer_fp)(struct ISubject *, struct IObserver *);
typedef void (*detach_observer_fp)(struct ISubject *, struct IObserver *);
typedef void (*notify_observer_fp)(struct ISubject *);
typedef void (*update_observer_fp)(struct IObserver *, struct ISubject *);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OBSERVER_H__
