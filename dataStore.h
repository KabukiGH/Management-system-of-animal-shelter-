#ifndef DATASTORE_H_INCLUDED
#define DATASTORE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

#define FILENAME "data.txt"


/* File operation func*/
int readData(void); // read data from file return 0 if success
void loadData(ListNodePtr *head); // load data to system
void writeData(ListNodePtr currentPtr);


#endif // DATASTORE_H_INCLUDED
