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
	//enable mutext lock
	pthread_mutex_lock(&lock->mlock);

	//check for runnign threads, loop till there's not
	while (lock->value == BUSY)
		;

	//add node in ascending order
	if (lock->head == NULL || priority > lock->head->priority) {
		// make new node and initialize variables
		node_t *node = malloc(sizeof(node_t));
		node->priority = priority;
		pthread_cond_init(&node->waitCV, NULL);
		//add to linked list
		node->next = lock->head;
		lock->head = node;
		//set CV to busy
		lock->value = BUSY;
	}
	else {
		//make new node and initialize variables
		node_t *node = malloc(sizeof(node_t));
		node->priority = priority;
		pthread_cond_init(&node->waitCV, NULL);
		node_t *n = lock->head;

		//travers linked list and add node in order
		while (n->next != NULL && n->next->priority >= priority)
			n = n->next;
		node->next = n->next;
		n->next = node;

		//put thread to sleep
		pthread_cond_wait(&node->waitCV, &lock->mlock);
	}

	//wake up thread with highest priority, should be at the head of the list
	pthread_cond_signal(&lock->head->waitCV);

	//destroy node
	node_t *n = lock->head;
	lock->head = lock->head->next;
	free(n);

	/*

	 wait (loop) checking if lock is free, and highest priority

	proceed when lock is free
	and when this thread is highest priority in system
	*/

	pthread_mutex_unlock(&lock->mlock);
}

/* This function checks the state variables of the plock data structure
 * to determine what update action to take. For example, it may need to
 * signal a waiting thread using the appropriate pthread library call
 */
void plock_exit (plock_t *lock) { 

	lock->value = FREE;
	/*
   if(lock->head == NULL) return;
   else{
       pthread_cond_signal(&lock->head->next->waitCV);
       pthread_mutex_unlock(&lock->mlock);
   }
   return;
	*/
}
