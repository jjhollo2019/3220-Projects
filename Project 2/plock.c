/* Name: Wesley Lewis && Jeremy Holloway
 * CPSC-3220-001
 * 6/3/2019
 * Project 2
 */

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

	pthread_mutex_lock(&lock->mlock);

	if (lock->head == NULL) {
		node_t *node = malloc(sizeof(node_t));
		node->priority = priority;
		pthread_cond_init(&node->waitCV, NULL);
		node->next = NULL;
		lock->head = node;
	}
	else {
		node_t *node = malloc(sizeof(node_t));
		node->priority = priority;
		pthread_cond_init(&node->waitCV, NULL);
		node->next = lock->head->next;
		lock->head->next = node;
	}

	pthread_mutex_unlock(&lock->mlock);
}

/* This function checks the state variables of the plock data structure
 * to determine what update action to take. For example, it may need to
 * signal a waiting thread using the appropriate pthread library call
 */
void plock_exit (plock_t *lock) { 
   if(lock->head == NULL) return;
   else{
        pthread_cond_signal(&lock->head);
   }
   return;

}
