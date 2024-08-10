#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];

/* the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/*the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/*the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* function return 0 if successful and -1 if not */
// int request_resources(int customer_num, int request[]);
int request_resources(int customer_num, int request[NUMBER_OF_RESOURCES], int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES], int available[NUMBER_OF_RESOURCES], int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]);
int safestate(int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES], int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES], int available[NUMBER_OF_RESOURCES]) ;
void display(int available[NUMBER_OF_RESOURCES], int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES], int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES], int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]);
void release_resources(int id, int release[], int available[NUMBER_OF_RESOURCES], int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES], int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]);


// Function to request resources
int request_resources(int customer_num, int request[NUMBER_OF_RESOURCES], int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES], int available[NUMBER_OF_RESOURCES], int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]) {
    for (int i=0; i<NUMBER_OF_RESOURCES; i++) {
        if (request[i] > need[customer_num-1][i]) {
            // Request exceeds need. Return 0
            printf("Invalid request");
            return 0;
        }
        if (request[i] > available[i]) {
            // Customer must wait, resources not available
            printf("\nSorry, your request has been denied");
            return 0;
        }
    }

    // pretending to allocate resources to customer
    // we will make copies
    int newavailable[NUMBER_OF_RESOURCES];
    int newalloc[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
    int newneed[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

    // copying data at address of old data to new data
    // Hypothetical data structures
    memcpy(newavailable, available, NUMBER_OF_RESOURCES * sizeof(int));
    memcpy(newalloc, allocation, NUMBER_OF_CUSTOMERS * NUMBER_OF_RESOURCES * sizeof(int));
    memcpy(newneed, need, NUMBER_OF_CUSTOMERS * NUMBER_OF_RESOURCES * sizeof(int));

    // adjusting data as algorithm describes
    // applying hypothetical values
    for (int i=0; i<NUMBER_OF_RESOURCES; i++) {
        newavailable[i] -= request[i];
        newalloc[customer_num-1][i] += request[i];
        newneed[customer_num-1][i] -= request[i];
    }

    // Determination is state leads to deadlock or not
    int result = safestate(newneed, newalloc, newavailable);

    // If safe state, assigning hypotheticals to actual values
    if (result==1) {
        memcpy(available, newavailable,  NUMBER_OF_RESOURCES * sizeof(int));
        memcpy(allocation, newalloc, NUMBER_OF_CUSTOMERS * NUMBER_OF_RESOURCES * sizeof(int));
        memcpy(need, newneed, NUMBER_OF_CUSTOMERS * NUMBER_OF_RESOURCES * sizeof(int));
        return 1;
    }
    else {
        // If state isn't safe, informing the user of such
        printf("\nSorry, your request has been denied");
        return 0;
    }
}


// Checks if state is safe, 
int safestate(int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES], int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES], int available[NUMBER_OF_RESOURCES]) {
    // Initializing workable resources and array of whether or not customers finished with needs
    int work[NUMBER_OF_RESOURCES];
    bool finish[NUMBER_OF_CUSTOMERS] = {false};
    // Initializing work to be equal to available
    for (int i=0; i<NUMBER_OF_RESOURCES; i++) {
        work[i] = available[i];
    }
    // Nested loop determining if need is valid and customer isn't finished
    // If so, allocating their resources back to working resources and setting their completion boolean to false
    for (int i=0; i<NUMBER_OF_CUSTOMERS; i++) {
        for (int j=0; j<NUMBER_OF_RESOURCES; j++) {
            if (need[i][j]<=work[j] && finish[i]==false) {
                // allocating resources to work and setting customer's boolean to true
                work[j] += allocation[i][j];
                finish[i] = true;
            }
            else {
                continue;
            }
        }
    }
    //  If all the customers have finished processes, state is safe
    // Otherwise, state is not safe. Deadlock is possible
    for (int i=0; i<NUMBER_OF_CUSTOMERS; i++) {
        if (finish[i] == false) {
            return 0;
        }
    }
    return 1;
}

// Display function filled with nested loops to display data
void display(int available[NUMBER_OF_RESOURCES], int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES], int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES], int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]) {
    printf("Available Resources: ");
    // Displaying available resources
    for (int i=0; i<NUMBER_OF_RESOURCES; i++) {
        printf("%d ", available[i]);
    }
    // Displaying need for each customer
    printf("\n\n");
    printf("Maximum Need per Customer: ");
    for (int i=0; i<NUMBER_OF_CUSTOMERS; i++) {
        printf("\n");
        printf("Customer %d: \n", i+1);
        for (int j=0; j<NUMBER_OF_RESOURCES; j++) {
            printf("Resource %d: %d \n", j+1, maximum[i][j]);
        }
    }
    // Displaying allocation resources
    printf("\n\n");
    printf("Allocated Resources per Customer: ");
    for (int i=0; i<NUMBER_OF_CUSTOMERS; i++) {
        printf("\n");
        printf("Customer %d: \n", i+1);
        for (int j=0; j<NUMBER_OF_RESOURCES; j++) {
            printf("Resource %d: %d \n", j+1, allocation[i][j]);
        }
    }
    // Displaying resources still needed by each customer
    printf("\n\n");
    printf("Resources Needed per Customer: ");
    for (int i=0; i<NUMBER_OF_CUSTOMERS; i++) {
        printf("\n");
        printf("Customer %d: \n", i+1);
        for (int j=0; j<NUMBER_OF_RESOURCES; j++) {
            printf("Resource %d: %d \n", j+1, need[i][j]);
        }
    }
    printf("\n");
}

