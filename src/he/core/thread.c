#include "he/core/thread.h"
#include "he/core/types.h"

#ifdef HE_TARGET_WINDOWS
#else
#include "he/core/unix_thread.h"
#endif

bool heInitMutex(struct HeMutexDesc *desc, struct HeMutex *pMutex) {
  pMutex->spinCount = desc->spinCount;
#ifdef HE_TARGET_WINDOWS
  #error UNHANDLED
#else
    pMutex->handle = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    pthread_mutexattr_t attr;
    int                 status = pthread_mutexattr_init(&attr);
    status |= pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    status |= pthread_mutex_init(&pMutex->handle, &attr);
    status |= pthread_mutexattr_destroy(&attr);
#endif
  return true;
}
void heDestroyMutex(struct HeMutex* pMutex) {
#ifdef HE_TARGET_WINDOWS
  #error UNHANDLED
#else
 pthread_mutex_destroy(&pMutex->handle);
#endif
}

void heAcquireMutex(struct HeMutex *pMutex) {
  uint32_t count = 0;

  while (count < pMutex->spinCount &&
         pthread_mutex_trylock(&pMutex->handle) != 0)
    ++count;

  if (count == pMutex->spinCount) {
    int r = pthread_mutex_lock(&pMutex->handle);
    ASSERT(r == 0 && "Mutex::Acquire failed to take the lock");
  }
}
bool heTryAcquireMutex(struct HeMutex* pMutex) {

}

void heReleaseMutex(struct HeMutex* pMutex) {

}


