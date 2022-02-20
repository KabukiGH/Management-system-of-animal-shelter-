#include "dataStore.h"
#include "LinkedList.h"
#include <stdbool.h>


/* global variables */
int current_animals_number = 0;
char **result_read_info = NULL; // readData
int line_count = 0; // readData

int main()
{

      ListNodePtr head = NULL;

      int read_data_result = readData();

    if(read_data_result==0)
    {
        loadData(&head);
        printList(head);
    }
    else
    {
        fprintf(stderr, "Error reading file '%s'\n", FILENAME);
        exit(EXIT_FAILURE);
    }


    while(true)
    {
        displayUserInterface(&head);

    } // end while

    return 0;
} // end main

