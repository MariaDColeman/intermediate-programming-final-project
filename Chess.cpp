#undef FOR_RELEASE

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "Game.h"
#include "Chess.h"
#include "Prompts.h"

using std::cout;
using std::endl;
using std::abs;

int ChessPiece::properAloneMove(Position start, Position end) const {
  if(!((properDirection(getDirection(start,end)) && (properSpaces(start, end))))) {
    //cout << getDirection(start,end) << endl;
    //cout << getSpaces(start,end) << endl;
    Prompts::illegalMove();
    return MOVE_ERROR_ILLEGAL;
  }
  else {
    return SUCCESS;
  }
}

char ChessPiece::getDirection(Position start, Position end) const{

  if (start.x == end.x) {
    return 'V';
  }

  if (start.y == end.y) {
    return 'H';
  }

  if ((end.x - start.x) == (end.y - start.y)) {
    return '/';
  }

  if ((end.x - start.x) == -(end.y - start.y)) {
    return '\\';
  }

  if (((end.x - start.x)*(end.x - start.x) + (end.y - start.y)*(end.y - start.y)) == 5) {
    return 'L';
  }

  else {
    return '0';
  }
}

int ChessPiece::getSpaces(Position start, Position end) const{

  if (start.x == end.x) {
    return (int)end.y - (int)start.y;
  }

  if (start.y == end.y) {
    return (int)end.x - (int)start.x;
  }

  if ((end.x - start.x) == (end.y - start.y)) {
    return (int)end.x - (int)start.x;
  }

  if ((end.x - start.x) == -(end.y - start.y)) {
    return (int)end.y - (int)start.y;
  }

  if (((end.x - start.x)*(end.x - start.x) + (end.y - start.y)*(end.y - start.y)) == 5) {
    return 5;
  }

  return 0;
		       
}


int ChessPiece::noPeopleInWay(Position start, Position end, const Board& board)const  {
  //char directionCode;
  //int backwards = 0; //0 if not backwards(to the right, up, up-right, up-left)
  Position tempStart = start;
  Position tempEnd = end;
  
  char dir = getDirection(start, end);
  int spaces = getSpaces(start, end);
  int backwards;
  
  if (spaces < 0) {
    //going in the negative x or the negative y
    backwards = 1;
  }

  if (backwards) {
    tempStart = end;
    tempEnd = start;
  }

  if (dir == 'H') {
    for (int i = tempStart.x + 1; i < tempEnd.x; i++ ) {
      if (board.m_pieces.at(tempStart.y * m_width + i) != NULL) {
	Prompts::blocked();
	return MOVE_ERROR_BLOCKED;
      }
    }
  }

  return SUCCESS;
}


// Make a move on the board. Return an int, with < 0 being failure
int ChessGame::makeMove(Position start, Position end){
    // Possibly implement chess-specific move logic here
    //
    // We call Board's makeMove to handle any general move logic
    // Feel free to use this or change it as you see fit
  
  int retCode = Board::makeMove(start, end);
 
  if (retCode==1) {
      if ((m_pieces.at(index(start)))->validMove(start, end, *this) >= 0) {
      m_pieces.at(index(end)) = m_pieces.at(index(start));
      m_pieces.at(index(start)) = NULL;
      return 1;
      }
      else {
	return -1;
      }
         
  }
  else {
    return -1;
  }
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
