#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define Max 1000

int K, N, L ;

struct status
{
    int A[Max][2];          // Array to store the coordinates (x, y) of each block (1, ..., N)
    double running_cost;    // The cost g(n) [theory] = cost of reaching this status from the initial status
    struct status *extension;
    struct status *childs;
    struct status *path;
    struct status *closed_set;
};

struct status *head = NULL;
struct status *tail = NULL;
long int total_neighbor_states = 1;   // Number of generated neighbor states
long int total_extension = 0;         // Number of extensions performed (number of states pruned from the MA)
struct status *closed_h = NULL;
struct status *closed_t = NULL;       // Start and end of the closed set (list of explored states)


int valid_one(const int B[Max][2]) {
    for (int i = 0; i < N; i++) {
        if (B[i][1] >= 1) { // Check if the block is not in the first row
            int found = 0; // Flag to track if a valid block below is found

            // Iterate through the blocks to find a valid block below the current block
            for (int j = 0; j < N; j++) {
                if (j != i && B[j][0] == B[i][0] && B[j][1] == B[i][1] - 1) {
                    found = 1; // A valid block below is found
                    break;
                }
            }

            if (!found)
                return 0; // If no valid block below is found, the configuration is invalid
        }
    }

    return 1; // All blocks have a valid block below them, so the configuration is valid
}


int empty_one(const int A[Max][2], int x, int y) {
    for (int i = 0; i < N; i++) {
        if (A[i][0] == x && A[i][1] == y)
            return 0; // A block exists at the specified position, so it is not empty
    }
    return 1; // No block exists at the specified position, so it is empty
}

int free_one(int A[Max][2], int nx, int ny) {
    int isTopRow = (ny == 2); // Check if it is in the top row

    if (isTopRow) {
        return 1; // It is free
    }

    int isAboveEmpty = empty_one(A, nx, ny + 1); // Check if the position above is empty

    if (isAboveEmpty) {
        return 1; // It is free
    }

    return 0; // It is not free
}


void initialization(void) {
    struct status *state = NULL; // Pointer to the newly created status
    int i, x, y;
    int validStatus = 0; // Flag to track the validity of the initial configuration

    do {
        state = (struct status *)malloc(sizeof(struct status)); // Create a new status (struct status)
        if (state == NULL) {
            printf("No Memory.\n");
            exit(-1);
        }
        state->running_cost = 0.0; // Set the running cost of the status to 0
        state->extension = NULL; // Initialize the extension pointer to NULL
        state->childs = NULL; // Initialize the childs pointer to NULL
        state->closed_set = NULL; // Initialize the closed_set pointer to NULL
        state->path = NULL; // Initialize the path pointer to NULL

        for (i = 0; i < N; i++) {
            printf("Give me the coordinates (x,y) of %d cube: \n", i + 1);
            scanf("%d %d", &x, &y);
            state->A[i][0] = x; // Store the x-coordinate of the cube in the A array
            state->A[i][1] = y; // Store the y-coordinate of the cube in the A array
        }

        if (valid_one(state->A) == 1)
            validStatus = 1; // If the initial configuration is valid, set the validStatus flag to 1
        else {
            printf("Something is wrong!!!! \n");
            free(state); // Free the memory allocated for the invalid status
        }
    } while (!validStatus); // Repeat the loop until a valid initial configuration is entered

    head = tail = state; // Set the head and tail pointers to the newly created status
}



int is_solution(void) {
    if (head == NULL) {
        printf("No good solution.\n");
        return 0; // No good solution found, return 0 to indicate failure
    }

    return 1; // Good solution found, return 1 to indicate success
}


struct status *find_the_lower(void) {
    if (head == NULL) {
        printf("You give an empty list.\n");
        return NULL; // Return NULL when the list is empty
    }

    struct status *previous_one = NULL; // Pointer to the previous status
    struct status *current_one; // Pointer to the current status
    struct status *best_one = head; // Pointer to the status with the lowest cost
    double lower_cost = best_one->running_cost; // Initialize the lower_cost with the cost of the first status

    current_one = head; // Set the current_one pointer to the head of the list
    while (current_one->childs != NULL) { // Traverse the list until the last status is reached
        if (current_one->childs->running_cost < lower_cost) { // If the cost of the next status is lower than the current lowest cost
            lower_cost = current_one->childs->running_cost; // Update the lower_cost with the cost of the next status
            best_one = current_one->childs; // Update the best_one pointer with the next status
            previous_one = current_one; // Update the previous_one pointer with the current status
        }
        current_one = current_one->childs; // Move to the next status in the list
    }

