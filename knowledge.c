/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * knowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chat1002.h"
extern IER_Node *head_what;
extern IER_Node *head_who;
extern IER_Node *head_where;

/*
 * Get the response to a question.
 *
 * Input:
 *   intent   - the question word
 *   entity   - the entity
 *   response - a buffer to receive the response
 *   n        - the maximum number of characters to write to the response buffer
 *
 * Returns:
 *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
 *   KB_NOTFOUND, if no response could be found
 *   KB_INVALID, if 'intent' is not a recognised question word
 */
int knowledge_get(const char *intent, const char *entity, char *response, int n) {

	/* to be implemented */
		IER_Node *search;
	int Result = KB_NOTFOUND;

	if (compare_token(intent, "what") == 0) {
		// Intent is what.
		search = head_what;
	} else if (compare_token(intent, "where") == 0) {
		// Intent is where.
		search = head_where;
	} else if (compare_token(intent, "who") == 0) {
		// Intent is who.
		search = head_who;
	} else {
		// Invalid intent.
		return KB_INVALID;
	}

	while(search != NULL){
		if (compare_token(entity, search->entity) == 0){
			// Found response in knowledge base.
			snprintf(response, n, "%s" , search->response);
			Result = KB_OK;
			break;
		}
		search = search->next;
	}

	return Result;
}



/*
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_OK, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char *intent, const char *entity, const char *response) {
	int Result;
	/* to be implemented */
	// If intent is "what".
	if (compare_token(intent, "what") == 0) {
		Result = kb_update_what(MakeNode(entity, response));

	} 
	// Else if intent is "who".
	else if (compare_token(intent, "who") == 0) {
		Result = kb_update_where(MakeNode(entity, response));
	} 
	// Otherwise if intent is "where".
	else if (compare_token(intent, "where") == 0) {
		
		Result = kb_update_who(MakeNode(entity, response));
	} else {
		// Intent not "what", "who" or "where"
		Result = KB_INVALID;
	}

	return Result;

}


/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE *f) {

	/* to be implemented */

	/*
		ResCount: No. of results from the file
		Result: Entity/response pair insertion result
		ContentBuff: Buffer for storing file contents
	*/
	int ResCount = 0, Result;
	char ContentBuff[MAX_INPUT];
	char *f_intent;

	const size_t MAX_READLEN = (size_t)(MAX_ENTITY + MAX_RESPONSE);

	while (fgets(ContentBuff, MAX_READLEN, (FILE*) f)) {
		char *f_entity, *f_response;
		char *CarRet, *NewL;

		// Read through the file row by row
		if (
			strcmp(ContentBuff, "\n") == 0 ||
			strcmp(ContentBuff, "\r\n") == 0
		) {
			// Empty row
			continue;
		} 
		// else either a carriage or a newline found, code to replace them with a null char.
		else {
			CarRet = strchr(ContentBuff, '\r');
			NewL = strchr(ContentBuff, '\n');

			if (CarRet != NULL) {
				*CarRet = '\0';
			} else if (NewL != NULL) {
				*NewL = '\0';
			} else {
				int c;
				while ((c = getchar()) != '\n' && c != EOF) {
					continue;
				}
			}

			if (
				strchr(ContentBuff, '[') != NULL &&
				strchr(ContentBuff, ']') != NULL
			) {
				// User keyed in square brackets Check intent.
				if (compare_token(ContentBuff, "[what]") == 0) {
					
					f_intent = "what";
				} else if (compare_token(ContentBuff, "[who]") == 0) {
					
					f_intent = "who";
				} else if (compare_token(ContentBuff, "[where]") == 0) {
					
					f_intent = "where";
				} else {
					// Invalid intent.
					f_intent = NULL;
				}
			} else if (
				f_intent != NULL &&
				strchr(ContentBuff, '=') != NULL
			) {
				// Entity-Response pair row.
				f_entity = strtok(ContentBuff, "=");
				f_response = strtok(NULL, "=");
				Result = knowledge_put(f_intent, f_entity, f_response);

				if (Result == KB_OK) {
					// InCarRetement the successful counter.
					ResCount++;
				}
			}
		}
	}

	return ResCount;
}


/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {

	/* to be implemented */

	FreeLL(head_what);
	FreeLL(head_where);
	FreeLL(head_who);

	head_what = NULL;
	head_where = NULL;
	head_who = NULL;
}



/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f) {

	/* to be implemented */
	
	IER_Node *what = head_what;
	IER_Node *who = head_who;
	IER_Node *where = head_where;

	// Save "what" intent's entitiy-responses.
	fprintf(f, "[what]\n");
	while (what != NULL) {
		fprintf(f, "%s=%s\n", what->entity, what->response);
		what = what->next;
	}

	// Save "what" intent's entitiy-responses.
	fprintf(f, "\n[who]\n");
	while (who != NULL) {
		fprintf(f, "%s=%s\n", who->entity, who->response);
		who = who->next;
	}

	// Save "where" intent's entitiy-responses.
	fprintf(f, "\n[where]\n");
	while (where != NULL) {
		fprintf(f, "%s=%s\n", where->entity, where->response);
		where = where->next;
	}
}
