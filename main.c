#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


#define   BOARD_LENGTH    9
#define   true            1
#define   false           0
#define   bool            int
#define   EMPTY_SPACE     '-'


/**
 * Represents a player in the game.
 */
typedef struct Player Player;
struct Player {
  bool isAI;
  char *name;
  char symbol;
  Player *opponent;
};

/**
 * Represents a board in the game.
 */
typedef struct {
  char spaces[BOARD_LENGTH];
} Board;

/**
 * Represents the game.
 */
typedef struct {
  Player *currentPlayer;
  Player *playerOne;
  Player *playerTwo;
  Board *board;
} Game;

/**
 * Constructs a new game.
 */
void new_game(Game *game, Board *b, Player *playerOne, Player *playerTwo, char playerOneSymbol, char playerTwoSymbol, char *playerOneName, char *playerTwoName, bool isAiPlayerOne, bool isAiPlayerTwo);
/**
 * Checks if the board is full.
 */
bool is_over(Board *b);
/**
 * Checks if a move is valid.
 */
bool is_valid_move(int move, Board *b);
/**
 * Gets an input from the user.
 */
int get_input();
/**
 * Prints the board.
 */
void print_board(Board *b);
/**
 * Clear a space in the specifield board.
 */
void clear_space(Board *b, int index);
/**
 * Checks if a specific position of the board is occupied.
 */
bool is_occupied(Board *b, int index);
/**
 * Checks if a specific position of the board is occupied by a specific player.
 */
bool is_occupied_by_player(Board *b, Player *p, int index);
/**
 * Checks if a specific player won the game.
 */
bool check_winner(Board *b, Player *p);
/**
* Creates a new copy of the situation of the board.
*/
void new_copy(Board *b, Board *newb);
/**
 * Gets a movement, if the player is a computer player, the minimax algorithm will be executed, otherwise, a user input will be realized.
 */
int get_move(Board *b, Player *p);
/**
 * Occupies a position in the board.
 */
void set_occupied_by(Player *p, Board *b, int index);
/**
* The minimax based algorithm for the computer moves. This function will run recursivaly ultil it reaches a final state, then will return a ponctuation of the position provided.
* The best move for the computer occurs in the index of the highest ponctuation. Run this function for each free space available in the game to find the best move.
*/
int minimax(Board *b, Player *p_maximize, Player *p, int index, int depth);
/**
 * To play the game. This function will checks the situation and the winners of the game, and gets the moves for the human and computer player's.
 */
void play(Game *game);

int main(int argc, char const *argv[]) {
  Game game;
  Board b;
  Player playerOne;
  Player playerTwo;
  new_game(&game, &b, &playerOne, &playerTwo, 'x', 'o', "Player 1", "Player 2", true, true);
  play(&game);
  return 0;
}

void new_game(Game *game, Board *b, Player *playerOne, Player *playerTwo, char playerOneSymbol, char playerTwoSymbol, char *playerOneName, char *playerTwoName, bool isAiPlayerOne, bool isAiPlayerTwo) {
  playerOne->symbol = playerOneSymbol;
  playerTwo->symbol = playerTwoSymbol;
  playerOne->opponent = playerTwo;
  playerTwo->opponent = playerOne;
  playerOne->isAI = isAiPlayerOne;
  playerTwo->isAI = isAiPlayerTwo;
  playerOne->name = playerOneName;
  playerTwo->name = playerTwoName;
  game->board = b;
  int i;
  for (i = 0; i < BOARD_LENGTH; i++) {
    clear_space(b, i);
  }
  game->playerOne = playerOne;
  game->playerTwo = playerTwo;
  game->currentPlayer = game->playerOne;  // Default value for currentPlayer
}

void clear_space(Board *b, int index) {
  b->spaces[index] = EMPTY_SPACE;
}

bool is_occupied(Board *b, int index) {
  return b->spaces[index] != EMPTY_SPACE;
}

bool is_occupied_by_player(Board *b, Player *p, int index) {
  return b->spaces[index] == p->symbol;
}

