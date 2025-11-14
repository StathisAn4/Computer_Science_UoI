//ANDREOPOULOS STATHIS AM: 4630
//KONTAKIS SPYROS AM: 4702

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M_Max 1000   // Maximum value for M
#define MAX 1000    // Maximum value for MAX
#define MIN -1000   // Minimum value for MIN

int *A;             // Array to store the number of cards each team has
int *B;             // Array to store the number of cards that can be drawn from each team
int M, K;           // Variables to store the total number of cards and the number of teams

struct min_max
{
    struct min_max *kids[M_Max];    	// Array of pointers to child nodes in the min-max tree
    struct min_max *best_kid;           // Pointer to the next best move in the tree
    int *N;                             // Array to store the number of cards each team has in the current state
};

void initialization(void)
{
    int i, j;
    int sum = 0;

    // Get the number of cards (M)
    do {
        printf("Give me the number of cards M: ");
        scanf("%d", &M);
        if (M <= 0) {
            printf("You need to give a positive number for cards (M).\n");
            printf("Try again!!!).\n");
			printf("\n");
        }
    } while (M <= 0);
	printf("\n");
	
    // Get the number of teams (K)
    do{
		do {
			printf("Give me the number of teams K: ");
			scanf("%d", &K);
			if (K <= 0) {
				printf("You need to give a positive number for the teams (K).\n");
				printf("Try again!!!).\n");
			}
		} while (K <= 0);							
		if (M <= K) {
            printf("The number of teams(K) is higher than the number of cards.\n");
            printf("Try again!!!).\n");
			printf("\n");
		}
	}while(M <= K);
	printf("\n");
	
    // Allocate memory for the array A
    do {
        A = (int *)malloc(K * sizeof(struct min_max));
        for (i = 0; i < K; i++)
        {
            // Get the number of cards each team will have (A[i])
            do {
				j=i+1;
                printf("Give the number of cards each team will have A[%d]: ", j);
                scanf("%d", &A[i]);
                sum += A[i];
                if (A[i] <= 0) {
                    printf("You need to give a positive number for the possible number of cards a team can have (A[%d]).\n",j);
                    printf("Try again!!!).\n");
					printf("\n");
                }
            } while (A[i] <= 0);
        }
        if (sum != M) {
            printf("The total number of cards (M) is different from the sum of the cards in the teams.\n");
            printf("Try again!!!).\n");
			printf("\n");
			sum = 0;
        }
    } while (sum != M);
	printf("\n");

	// Allocate memory for the array B
	do {
		B = (int *)malloc(K * sizeof(int));
		for (i = 0; i < K; i++) {
			// Get the number of cards you can draw from each team (B[i])
			do {
				j=i+1;
				printf("Give the number of cards you can draw from each team B[%d]: ", j);
				scanf("%d", &B[i]);
				if (B[i] <= 0) {
					printf("You need to give a positive number for the possible number of cards you can draw from a team (B[%d]).\n",j);
					printf("Try again!!!).\n");
					printf("\n");
					j=j-1;
				} else if (B[i] > A[i]) {
					printf("The number of cards you can draw from a team (B[%d]) cannot be greater than the number of cards the team has (A[%d]).\n", j, j);
					printf("Try again!!!).\n");
					printf("\n");
				}
			} while (B[i] <= 0 || B[i] > A[i]);

		}
	} while (i < K);
	printf("\n");
}

int find_Winner(const struct min_max *state)
{
    // Check if all teams have zero cards
    int sum = 0;
    for (int i = 0; i < K; i++)
    {
        sum += state->N[i];  // Add the number of cards in each team to the sum
    }
    return (sum == 0) ? 1 : 0;  // If the sum is zero, all teams have zero cards and return 1 (true), otherwise return 0 (false)
}

void copy_table(const int X[K], int Y[K])
{
    // Copy the contents of array X to array Y using memcpy
    // The memcpy function copies the specified number of bytes from the source (X) to the destination (Y)
    // The number of bytes to copy is calculated by multiplying the size of each element (sizeof(int)) by the number of elements (K)
    memcpy(Y, X, K * sizeof(int));
}


