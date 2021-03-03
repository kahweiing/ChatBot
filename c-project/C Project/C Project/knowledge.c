/*
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
#include <ctype.h>


 /* ===================================================================== START OF KNOWLEDGE_GET ===================================================================== */
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
 *   R_PASS, if a response was found for the intent and entity (the response is copied to the response buffer)
 *   R_FAIL, if no response could be found
 *   R_INVALID, if 'intent' is not a recognised question word
 */
int knowledge_get(const char* intent, const char* entity, char* response, int n) {


    // When the intent is "what"
    if (compare_token(intent, "what") == 0 && WhatHeadPtr != NULL)                  // If intent is what and verify if Linked-list exists                                     
        {                                                                       
            whatIteratorPtr = WhatHeadPtr;                                      //Point the whatIteratorPtr to the heat WhatHeadPtr which is the start of the linked list
            do 
            {                                                                   // Point to next node if not found. Will point to NULL if it's last node
                if (compare_token(whatIteratorPtr->entity, entity) == 0) 
                {
                    strncpy(response, whatIteratorPtr->response, n);            // Copy to response buffer if node with similar entity found
                    return R_PASS;                                               // Return pass after copy
                }

                whatIteratorPtr = whatIteratorPtr->next;                        // Point to next node if no match found
            } 
            while (whatIteratorPtr);
            return R_FAIL;                                                     // Call back knowledge_put If not run inside the loop
        }
    /************************************************* ELSE IF INTENT = "WHERE" *******************************************************/
    else if (compare_token(intent, "where") == 0 && WhereHeadPtr != NULL)        // If intent is where and verify if Linked-list exists
    {                                           
            whereIteratorPtr = WhereHeadPtr;                                   //Point the whereIteratorPtr to the heat WhereHeadPtr which is the start of the linked list
            do                                                                 // if no match then point to the next node, if in last point then point to null
            {                                                                  
                if (compare_token(whereIteratorPtr->entity, entity) == 0)      // Check if a node with same entity exist
                {                                                              
                    strncpy(response, whereIteratorPtr->response, n);          // Copy to response buffer
                    return R_PASS;                                              // return ok after copy
                }

                whereIteratorPtr = whereIteratorPtr->next;                     // Point the iterator to the next node, if entity not the same
            }            
            while (whereIteratorPtr);
            // If code reaches this point, it means that after iterating through all nodes in Linked-list, no match was found
            // Hence, return a R_FAIL code (-1), this makes chatbot_do_question invoke knowledge_put()
            return R_FAIL;
    }
    /**************************************************ELSE IF INTENT = "WHO" ***********************************************************/
    else if (compare_token(intent, "who") == 0 && WhoHeadPtr != NULL)   // If intent is who and verify if Linked-list exists
    {                                                            
            whoIteratorPtr = WhoHeadPtr;                                //Point the whoIteratorPtr to the heat WhoHeadPtr which is the start of the linked list
            do 
            {                                                     // While no match is found, Make the iterator point to the next node (if last node, it will point to NULL)
                if (compare_token(whoIteratorPtr->entity, entity) == 0) 
                {                                                       // If a node with the same entity exists
                    strncpy(response, whoIteratorPtr->response, n);      // Copy it to the response buffer
                    return R_PASS;                                      // return ok after copy
                }

                whoIteratorPtr = whoIteratorPtr->next;                   // Point the iterator to the next node if it does not enter the if statetment
            } while (whoIteratorPtr);
            return R_FAIL;
    }
    // If code reaches this point, it means that after iterating through all nodes in Linked-list, no match was found
    // Hence, return a R_FAIL code (-1), this makes chatbot_do_question invoke knowledge_put()
    /***************************************** ELSE IF INTENT NOT RECOGNIZED, RETURN R_FAIL *************************************************/
    else 
    {
        return R_FAIL;
    }
}
/* ===================================================================== END OF KNOWLEDGE_GET ===================================================================== */


