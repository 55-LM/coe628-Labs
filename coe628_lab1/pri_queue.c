#include <stdlib.h>
#include <stdio.h>
#include "pri_queue.h"
/** @file pri_queue.c */
static Node_ptr_t head = NULL;
/**
 * Insert a Node into a priority queue.
 * @param priority
 * @param data
 * @author Alvi Alam
 */
void PQ_insert(int priority, char * data) {
  //FIXED
    //Memory allocation for a node pointer for a new Node_t structure
    Node_ptr_t new_node = (Node_ptr_t) malloc(sizeof(Node_t));
 
    //Setting the priority field of the new node to be inserted given the specified priority integer
    new_node->priority = priority;
    
    //Setting the data field of the new node to be inserted given the specified data string 
    new_node->data = data;
    
    //Setting the next field of the new node to be inserted as NULL (since last node in the list doesn't point to another node)
    new_node->next = NULL;

    //If the new node has greater priority than the head (starting node) or the head is empty, set the new node as the starting node
    if (head == NULL || head->priority < priority) {
        new_node->next = head;//Setting the next field of the new node to head (starting node)
        head = new_node; //The new node is set as the head through the head pointer
        
    //Otherwise, if the new node has a specified priority that isn't the greatest or the queue isn't null, the new node must be inserted in the correct position 
    } else { 
        Node_ptr_t tmp = head; //Temporary node pointer set to the current head (temp is used to go through the the queue)
        
        //While the temporary node pointer points to the next node with a priority greater than or equal to the priority of the new node and that the next node being pointed to isn't empty 
        while (tmp->next->priority >= priority && tmp->next != NULL) {
            tmp = tmp->next; //The temporary node pointer is set to point to the next node
        }
        new_node->next = tmp->next; //The new node is set as the next node through the next pointer and is set to the node that the current next node that the temp node pointer is pointing to
        tmp->next = new_node; //The temp node pointer points to the next node which is now the new node
    }

}
/**
 * Delete and return the node with the highest priority.
 * @return The highest priority Node.
 */
Node_ptr_t PQ_delete() {
  //FIXED
    //If the head (starting node) is NULL the list is empty so return NULL
    if (head == NULL) {
        return NULL; 
        
    //Otherwise, if the list is not empty
    } else {
        Node_ptr_t tmp = head; //A temporary node pointer is set to the node with the greatest priority which by default is the head
        head = head->next; //The head node is set as the next node from the current head (which means it is no longer prioritized and "removed")
        tmp->next = NULL; //The temporary node pointer (head) is set to point the the next node which is set as NULL (head node is fully "removed")
        return tmp; //The node that was removed (head node/highest priority node) is returned 
    }
}

/**
 * Do NOT modify this function.
 * @return A pointer to the head of the list.  (NULL if list is empty.)
 */
Node_ptr_t PQ_get_head() {
    return head;
}

/**
 * Do NOT modify this function.
 * @return the number of items in the queue
 */
int PQ_get_size() {
    int size = 0;
    Node_ptr_t tmp;
    for(tmp = head; tmp != NULL; tmp = tmp->next, size++)
        ;
    return size;
}