    // Remove the best_one status from the list
    if (previous_one != NULL) { // If the best_one status is not the head of the list
        previous_one->childs = best_one->childs; // Update the next pointer of the previous status
        if (best_one->childs == NULL)
            tail = previous_one; // Update the tail pointer if the best_one status is the last status
        best_one->childs = NULL; // Set the next pointer of the best_one status to NULL
    } else { // If the best_one status is the head of the list
        head = best_one->childs; // Update the head pointer to the next status
        if (best_one->childs == NULL)
            tail = NULL; // Update the tail pointer if the best_one status is the only status
        best_one->childs = NULL; // Set the next pointer of the best_one status to NULL
    }

    return best_one; // Return the status with the lowest cost
}


int is_final_state(struct status *state) {
    // Check the coordinates of the first set of cubes
    for (int i = 0; i < K; ++i) {
        if (state->A[i][0] != i || state->A[i][1] != 0) {
            return 0; // Not the final state, return 0
        }
    }

    // Check the coordinates of the second set of cubes
    for (int i = K; i < 2 * K; ++i) {
        if (state->A[i][0] != (i - K) || state->A[i][1] != 1) {
            return 0; // Not the final state, return 0
        }
    }

    // Check the coordinates of the third set of cubes
    for (int i = 2 * K; i < 3 * K; ++i) {
        if (state->A[i][0] != (i - 2 * K) || state->A[i][1] != 2) {
            return 0; // Not the final state, return 0
        }
    }

    return 1; // It is the final state, return 1
}

void best_sequence(struct status *st) {
    struct status *state = st;
    struct status *st1 = st;
    int i = 1;
    int j;

    // Traverse the path from the last state to the initial state
    while (state->extension != NULL) {
        state->extension->path = state;
        state = state->extension;
    }

    // Print the sequence of states from the initial state to the final state
    while (state != st1) {
        printf("The %d state with cost: %f is: ", i, state->running_cost);
        
        // Print the coordinates of each cube in the state
        for (j = 0; j < N; ++j) {
            printf("%d:(%d,%d) ", j, state->A[j][0], state->A[j][1]);
        }
        printf("\n");
        
        state = state->path;
        ++i;
    }

    // Print the final state
    printf("The %d final state with cost: %f is: ", i, state->running_cost);
    
    // Print the coordinates of each cube in the final state
    for (j = 0; j < N; ++j) {
        printf("%d:(%d,%d) ", j, state->A[j][0], state->A[j][1]);
    }
    printf("\n");
}


int searching_algorithm(int B[Max][2]) {
    int my_count = 0;
    struct status *st = closed_h;

    // Traverse the closed set of states
    while (st != NULL) {
        int help = 1;
        int i = 0;

        // Compare the cube coordinates of state B with each state in the closed set
        while (i < N && (B[i][0] == st->A[i][0] && B[i][1] == st->A[i][1])) {
            i++;
        }

        // If all cube coordinates match, set my_count to 1 (indicating a match) and break the loop
        if (i == N) {
            my_count = 1;
            break;
        }

        st = st->closed_set; // Move to the next state in the closed set
    }

    return my_count; // Return my_count (1 if a match is found, 0 otherwise)
}


