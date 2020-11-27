#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NUM_ROWS 8
#define NUM_COLS 8

#define SPACE 0
#define WALL - 1
#define EXIT - 2

// Initializes the road map, on an integer array
// Using pre-defined numbers to represent empty lots, walls, and the exit
void InitialiseRoad(int road[NUM_ROWS][NUM_COLS], char side, int pos) {

  // goes over each co-ordinate in the 2D array,
  // recognizes if the co-ordinate is an edge, and places a wall
  // or if it's not an edge, and places a space (empty lot).
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      if ((i == 0 || i == NUM_ROWS - 1) || (j == 0 || j == NUM_COLS - 1)) {
        road[i][j] = WALL;
      } else {
        road[i][j] = SPACE;
      }
    }
  }
  // places the exit, at whatever side was specified by the parameter side.
  // North for the top of the 2D array, East for the right hand side, and so on.
  // Uses pos for the coloumn number, if in the top/bottom row (N/S)
  // Or pos for the row number, if on the left/right coloumn (E/W)
  if (side == 'N') {
    road[0][pos] = EXIT;
  } else if (side == 'E') {
    road[pos][NUM_COLS - 1] = EXIT;
  } else if (side == 'S') {
    road[NUM_ROWS - 1][pos] = EXIT;
  } else {
    road[pos][0] = EXIT;
  }

}

void PrintRoad(int road[NUM_ROWS][NUM_COLS]) {
  // Prints out the road represented by the integer array
  // If the number representing, a space, wall, or exit is found at specific co-ordinate,
  // a space, #, or capital O will be printed out to represent that (respective to that order)
  // Anything else must be an ascii integer representing a character,
  // and the associated letter to that ascii is printed instead of the integer.
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      if (road[i][j] == SPACE) {
        printf(" ");
      } else if (road[i][j] == WALL) {
        printf("#");
      } else if (road[i][j] == EXIT) {
        printf("O");
      } else {
        printf("%c", road[i][j]);
      }
    }
    // New line for format
    printf("\n");
  }
}

double PercentUsed(int road[NUM_ROWS][NUM_COLS]) {
  // Sum representing the amount of unique co-ordinates with a car in that location
  // on the 2D integer array road
  // total representing the total amount of lots (discluding walls and the exit)
  double sum = 0;
  double total = (NUM_COLS - 2) * (NUM_ROWS - 2);
  for (int i = 1; i < NUM_ROWS - 1; i++) {
    for (int j = 1; j < NUM_COLS - 1; j++) {
      // Checks if the current co-ordinate is not a wall, space, or exit
      // if so, then add 1 to sum.
      if (road[i][j] != WALL && road[i][j] != SPACE && road[i][j] != EXIT) {
        sum++;
      }
    }
  }
  // Returns the decimal percentage value of lots taken by a car
  return (100 * sum / total);
}

void AddCar(int road[NUM_ROWS][NUM_COLS], int row, int col, int size) {
  // something checker variable used to check if there is a collision at the positions
  // that the car has been assigned
  // current integer variable used to keep track of the ASCII value of the letter we should put inside of the road array.
  char current = 65;
  int something = 0;
  // Finds the largest letter currently inside of road, and adds 1 to it's ASCII value, and stores it in current. To get the next letter of the alphabet
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      if (road[i][j] >= current) {
        current = road[i][j] + 1;
      }
    }
  }
  // If the size is a negative, car to be placed vertically, check vertical spots
  if (size < 0) {
    size = size * -1;
    // checks if something is in the way where car is to be stored
    // if so, set something to 1
    for (int i = row; i < size + row; i++) {
      if (road[i][col] != SPACE) {
        something = 1;
        size = size * -1;
        break;
      }
    }
    size = size * -1;
    // else horizontal positions get checked
  } else {
    // checks if something is in the way where care is to be stored
    // if so, set something to 1
    for (int i = col; i < size + col; i++) {
      if (road[row][i] != SPACE) {
        something = 1;
        break;
      }
    }
  }
  // if nothing was in the way, something will be 0
  if (something == 0) {
    // if size is negative, place car vertically, in positions specificed.
    if (size < 0) {
      size = size * -1;
      for (int i = row; i < size + row; i++) {
        (road[i][col] = current);
      }
      // otherwise place positions horizontally, in positions specified.
    } else {
      for (int i = col; i < size + col; i++) {
        (road[row][i] = current);
      }
    }
  }
}

void FindCar(int road[NUM_ROWS][NUM_COLS], char move, int * rowStart, int * colStart, int * rowEnd, int * colEnd) {
  // integer checker found, to remember if a car has been found or not
  int found = 0;
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      // go over each spot in array, and as soon as we find the first co-ordinate where the car appears
      // save those co-ordinates using pointer inputs, and say found == 1
      // break
      if (road[i][j] == move) {
        * rowStart = i;
        * colStart = j;
        found = 1;
        break;
      }
    }
    // fully break out of the loop
    if (found == 1) {
      break;
    }
  }
  // checks if car is vertical, and if so, look over each spot vertically until there is no more of the car "move", there
  if (road[ * rowStart + 1][ * colStart] == move) {
    int i = * rowStart + 1;
    * colEnd = * colStart;
    while (road[i][ * colStart] == move) {
      * rowEnd = i;
      i++;
    }
    // follows the same procedure but assuming the car is horizontal
  } else {
    int i = * colStart + 1;
    * rowEnd = * rowStart;
    while (road[ * rowStart][i] == move) {
      * colEnd = i;
      i++;
    }
  }
}

