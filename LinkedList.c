#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
//#include <setjmp.h>

#include "LinkedList.h"
#include "dataStore.h"

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


void displayUserInterface(ListNodePtr *head)
{
    int choose_operation = 0;
    char *pet_name =  NULL;
    pet_name = (char*) malloc( MAX_NAME_LENGTH * sizeof(char));
    char *animal_type =  NULL;
    animal_type = (char*) malloc( MAX_NAME_LENGTH * sizeof(char));

    printf("\n---------------------------------\n");
    printf("\nAnimal shelter management system\n");
    printf("\n---------------------------------\n");
    printf("\n1.Insert pet to the list ");
    printf("\n2.Remove pet from list");
    printf("\n3.Update pet's information ");
    printf("\n4.Show more details about pet ");
    printf("\n5.Check shelter status");
    printf("\n6.Exit and save\n");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("\nEnter your choice: ");
    scanf("%d", &choose_operation);

    switch (choose_operation)
    {
    case 1:
        printf("\nEnter a pet name: ");
        scanf("%s", pet_name);
        printf("\nEnter a kind of animal: ");
        scanf("%s", animal_type);
        insertNodeAtTheBeginning(head, pet_name,animal_type);
        printList(*head); // value function
        break;
    case 2:
        printf("\nEnter a pet name or ID: ");
        scanf("%s", pet_name);
        // printf("\n\nDEBUG %s\n", pet_name);
        removeNodeAtPosition(head,pet_name);
        printList(*head); // value functio
        break;
    case 3:
        printf("Enter a pet name or ID: ");
        scanf("%s", pet_name);
        updateNodeValues(head,pet_name);
        printList(*head);
        break;
    case 4:
        //show m
        printf("Enter a pet name or ID: ");
        scanf("%s", pet_name);
        showNodeValuesAtPosition(head,pet_name);
        break;
    case 5:
        checkShelterStatus();
        break;
    case 6:
        printf("\n...Exiting...\n");
        writeData(*head);
        exit(0);
    default:
        printf("\n...Invalid Choice...\n");
        break;
    }

    free(pet_name);
    free(animal_type);
}


void insertNodeAtTheBeginning(ListNodePtr *head, char* name, char* pet_type)
{
    ListNodePtr new_node = malloc(sizeof(node_t));
    new_node -> animal_ID = createID(); //
    strcpy(new_node -> animal_name, name);//
    strcpy(new_node -> animal_species, pet_type);//
    new_node -> nextPtr = *head;
    *head = new_node;

    // Increment current animals number in shelter (change later)
    current_animals_number++;
}

void removeNodeAtPosition(ListNodePtr *head, char* name)
{
    int position = getNodePosition(head,name);

    if(position == 0)
    {
        printf("There is no such animal at the list, check its name!\n");
    }
    else
    {
        ListNodePtr current = *head; // create pointer to 1st element to count numbers of NODEs in list
        ListNodePtr prev = NULL; // create pointer

        if ( position == 1 )
        {
            *head = current->nextPtr;
            printf("\nElement deleted\n");
        }
        else
        {
            for (int i = 1; i < position; i++)
            {
                prev = current;
                current = current -> nextPtr;
            }
            prev->nextPtr = current->nextPtr;
        }

        free(current);
        printf("\nElement deleted\n");
    }
}

int getNodePosition(ListNodePtr *head, char* searchedName)
{
    ListNodePtr current = *head;
    int flag=0, position =1;
    long searchedID = strtol(searchedName, NULL, 10);
    int result_names_comparison = 0;
    long current_node_ID = 0;

    if( current == NULL)
    {
        printf("\n List is empty !\n");
        return 0;
    }
    else
    {
        while(current != NULL)
        {
            result_names_comparison = strcmp(current->animal_name, searchedName);
            current_node_ID = current->animal_ID;

            if(result_names_comparison == 0) //search by name
            {
                flag = 1; // set flag animal found at the list
                printf("\nAnimal %s found at the list at position %d\n", searchedName,position);
                break;
            }
            else if(searchedID == current_node_ID) //search by ID
            {
                flag = 1; // set flag animal found at the list
                printf("\nAnimal ID: %ld found at the list at position %d\n", searchedID,position);
                break;
            }
            else
            {
                position++;
                current = current->nextPtr; // set pointer to next NODE
            }
        }
        // Check flag/ result
        if(flag)
        {
            return position;
        }
        else
        {
            printf("\nAnimal %s not found on the list\n",  searchedName);
            return 0;
        }
    }
}



long createID(void)
{
    char current_time[100];
    long ID_number = 0;
    time_t temp;
    struct tm *timeptr;

    temp = time(NULL);
    timeptr = localtime(&temp);

    strftime(current_time,sizeof(current_time),"%Y%m%d%H%M%S", timeptr);
    ID_number = atol(current_time);
    return ID_number;
}


void checkShelterStatus(void)
{
    if(current_animals_number < MAX_PLACE_NUMBER)
    {
        printf("There are free places in the shelter\n");
    }
    else if(current_animals_number == MAX_PLACE_NUMBER)
    {
        printf("The shelter is full\n");
    }
    else
    {
        printf("The shelter is overflowing\n");
    }

}

void updateNodeValues(ListNodePtr *head, char* name)
{
    int position = getNodePosition(head,name);

    if(position == 0)
    {
        printf("There is no such animal at the list, check its name!\n");
    }
    else
    {
        int select_action = showUpdateOptions();
        changeNodeValue(head, position, select_action);
    }
}


void changeNodeValue(ListNodePtr *head, int animal_position_on_list, int selected_action)
{
    ListNodePtr current = *head;
    char *new_name =  NULL;
    new_name = (char*) malloc( MAX_NAME_LENGTH * sizeof(char));
    char *new_type =  NULL;
    new_type = (char*) malloc( MAX_NAME_LENGTH * sizeof(char));
    long new_ID;

    /* set pointer to indicated node */
    for(int i=1; i<animal_position_on_list; i++)
    {
        current = current -> nextPtr;
    }

    switch(selected_action)
    {
    case 1:
        printf("Enter new pet name: ");
        scanf("%s", new_name);
        strcpy(current->animal_name, new_name);
        break;
    case 2:
        printf("Enter new pet ID: ");
        scanf("%ld", &new_ID);
        current->animal_ID = new_ID;
        break;
    case 3:
        printf("Enter pet type: ");
        scanf("%s", new_type);
        strcpy(current->animal_species, new_type);
        break;
    default:
        printf("\n...Invalid Choice...\n");
        break;
    }

    free(new_name);
    free(new_type);
}

int showUpdateOptions(void)
{
    int select_action = 0;

    printf("\nChoose information to edit ");
    printf("\n1.Name ");
    printf("\n2.Id ");
    printf("\n3.Type ");
    printf("\n4.None of the above\n");
    scanf("%d",&select_action);

    return select_action;
}

void showNodeValuesAtPosition(ListNodePtr *head, char* name)
{
    ListNodePtr currentPtr = *head;

    int position = getNodePosition(head,name);

    if(position == 0)
    {
        printf("There is no such animal at the list, check its name!\n");
    }
    else
    {
        /* set pointer to indicated node */
        for(int i=1; i<position; i++)
        {
            currentPtr = currentPtr -> nextPtr;
        }

        printf("ID: %ld\nName: %s\nType: %s\n", currentPtr -> animal_ID,currentPtr -> animal_name, currentPtr->animal_species);
    }

}
