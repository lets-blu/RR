#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    struct IObserver;

    struct ISubject {
        void(*registerObserver)(struct ISubject *, struct IObserver *);
        void(*removeObserver)(struct ISubject *, struct IObserver *);
        void(*notifyAllObservers)(struct ISubject *);
    };

    struct IObserver {
        void(*update)(struct IObserver *, struct ISubject *);
    };

    typedef void(*register_observer_fp)(struct ISubject *, struct IObserver *);
    typedef void(*remove_observer_fp)(struct ISubject *, struct IObserver *);
    typedef void(*notify_observers_fp)(struct ISubject *);
    typedef void(*update_fp)(struct IObserver *, struct ISubject *);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OBSERVER_H__
