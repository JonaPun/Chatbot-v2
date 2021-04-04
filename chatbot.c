/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "chat1002.h"

extern IER_Node *head_what;
extern IER_Node *head_where;
extern IER_Node *head_who;

/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char *chatbot_botname() {

	return "Chatbot";

}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char *chatbot_username() {

	return "User";

}


/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char *inv[], char *response, int n) {

	/* check for empty input */
	if (inc < 1) {
		snprintf(response, n, "");
		return 0;
	}

	/* look for an intent and invoke the corresponding do_* function */
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);
	else if (chatbot_is_smalltalk(inv[0]))
		return chatbot_do_smalltalk(inc, inv, response, n);
	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n);
	else if (chatbot_is_question(inv[0]))
		return chatbot_do_question(inc, inv, response, n);
	else if (chatbot_is_reset(inv[0]))
		return chatbot_do_reset(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n);
	else {
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}

}


/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char *intent) {

	return compare_token(intent, "exit") == 0 
	|| compare_token(intent, "quit") == 0
	|| compare_token(intent, "bye") == 0
	|| compare_token(intent, "goodbye") == 0;

}


/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char *inv[], char *response, int n) {

	snprintf(response, n, "Goodbye!");

	return 1;

}


/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char *intent) {

	/* to be implemented */

	return compare_token(intent, "load") == 0;

}


/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char *inv[], char *response, int n) {

	/* to be implemented */
		/*
		fptr:		Pointer for the file
		Count:	count of the number of successful returns from the file.
	*/
	FILE *fptr;
	int Count = 0;
	char FilePath[MAX_INPUT];

	// get the name of the file from the user input
	// if the second string is "from", take the next string(index[2]) from the user input
	if (compare_token(inv[1], "from") == 0) {
		
		strcpy(FilePath, inv[2]);
	} 
	//otherwise get the name from index[1] instead
	else {
	
		strcpy(FilePath, inv[1]);

	// 
	fptr = fopen(FilePath, "r");
	}

	if (fptr != NULL) {
		// if the file exists, read and load memory 
		Count = knowledge_read(fptr);
		fclose(fptr);
		snprintf(response, n, "Memory loaded: %d result(s) from knowledge [%s].", Count, FilePath);
	} 
	
	else {
		// If the file is not found, print an error
		snprintf(response, n, "Error: Cannot load knowledge file [%s].", FilePath);
	}

	return 0;

}


/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char *intent) {

	/* to be implemented */

	return ( //if the intent is what, who or where
		compare_token(intent, "what") == 0 ||
		compare_token(intent, "Who") == 0 ||
		compare_token(intent, "where") == 0
	);

}


/*
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char *inv[], char *response, int n) {

	/* to be implemented */

	/*
		unsure:		A string for unsure questions.
		entity:		A string to store the entity.
		ans:		A string to store the answer to the question.
	*/
	char unsure[MAX_RESPONSE] = "I don't know. ";
	char entity[MAX_ENTITY] = "";
	char ans[MAX_RESPONSE] = "";

	size_t offset = 1;
	
	// Check where does the question start.
	if (
		compare_token(inv[1], "is") == 0 ||
		compare_token(inv[1], "are") == 0
	) {
		offset = 2;
	}

	strcat_special(entity, inv, inc, (MAX_ENTITY - 1), offset);

	if (knowledge_get(inv[0], entity, response, n) == KB_NOTFOUND) {
		strcat_special(unsure, inv, inc, (MAX_RESPONSE - 1), 0);
		strcat(unsure, "?");

		prompt_user(ans, MAX_RESPONSE, "%s", unsure);
		knowledge_put(inv[0], entity, ans);
		snprintf(response, n, ".");
	}
	
	return 0;
}


/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
int chatbot_is_reset(const char *intent) {

	/* to be implemented */

	return compare_token(intent, "reset") == 0;

}