/* ===================================================================== START OF KNOWLEDGE_PUT ===================================================================== */
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
 *   KB_FOUND, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char* intent, const char* entity, const char* response) {

    /* ===================================================== CHECK IF ENTITY & INTENT PAIR EXIST ===================================================== */
    char TempBuffer[MAX_RESPONSE];                                        //Create a temporary buffer which is needed to call knowledge_get 
    int code = knowledge_get(intent, entity, TempBuffer, MAX_RESPONSE);   //Check if the Intent and Entity pair already exists by calling knowledge_get 
    memset(TempBuffer, '\0', MAX_RESPONSE);                               // Clear temporary buffer 

    /* Return R_INVALID if Intent is invalid */
    if (abs(code) == 2) {
        return R_INVALID;
    }

    /* Else, depending on the return code, it will either overwrite or inside the node, and the Intent */
    else {
        /* ===================================================== IF INTENT = "WHAT" ===================================================== */
        if (compare_token(intent, "what") == 0) {                    
            //  Iterate the 'WHAT' linked-list & overwrite current response data if Intent and Entity pair already exists
            if (abs(code) == 0) {
                whatIteratorPtr = WhatHeadPtr;                                   //Point the whatIteratorPtr to the heat WhatHeadPtr which is the start of the linked list
                do {                                                             //Iterator point to the next node if no match is found, if it's the last node, it will point to NULL 
                    if (compare_token(whatIteratorPtr->entity, entity) == 0) {          //Node with the same Entity exists 
                        strncpy(whatIteratorPtr->response, response, MAX_RESPONSE);        //Overwrite existing response
                        //printf("\nUpdated Intent: '%s'\nUpdated Entity: '%s'\nUpdated Response: '%s'\n", intent, entity, response);
                        return R_PASS;                                           // return ok after copy
                    }

                    whatIteratorPtr = whatIteratorPtr->next;                       //Point the iterator to the next node if it does not enter the if statetment
                } while (whatIteratorPtr);
            }

            //  Create & insert the node to the head of 'WHAT' linked-list if no existing record of Intent and Entity exists*/
            else if (abs(code) == 1) {
                knowledge_node* temp = malloc(sizeof(knowledge_node));          //Create node first 
                if (temp == NULL) {                                           //If not enough memory
                    return R_MEMNULL;                                          //Return no memory
                }

                strcpy(temp->intent, intent);                                 //Setting node's intent attribute to intent 
                strcpy(temp->entity, entity);                                 //Setting node's entity attribute to entity 
                strcpy(temp->response, response);                             //Setting node's response attribute to response 
                temp->next = WhatHeadPtr;                                     //Setting created node to point to where the head of the WhatHeadPtr linked-list is pointing to 
                WhatHeadPtr = temp;                                           //Set 'WhatHeadPtr' to point to temp

                /* Testing code */
                //printf("\nAdding Intent of WhatHeadPtr: '%s'\nAdding Entity of WhatHeadPtr: '%s'\nAdding Response of WhatHeadPtr: '%s'\n", WhatHeadPtr->intent, WhatHeadPtr->entity, WhatHeadPtr->response);
                return R_PASS;                                                 // return ok after copy
            }
        }

        /* ===================================================== ELSE IF INTENT = "WHERE" ===================================================== */
        else if (compare_token(intent, "where") == 0) {              
            //  Iterate the 'WHERE' linked-list & overwrite current response data, if Intent and Entity pair already exists
            if (abs(code) == 0) {
                whereIteratorPtr = WhereHeadPtr;                                  //Point the whereIteratorPtr to the heat WhereHeadPtr which is the start of the linked list
                do {                                                              //Iterator point to the next node if no match is found, if it's the last node, it will point to NULL
                    if (compare_token(whereIteratorPtr->entity, entity) ==
                        0) {          /* If a node with the same Entity exists */
                        strncpy(whereIteratorPtr->response, response, MAX_RESPONSE);        //Overwrite the existing response 
                        //printf("\nUpdated Intent: '%s'\nUpdated Entity: '%s'\nUpdated Response: '%s'\n", intent, entity, response);
                        return R_PASS;                                         // return ok after copy
                    }

                    whereIteratorPtr = whereIteratorPtr->next;                      // Point the iterator to the next node if it does not enter the if statetment
                } while (whereIteratorPtr);
            }

            //  If no existing record of Intent and Entity exists, create & insert the node to the head of 'WHERE' linked-list */
            else if (abs(code) == 1) {
                knowledge_node* temp = malloc(sizeof(knowledge_node));          //Create node first 
                if (temp ==
                    NULL) {                                                   //If not enough memory
                    return R_MEMNULL;                                          //Return no memory
                }

                strcpy(temp->intent, intent);                                //Setting node's intent attribute to intent 
                strcpy(temp->entity, entity);                                //Setting node's entity attribute to entity 

                strcpy(temp->response, response);                             //Setting node's response attribute to response
                temp->next = WhereHeadPtr;                                     //Setting created node to point to where the head of the WhereHeadPtr linked-list is pointing to 
                WhereHeadPtr = temp;                                            //Set 'WhereHeadPtr' to point to temp

                /* Testing code */
                //printf("\nAdding Intent of headofWHERE: '%s'\nAdding Entity of WhereHeadPtr: '%s'\nAdding Response of WhereHeadPtr: '%s'\n", WhereHeadPtr->intent, WhereHeadPtr->entity, WhereHeadPtr->response);
                return R_PASS;                                                 // return ok after copy
            }
        }

        /* ===================================================== ELSE IF INTENT = "WHO" ===================================================== */
        else if (compare_token(intent, "who") == 0) {             /* If intent is 'WHERE' */
            //  If Intent and Entity pair already exists, iterate the 'WHO' linked-list & overwrite current response data
            if (abs(code) == 0) {
                whoIteratorPtr = WhoHeadPtr;                                    //Point the whoIteratorPtr to the heat WhoHeadPtr which is the start of the linked list
                do {                                                           //Iterator point to the next node if no match is found, if it's the last node, it will point to NULL
                    if (compare_token(whoIteratorPtr->entity, entity) == 0) {               //Node with the same Entity exists
                        strncpy(whoIteratorPtr->response, response, MAX_RESPONSE);        //Overwrite existing response
                        //printf("\nUpdated Intent: '%s'\nUpdated Entity: '%s'\nUpdated Response: '%s'\n",intent, entity, response);
                        return R_PASS;                                        // return ok after copy
                    }

                    whoIteratorPtr = whoIteratorPtr->next;                           //Point the iterator to the next node if it does not enter the if statetment
                } while (whoIteratorPtr);
            }

            //  If no existing record of Intent and Entity exists, create & insert the node to the head of 'WHO' linked-list */
            else if (abs(code) == 1) {
                knowledge_node* temp = malloc(sizeof(knowledge_node));         //Create node first 
                if (temp == NULL) {                                             //If not enough memory
                    return R_MEMNULL;                                         //Return no memory
                }

                strcpy(temp->intent, intent);                                  //Setting node's intent attribute to intent 
                strcpy(temp->entity, entity);                                  //Setting node's entity attribute to entity
                strcpy(temp->response, response);                              //Setting node's response attribute to response
                temp->next = WhoHeadPtr;                                       //Setting created node to point to where the head of the WhoHeadPtr linked-list is pointing to 
                WhoHeadPtr = temp;                                             //Set 'WhoHeadPtr' to point to temp

                /* Testing code */
                //printf("\nAdding Intent of WhoHeadPtr: '%s'\nAdding Entity of WhoHeadPtr: '%s'\nAdding Response of WhoHeadPtr: '%s'\n",WhoHeadPtr->intent, WhoHeadPtr->entity, WhoHeadPtr->response);
                return R_PASS;                                                // return ok after copy
            }

        }
    }

}

