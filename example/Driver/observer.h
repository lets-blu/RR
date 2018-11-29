#ifndef __OBSERVER_H__
#define __OBSERVER_H__

struct IObserver;

struct ISubject {
    void (*update)(struct ISubject * this, struct IObserver * observer);
};

struct IObserver {
    void (*registerSubject)(struct IObserver * this, struct ISubject * subject);
    void (*removeSubject)(struct IObserver * this, struct ISubject * subject);
    void (*notifyAllSubjects)(struct IObserver * this);
};

#endif // __OBSERVER_H__