int MoveCar(int road[NUM_ROWS][NUM_COLS], int r0, int c0, int r1, int c1) {
  // space integer to remember how much space is either to the left/right/up/down of the car
  int Availabe = 0;
  int car = road[r0][c0];
  // if car is horizontal
  if (r0 == r1) {
    // remember car ascii value
    for (int c = c0 - 1; c > 0; c--) {
      // checks how much space is to the left of the car, add one for every slot that's free, until there are no more slots left.
      if (road[r0][c] != SPACE) {
        break;
      } else {
        Availabe++;
      }
    }
    // if there were spaces, then remove the car on the board from its original location
    // and move it as far left as it can go according to the car game rules
    if (Availabe > 0) {
      for (int c = c0; c <= c1; c++) {
        road[r0][c] = SPACE;
      }
      for (int c = c0 - Availabe; c <= c1 - Availabe; c++) {
        road[r0][c] = car;
      }
      c0 = c0 - Availabe;
      c1 = c1 - Availabe;
    } else {
      // same process but if there was no space to the left, it moves the car as far to the right as possible instead.
      for (int c = c1 + 1; c < NUM_COLS-1; c++) {
        if (road[r0][c] != SPACE) {
          break;
        } else {
          Availabe++;
        }
      }
      for (int c = c0; c <= c1; c++) {
        road[r0][c] = SPACE;
      }
      for (int c = c0 + Availabe; c <= c1 + Availabe; c++) {
        road[r0][c] = car;
      }
      c0 = c0 + Availabe;
      c1 = c1 + Availabe;
    }
    // Assuming the car is vertical, it does the same process, but it checks how much space is above, and moves it upwards, otherwise downwards, if there's no space.
    // In all cases, if there is nowhere for the car to go, it does not move.
  } else {
    for (int r = r0 - 1; r >= 1; r--) {
      if (road[r][c0] == SPACE) {
        Availabe++;
      } else {
        break;
      }
    }
    if (Availabe > 0) {
      for (int r = r0; r <= r1; r++) {
        road[r][c0] = SPACE;
      }
      for (int r = r0 - Availabe; r <= r1 - Availabe; r++) {
        road[r][c0] = car;
      }
      r0 = r0 - Availabe;
      r1 = r1 - Availabe;
    } else {
      Availabe = 0;
      for (int r = r1 + 1; r < NUM_ROWS - 1; r++) {
        if (road[r][c0] == SPACE) {
          Availabe++;
        } else {
          break;
        }
      }
      for (int r = r0; r <= r1; r++) {
        road[r][c0] = SPACE;
      }
      for (int r = r0 + Availabe; r <= r1 + Availabe; r++) {
        road[r][c0] = car;
      }
      r0 = r0 + Availabe;
      r1 = r1 + Availabe;
    }

  }
  // checks if position car is moved to is right next to an exit, and if it can actually go through it

  // a horizontal car can only go through an E/W exit
  // a vertical car can only go through a N/S exit
  if ((road[r0][c0 - 1] == EXIT || road[r0][c1 + 1] == EXIT) && (r0 == r1)) {
    return 1;
  } else if ((road[r0 - 1][c0] == EXIT || road[r1 + 1][c0] == EXIT) && c0 == c1) {
    return 1;
  } else {
    return 0;
  }
}

/***********************************************************/
/***********************************************************/
/********* DO NOT MODIFY ANY CODE BELOW THIS POINT *********/
/***********************************************************/
/***********************************************************/

/* Function to obtain capital letter as input */
char GetMove(void) {
  char move;
  printf("\nMove car: ");
  scanf("%c", & move);
  // Ignore non-capital letter inputs
  while ((move < 'A') || (move > 'Z')) {
    scanf("%c", & move);
  }
  return move;
}

/* The main Traffic Jam simulation */
int main(void)
{
	int gameOver = 0;
	int road[NUM_ROWS][NUM_COLS];
	int rowStart, colStart, rowEnd, colEnd;
	char input;

	/* Print banner */
	printf(" _____           __  __ _            _                                        \n");
	printf("|_   _| __ __ _ / _|/ _(_) ___      | | __ _ _ __ ___           ____          \n");
	printf("  | || '__/ _` | |_| |_| |/ __|  _  | |/ _` | '_ ` _ \\  --   __/  |_\\_      \n");
	printf("  | || | | (_| |  _|  _| | (__  | |_| | (_| | | | | | | --- |  _     _``-.    \n");
	printf("  |_||_|  \\__,_|_| |_| |_|\\___|  \\___/ \\__,_|_| |_| |_| --  '-(_)---(_)--'\n\n");

	/* Initialise the road and add cars */
	InitialiseRoad(road, 'E', 3);
	AddCar(road, 3, 3, 2);
	AddCar(road, 1, 1, 2);
	AddCar(road, 2, 1, 3);
	AddCar(road, 3, 2, -2);
	AddCar(road, 5, 2, -2);
	AddCar(road, 4, 4, -2);
	AddCar(road, 6, 3, 3);
	AddCar(road, 1, 5, -3);
	AddCar(road, 2, 6, -2);

	/* Print status */
	printf("ENGGEN131 2020 - C Project\nTraffic Jam!  There is a lot of traffic on the road!\n");
	printf("In fact, %.2f%% of the road is cars!\n\n", PercentUsed(road));

	/* Main simulation loop */
	while (!gameOver) {
		PrintRoad(road);
		input = GetMove();
		FindCar(road, input, &rowStart, &colStart, &rowEnd, &colEnd);
		gameOver = MoveCar(road, rowStart, colStart, rowEnd, colEnd);
	}

	/* A car has exited - the simulation is over */
	PrintRoad(road);
	printf("\nCongratulations, you're on your way again!");

	return 0;
}