/* ===================================================================== END OF KNOWLEDGE_PUT ===================================================================== */

/* ===================================================================== START OF KNOWLEDGE_READ ===================================================================== */

/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE* f) {

    int NoOfLines = 0;
    char intent[MAX_INPUT], entity[MAX_ENTITY], response[MAX_RESPONSE];
    char line[MAX_ENTITY + 1 + MAX_RESPONSE +1];
    char* splitLine;
    intent[0] = '\0';

    /************************************************* READ LINE *******************************************************/
    while (fgets(line, sizeof line, f) != NULL) {

        int ERROR_INDICATOR = 1;
        
        line[strcspn(line, "\n")] = 0;// Remove trailing newline

        /************************************************* IF INTENT = "WHAT" *******************************************************/
        if (strcmp("[what]", line) == 0) {
            strcpy(intent, "WHAT");
            continue;
        }
        /************************************************* IF INTENT = "WHERE" *******************************************************/
        if (strcmp("[where]", line) == 0 ) {
            strcpy(intent, "WHERE");
            continue;
        }
        /************************************************* IF INTENT = "WHO" *******************************************************/
        if (strcmp("[who]", line) == 0) {
            strcpy(intent, "WHO");
            continue;
        }
        
        /************************************************* CHECK IF "=" EXIST *******************************************************/
        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == '=') {
                ERROR_INDICATOR = 0;
            }
        }

        /************************************************* IF BLANK LINE FOUND OR INTENT NOT FOUND YET *******************************************************/
        if (line[0] == '\n' || intent[0] == '\0') {    
            continue;
        }
        else {
            /************************************************* IF "=" DOES NOT EXIST *******************************************************/
            if (ERROR_INDICATOR == 1) {                         // if ERROR_INDICATOR is 1 ("=" does not exist in line)
                continue;
            }
            /************************************************* ELSE IF INSERT TO LIST*******************************************************/
            // Separate line by the "="
            splitLine = strtok(line, "=");                      // Get the Entity from the line
            strcpy(entity, splitLine);                          
            splitLine = strtok(NULL, "=");                      // Get the Response from the line
            splitLine[strcspn(splitLine, "\n")] = 0;            // Get rid of "\n"
            strcpy(response, splitLine);                        
            knowledge_put(intent, entity, response);            // Call knowledge_put to insert into the list
            NoOfLines++;
        }
    }                                      
    return NoOfLines;                                           // Return the number of entity/response pairs successful read from the file
}