void extend(struct status *st) {
    struct status *state;
    int i, a, desiredcoord, y;
    int B[Max][2];

    // Iterate through each cube in state st
    for (i = 0; i < N; i++) {
        if (free_one(st->A, st->A[i][0], st->A[i][1]) == 1) {

            // Check available empty positions in the L coordinate
            for (a = 0; a < L; a++) {
                if (empty_one(st->A, a, 0) == 1) {

                    // Create a copy of the current state st into B
                    memcpy(B, st->A, sizeof(int) * N * 2);
                    B[i][0] = a;
                    B[i][1] = 0;

                    // Check if the updated state B is valid
                    if (valid_one(B) == 1) {
                        state = (struct status *)malloc(sizeof(struct status));
                        if (state == NULL) {
                            printf("There's no memory.\n");
                            exit(-1);
                        }
                        total_neighbor_states++;
                        memcpy(state->A, st->A, sizeof(int) * N * 2);
                        state->A[i][0] = a;
                        state->A[i][1] = 0;

                        desiredcoord = 0;
                        y = st->A[i][1];

                        // Calculate the running cost for the new state
                        if (desiredcoord > y)
                            state->running_cost = st->running_cost + desiredcoord - y;
                        else if (desiredcoord < y)
                            state->running_cost = st->running_cost + 0.5 * (y - desiredcoord);
                        else
                            state->running_cost = st->running_cost + 0.75;

                        state->extension = st;
                        state->childs = NULL;
                        state->closed_set = NULL;

                        // Add the new state to the list of states
                        if (head == NULL) {
                            head = state;
                            tail = state;
                        } else {
                            tail->childs = state;
                            tail = state;
                        }

                        // Break the loop if the desired coordinate exceeds K
                        if (a >= K)
                            break;
                    }
                }
            }

            // Check available empty positions in the K coordinate
            for (a = 0; a < K; a++) {
                if (empty_one(st->A, a, 1) == 1) {

                    // Create a copy of the current state st into B
                    memcpy(B, st->A, sizeof(int) * N * 2);
                    B[i][0] = a;
                    B[i][1] = 1;

                    // Check if the updated state B is valid
                    if (valid_one(B) == 1) {
                        state = (struct status *)malloc(sizeof(struct status));
                        if (state == NULL) {
                            printf("There's no memory.\n");
                            exit(-1);
                        }
                        total_neighbor_states++;
                        memcpy(state->A, st->A, sizeof(int) * N * 2);
                        state->A[i][0] = a;
                        state->A[i][1] = 1;

                        desiredcoord = 1;
                        y = st->A[i][1];

                        // Calculate the running cost for the new state
                        if (desiredcoord > y)
                            state->running_cost = st->running_cost + desiredcoord - y;
                        else if (desiredcoord < y)
                            state->running_cost = st->running_cost + 0.5 * (y - desiredcoord);
                        else
                            state->running_cost = st->running_cost + 0.75;

                        state->extension = st;
                        state->childs = NULL;
                        state->closed_set = NULL;

                        // Add the new state to the list of states
                        if (head == NULL) {
                            head = state;
                            tail = state;
                        } else {
                            tail->childs = state;
                            tail = state;
                        }
                    }
                }
            }

            // Check available empty positions in the 2K coordinate
            for (a = 0; a < K; a++) {
                if (empty_one(st->A, a, 2) == 1) {

                    // Create a copy of the current state st into B
                    memcpy(B, st->A, sizeof(int) * N * 2);
                    B[i][0] = a;
                    B[i][1] = 2;

                    // Check if the updated state B is valid
                    if (valid_one(B) == 1) {
                        state = (struct status *)malloc(sizeof(struct status));
                        if (state == NULL) {
                            printf("There's no memory.\n");
                            exit(-1);
                        }
                        total_neighbor_states++;
                        memcpy(state->A, st->A, sizeof(int) * N * 2);
                        state->A[i][0] = a;
                        state->A[i][1] = 2;

                        desiredcoord = 2;
                        y = st->A[i][1];

                        // Calculate the running cost for the new state
                        if (desiredcoord > y)
                            state->running_cost = st->running_cost + desiredcoord - y;
                        else if (desiredcoord < y)
                            state->running_cost = st->running_cost + 0.5 * (y - desiredcoord);
                        else
                            state->running_cost = st->running_cost + 0.75;

                        state->extension = st;
                        state->childs = NULL;
                        state->closed_set = NULL;

                        // Add the new state to the list of states
                        if (head == NULL) {
                            head = state;
                            tail = state;
                        } else {
                            tail->childs = state;
                            tail = state;
                        }
                    }
                }
            }
        }
    }

    // Add the current state st to the closed set of states
    if (closed_h == NULL) {
        closed_h = st;
        closed_t = st;
    } else {
        closed_t->closed_set = st;
        closed_t = st;
    }
}

 
void ucs_algorithm(void)
{
    struct status *currentState;
    int isFound = 0, userInput, i, isSafe, isFinalState;

    initialization(); // Algorithm Step 1: Initialize the initial state

    // Display the AK
    for (i = 0; i < N; i++) {
        printf("%d: (%d, %d) ", i, head->A[i][0], head->A[i][1]);
    }
    printf("\n");

    printf("Enter a value: ");
    scanf("%d", &userInput);

    while (!isFound) {
        is_solution(); // Check if the current state is a solution
        currentState = find_the_lower(); // Find the state with the lowest cost

        isSafe = searching_algorithm(currentState->A); // Check if the current state is safe
        if (!isSafe) {
            isFinalState = is_final_state(currentState); // Check if the current state is the final goal state
            if (isFinalState) {
                isFound = 1; // Goal state is found
                best_sequence(currentState); // Generate and display the best sequence of states
            } else {
                total_extension++;
                if (total_extension % 3000 == 0) {
                    printf("Number of parents: %ld, Current cost: %f\n", total_extension, currentState->running_cost);
                }
                extend(currentState); // Generate and add new states to the search space
            }
        }
    }
}


int main()
{
    printf("Enter K: ");
    scanf("%d", &K);
    N = 3 * K;
    L = 4 * K;

    ucs_algorithm(); // Run the UCS algorithm

    printf("The value of total_neighbor_states is %ld.\n", total_neighbor_states);
    printf("The value of total_extension is %ld.\n", total_extension);

    return 0;
}