bool check_winner(Board *b, Player *p) {
  int i;
  for (i = 0; i <= 6; i += 3) {
    if (is_occupied_by_player(b, p, 0 + i) && is_occupied_by_player(b, p, 1 + i) && is_occupied_by_player(b, p, 2 + i)) {
      return true;
    }
  }
  for (i = 0; i < 3; i++) {
    if (is_occupied_by_player(b, p, 0 + i) && is_occupied_by_player(b, p, 3 + i) && is_occupied_by_player(b, p, 6 + i)) {
      return true;
    }
  }
  if (is_occupied_by_player(b, p, 0) && is_occupied_by_player(b, p, 4) && is_occupied_by_player(b, p, 8)) {
    return true;
  } else if (is_occupied_by_player(b, p, 2) && is_occupied_by_player(b, p, 4)  && is_occupied_by_player(b, p, 6)) {
    return true;
  }
  return false;
}

void new_copy(Board *b, Board *newb) {
  int i;
  for (i = 0; i < BOARD_LENGTH; i++) {
    newb->spaces[i] = b->spaces[i];
  }
}

void print_board(Board *b) {
  int i, j;
  puts("------------\n");
  for (i = 0; i <= BOARD_LENGTH - 3; i += 3) {
    for (j = 0; j < sqrt(BOARD_LENGTH); j++) {
      printf("  %c", (b->spaces)[i + j]);
    }
    puts("\n");
  }
  puts("------------");
}

int minimax(Board *b, Player *p_maximize, Player *p, int index, int depth) {
  Board bc;
  new_copy(b, &bc);
  set_occupied_by(p, &bc, index);
  if (check_winner(&bc, p)) {
    if (p == p_maximize) return 10 - depth;
    return depth - 10;
  } else if (is_over(&bc)) {
    return 0;
  }

  int i;
  int bestScore;
  bool scoreNull;
  scoreNull = true;
  for (i = 0; i < BOARD_LENGTH; i++) {
    if (!is_occupied(&bc, i)) {
      int score = minimax(&bc, p_maximize, p->opponent, i, depth + 1);
      if (scoreNull || (p != p_maximize && bestScore < score) || (p == p_maximize && bestScore > score)) {
        bestScore = score;
        scoreNull = false;
      }
    }
  }
  return bestScore;
}

int get_input() {
  int move;
  puts("Enter a valid index to move: ");
  scanf("%d", &move);
  return move;
}

bool is_valid_move(int move, Board *b) {
  return move >= 0 && move < BOARD_LENGTH && !is_occupied(b, move);
}

int get_move(Board *b, Player *p) {
  int move;
  if (p->isAI) {
    int i, j;
    int bestScore = -1000;
    for (j = 0, i = rand() % BOARD_LENGTH; j < BOARD_LENGTH; j++, i++) {
      if (i == BOARD_LENGTH) i = 0;
      if (!is_occupied(b, i)) {
        int score = minimax(b, p, p, i, 0);
        if (score > bestScore) {
          bestScore = score;
          move = i;
        }
      }
    }
  } else {
    do {
      move = get_input();
    } while(!is_valid_move(move, b));
  }
  return move;
}

void set_occupied_by(Player *p, Board *b, int index) {
  /*
  * The position will be occupied only if is not occupied.
  */
  if (!is_occupied(b, index)) {
    b->spaces[index] = p->symbol;
  }
}

bool is_over(Board *b) {
  int i;
  for (i = 0; i < BOARD_LENGTH; i++) {
    if (!is_occupied(b, i)) {
      return false;
      printf("%d\n", i);
    }
  }
  return true;
}

void play(Game *game) {
  srand(time(NULL));
  print_board(game->board);
  do {
    printf("%s's turn\n", game->currentPlayer->name);
    set_occupied_by(game->currentPlayer, game->board, get_move(game->board, game->currentPlayer));
    print_board(game->board);
    if (check_winner(game->board, game->currentPlayer)) {
      printf("\n*** Congratulations %s ***\n\n", game->currentPlayer->name);
      break;
    } else if (is_over(game->board)) {
      puts("\n*** There is a tie ***\n");
      break;
    }
    if (game->currentPlayer == game->playerTwo) {
      game->currentPlayer = game->playerOne;
    } else {
      game->currentPlayer = game->playerTwo;
    }
  } while(true);
}
