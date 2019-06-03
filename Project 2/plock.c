/* Name: Wesley Lewis && Jeremy Holloway
 * CPSC-3220-001
 * 6/3/2019
 * Project 2
 */

#include "plock.h"
#include "assert.h"
#include "unistd.h"

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

    free(lock);

    return;
}

void plock_enter (plock_t *lock, int priority) {
    
    //enable mutext lock
	pthread_mutex_lock(&lock->mlock);

    //create a new node
    node_t *node = malloc(sizeof(node_t));
	node->priority = priority;
	pthread_cond_init(&node->waitCV, NULL);
    node->next = NULL;

	//add node if it's the first or highest priority
	if (lock->head == NULL || priority > lock->head->priority) {
		//add to linked list
		node->next = lock->head;
		lock->head = node;
	}
    //else find it's place in the list
	else {
		node_t *n = lock->head;

		//travers linked list and add node in order
		while (n->next != NULL && n->next->priority >= priority)
			n = n->next;
		node->next = n->next;
		n->next = node;
	}
	
	//check for running threads, loop till there's not
	while (lock->value == BUSY)
		pthread_cond_wait(&node->waitCV, &lock->mlock);
    
    //set the lock as busy
    lock->value = BUSY;

    //check if there is anything after the head node
    if(lock->head->next == NULL)
        lock->head = NULL;
    //else set the new head
    else{
        lock->head = lock->head->next;
    }
    //destroy the condition variable
    pthread_cond_destroy(&node->waitCV);
    //free the malloc'd memory
    free(node);

    //release the lock
	pthread_mutex_unlock(&lock->mlock);
}

/* This function checks the state variables of the plock data structure
 * to determine what update action to take. For example, it may need to
 * signal a waiting thread using the appropriate pthread library call
 */
void plock_exit (plock_t *lock) { 
    //get the lock
    pthread_mutex_lock(&lock->mlock);
   
    if(lock->head != NULL)
        pthread_cond_signal(&lock->head->waitCV);
    lock->value = FREE;
    
    //release the lock
    pthread_mutex_unlock(&lock->mlock);
}