/* ===================================================================== END OF KNOWLEDGE_READ ===================================================================== */

/* ===================================================================== START OF KNOWLEDGE_RESET ===================================================================== */
/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {
    // We have to manually headPtr = headPtr->next because if we just do iteratorPtr = headPtr->next, the memory is still uninitialised
    // Remember: IteratorPtr is used to walk through the linked list so we use that to free the memory, not the original headPtr
    // In order to reset the bot's memory, we just free the memory allocated by the whatheadptr, whoheadptr and the whereheadptr at the start. When the whole program restarts, it will reassign it again
    
    /************************************************* LOOP UNTIL ALL LINK-LIST IS REMOVED *******************************************************/
    while (WhatHeadPtr != NULL || WhereHeadPtr != NULL || WhoHeadPtr != NULL) {         // Keep looping through to free all the memory until it hits a null

        /************************************************* REMOVE "WHAT" LINKED-LIST *******************************************************/
        if (WhatHeadPtr != NULL) {                                      // Check to make sure the ptr has something
            whatIteratorPtr = WhatHeadPtr;                              // Point the what iterator to the what head ptr which is the start of the linked list
            WhatHeadPtr = WhatHeadPtr->next;                            // Point to the next node first before the next loop
            free(whatIteratorPtr);                                      // Free the memory allocated by the what head pointer
            whatIteratorPtr = WhatHeadPtr;                              // Assign the WhatHeadPtr that we updated earlier before the free to the iterator for the second loop
    }
        /************************************************* REMOVE "WHERE" LINKED-LIST *******************************************************/
        if (WhereHeadPtr != NULL) {                                     // Check to make sure that the ptr has something
            whereIteratorPtr = WhereHeadPtr;                            // Point the where iterator to the where head ptr which is the start of the linked list                               
            WhereHeadPtr = WhereHeadPtr->next;                          // Point to the next node first before the next loop
            free(whereIteratorPtr);                                     // Free the memory allocated by the where head pointer that we copied to the whereIterator
            whereIteratorPtr = WhereHeadPtr;                            // Assign the whereHeadPtr that we updated with the next node before the free to the Iterator for the second loop
       
    }
        /************************************************* REMOVE "WHO" LINKED-LIST *******************************************************/
        if (WhoHeadPtr != NULL) {                                       // Check to make sure that the ptr has something
            whoIteratorPtr = WhoHeadPtr;                                // Point the who iterator to the who head ptr which is the start of the linked list
            WhoHeadPtr = WhoHeadPtr->next;                              // Point to the next node first before the next loop
            free(whoIteratorPtr);                                       // Free the memory allocated by the whoIteratorPtr
            whoIteratorPtr = WhoHeadPtr;                                // Assign the whoHeadPtr that we updated with the next node before the free to the Iterator for the second loop
        }
    }
}
/* ===================================================================== END OF KNOWLEDGE_RESET ===================================================================== */


