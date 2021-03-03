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

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "chat1002.h"

#define MAX_REPLIES 5

 // Define the file name so easy to configure in the future (?) Should probably create a config file and pull from there instead
const char* INI_FILE_NAME = "ICT1002_Group Project Assignment_Sample.ini";

// Create the arrays for the predefined trigger keywords and the bot replies
char* USER_GREETINGS[MAX_REPLIES] = {
		"Hello", "Hi", "Greetings", "Sup", "Morning"
};
char* CHATBOT_GREETINGS[MAX_REPLIES] = {
		"Hello", "Hi", "Greetings", "Sup", "What la"
};


/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char* chatbot_botname() {

	return "Mr Robot";

}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char* chatbot_username() {

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
int chatbot_main(int inc, char* inv[], char* response, int n) {

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
int chatbot_is_exit(const char* intent) {
	if (compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0 || compare_token(intent, "goodbye") == 0 || compare_token(intent, "bye") == 0) {
		return 1;
	}
	else {
		return 0;
	}
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
int chatbot_do_exit(int inc, char* inv[], char* response, int n) {
	knowledge_reset();                                                        // We need to reset the knowledge base as well to clear the memory
	snprintf(response, n, "Goodbye! Thanks for talking with me!");
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
int chatbot_is_load(const char* intent) {
	if (compare_token(intent, "load") == 0) {
		return 1;
	}
	else {
		return 0;
	}
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
int chatbot_do_load(int inc, char* inv[], char* response, int n) {
	int line = 0;

	if (inv[1] == NULL) {
		strcpy(response, "ERROR! No file was selected");                    // Error check if the user did not select a file
	}
	else {
		FILE* file;

		if (file = fopen(inv[1], "r")) {                                    // Open the file in read only mode
			line = knowledge_read(file);                                    // Call the knowledge_read function in knowledge.c to get the number of entries from the file
			fclose(file);                                                   // Remember to close the file
			snprintf(response, n, "Read %d responses from %s", line, inv[1]);    // Output to user to say that the file was read
		}
		else {
			strcpy(response, "No such file exist!");                        // Error check if the file is not found
		}
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
int chatbot_is_question(const char* intent) {
	if (compare_token(intent, "what") == 0 || compare_token(intent, "who") == 0 || compare_token(intent, "where") == 0) {
		return 1;
	}
	else {
		return 0;
	}
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
int chatbot_do_question(int inc, char* inv[], char* response, int n) {

	char inputIntent[MAX_INTENT];
	char inputNoun[MAX_INPUT];
	char inputEntity[MAX_ENTITY];
	char botResponse[MAX_RESPONSE];
	char response_not_found[MAX_RESPONSE];
	int  resultResponse;
	bool foundIntent = false;
	strcpy(inputNoun, "\0");
	snprintf(inputEntity, n, "");

	if ((inc == 1) || (inc == 2 && ((compare_token(inv[1], "are") == 0) || (compare_token(inv[1], "is") == 0)))) {              // If inc == 1, means it bypass chatbot_main just by only entering (What, Where, Who). If inc == 2, then it cannot be a noun, it must be an entity
		snprintf(response, n, "Sorry, but I don't get what you are asking about.\nPlease complete your sentence.");             // Not required to check if intent is "What, Where, Who" since chatbot_main function already done so
		return 0;
	}

	/* Everything after this line will automatically assume that user has typed the correct question format (Intent was found)
	*  Noun is optional, Entity must be present
	*/
	strncpy(inputIntent, inv[0], sizeof(inputIntent) / sizeof(inputIntent[0]));


	for (int i = 1; i < inc; i++) {
		if ((i == 1) && ((compare_token(inv[1], "is") == 0) || (compare_token(inv[1], "are") == 0))) {
			strncpy(inputNoun, inv[1], sizeof(inv[1]));
		}
		else {
			strcat(inputEntity, inv[i]);
			if (i < (inc - 1)) {
				strcat(inputEntity, " ");
			}
		}
	}
	//memmove(inputEntity, inputEntity + 1, strlen(inputEntity));

	// 3 different response will be thrown back by knowledge_get: INVALID, PASS, FAIL
	resultResponse = knowledge_get(inputIntent, inputEntity, botResponse, n);
	snprintf(response, n, "%d", resultResponse);

	if (resultResponse == R_INVALID) { // This is kind of redundant since top part already did such a check, but is okay. Double the protection! 
		snprintf(response, n, "Sorry, but I don't what you are asking about.\nPlease start your sentence with either who, where or what");
	}
	else if (resultResponse == R_PASS) {
		snprintf(response, n, "%s", botResponse);
	}
	else if (resultResponse == R_FAIL) {

		// Requires rework

		do {
			if (compare_token(inputNoun, "\0") == 0) {																				// When the user original input doesnt have any noun (inputNoun is the null), the bot will ask the question without a noun
				prompt_user(response_not_found, MAX_INPUT, "I don't know. %s %s?", inputIntent, inputEntity);
			}
			else {
				prompt_user(response_not_found, MAX_INPUT, "I don't know. %s %s %s?", inputIntent, inputNoun, inputEntity);		// Else print the question with the noun
			}

		} while ((strcmp(response_not_found, "") == 0));																			// Force an answer from user.


		//  Since the response is not in the knowledge base, we store it in the knowledge base with the user original input
		resultResponse = knowledge_put(inputIntent, inputEntity, response_not_found);												// Pass in the intent array, the entity array and the response_not_found array

		if (resultResponse == R_PASS) {																								// When the response from the method call is good
			snprintf(response, n, "Thank you.");
		}
		else if (resultResponse == R_MEMNULL) {																						// If there is not enough memory
			snprintf(response, n, "Knowledge Base is out of memory!");
			exit(1);
		}
		else if (resultResponse == R_INVALID) {																						// Else the intent is just not correct
			snprintf(response, n, "Sorry, I don't understand '%s'.", inputIntent);
		}
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
int chatbot_is_reset(const char* intent) {

	if (compare_token(intent, "reset") == 0) {
		return 1;
	}
	else {
		return 0;
	}
}


/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after being reset)
 */
int chatbot_do_reset(int inc, char* inv[], char* response, int n) {
	knowledge_reset();                                                          // We also need to free the memory that was occupied by the linked list in knowledge.c so we call knowledge_reset
	system("cls");                                                              // Just to make it neater. IT IS NOT PORTABLE! ONLY FOR WINDOWS!
	snprintf(response, n, "Chatbot reset.");
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
int chatbot_is_save(const char* intent) {

	if (compare_token(intent, "save") == 0) {
		return 1;
	}
	else {
		return 0;
	}

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
int chatbot_do_save(int inc, char* inv[], char* response, int n) {

	char filename[64];

	if (inv[1] == NULL) {
		strcpy(response, "User did not select a file for input");       // Error check if user did not select a file
	}
	else {
		strcpy(filename, inv[1]);

		int i;
		FILE* dump;
		dump = fopen(filename, "w");

		if (dump == NULL) {
			strcpy(response, "Error unable to create file");                                        // Error check for when the program cannot fopen
		}
		else {
			knowledge_write(dump);                                                                  // Save the knowledge base using the knowledge_write function in knowledge.c
			snprintf(response, n, "My knowledge has been saved to %s.", filename);        // Output message to user to let them know that the file has been saved
		}
		fclose(dump);
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
int chatbot_is_smalltalk(const char* intent) {
	int flag = 0;

	// This loop will loop through the whole length of the intent
	for (int i = 0; i < MAX_REPLIES; i++) {
		// The loop is checking through each char and if the user intent matches the predefined greetings, all the chars will set the flag to 1 each time ending up with flag = 1 at the end
		if (compare_token(intent, USER_GREETINGS[i]) == 0) {
			flag = 1;                                                                           // Use compare_token method to check through the predefined greeting message to see if user input has a keyword
		}
		else if (strcmp(intent, "it's") == 0 || strcmp(intent, "It's") == 0) {                  // Check if intent has it's for both lower caps and upper caps. Not sure why compare_token doesn't like It's
			flag = 1;
		}
	}
	if (flag == 1) {                                                                            // If the flag at the end of the loop is 1, that means the trigger words matches the user input so we return 1
		return 1;
	}
	else {                                                                                      // If 1 char is not the same, then the word does not match at all so we return 0
		return 0;
	}
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
int chatbot_do_smalltalk(int inc, char* inv[], char* response, int n) {
	time_t t;

	for (int i = 0; i < MAX_REPLIES; i++) {
		if (compare_token(inv[0], USER_GREETINGS[i]) == 0) {
			srand((unsigned)time(&t));                                                          // Initialise a random number generator
			int rng = rand() % MAX_REPLIES;                                                     // Gets a ramdom number between and MAX_REPLIES
			sprintf(response, "%s", CHATBOT_GREETINGS[rng]);                                    // Print out the response from the bot
		}
		else if (strncmp(inv[0], "it's", 4) == 0 || strncmp(inv[0], "It's", 4) == 0) {          // Check the user input for the predefined keyword "it's" or "It's". Somehow compare_token hates It's
			snprintf(response, n, "Indeed it is.");                                             // Print out the proper response from the bot
			return 0;
		}
	}
	return 0;
}
