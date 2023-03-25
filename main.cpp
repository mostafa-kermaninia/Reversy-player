#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define EMPTY 0
#define NUT 1
#define WALL 2
#define BOARD_SIZE 7

struct move
{
    int moverRow;
    int moverCol;
    int horizontalChange;
    int VerticalChange;
};

char rowNumToChar(int rowNum);
string moveNumToStr(int horizontalChange, int VerticalChange);

// void printBoard(vector<vector<int>> gameBoard);
int countOfNuts(vector<vector<int>> gameBoard);

bool moveSelectedNut(vector<vector<int>> &gameBoard, struct move move, vector<struct move> &moves);
void undoSelectedMove(vector<vector<int>> &gameBoard, struct move move, vector<struct move> &moves);

bool continueTheGame(vector<vector<int>> &gameBoard, vector<struct move> &moves);

int main()
{
    char gamePiece;
    vector<vector<int>> gameBoard(BOARD_SIZE, vector<int>(BOARD_SIZE, EMPTY));

    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            cin >> gamePiece;
            if (gamePiece == 'N')
                gameBoard[row][col] = NUT;
            else if (gamePiece == 'O')
                gameBoard[row][col] = EMPTY;
            else if (gamePiece == 'X')
                gameBoard[row][col] = WALL;
        }
    }

    vector<struct move> moves;
    if (!continueTheGame(gameBoard, moves)) // If we cant do anything, we lose!
        cout << "Loser" << endl;

    return 0;
}

char rowNumToChar(int rowNum)
{
    switch (rowNum)
    {
    case 0:
        return 'A';
    case 1:
        return 'B';
    case 2:
        return 'C';
    case 3:
        return 'D';
    case 4:
        return 'E';
    case 5:
        return 'F';
    case 6:
        return 'G';
    }
}
string moveNumToStr(int horizontalChange, int VerticalChange)
{
    if (horizontalChange == 2)
        return "RIGHT";
    if (horizontalChange == -2)
        return "LEFT";
    if (VerticalChange == -2)
        return "UP";
    if (VerticalChange == 2)
        return "DOWN";
}

// void printBoard(vector<vector<int>> gameBoard)
// {
//     cout << '\n';
//     for (int row = 0; row < BOARD_SIZE; row++)
//     {
//         for (int col = 0; col < BOARD_SIZE; col++)
//             cout << gameBoard[row][col];
//         cout << '\n';
//     }
//     cout << "----------------\n";
// }
int countOfNuts(vector<vector<int>> gameBoard)
{
    int count = 0;
    for (int row = 0; row < BOARD_SIZE; row++)
        for (int col = 0; col < BOARD_SIZE; col++)
            if (gameBoard[row][col] == NUT)
                count++;
    return count;
}

bool moveSelectedNut(vector<vector<int>> &gameBoard, struct move move, vector<struct move> &moves)
{
    // If its an invalid move
    int newCol = move.moverCol + move.horizontalChange, newRow = move.moverRow + move.VerticalChange;
    int *middleNut;

    if (move.horizontalChange == 2) // Go right
    {
        if (move.moverCol >= gameBoard[0].size() - 2) // We cant go out of board
            return false;
        middleNut = &gameBoard[move.moverRow][move.moverCol + 1];
    }
    else if (move.horizontalChange == -2) // Go left
    {
        if (move.moverCol <= 1) // We cant go out of board
            return false;
        middleNut = &gameBoard[move.moverRow][move.moverCol - 1];
    }
    else if (move.VerticalChange == -2) // Go up
    {
        if (move.moverRow <= 1) // We cant go out of board
            return false;
        middleNut = &gameBoard[move.moverRow - 1][move.moverCol];
    }
    else if (move.VerticalChange == 2) // Go down
    {
        if (move.moverRow >= gameBoard.size() - 2) // We cant go out of board
            return false;
        middleNut = &gameBoard[move.moverRow + 1][move.moverCol];
    }

    if (gameBoard[newRow][newCol] != EMPTY || *middleNut != NUT) // If the move is invalid
        return false;

    // If move is valid, change the board and build a copy from current board for change it later and add move to moves vector
    vector<vector<int>> curGameBoard = gameBoard;
    gameBoard[move.moverRow][move.moverCol] = EMPTY;
    *middleNut = EMPTY;
    gameBoard[newRow][newCol] = NUT;

    moves.push_back(move);

    // printBoard(gameBoard);
    return true; // If the move was successful, return true
}
void undoSelectedMove(vector<vector<int>> &gameBoard, struct move move, vector<struct move> &moves)
{
    gameBoard[move.moverRow][move.moverCol] = NUT;
    gameBoard[move.moverRow + move.VerticalChange][move.moverCol + move.horizontalChange] = EMPTY;
    moves.pop_back();

    if (move.horizontalChange == 2) // We have moved the nut to the right
        gameBoard[move.moverRow][move.moverCol + 1] = NUT;
    else if (move.horizontalChange == -2) // We have moved the nut to the left
        gameBoard[move.moverRow][move.moverCol - 1] = NUT;
    else if (move.VerticalChange == -2) // We have moved the nut up
        gameBoard[move.moverRow - 1][move.moverCol] = NUT;
    else if (move.VerticalChange == 2) // We have moved the nut down
        gameBoard[move.moverRow + 1][move.moverCol] = NUT;
}

bool continueTheGame(vector<vector<int>> &gameBoard, vector<struct move> &moves)
{
    // condition for end of game (WIN)
    if (countOfNuts(gameBoard) == 1 && gameBoard[BOARD_SIZE / 2][BOARD_SIZE / 2] == NUT)
    {
        for (auto move : moves)
            cout << rowNumToChar(move.moverRow) << move.moverCol + 1 << ' ' << moveNumToStr(move.horizontalChange, move.VerticalChange) << endl;
        exit(0);
    }

    // Find all nuts in board
    for (int row = 0; row < gameBoard.size(); row++)
    {
        for (int col = 0; col < gameBoard[0].size(); col++)
        {
            if (gameBoard[row][col] == NUT)
            {
                // Try every possible move
                struct move currentMove;
                bool canWin;

                // Go up
                currentMove = {row, col, 0, -2};
                if (moveSelectedNut(gameBoard, currentMove, moves))
                {
                    canWin = continueTheGame(gameBoard, moves);
                    if (canWin == false)
                        undoSelectedMove(gameBoard, currentMove, moves); // if : 1- we can do this move ,and 2- it won't be successfull, we should move back the nut
                }
                // Go down
                currentMove = {row, col, 0, 2};
                if (moveSelectedNut(gameBoard, currentMove, moves))
                {
                    canWin = continueTheGame(gameBoard, moves);
                    if (canWin == false)
                        undoSelectedMove(gameBoard, currentMove, moves);
                }
                // Go right
                currentMove = {row, col, 2, 0};
                if (moveSelectedNut(gameBoard, currentMove, moves))
                {
                    canWin = continueTheGame(gameBoard, moves);
                    if (canWin == false)
                        undoSelectedMove(gameBoard, currentMove, moves);
                }
                // Go left
                currentMove = {row, col, -2, 0};
                if (moveSelectedNut(gameBoard, currentMove, moves))
                {
                    canWin = continueTheGame(gameBoard, moves);
                    if (canWin == false)
                        undoSelectedMove(gameBoard, currentMove, moves);
                }
            }
        }
    }
    return false;
}