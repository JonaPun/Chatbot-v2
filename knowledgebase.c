#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chat1002.h"
extern IER_Node *head_what;
extern IER_Node *head_where;
extern IER_Node *head_who;


void strcat_special(char *DestPtr, char *Source[], size_t SourceCount, size_t Max_size, int offset) {
	/*
		Special function to concat specific strings together

		Arguments:
			DestPtr 	[char *]:	The pointer to the destination.
			Source 		[char *[]]:	The source array of strings.
			SourceCount	[size_t]:	The number of elements in Source.
			Max_size	[size_t]:	The maximum size of the destination.
			offset		[int]:		The position of Source to start from.
	*/
	size_t len_check, remainder, last;

	for (int i = offset; i < SourceCount; i++) {
		if (i != (SourceCount - 1)) {
			//makes sure space is accounted for by running a check
			len_check = strlen(DestPtr) + strlen(Source[i]) + 1;
		} else {
			len_check = strlen(DestPtr) + strlen(Source[i]);
		}

		// "DestPtr" pointer is still able to take another string
		if (len_check < Max_size) {
			
			strncat(DestPtr, Source[i], Max_size);

			if (i != (SourceCount - 1)) {
				//adds a space between words if the number of elements not the same as i
				strcat(DestPtr, " ");
			}
		} 
		// insufficient space to store present string
		else {
			
			remainder = Max_size - strlen(DestPtr);

			// Means out of space for string, so break
			if (remainder > 0) {
				strncat(DestPtr, Source[i], remainder);
			}
			break;
		}
	}

	// safety net in case the string happens to not end in null character
	last = strlen(DestPtr);
	DestPtr[last] = '\0';
}


IER_Node * MakeNode(const char *entity, const char *resp) {

	IER_Node *node = malloc(sizeof(IER_Node));

	if (node == NULL) {
		printf("[!] Memory allocation failure.\n");
		return NULL;
	}

	snprintf(node->entity, MAX_ENTITY, "%s", entity);
	snprintf(node->response, MAX_RESPONSE, "%s", resp);
	node->next = NULL;

	return node;
}


void AddtoLL(IER_Node *First, IER_Node *New) {
//function that adds new node to linked list
	IER_Node *current = First;

	while (current != NULL) {
		//if the entity already exists, replace its response with new response
		if (compare_token(current->entity, New->entity) == 0) {
			
			snprintf(current->response, MAX_RESPONSE, "%s", New->response);
			break;
		} 
		//end of the linked list
		else if (current->next == NULL) {
			
			current->next = New;
			break;
		}
		// update the current pointer
		current = current->next;
	}
}


void FreeLL(IER_Node *node) {
//function to free all the nodes in the linked list
	IER_Node *current;

	while (node != NULL) {
		current = node;
		node = node->next;
		free(current);
	}
}

//function to add a new node to the "what" knowledge base.
int kb_update_what(IER_Node *CreatedNode) {
	
	// if node creation failed due to low memory
	if (CreatedNode == NULL) {
		return KB_NOMEM;
	}
	//if the linked list is empty, newly created node is put as the first node
	if (head_what == NULL) {
		head_what = CreatedNode;
	} 
	//otherwise append the newly created node to the linked list.
	else {
		AddtoLL(head_what, CreatedNode);
	}
	return KB_OK;
}

//function to add a new node to the "who" knowledge base.
int kb_update_who(IER_Node *CreatedNode) {
	
	//if node creation failed due to low memory return no memory message
	if (CreatedNode == NULL) {
		return KB_NOMEM;
	}
	//if the linked list is empty, newly created node is put as the first node
	if (head_who == NULL) {
		head_who = CreatedNode;
	} 
	//otherwise append the newly created node to the linked list.
	else {
		AddtoLL(head_who, CreatedNode);
	}
	return KB_OK;
}

//Function to add a new node to the "where" knowledge base.
int kb_update_where(IER_Node *CreatedNode) {
	
	//if node creation failed due to low memory
	if (CreatedNode == NULL) {
		return KB_NOMEM;
	}
	//if the linked list is empty, newly created node is put as the first node
	if (head_where == NULL) {
		head_where = CreatedNode;
	//otherwise append the newly created node to the linked list.
	} else {
		AddtoLL(head_where, CreatedNode);
	}
	return KB_OK;
}