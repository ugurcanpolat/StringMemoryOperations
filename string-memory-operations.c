   /* ***************** */
  /*   Ugurcan Polat   */
 /* polatu@itu.edu.tr */
/* ***************** */

#include <stdio.h> // printf(), scanf(), getchar(), fgets(), stdin, NULL
#include <stdlib.h> // malloc(), realloc(), free()
#include <string.h> // strlen(), strcpy()
	
int user_menu(void);
int set_ccs(char **ccs);
void get_indexes(char **ccs, char **result, int *begin, int *end, int operation);
char *sub_string (char *ccs, int begin_index, int end_index);
char *remove_string(char **ccs, int begin_index, int end_index);
void get_insert(char **ccs);
int insert_string(char **ccs, char *insert, int begin_index);
void get_replace(char **ccs, int *replacement);
int replace_string(char **ccs, char *find, char *replace);
void printer(char **ccs, char *result, int begin, int end, int operation);

int main() {
	int operation, begin, end, replacement;
	char *ccs = NULL, *result = NULL;
	
	/* Do operations until user enters '0' to exit */
	do {
		operation = user_menu(); 
		
		/* Check if the selected option is other than '1' before setting 'ccs' */
		if (ccs == NULL && operation != 1 && operation != 0) {
			printf("\n\nYou have to set Current Character Sequence "); 
			printf("(CCS) before doing any operations!\n\n\n");
			/* Operation '6' will be used to break the switch */
			operation = 6;
		} //if
		
		/* 'result' will only be used by sub_string() or remove_string() */
		/* Allocate 'result' by size of 80 char */
		if (operation == 2 || operation == 3) 
			result = malloc(80 * sizeof(char));
		
		switch (operation) {
			case 0: break;
			case 1: set_ccs(&ccs); break;
			case 2: get_indexes(&ccs, &result, &begin, &end, 2); break;
			case 3: get_indexes(&ccs, &result, &begin, &end, 3); break;
			case 4: get_insert(&ccs); break;
			case 5: get_replace(&ccs, &replacement); break;
			case 6: break;
			default:{ 
						printf("\n\nPlease choose one of the operations listed below! \n");
						break;
					} //default
		} //switch	
		
		/* If replace_string() is used, print number of replacements. */
		if (operation == 5)
			printf("\nThere were %d replacement.", replacement);
		
		/* Call printer() to print results */	
		if (operation != 0 && operation != 1 && operation != 6){
			printer(&ccs, result, begin, end, operation);
		}
		
		/* Free the memory used by 'result' */
		if (operation == 2 || operation == 3)
			free(result);
			
	} while (operation != 0);
	
	free(ccs);
	/* Say goodbye! */
	printf("\nGoodbye!\n\n");
	return 0;
} //main()

int user_menu(void) {
	int operation;
	/* Print operation options and get an operation from user */
	printf("\n0: Exit the program\n");
	printf("1: Set Current Character Sequence\n2: Get Substring\n");
	printf("3: Remove Substring\n4: Insert\n5: Replace\n");
	printf("\n\nEnter Your Choice: ");
	scanf("%d", &operation);
	
	/* Remove '\n' from stdin buffer */
	while (getchar() != '\n');
	
	return operation;
} //user_menu()

int set_ccs(char **ccs){
	int allocator, counter;
	/* Allocate memory for 'ccs' by size of 80 char */
	*ccs = malloc(80 * sizeof(char));
	printf("Enter CCS: ");
	/* Get input (max. 80 character) for 'ccs' from user */
	fgets(*ccs, 80, stdin);
	
	/* Get rid of '\n' comes from fgets() by changing them with null-terminator */
	for (counter = 0; counter < strlen(*ccs); counter++){
		if (*(*ccs+counter) == '\n')
			*(*ccs+counter) = '\0';
	} //for
	
	/* Determine how much memory will be used for 'ccs' with its new length */
	allocator = strlen(*ccs) + 1;
	/* Re-allocate 'ccs' by its new length */
	*ccs = realloc(*ccs, allocator * sizeof(char));
	
	/* If it is unable to re-allocate 'ccs', return -1 */
	if (!(*ccs)) 
	   return -1;
  
	printf("\n");
	/* Return how many bytes consumed by 'ccs' */
	return allocator * sizeof(char);
} //set_ccs()

