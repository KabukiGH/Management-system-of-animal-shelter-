#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED


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

/* global variables */
extern int current_animals_number;
extern char **result_read_info; // readData
extern int line_count; // readData

/* Functions prototypes */
/* Link list operation func*/
void displayUserInterface(ListNodePtr *head);
void insertNodeAtTheBeginning(ListNodePtr *head, char*, char*);
void removeNodeAtPosition(ListNodePtr *head, char*);
void printList( ListNodePtr currentPtr);
void updateNodeValues(ListNodePtr *head, char*);
void showNodeValuesAtPosition(ListNodePtr *head, char*);

/* Helper func*/
int getNodePosition(ListNodePtr *head, char*); // search that pet is on the list if "YES" return position, else return 0
long createID(void); // generated ID based on time of admission to the shelter
void checkShelterStatus(void);
int showUpdateOptions(void);
void changeNodeValue(ListNodePtr *head, int, int);

#endif // LINKEDLIST_H_INCLUDED
