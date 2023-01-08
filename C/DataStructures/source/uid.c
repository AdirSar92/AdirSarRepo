#include "uid.h"	 /* struct uid_t */
#include <pthread.h> /* mutex */
#include <stdatomic.h> /* atomic_int */

const UID_t badUID = {(clock_t)0, (pid_t)0, (size_t)0};

atomic_size_t p_counter = 0;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

UID_t UIDCreate(void)
{
	UID_t new_uid = badUID;

	pthread_mutex_lock(&lock);

	new_uid.time = clock();
	new_uid.pid = getpid();
	new_uid.counter = p_counter;
	atomic_fetch_add(&p_counter, 1);

	pthread_mutex_unlock(&lock);

	return (new_uid);
}

int UIDIsSame(UID_t UID1, UID_t UID2)
{
	return (UID1.time == UID2.time &&
			UID1.pid == UID2.pid &&
			UID1.counter == UID2.counter);
}
