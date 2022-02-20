#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <setjmp.h>

#define FILENAME "data.txt"

#define MAX_PLACE_NUMBER 10
#define MAX_NAME_LENGTH 25
#define MAX_ROW_LENGTH 255

/* self-referential structure */
typedef struct node
{
    long animal_ID;
    char animal_name[MAX_NAME_LENGTH];
    char animal_species[MAX_NAME_LENGTH];
    struct node *nextPtr;
} node_t;

typedef node_t *ListNodePtr;

/* Functions prototypes */
/* Link list operation func*/
void displayUserInterface(ListNodePtr *head);
void insertNodeAtTheBeginning(ListNodePtr *head, char*, char*);
void removeNodeAtPosition(ListNodePtr *head, char*);
void printList( ListNodePtr currentPtr);
void updateNodeValues(ListNodePtr *head, char*);
void showNodeValuesAtPosition(ListNodePtr *head, char*);
/* File operation func*/
int readData(void); // read data from file return 0 if success
void loadData(ListNodePtr *head); // load data to system
void writeData(ListNodePtr currentPtr);
/* Helper func*/
int getNodePosition(ListNodePtr *head, char*); // search that pet is on the list if "YES" return position, else return 0
long createID(void); // generated ID based on time of admission to the shelter
void checkShelterStatus(void);
int showUpdateOptions(void);
void changeNodeValue(ListNodePtr *head, int, int);

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

int readData(void)
{
    /*allocate memory for 10 strings */
    result_read_info = calloc(MAX_PLACE_NUMBER,sizeof(char*));

    /* Open the file for reading */
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    // int line_count = 0;
    ssize_t line_size;
    FILE *fp = fopen(FILENAME, "r");

    if (!fp)
    {
        fprintf(stderr, "Error opening file '%s'\n", FILENAME);
        return EXIT_FAILURE;
    }
    else
    {
        /* Get the first line of the file. */
        line_size = getline(&line_buf, &line_buf_size, fp);
        /* allocate memory for 1st element and save data in main buffor */
        result_read_info[line_count] = malloc(MAX_ROW_LENGTH*sizeof(char));
        strcpy(result_read_info[line_count],line_buf);

        /* Loop through until we are done with the file. */
        while (line_size >= 0)
        {
            /* Increment our line count */
            line_count++;

            /* Get the next line */
            line_size = getline(&line_buf, &line_buf_size, fp);

            /* Allocate 255b for each string Saved data to main buffor */
            result_read_info[line_count] = malloc(MAX_ROW_LENGTH*sizeof(char));
            strcpy(result_read_info[line_count],line_buf);
        }
        /* Free the allocated line buffer */
        free(line_buf);

        /* Close the file now that we are done with it */
        fclose(fp);

        return EXIT_SUCCESS;
    }
}


void loadData(ListNodePtr *head)
{
    char* readID_number = NULL;
    long ID = 0;
    char* name = NULL;
    char* endID_ptr = NULL;
    char delimit[]=" \t\r\n\v\f"; // POSIX whitespace characters
    char* type = NULL;

    for (int i = 0; i < line_count; i++)
    {
        // printf ("%d: %s", i, result_read_info[i]);

        /*extract ID NUMBER */
        readID_number = strtok(result_read_info[i],delimit);
        ID = strtol(readID_number, &endID_ptr, 10);
        //  printf("ID num: %ld\n", ID);

        /*extract Animal NAME if exist*/
        name = strtok(NULL,delimit);
        // printf("Pet name: %s\n", name);

        /*extract Animal TYPE if exist */
        type = strtok(NULL,delimit);


        /* When you have all necessary parameters ID, Name, spiecies, etc. call load function*/
        ListNodePtr new_node = malloc(sizeof(node_t));
        new_node -> animal_ID = ID; //
        strcpy(new_node -> animal_name, name);//
        if(type !=NULL){
        strcpy(new_node -> animal_species, type);}//
        else{
        strcpy(new_node -> animal_species, " ");}//
        new_node -> nextPtr = *head;
        *head = new_node;

        // Increment current animals number in shelter (change later)
        current_animals_number++;
    }

    /* Free the allocated memory from main buffer */
    for(int i=0; i< line_count; i++)
    {
        free(result_read_info[i]);
    }

    free(result_read_info);

}


void writeData(ListNodePtr currentPtr)
{
    /* Create file pointer*/
    FILE *fp = fopen(FILENAME, "w");

    if (!fp)
    {
        fprintf(stderr, "Error opening file '%s'\n", FILENAME);
        exit(1);
    }
    else
    {
        while(currentPtr !=NULL)
        {
            fprintf(fp,"%ld %s %s\n",currentPtr ->animal_ID, currentPtr ->animal_name,currentPtr ->animal_species);
            currentPtr = currentPtr ->nextPtr;
        }
        /* Close the file now that we are done with it */
        fclose(fp);
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
