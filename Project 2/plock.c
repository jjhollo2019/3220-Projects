#include "plock.h"
#include "assert.h"

plock_t *plock_create() {

	plock_t *ptr = malloc(sizeof(plock_t));
	ptr->value = FREE;
	pthread_mutex_init(&ptr->mlock, NULL);
	ptr->head = NULL;

	return ptr;
}

void plock_destroy (plock_t *lock) {
    node_t *ptr = lock->head;
    while(ptr != NULL){

        int ret = pthread_cond_destroy(&ptr->waitCV);
        assert(ret == 0);

        node_t *temp = ptr;
        ptr = ptr->next;
        free(temp);
    }
    int ret = pthread_mutex_destroy(&lock->mlock);
    assert(ret == 0);

    return;
}

void plock_enter (plock_t *lock, int priority) {

}

void plock_exit (plock_t *lock) { 

}
