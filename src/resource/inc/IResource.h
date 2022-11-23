#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct IResourceUser;

typedef enum {
    RESOURCE_UNAVAILANLE,
    RESOURCE_TAKING,
    RESOURCE_AVAILABLE,
    RESOURCE_GIVING
} ResourceState;

struct IResource {
    ResourceState (*take)(
        struct IResource *pThis,
        struct IResourceUser *user);

    ResourceState (*give)(
        struct IResource *pThis,
        struct IResourceUser *user);
};

struct IResourceUser {
    void (*resourceStateCallback)(
        struct IResourceUser *pThis,
        struct IResource *resource,
        ResourceState state);
};

typedef ResourceState (*IResourceTakeMethod)(
    struct IResource *,
    struct IResourceUser *);

typedef ResourceState (*IResourceGiveMethod)(
    struct IResource *,
    struct IResourceUser *);

typedef void (*IResourceUserResourceStateCallback)(
    struct IResourceUser *,
    struct IResource *,
    ResourceState);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __RESOURCE_H__
