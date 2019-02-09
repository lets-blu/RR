#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    struct ISubject;
    struct IObserver;

    typedef void(*update_fp)(struct ISubject *, struct IObserver *);
    typedef void(*register_subject_fp)(struct IObserver *, struct ISubject *);
    typedef void(*remove_subject_fp)(struct IObserver *, struct ISubject *);
    typedef void(*notify_subjects_fp)(struct IObserver *);

    struct ISubject {
        update_fp update;
    };

    struct IObserver {
        register_subject_fp registerSubject;
        remove_subject_fp removeSubject;
        notify_subjects_fp notifyAllSubjects;
    };

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __OBSERVER_H__
