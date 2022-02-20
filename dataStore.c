#include "dataStore.h"


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