/* ===================================================================== START OF KNOWLEDGE_WRITE ===================================================================== */
/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE* f) {

    if (WhatHeadPtr != NULL) {                              // Check if the head pointer for the what node is actually pointing to something
        knowledge_node* temp = WhatHeadPtr;                 // Create a temp pointer which points to the head of the what entity node
        fprintf(f, "[%s]\n", "what");                       // Then we print out the header in the ini file [what]
        while (temp != NULL) {                              // After printing the header, we go to the next line and start looping through the arrays until the temp pointer hits a null
            fprintf(f, "%s=", temp->entity);                // Then we print out into the ini file the entity (Example: SIT=)
            fprintf(f, "%s\n", temp->response);             // On the same line we also print out the response for the associate entity (SIT=SIT is an autonomous university in Singapore.)
            temp = temp->next;                              // After we print a complete line, we advance the temp pointer to the address of the next node head
        }
    }

    fprintf(f, "%s", "\n");

    if (WhoHeadPtr != NULL) {                               // Check if the head pointer for the who node is actually pointing to something
        knowledge_node* temp = WhoHeadPtr;                  // Create a temp pointer which points to the head of the who entity node
        fprintf(f, "[%s]\n", "who");                        // Print out the header in the ini file [who]
        while (temp != NULL) {                              // Start looping through the arrays until the temp pointer hits a null
            fprintf(f, "%s=", temp->entity);                // Print out the entity at the start followed by the =
            fprintf(f, "%s\n", temp->response);             // Print out the response after the =
            temp = temp->next;                              // Move to the next node
        }
    }

    fprintf(f, "%s", "\n");

    if (WhereHeadPtr != NULL) {                             // Check if the head pointer for the where node is actually pointing to something
        knowledge_node* temp = WhereHeadPtr;                // Create a temp pointer and assign the value of the where head pointer to it
        fprintf(f, "[%s]\n", "where");                      // Print out the header in the ini file [where]
        while (temp != NULL) {                              // Start looping through the array until the temp pointer hits a null
            fprintf(f, "%s=", temp->entity);                // Print out the entity at the start followed by =
            fprintf(f, "%s\n", temp->response);             // Print out the response after the =
            temp = temp->next;                              // Move to the next node
        }
    }

}
/* ===================================================================== END OF KNOWLEDGE_WRITE ===================================================================== */
