#ifndef SEMAPHORE_CLASS_HEADER
#define SEMAPHORE_CLASS_HEADER

#include <cerrno>
#include <cstdlib>

#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else

#include <semaphore.h>

#endif

struct uniform_sem {
#ifdef __APPLE__
    dispatch_semaphore_t    sem;
#else
    sem_t sem;
#endif
};


class SemClass {
public:
    SemClass();

    SemClass(unsigned int);

    ~SemClass();

    void wait();

    void post();

private:
    struct uniform_sem s;

    static inline void
    uniform_sem_init(struct uniform_sem *s, unsigned int value) {
#ifdef __APPLE__
        dispatch_semaphore_t *sem = &s->sem;

    *sem = dispatch_semaphore_create(value);
#else
        sem_init(&s->sem, 0, value);
#endif
    }

    static inline void
    uniform_sem_wait(struct uniform_sem *s) {

#ifdef __APPLE__
        dispatch_semaphore_wait(s->sem, DISPATCH_TIME_FOREVER);
#else
        int r;

        do {
            r = sem_wait(&s->sem);
        } while (r == -1 && errno == EINTR);
#endif
    }

    static inline void
    uniform_sem_post(struct uniform_sem *s) {

#ifdef __APPLE__
        dispatch_semaphore_signal(s->sem);
#else
        sem_post(&s->sem);
#endif
    }

    static inline void
    uniform_sem_destroy(struct uniform_sem *s) {

#ifdef __APPLE__
        dispatch_release(s->sem);
#else
        sem_destroy(&s->sem);
#endif

    }
};

#endif