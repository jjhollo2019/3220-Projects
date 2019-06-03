/* Name: Wesley Lewis && Jeremy Holloway
 * CPSC-3220-001
 * 6/3/2019
 * Project 2
 */

#include "plock.h"
#include "assert.h"

plock_t *plock_create() {
    //create space for plock_t struct
	plock_t *ptr = malloc(sizeof(plock_t));
    //set the lock to free
	ptr->value = FREE;
    //initialize the mutex
	int ret = pthread_mutex_init(&ptr->mlock, NULL);
    assert(ret == 0);
    //set head pointer to null
	ptr->head = NULL;

    //return lock pointer
	return ptr;
}

void plock_destroy (plock_t *lock) {
    //create a traversal pointer
    node_t *ptr = lock->head;
    //check for any remaining condition variables
    while(ptr != NULL){
        
        //destroy the context
        int ret = pthread_cond_destroy(&ptr->waitCV);
        //check the return value
        assert(ret == 0);

        //traverse the list 
        node_t *temp = ptr;
        ptr = ptr->next;
        //free the malloc'd memory
        free(temp);
    }
    //destroy the lock
    int ret = pthread_mutex_destroy(&lock->mlock);
    //check the return
    assert(ret == 0);

    //free the memory location
    free(lock);

}

void plock_enter (plock_t *lock, int priority) {
    //create a return int for error checking
    int ret;

    //enable mutext lock
	ret = pthread_mutex_lock(&lock->mlock);
    assert(ret == 0);

    //create a new node
    node_t *node = malloc(sizeof(node_t));
	node->priority = priority;
	ret = pthread_cond_init(&node->waitCV, NULL);
    assert(ret == 0);
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
	while (lock->value == BUSY) {
		ret = pthread_cond_wait(&node->waitCV, &lock->mlock);
        assert(ret == 0);
    }
    
    //set the lock as busy
    lock->value = BUSY;

    //check if there is anything after the head node
    if(lock->head->next == NULL)
        lock->head = NULL;
    //else set the new head
    else {
        lock->head = lock->head->next;
    }
    //destroy the condition variable
    ret = pthread_cond_destroy(&node->waitCV);
    assert(ret == 0);
    //free the malloc'd memory
    free(node);

    //release the lock
	ret = pthread_mutex_unlock(&lock->mlock);
    assert(ret == 0);
}

void plock_exit (plock_t *lock) { 
    int ret;
    //get the lock
    ret = pthread_mutex_lock(&lock->mlock);
    assert(ret == 0);

    //check for any remaining threads and signal if so
    if(lock->head != NULL){
        ret = pthread_cond_signal(&lock->head->waitCV);
        assert(ret == 0);
    }
    //free the lock
    lock->value = FREE;
    
    //release the lock
    ret = pthread_mutex_unlock(&lock->mlock);
    assert(ret == 0);
}
