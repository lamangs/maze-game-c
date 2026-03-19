#include <stdio.h>
#include <string.h> // does the string handling (strcmp, strcspn)
#include <stdlib.h>  // imported for rand, srand
#include <time.h>    // time used for the random generator 
#include <ctype.h> // used for (toupper, tolower)
#define ROWS 16 //number of rows in the grid
#define COLUMNS 16 //number of columns in the grid


//measures the grid distance when u can move up, down, left, right (the number of moves basically) **NO diagonals allowed
int manhattan(int r1, int c1, int r2, int c2) { 
    return abs(r1 - r2) + abs(c1 - c2);
}



// a function for all the walls in the grid
void setwalls(char grid[ROWS][COLUMNS]){

    grid[0][8] = 'W';
    grid[1][8] = 'W';    // the vertical wall at the top

    grid[2][2] = 'W';
    grid[3][2] = 'W';    // the vertical wall from the left at the top

    grid[2][13] = 'W';
    grid[3][13] = 'W';   // the vertical wall from the right at the top

    grid[5][6] = 'W';
    grid[6][6] = 'W'; 
    grid[5][7] = 'W';
    grid[5][8] = 'W';  //the horizantal tilted L shaped wall in the middle

    grid[9][3] = 'W';
    grid[9][4] = 'W'; 
    grid[9][5] = 'W';
    grid[10][5] = 'W';  //the horizantal tilted L shaped wall from the left at the bottom

    grid[8][10] = 'W';
    grid[9][10] = 'W'; 
    grid[10][10] = 'W';
    grid[11][10] = 'W'; 
    grid[11][11] = 'W';  // the vertical L shaped wall from the right

    grid[14][2] = 'W';
    grid[14][3] = 'W'; 
    grid[14][4] = 'W'; // the horizantal  wall right at the far bottom

}



// the function for the hard AI movement logic of the police
void move_police_hard(char grid[ROWS][COLUMNS], int *pr, int *pc, int tr, int tc) {
    int best_rows[4]; // stores the rows of best move
    int best_columns[4]; // stores the columns of the best move
    int best_valid = 0; // stores the number of the best valid moves 
    int best_distance = 1000000; // the best smallest distance found (1000000 is a placeholder it can be anything because we compare the number of moves under it. not with it.)


    int row_change[4] = {-1, 1, 0, 0}; // the change of the row movements in the 4 directions (up, down, left, right)
    int column_change[4] = {0, 0, -1, 1}; // the change of the column movements in the 4 directions (up, down, left, right)

    for (int i = 0; i < 4; i++) { //loops through the 4 directions

        //new movements of the police if it were to move in direction i
        int new_r = *pr + row_change[i]; // for the rows
        int new_c = *pc + column_change[i]; // for the columns


        // if it moves out of the bounds, skip it (rows and columns CANNOT be less than 0 and more than 16)
        if (new_r < 0 || new_r >= ROWS || new_c < 0 || new_c >= COLUMNS) continue; 

        // if it goes through a wall, skip
        if (grid[new_r][new_c] == 'W') continue;


        //measures how many moves it would take to reach the thief if no walls with the manhattan equation
        int d = manhattan(new_r, new_c, tr, tc); 

        if (d < best_distance) { // if the distance is less than any other previous move
            best_distance = d; // d is stored as the best distance 
            best_valid = 0; // forgets any best move we previously stored

            //updates the best row and column moves
            best_rows[best_valid] = new_r; 
            best_columns[best_valid] = new_c;
            best_valid++; //updates the best valid moves
        } else if (d == best_distance) { // if the number of moves left is the same as the stored best distance
            best_rows[best_valid] = new_r;
            best_columns[best_valid] = new_c;
            best_valid++;
        }
    }

    if (best_valid == 0) return; // if the police is stuck and there are no move, it stays stuck

    int choice = rand() % best_valid; // if there are multiple best moves, pick one randomly
    int final_r = best_rows[choice]; // finalized best row move updated
    int final_c = best_columns[choice]; // finalized best column move updated

    // clear the previous police cell (and restore E if needed)
    if (*pr == 7 && *pc == 15) {
        grid[*pr][*pc] = 'E'; }
    else {
       grid[*pr][*pc] = '-';
    }

    // updates the pr and pc, puts P on the grid at a new location
    *pr = final_r;
    *pc = final_c;
    grid[*pr][*pc] = 'P';
}