/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char *inv[], char *response, int n) {

	/* to be implemented */

	// reinitialise the random number generator
	srand((unsigned)(time(NULL)));
	// Reset knowledge in the memory
	knowledge_reset();
	snprintf(response, n, "Knowledge reset!");
	
	return 0;

}


/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_save(const char *intent) {

	/* to be implemented */

	return compare_token(intent, "save") == 0;

}


/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char *inv[], char *response, int n) {

	/* to be implemented */
	//fptr = pointer to the file
	FILE *fptr;
	char FilePath[MAX_INPUT];

	// when the user inputs file name, if there is a "to" or an "as", take the next string as the file name.
	if (
		compare_token(inv[1], "to") == 0 ||
		compare_token(inv[1], "as") == 0 
	) {
		
		strcpy(FilePath, inv[2]);
	} 
	
	else {
		
		strcpy(FilePath, inv[1]);
	}

	// open and write to file
	fptr = fopen(FilePath, "w");

	if (fptr != NULL) {
		// check to see if the file is there
		knowledge_write(fptr);
		fclose(fptr);
		snprintf(response, n, "Knowledge uploaded to file: [%s] .", FilePath);
	} 
	
	else {
		// file is not there, print error
		snprintf(response, n, "Error: file [%s] does not exist! Unable to upload knowledge.", FilePath);
	}
	return 0;
}
 


/*
 * Determine which an intent is smalltalk.
 *
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is the first word of one of the smalltalk phrases
 *  0, otherwise
 */
int chatbot_is_smalltalk(const char *intent) {

	const char *smalltalk[] = {
		"are", "yo", "hey", "hi", "sup", "its", "it's", "it", "hello", "can"
	};

	for (int i = 0; i < 10; i++){
		if (compare_token(intent, smalltalk[i]) == 0){
			return 1;
		}
	}
	return 0;

}


/*
 * Respond to smalltalk.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */
int chatbot_do_smalltalk(int inc, char *inv[], char *response, int n) {
	const char *RandomGreet[] = {"Hi!", "Hello there", "Hallo!", "What's up", "Yo!", };
	const char *RandomRequest[] = {"Ok sure!", "I'll see what I can do"};

	/* to be implemented */
	/* Intializes random number generator */
	size_t RandomInt = (size_t)(rand() % 5);

	if ( //if the user greets the bot
		compare_token("hello", inv[0]) == 0 ||
		compare_token("hey", inv[0]) == 0 ||
		compare_token("hi", inv[0]) == 0 ||
		compare_token("yo", inv[0]) == 0
	) {
		//Robot responds with a hardcoded random greeting 
		snprintf(response, n, "%s", RandomGreet[RandomInt]);
	} 
	
	else if (compare_token("can", inv[0]) == 0) {
		// If the user requests something
		if (RandomInt == 3) {
			if (compare_token(inv[1], "you") == 0) {
				snprintf(response, n, "I don't know, can I?");
			} 
			
			else if (compare_token(inv[1], "i") == 0) {
				snprintf(response, n, "Hmm I don't know, can you?");
			} 
			
			else if (compare_token(inv[1], "it") == 0) {
				snprintf(response, n, "Not sure, can it?");
			} 
			
			else {
			snprintf(response, n, "%s", RandomRequest[RandomInt]);
			}
		} else if (RandomInt == 4) {
		char qns[MAX_INPUT] = "";
		strcat_special(qns, inv, inc, (MAX_INPUT - 1), 0);
		snprintf(response, n, "Hmm, %s?", qns);
		} else {
			snprintf(response, n, "%s", RandomRequest[RandomInt]);
		}
	} 
	
	else if (
		compare_token("it", inv[0]) == 0 ||
		compare_token("its", inv[0]) == 0 ||
		compare_token("it's", inv[0]) == 0
	) {
		// anything that starts with it will be responded by the robot as this.
		snprintf(response, n, "Yup it is!");
	}
	return 0;
}