// Function to release resources
// Very similar to request, but reversing the operations
void release_resources(int id, int release[], int available[NUMBER_OF_RESOURCES], int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES], int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]) {
    int flag;
    // Flag to determine that released resources doesn't exceed actual amount of allocated resources
    for (int i=0; i<NUMBER_OF_RESOURCES; i++) {
        if (release[i] > allocation[id-1][i]) {
            int flag = 0;
        }
        else {
            int flag = 1;
        }
    }

    // If the flag is 1, release is valid and making the hypothetical arrays
    if (flag == 1) {
        // pretending to allocate resources to customer
        // These are the hypothetical arrays
        int newavailable[NUMBER_OF_RESOURCES];
        int newalloc[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
        int newneed[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

        // copying data at address of old data to new data
        // Populating hypothetical arrays
        memcpy(newavailable, available, NUMBER_OF_RESOURCES * sizeof(int));
        memcpy(newalloc, allocation, NUMBER_OF_CUSTOMERS * NUMBER_OF_RESOURCES * sizeof(int));
        memcpy(newneed, need, NUMBER_OF_CUSTOMERS * NUMBER_OF_RESOURCES * sizeof(int));

        // Adjusting hypothetical arrays to accound for release
        // Available resources increases, allocated resources decreases for the customer who released
        for (int i=0; i<NUMBER_OF_RESOURCES; i++) {
            newavailable[i] += release[i];
            newalloc[id-1][i] -= release[i];
        }

        // int telling if state is safe or not
        int result = safestate(newneed, newalloc, newavailable);

        // if state is safe, assigning hypothetical values to actual array
        if (result==1) {
            memcpy(available, newavailable,  NUMBER_OF_RESOURCES * sizeof(int));
            memcpy(allocation, newalloc, NUMBER_OF_CUSTOMERS * NUMBER_OF_RESOURCES * sizeof(int));
            memcpy(need, newneed, NUMBER_OF_CUSTOMERS * NUMBER_OF_RESOURCES * sizeof(int));
            printf("Your release has been granted.");
        }
        else {
            // If state isn't safe, informing the user as such
            printf("\nSorry, your release has been denied");
        }
    }
    // If user release exceeds what they actually have, deny the release
    else {
        printf("Sorry, your release has been denied");
    }
}

int main()
{
    // Maximum resources required by each customer (row)
    // Arbitrary array of maximum needs to test code
    int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES] = {
        6,4,7,3,
        4,2,3,2,
        2,5,3,3,
        6,3,3,2,
        5,6,7,5
    };
    // Initializing allocation array to 0, as no customers have any resources
    int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES] = {0};

    // First initializing the need equal to maximum
    for (int i=0; i<NUMBER_OF_CUSTOMERS; i++) {
        for (int j=0; j<NUMBER_OF_RESOURCES; j++) {
            need[i][j] = maximum[i][j];
        }
    }

    // Prompt for user to input available resources
    printf("Banker, please enter the amount of resources available... \n");

    // Retrieves input for each resource and stores it in the address of the 'available' array
    for (int i=1; i<NUMBER_OF_RESOURCES+1; i++) {
        printf("Resource %d: ", i);
        scanf("%d", &available[i-1]);
    }
    
    // Declaring command arrays
    // Determine the customer id, command, requested or released resources
    char command[2];
    int id;
    int requested[NUMBER_OF_RESOURCES];
    int release[NUMBER_OF_RESOURCES];

    // Prompts the user for a command
    printf("Please enter a command: ");
    scanf("%s", command);

    // Retrieves user id
    printf("Please enter your customer ID: ");
    scanf("%d", &id);

    printf("Your command is: %s", command);
    printf("\nYour ID is: %d", id);

    // Request Command Prompt
    if (strcmp(command, "RQ") == 0) {
        printf("\nRequest Command Detected\n");
        printf("Please enter your requested resource amounts... \n");
        for (int i=0; i<NUMBER_OF_RESOURCES; i++) {
            printf("Resource %d: ", i+1);
            scanf("%d", &requested[i]);
        }
        printf("\nYour requested amount is: ");
        for (int i=0; i<NUMBER_OF_RESOURCES; i++) {
            printf("%d ", requested[i]);
        }
        int res = request_resources(id, requested, need, available, allocation);
        if (res==1) {
            printf("Request Granted!");
        }
        else if(res==0) {
            printf("Sorry, your request has been denied");
        }
    }

    // Release Command Prompt
    else if (strcmp(command, "RL") == 0)
    {
        printf("\nRelease Command Detected\n");
        printf("Please enter your release amounts... \n");
        for (int i=0; i<NUMBER_OF_RESOURCES; i++) {
            printf("Resource %d: ", i+1);
            scanf("%d", &release[i]);
        }
        release_resources(id, release, available, allocation, need);
    }
    
    // Display Command
    else if (strcmp(command, "**") == 0)
    {
        printf("\nDisplay Command Detected\n");
        display(available, maximum, allocation, need);
    }
    return 0;
}