// the function for the easy AI logic movement of the police
void move_police_easy(char grid[ROWS][COLUMNS], int *pr, int *pc) {
    // since the police can move in 4 directions it can have 4 possible rows/columns
    int rowmoves[4]; 
    int colmoves[4];
    int valid_moves = 0; // this will indicate how many valid moves have been found

    int row_change[4] = {-1, 1, 0, 0}; // the change of the row in 4 different directions (up, down, left, right)
    int column_change[4] = {0, 0, -1, 1}; // the change of the column in 4 different directions (up, down, left, right)

    for (int i = 0; i < 4; i++) { //loops through every direction

        //new movements of the police if it were to move in direction i
        int new_r = *pr + row_change[i]; // for the rows
        int new_c = *pc + column_change[i]; // for the columns

        if (new_r < 0 || new_r >= ROWS || new_c < 0 || new_c >= COLUMNS) continue; // skip the move if it moves out of the bounds
        if (grid[new_r][new_c] == 'W') continue; // prevents the police from going through the walls

        //saves the valid moves
        rowmoves[valid_moves] = new_r;
        colmoves[valid_moves] = new_c;
        valid_moves++;
    }

    if (valid_moves == 0) return; // if no possible moves police stays still

    int choice = rand() % valid_moves; //picks one of the saved valid moves randomly

    // the square that the police will move to
    int finalrow = rowmoves[choice];
    int finalcol = colmoves[choice];

    // clears old police cell 
    if (*pr == 7 && *pc == 15) {
        grid[*pr][*pc] = 'E'; // restore the E(escape) cell if the police is moving off it
    } 
    else {
        grid[*pr][*pc] = '-';  // otherwise clear the old police position to an empty cell
    }
        

    // updates the pr and pc, puts P on the grid at a new location
    *pr = finalrow;
    *pc = finalcol;
    grid[*pr][*pc] = 'P';
}




// the function for the thief movement by the user
void moving_thief(char grid[ROWS][COLUMNS], int *tr, int *tc){

    char move; //initializes the variable for the movement keys (W/A/S/D)
    int current_r = *tr; // current position of the row after user input
    int current_c = *tc; // current position of the column after user input

    printf("Move (W/A/S/D):   "); // asks the user to input a key
    scanf(" %c", &move); //stores the users input as the move variable

    move = toupper(move); // converts the input into uppercase if it is not (this is what <ctype.h> was imported for)


    switch (move) {
        case 'W': //if the user input w (up)
        current_r--; // row - 1 (indicates going up)
        break;

        case 'A': // if the user input a (left)
        current_c--; // column - 1 (indicates going left)
        break;

        case 'S': // if the user input s(down)
        current_r++; // row + 1 (indicates going down)
        break;

        case 'D': // if the user input d (right)
        current_c++; // column + 1 (indicates going right)
        break;

        default: // an else case
        printf("Invalid key."); // any other input would be invalid
        return;

    }


    // if the move is attempted to go out of the bounds, skip
    if (current_r < 0 || current_r >= ROWS || current_c < 0 || current_c >= COLUMNS)
      return;

 
      // if the move is attempted to go across a wall, skip
    if (grid[current_r][current_c] == 'W')
      return;

  
      // if the thief makes it to the escape cell keep the 'E' as it is
     if (*tr == 7 && *tc == 15) {
        grid[*tr][*tc] = 'E';
     }
     // clear the thief from the previous position
     else {
        grid[*tr][*tc] = '-';
     }

     // updates the tr and tc and places the T on the grid
     *tr = current_r;
     *tc = current_c;
     grid[*tr][*tc] = 'T';

}





