#undef FOR_RELEASE

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Game.h"
#include "Chess.h"
#include "Prompts.h"

using std::cout;
using std::endl;

// Make a move on the board. Return an int, with < 0 being failure
int ChessGame::makeMove(Position start, Position end) {
    // Possibly implement chess-specific move logic here
    //
    // We call Board's makeMove to handle any general move logic
    // Feel free to use this or change it as you see fit
    int retCode = Board::makeMove(start, end);
    return retCode;
}

// Setup the chess board with its initial pieces
void ChessGame::setupBoard() {
    std::vector<int> pieces {
        ROOK_ENUM, KNIGHT_ENUM, BISHOP_ENUM, QUEEN_ENUM,
        KING_ENUM, BISHOP_ENUM, KNIGHT_ENUM, ROOK_ENUM
    };
    for (size_t i = 0; i < pieces.size(); ++i) {
        initPiece(PAWN_ENUM, WHITE, Position(i, 1));
        initPiece(pieces[i], WHITE, Position(i, 0));
        initPiece(pieces[i], BLACK, Position(i, 7));
        initPiece(PAWN_ENUM, BLACK, Position(i, 6));
    }
}

//print the chess board
void Board::printBoard() {
  for (int i = m_height - 1; i >= 0; i--) {
    for (int j=0; j < m_width; j++) {
      if (m_pieces.at((i*m_width) + j) != NULL) {
	cout << m_pieces[(i*m_width) + j]->owner() << m_pieces[(i*m_width) + j]->id();
	cout << " ";
    }
      else {
	cout << "__ ";
      }
  }
    cout << endl;
  }
}

/*
int ChessGame::run() {
  printBoard();
  return 0;
}
*/


int main() {
    ChessGame chess;
    chess.run();
}
