#include "SemaphoreClass.h"


SemClass::SemClass() {
    uniform_sem_init(&s, 0);
}

SemClass::SemClass(unsigned int startCount) {
    uniform_sem_init(&s, startCount);
}

SemClass::~SemClass() {
    uniform_sem_destroy(&s);
}

void SemClass::wait() {
    uniform_sem_wait(&s);
}

void SemClass::post() {
    uniform_sem_post(&s);
}