/* get_indexes() gets 'result' to store returned strings. */
/* The last parameter the function gets is the operation. */
void get_indexes(char **ccs, char **result, int *begin, int *end, int operation){
	int begin_index, end_index;
	printf("Enter the BEGIN INDEX and END INDEX numbers: ");
	/* Get BEGIN INDEX and END INDEX, respectively */
	scanf("%d%d", &begin_index, &end_index);
	
	/* Assign entered values to variables that will be used in printer() */
	*begin = begin_index;
	*end = end_index;
	
	/* If option 2 is selected, call sub_string() */
	if (operation == 2)
		*result = sub_string(*ccs, begin_index, end_index);
	/* If option 3 is selected, call remove_string() */
	else if (operation == 3)
		*result = remove_string(ccs, begin_index, end_index);
} //get_indexes()

char *sub_string (char *ccs, int begin_index, int end_index){
	int counter, counter2;
	/* Determine how much memory will be used by 'sub_string' */
	int allocator = (end_index - begin_index) + 2;
	/* Allocate 'sub_string' by enough space for sub string will be extracted */
	char *sub_string = malloc(allocator * sizeof(char));
	
	/* Check if there is buggy situation. if there is, return NULL */
	if (begin_index < 0 || begin_index > strlen(ccs) || end_index < 0 ||
	    end_index > strlen(ccs) || end_index < begin_index){
	    printf("\nEntered indexes are out of bound or ");
	    printf("END INDEX is less than BEGIN INDEX\n");
		return NULL;
	} //if
	
	/* Extract sub string by given indexes from 'ccs' into 'sub_string' */
	counter2 = begin_index;
	for (counter = 0; counter2 <= end_index; counter2++) {
		*(sub_string+counter) = *(ccs+counter2);
		counter++;
	} //for
	
	/* Add null-terminator to the end of 'sub_string' */
	*(sub_string+(allocator-1)) = '\0';
	
	return sub_string;
} // sub_string()

char *remove_string(char **ccs, int begin_index, int end_index){
	int counter, counter2, current_length, previous_length;
	int allocator = (end_index - begin_index) + 2;
	/* Allocate 'removed' by enough space for string that will be */
	/* extracted from 'ccs' before removing it. */
	char *removed = malloc(allocator * sizeof(char));
	
	/* Check if there is buggy situation. if there is, return NULL */
	if (begin_index < 0 || begin_index > strlen(*ccs) || end_index < 0 ||
	    end_index > strlen(*ccs) || end_index < begin_index){
	    printf("\nEntered indexes are out of bound or ");
	    printf("END INDEX is less than BEGIN INDEX\n");
		return NULL;
	}
    
    /* Call sub_string() to get the sub string that will be removed */
    removed = sub_string(*ccs, begin_index, end_index);
    
    /* Get the length before removing the substring. */
	previous_length = strlen(*ccs) - 1;

	/* Shift characters come after the 'end_index' to the left until */
	/* they reach to 'begin_index'. This process will remove the */
	/* sub string 'removed'. */
	counter = begin_index; 
	counter2 = end_index + 1; 
	for (; counter2 <= strlen(*ccs) ; counter2++){
		*(*ccs+counter) = *(*ccs+counter2);
		counter++;
	} //for
	
	/* Get rid of characters that come after shifted characters. */
	/* It is necessary to avoid bugs while using this function in */
	/* replace_string(). Without this loop, there will be undesired */
	/* characters because of the shifting used to remove substring. */
	/* If we remove the following loop, the effect can be seen after */
	/* re-allocation of the memory by increasing after removing */
	/* the substring (e.g. (3 * strlen(*ccs)). Undesired characters */
	/* that are located in the memory before removing string. */
	current_length = strlen(*ccs);
	for (; current_length <= previous_length; current_length++)
		*(*ccs+current_length) = '\0';
	
	/* Re-allocate 'ccs' by its new length */
	allocator = strlen(*ccs) + 1;
	*ccs = realloc(*ccs, allocator * sizeof(char));
	
	return removed;
} //remove_string()

