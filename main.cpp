#include <iostream>
#include <vector>

using namespace std;

struct Piece{
    int upperLeft, upperRight, lowerRight, lowerLeft;
    bool placed = false;
    vector<int> right, bottom;
};

vector<Piece> pieces;
vector<int> colors;

int boardWidth;
int boardHeight;

void placePiece(int *board, int index, int placedPieces){
    board[placedPieces] = index;
    
    int startIndex = index - (index % 4);
    for (int i = startIndex; i < startIndex + 4; i++)
        pieces[i].placed = true;
}

void removePiece(int *board, int index, int placedPieces){
    int startIndex = index - (index % 4);
    for (int i = startIndex; i < startIndex + 4; i++)
        pieces[i].placed = false;
    
    board[placedPieces] = 0;
}

// Checks in array bottom if any rotation of piece with a certain index exists
bool checkPiece(vector<int> bottom, int index){
    for (int piece : bottom)
        if (piece == index)
            return true;
    
    return false;
}

void printBoard(int *board){
    for (int line = 0; line < boardHeight; line++){
        if (line != 0)
            printf("\n");
        
        for (int column = 0; column < boardWidth; column++){
            int piece_index = board[line * boardWidth + column];
            Piece piece = pieces[piece_index];
            printf("%d %d", piece.upperLeft, piece.upperRight);
            if (column < boardWidth - 1)
                printf("  ");
            else
                printf("\n");
        }
        for (int column = 0; column < boardWidth; column++){
            int piece_index = board[line * boardWidth + column];
            Piece piece = pieces[piece_index];
            printf("%d %d", piece.lowerLeft, piece.lowerRight);
            if (column < boardWidth - 1)
                printf("  ");
            else
                printf("\n");
        }
    }
}

bool getPieces(int n){
    for (int i = 0; i < n; i++){
        int array[4];
        cin >> array[0] >> array[1] >> array[2] >> array[3];
        
        // Color count update
        colors[array[0]]++;
        colors[array[1]]++;
        colors[array[2]]++;
        colors[array[3]]++;
        
        // Piece rotations
        for (int j = 0; j < 4; j++){
            Piece newPiece;
            newPiece.upperLeft = array[j % 4];
            newPiece.upperRight = array[(j + 1) % 4];
            newPiece.lowerRight = array[(j + 2) % 4];
            newPiece.lowerLeft = array[(j + 3) % 4];
            
            int count = 0;
            for (vector<Piece>::iterator piece = pieces.begin(); piece < pieces.end(); piece++){
                if (4 * i + j - (j % 4) != i){
                    // Check if the new piece can be placed on the right
                    if (piece->upperRight == newPiece.upperLeft and piece->lowerRight == newPiece.lowerLeft)
                        piece->right.push_back(4 * i + j);
                    // Check if the new piece can be placed on the bottom
                    if (piece->lowerLeft == newPiece.upperLeft and piece->lowerRight == newPiece.upperRight)
                        piece->bottom.push_back(4 * i + j);
                    
                    // Check if the piece can be placed on the right of the new piece
                    if (newPiece.upperRight == piece->upperLeft and newPiece.lowerRight == piece->lowerLeft)
                        newPiece.right.push_back(count);
                    // Check if the piece can be placed on the bottom of the new piece
                    if (newPiece.lowerLeft == piece->upperLeft and newPiece.lowerRight == piece->upperRight)
                        newPiece.bottom.push_back(count);
                }
                
                count++;
            }

            pieces.push_back(newPiece);
            
        }
    }
    
    // Check if there are more than 4 colors with odd numbers (corner technique)
    int count = 0;
    for (int i = 0; i < 1000; i++)
        if (colors[i] % 2 != 0)
            if (++count > 4)
                return false;
        
    return true;
}

bool updateBoard(int *board, int placedPiece, int placedPieces){
    if (placedPieces == boardWidth * boardHeight){
        return true;
    }
    
    if (placedPieces < boardWidth){
        for (int rightPiece : pieces[placedPiece].right){
            if (!pieces[rightPiece].placed){
                placePiece(board, rightPiece, placedPieces);
                if (updateBoard(board, rightPiece, placedPieces + 1))
                    return true;
                
                removePiece(board, rightPiece, placedPieces);
            }
        }
    }
    else if (placedPieces % boardWidth == 0){
        int upperPiece_index = board[placedPieces - boardWidth];
        Piece upperPiece = pieces[upperPiece_index];
        
        for (int bottomPiece : upperPiece.bottom){
            if (!pieces[bottomPiece].placed){
                placePiece(board, bottomPiece, placedPieces);
                if (updateBoard(board, bottomPiece, placedPieces + 1))
                    return true;
                
                removePiece(board, bottomPiece, placedPieces);
            }
        }
    }
    else{
        int upperPiece_index = board[placedPieces - boardWidth];
        int previousPiece_index = board[placedPieces - 1];
        Piece upperPiece = pieces[upperPiece_index];
        Piece previousPiece = pieces[previousPiece_index];
        
        if(previousPiece.right.size()<=upperPiece.bottom.size()){
            for (int rightPiece : previousPiece.right){
                if (checkPiece(upperPiece.bottom, rightPiece)){
                    if (!pieces[rightPiece].placed){
                        placePiece(board, rightPiece, placedPieces);
                        if (updateBoard(board, rightPiece, placedPieces + 1))
                            return true;
                        
                        removePiece(board, rightPiece, placedPieces);
                    }
                }
            }
        }
        else{
            for (int bottomPiece : upperPiece.bottom){
                if (checkPiece(previousPiece.right, bottomPiece)){
                    if (!pieces[bottomPiece].placed){
                        placePiece(board, bottomPiece, placedPieces);
                        if (updateBoard(board, bottomPiece, placedPieces + 1))
                            return true;
                        
                        removePiece(board, bottomPiece, placedPieces);
                    }
                }
            }
        }
        
    }
    
    return false;
}

int main() {
    int nTests;
    cin >> nTests;
    
    for (int i = 0; i < nTests; i++){
        int nPieces;
        cin >> nPieces >> boardHeight >> boardWidth;
        
        pieces = vector<Piece>();
//        board[nPieces];
        int board[nPieces];
        memset(board, 0, sizeof(int)*nPieces);
        colors = vector<int>(1000, 0);
        
        if (!getPieces(nPieces))
            cout << "impossible puzzle!\n";
        else{
            placePiece(board, 0, 0);
            if (updateBoard(board, 0, 1))
                printBoard(board);
            else
                cout << "impossible puzzle!\n";
        }
    }
    
    return 0;
}

