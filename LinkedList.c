#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
//#include <setjmp.h>

#include "LinkedList.h"

void printList( ListNodePtr currentPtr)
{
    if (currentPtr == NULL)
    {
        printf("List is empty.\n\n");
    }
    else
    {
        printf("The list is:\n");
    }

    while(currentPtr !=NULL)
    {
        printf("%s --> ",currentPtr ->animal_name);
        currentPtr = currentPtr ->nextPtr;
    }
    printf("NULL\n\n");
}