void get_insert(char **ccs){
	char *insert = malloc(80 * sizeof(char));
	int begin_index, counter;
	
	/* Get BEGIN INDEX and the string that will be inserted */
	printf("Enter a new string for insertion: ");
	fgets(insert, 80, stdin);
	printf("Enter the BEGIN INDEX number where the new string begins: ");
	scanf("%d", &begin_index);
	
	/* Get rid of '\n' comes from fgets() by changing them with null-terminator */
	for (counter = 0; counter < strlen(insert); counter++){
		if (*(insert+counter) == '\n')
			*(insert+counter) = '\0';
	} //for
	
	/* Re-allocate 'insert' by its new length */
	insert = realloc(insert, (strlen(insert) + 1) * sizeof(char));	
	
	/* If it is unable to re-allocate the memory give error */
	/* Otherwise, call insert_string() to insert the string 'insert'. */
	if (!insert)
		printf("\nUnable to re-allocate memory!\n");
	else
		insert_string(ccs, insert, begin_index);
} //get_insert()

int insert_string(char **ccs, char *insert, int begin_index){
	int allocator = (strlen(*ccs) + strlen(insert)) + 1;
	int end_insert = begin_index + (strlen(insert) - 1);
	int counter2, counter;
	
	/* Re-allocate 'ccs' by its new length and prepare for inserting */
	*ccs = realloc(*ccs, allocator * sizeof(char));
	
	/* If it is unable to re-allocate the memory give error */
	if(!(*ccs))
		return -1;
	
	/* Open space for the string that will be inserted. To do the that, */ 
	/* shift the characters to the right by the length of the string */
	/* that will be inserted. The following loop will do the described */	
	/* process starting from the last item of the string and ends when */
	/* it reaches the item on the BEGIN INDEX. */
	counter2 = strlen(*ccs);
	counter = counter2 + strlen(insert);
	for (;counter2 >= begin_index ; counter--){
		*(*ccs+counter) = *(*ccs+counter2);
		counter2--;
	} //for
	
	/* After opening space for insertion, 'ccs' is ready to get the */
	/* insert string. The following loop will insert the given string. */
	for (counter = 0; begin_index <= end_insert ;counter++){
		*(*ccs+begin_index) = *(insert+counter);
		begin_index++;
	} //for

	/* Free the memory used by 'insert' */
	free(insert);
	
	return allocator * sizeof(char);
} //insert_string()

void get_replace(char **ccs, int *replacement){
	char *find = malloc(80 * sizeof(char));
	char *replace = malloc(80 * sizeof(char));
	int counter;
	
	/* Get 'find' and 'replace' strings from user (max. 80 char.) */
	printf("\nFind what: ");
	fgets(find, 80, stdin);
	printf("Replace with: ");
	fgets(replace, 80, stdin);
	
	/* Get rid of '\n' comes from fgets() by changing them with null-terminator */
	for (counter = 0; counter < strlen(find); counter++){
		if (*(find+counter) == '\n')
			*(find+counter) = '\0';
	} //for
	
	/* Get rid of '\n' comes from fgets() by changing them with null-terminator */
	for (counter = 0; counter < strlen(replace); counter++){
		if (*(replace+counter) == '\n')
			*(replace+counter) = '\0';
	} //for
	
	/* Re-allocate 'find' and 'replace' by their new lengths */
	find = realloc(find, strlen(find)+1);
	replace = realloc(replace, strlen(replace)+1);
	
	/* If it is unable to re-allocate memory, give error. Otherwise, */
	/* call replace_string() to replace 'find's with 'replace's */
	if (!find || !replace)
		printf("\nUnable to allocate the memory!\n");
	else
		*replacement = replace_string(ccs, find, replace);
} //get_replace()