int MINIMAX(struct min_max *tree, int turn)
{
    // Recursive function to compute the optimal move using the min-max algorithm

    // Determine the next player (opponent)
    int next_player = (turn == MAX) ? MIN : MAX;

    // Position counter for storing generated moves
    int level = 0;

    // Array to store evaluation values for each move
    int memory[K];

    // Array to store pointers to generated states
    struct min_max *n[K];

    // Best evaluation value and pointer to the best state
    int best_node;
    struct min_max *best_state;

    // Maximum number of cards that can be drawn from a team
    int seen;

    // Base case: check if the current state is a winning state
    if (find_Winner(tree))
        return (turn == MAX) ? -1 : 1;  // Return -1 for MAX player win, 1 for MIN player win

    // Generate all possible moves and evaluate them
    for (int i = 0; i < K; i++)
    {
        if (tree->N[i] > 0)
        {
            // Determine the maximum number of cards that can be drawn from the team
            seen = (B[i] < tree->N[i]) ? B[i] : tree->N[i];

            // Generate all possible moves for the current team
            for (int j = 1; j <= seen; j++)
            {
                // Allocate memory for the next state and its array
                tree->kids[level] = malloc(sizeof(struct min_max));
                tree->kids[level]->N = malloc(K * sizeof(int));

                // Create a copy of the current state and update it with the move
                copy_table(tree->N, tree->kids[level]->N);
                int decrement = j;
                tree->kids[level]->N[i] -= decrement;

                // Recursively evaluate the subtree
                memory[level] = MINIMAX(tree->kids[level], next_player);
                n[level] = tree->kids[level];
                level++;
            }
        }
    }

    // Determine the best move based on the evaluation
    best_node = memory[0];
    best_state = n[0];

    for (int i = 1; i < level; i++)
    {
        if ((turn == MAX && memory[i] > best_node) || (turn == MIN && memory[i] < best_node))
        {
            best_node = memory[i];
            best_state = n[i];
        }
    }

    // Update the tree with the best move and return the best move
    tree->best_kid = best_state;
    return best_node;
}


int main()
{
    // Initialize the game state
    struct min_max *state = malloc(sizeof(struct min_max));
    int turn = MAX, team, number, i, j;
	int temp;
    initialization();

    state->N = malloc(K * sizeof(int));
    copy_table(A, state->N);

    printf("---Game Starts---\n");
    for (i = 0; i < K; i++)
    {	
		j=i+1;
        printf("team %d : %d cards\n", j, state->N[i]);
    }
    printf("\n");

    // Game loop
    while (1)
    {
        // Check if the current player has won
        if (turn == MAX && find_Winner(state) == 1)
        {
            printf("---The Player(MIN) wins---\n");
            break;
        }
        else if (turn == MIN && find_Winner(state) == 1)
        {
            printf("---The AI(MAX) wins---\n");
            break;
        }
        else if (turn == MAX)
        {
            // AI's turn
            MINIMAX(state, MAX);
            state = state->best_kid;
            printf("AI's turn\n");
            for (i = 0; i < K; i++)
            {
				j=i+1;
                printf("team %d : %d cards\n", j, state->N[i]);
            }
            printf("\n");
            turn = MIN;
        }
        else
        {
            // Player's turn
            printf("Player's turn\n");
            printf("Time to choose\n");

            // Print the teams that still have cards
            for (i = 0; i < K; i++)
            {
				j=i+1;
                if (state->N[i] > 0)
                {
                    printf("Press %d to choose team %d\n", j, j);
                }
            }
            scanf("%d", &team); // Get the chosen team
			temp = team-1;
            printf("Choose the number of cards you will draw\n");
            int maxNumber = (B[temp] < state->N[temp]) ? B[temp] : state->N[temp];
            printf("Choose from 1 to %d\n", maxNumber);
            scanf("%d", &number); // Get the number of cards to draw

            state->N[temp] -= number; // Update the number of cards in the chosen team
            printf("Player's turn\n");
            for (i = 0; i < K; i++)
            {
				j=i+1;
                printf("team %d : %d cards\n", j, state->N[i]); // Print the updated number of cards in each team
            }
            printf("\n");
            turn = MAX; // Switch to AI's turn
        }
    }
}
