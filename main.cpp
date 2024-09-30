#include <iostream>
#include <limits>
#include <iomanip>
using namespace std;

const int MAX = 8;
const int MIN = 3;
int boardSize;
int winLength;
int maxDepth;

enum Player { HUMAN = 'X', COMPUTER = 'O', EMPTY = ' ' };

Player board[MAX][MAX];

void initBoard() { // inicjalizacja pola do gry
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++)
            board[i][j] = EMPTY;
}

void printBoard() {
    // czyszczenie
    system("cls");

    // kolumny
    cout << " ";
    for (int j = 0; j < boardSize; j++) {
        cout << " " << j + 1;
    }
    cout << endl;

    // pozioma linia
    cout << "  " << string(2 * boardSize - 1, '-') << endl;

    for (int i = 0; i < boardSize; i++) {
        // pionowa linia
        cout << i + 1 << "|";
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] == EMPTY)
                cout << ' ';
            else
                cout << (char)board[i][j];
            cout << "|";
        }

        cout << endl;

        if (i < boardSize - 1) {
            // pozioma linia oddzielająca wiersze
            cout << "  " << string(2 * boardSize - 1, '-') << endl;
        }
    }

    // pozioma linia na dole planszy
    cout << "  " << string(2 * boardSize - 1, '-') << endl;
}


bool isFull() { //sprawdzanie czy plansza pełna
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++)
            if (board[i][j] == EMPTY)
                return false;
    return true;
}
// funkcja która sprawdza wygranie
bool hasWon(Player player) {
    // rzad
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j <= boardSize - winLength; j++) {
            bool win = true;
            for (int k = 0; k < winLength; k++)
                if (board[i][j + k] != player) {
                    win = false;
                    break;
                }
            if (win) return true;
        }
    }
    // kolumna
    for (int j = 0; j < boardSize; j++) {
        for (int i = 0; i <= boardSize - winLength; i++) {
            bool win = true;
            for (int k = 0; k < winLength; k++)
                if (board[i + k][j] != player) {
                    win = false;
                    break;
                }
            if (win) return true;
        }
    }
    // przektna 1
    for (int i = 0; i <= boardSize - winLength; i++) {
        for (int j = 0; j <= boardSize - winLength; j++) {
            bool win = true;
            for (int k = 0; k < winLength; k++)
                if (board[i + k][j + k] != player) {
                    win = false;
                    break;
                }
            if (win) return true;
        }
    }
    //przekatna 2
    for (int i = 0; i <= boardSize - winLength; i++) {
        for (int j = boardSize - 1; j >= winLength - 1; j--) {
            bool win = true;
            for (int k = 0; k < winLength; k++)
                if (board[i + k][j - k] != player) {
                    win = false;
                    break;
                }
            if (win) return true;
        }
    }
    return false;
}

bool gameOver() { // stan gry
    return isFull() || hasWon(HUMAN) || hasWon(COMPUTER);
}

int evaluate() { // ocena ruchów graczy
    if (hasWon(HUMAN)) return -1;
    if (hasWon(COMPUTER)) return +1;
    return 0;
}

int minimax(int depth, int alpha, int beta, bool isMaximizingPlayer) {
    if (depth == 0 || gameOver()) return evaluate();

    if (isMaximizingPlayer) {
        int bestValue = numeric_limits<int>::min();
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if (board[i][j] != EMPTY) continue;

                board[i][j] = COMPUTER;

                int value =
                    minimax(depth - 1, alpha, beta, !isMaximizingPlayer);
                bestValue = max(bestValue, value);
                alpha     = max(alpha, value);

                board[i][j] = EMPTY;

                if (beta <= alpha) break;
            }
            if (beta <= alpha) break;
        }
        return bestValue;

    } else { // minimalizacja szans gracza
        int bestValue = numeric_limits<int>::max();
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if (board[i][j] != EMPTY) continue;

                board[i][j] = HUMAN;

                int value =
                    minimax(depth - 1, alpha, beta, !isMaximizingPlayer);
                bestValue = min(bestValue, value);
                beta      = min(beta, value);

                board[i][j] = EMPTY;

                if (beta <= alpha) break;
            }
            if (beta <= alpha) break;
        }
        return bestValue;
    }
}
// ruch komputera
void computerMove() {
    int bestValue = numeric_limits<int>::min();
    int bestI     = -1;
    int bestJ     = -1;
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] != EMPTY) continue;

            board[i][j] = COMPUTER;

            int value =
                minimax(maxDepth, numeric_limits<int>::min(), numeric_limits<int>::max(), false);
            if (value > bestValue) {
                bestValue = value;
                bestI     = i;
                bestJ     = j;
            }

            board[i][j] = EMPTY;
        }
    }
    board[bestI][bestJ] = COMPUTER;
}

void humanMove() { // ruch gracza
    int i, j;
    do {
        cout << "Podaj rzad i kolumne: ";
        cin >> i >> j;
        i--;
        j--;
    } while (i < 0 || i >= boardSize || j < 0 || j >= boardSize ||
             board[i][j] != EMPTY);
    board[i][j] = HUMAN;
}

int main() {
    cout << "Podaj rozmiar planszy: ";
    cin >> boardSize;

    cout << "Podaj ile znakow do wygranej: ";
    cin >> winLength;

    cout << "Podaj glebokosc rekursji: ";
    cin >> maxDepth;
    

    initBoard();
    printBoard();

    while (!gameOver()) {
        humanMove();
        printBoard();
        if (gameOver()) break;

        computerMove();
        printBoard();
    }

    if (hasWon(HUMAN))
        cout << "Wygrales!" << endl;
    else if (hasWon(COMPUTER))
        cout << "Komputer wygral!" << endl;
    else
        cout << "Remis" << endl;

    return 0;
}