// the function for spawning the thief on the grid
void spawn_thief(char grid[ROWS][COLUMNS], int *tr, int *tc){

    while (1){ // loops forever

    *tr = rand() % ROWS; // generates a random row position for the thief
    *tc = rand() % COLUMNS; // generates a random column position for the thief

    if (grid[*tr][*tc] != '-') {
        continue; //if the spot is not empty then skip
     } 
    if (manhattan(*tr, *tc, 7, 15) < 16) {
        continue; // if the distance between the thief and the escape point is less than 16 moves, skip (too close)
     } 

    grid[*tr][*tc] = 'T'; //finalized spot for the thief

    break; // the loop breaks
    }

}



// the function for spawning the police on the grid
void spawn_police(char grid[ROWS][COLUMNS], int *pr, int *pc, int tr, int tc){

    while (1){ // loops forever

        *pr = rand() % ROWS; // generates a random row position for the police
        *pc = rand() % COLUMNS; // generates a random column position for the police

        if (grid[*pr][*pc] != '-') {
            continue; // skips the spots that are not empty
         } 
        if (manhattan(*pr,*pc, tr, tc) < 16) {
            continue;  // skip if closer than 16 moves (needs to be 16 moves more far from the thief)
        }

        grid[*pr][*pc] = 'P'; // places the P on the grid
        break;

    }
}


// the function for the easymode

// the parameters are pointers as the arguments were memory addresses (pass by reference)
void easymode(char grid[ROWS][COLUMNS], int *tr, int *tc, int *pr, int *pc) {

    while (1) {
    // prints grid
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }

    moving_thief(grid, tr, tc); // thief movement is called (user input)

    if (*tr == 7 && *tc == 15) { // if the thief makes it to the E(escape) cell
        printf("You won!\n"); // print "you won"
        break;
    }

    move_police_easy(grid, pr, pc); // easy police movement is called

    //print you lost if the police catches the thief
    if (*pr == *tr && *pc == *tc) { 
        printf("You lost!\n");
        break;
    }
}

}



// the function for the hardmode

// the parameters are pointers as the arguments were memory addresses
void hardmode(char grid[ROWS][COLUMNS], int *tr, int *tc, int *pr, int *pc) {

   while (1) { // loops forever
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {   // prints the grid
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }

    moving_thief(grid, tr, tc); // calls the user input of the thief movement

    if (*tr == 7 && *tc == 15) {  // if the thief makes it to the escape cell print you won
        printf("You won!\n");
        break;
    }

    move_police_hard(grid, pr, pc, *tr, *tc); //calls the hard ai police logic

    if (*pr == *tr && *pc == *tc) { //if the police and the thief are on the same cell , print you lost
        printf("You lost!\n");
        break;
    }
}

}




int main(void) {

    char play_again; //initializes the char variable where the users input will be stored (y/n)
    srand((unsigned)time(NULL)); //imported for rand()


    do {
        char answer[10]; //initalizes the string variable where the users input will be stored (easy, hard)
        char grid[ROWS][COLUMNS]; // initializes the grid
        int tr;
        int tc;
        int pr;
        int pc;

        // reset grid
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                grid[i][j] = '-';
            }
        }

        grid[7][15] = 'E';
        setwalls(grid);

        // ask difficulty
        printf("\t *** POLICE CHASE GAME *** \t\n");
        printf("Choose a difficulty (Easy / Hard): ");
        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer, "\n")] = '\0'; // removes the leftover line fgets leaves

        // spawn characters
        spawn_thief(grid, &tr, &tc);
        spawn_police(grid, &pr, &pc, tr, tc);

        if (strcmp(answer, "easy") == 0 || strcmp(answer, "Easy") == 0) { // if the user chose easy or Easy
            easymode(grid, &tr, &tc, &pr, &pc);
        } else if (strcmp(answer, "hard") == 0 || strcmp(answer, "Hard") == 0) { // if the user chose hard or Hard
            hardmode(grid, &tr, &tc, &pr, &pc);
        } else {
            printf("Invalid choice.\n");
        }

        // ask to play again
        printf("Do you want to play again? (y/n): ");
        scanf(" %c", &play_again); // stores the users input inside the variable
        play_again = tolower(play_again); // converts the input into lowercase if its not

        // clears the leftover newline
        getchar();

    } while (play_again == 'y'); // run in a loop if the user input y

    printf("\n\n \t\t Thanks for playing! \n \t\t Have a nice day ! :) \n"); // print this if the user input n
    return 0;
}