int replace_string(char **ccs, char *find, char *replace){
	int i, j, num_replace = 0, end_index;
	
	/* The memory will be used by begin_index cannot be more than */
	/* length of 'ccs' / length of 'find'. Allocate 'begin_index' */
	/* its maximum length. */
	int allocator = strlen(*ccs) / strlen(find);
	int *begin_index = malloc(allocator * sizeof(int));
	
	/* 'replace_cpy' will be used to copy the string 'replace */
	char *replace_cpy = NULL;
	
	/* Search 'find' in 'ccs'. If it is found, assign the */
	/* begin index of the 'find' into 'begin_index' */
	for (i = 0; i <= strlen(*ccs)-1; i++){
		j = 0;
		if (*(*ccs+i) == *(find+j)){
			for (; j <= strlen(find)-1;){
				if (*(*ccs+i) == *(find+j)){
					i++;
					if (j == strlen(find)-1){
						num_replace++;
						*(begin_index+(num_replace-1)) = i - strlen(find);
					} //if-3
					j++;
				} //if-2
				else
					break;
			} //for-2
			i--;
		} //if-1
	} //for-1
	
	/* The process of replacing a sub string with another one: 
	 * 1. Remove every found sub string that is wanted to be replaced.
	 *    This will be done by the first following loop. 	
	 * 2. Insert the string 'replace' to beginning of the found 
	 *    sub string that is wanted to be replaced. This will be done  
	 *    by second loop for every removed strings in the first loop. 
	 * 																  
	 * First loop: it uses begin indexes of the found sub strings 
	 * stored in the array 'begin_index'. It calculates every time 
	 * end index of the string that will be removed (1). Then, the loop 
	 * calls remove_string() to remove sub string by begin_index and 
	 * calculated end index (2). After removing sub string, begin indexes
	 * will change. Begin indexes will decrease by the length of 'find'
	 * for every removing. The loop calculates and updates the next 
	 * begin index (3). 
	 * 																  
	 * Second loop: it firstly allocate enough memory to copy 'replace' 
	 * for 'replace_cpy'(4). Then, it copies 'replace' to 'replace'cpy'(5).
	 * After that, it calls insert_string() to insert 'replace' (6). 
	 * insert_string() frees the string that will be inserted at the end
	 * of the function; therefore, the loop uses copy of the 'replace'
	 * to avoid errors (e.g. Memory being freed is not allocated). 
	 * The begin indexes increases by insertion, thus loop calculates 
	 * and updates the next begin index (7). 
	 * 
	 * As remove_string() and insert_string() functions are used in the 
	 * process for replacing, there is no need of re-allocating 'ccs'
	 * Because these functions re-allocates 'ccs' every time they called */
	 
	for (i = 0; i <= num_replace-1; i++){ // first loop
		end_index = (*(begin_index+i) + strlen(find)) - 1; // 1
		remove_string(ccs, *(begin_index+i), end_index); // 2
		*(begin_index+(i+1)) -= (i + 1) * (strlen(find));  // 3
	} //for
 
	for (i = 0; i <= num_replace-1; i++){ //second loop
		replace_cpy = malloc((strlen(replace) + 1) * sizeof(char)); // 4
		strcpy(replace_cpy, replace); // 5
		insert_string(ccs, replace_cpy, *(begin_index+i)); // 6
		*(begin_index+(i+1)) += (i + 1) * (strlen(replace)); // 7
	} //for
	
	/* Free the memory used by the pointers */
	free(begin_index);
	free(find);
	free(replace);
	return num_replace;	
} //replace_string()

void printer(char **ccs, char *result, int begin, int end, int operation){
	/* print 'ccs' */
	printf("\nCCS: \"%s\"\n", *ccs);

	/* If operation '2' is selected, print sub string with indexes */ 
	if (operation == 2)
		printf("Substring(%d,%d): \"%s\"\n", begin, end, result);
		
	/* If operation '3' is selected, print removed string  with indexes */ 
	else if (operation == 3)
		printf("Removed String(%d,%d): \"%s\"\n", begin, end, result);
	
} //printer()
