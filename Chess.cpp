#undef FOR_RELEASE

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include<string>
#include "Game.h"
#include "Chess.h"
#include "Prompts.h"

using std::cout;
using std::endl;
using std::abs;
using std::ifstream;
using std::string;

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

int ChessPiece::getSpaces(Position start, Position end) const {

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
    return -((int)end.y - (int)start.y);
  }

  if (((end.x - start.x)*(end.x - start.x) + (end.y - start.y)*(end.y - start.y)) == 5) {
    return 5;
  }

  return 0;
		       
}


int ChessPiece::noPeopleInWay(Position start, Position end, const Board& board) const  {
  //  int ChessPiece::noPeopleInWay(Position start, Position end) const  {
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
      Position temp;
      temp.x = i;
      temp.y = tempStart.y;
      
      cout << "checking " << temp.x << " " << temp.y<< endl;//
      //cout << "piece is "<<board.getPiece(temp)->owner()<< endl;//
       
      if (board.getPiece(temp) != NULL) {
	Prompts::blocked();
	return MOVE_ERROR_BLOCKED;
      }
    }
  }

 if (dir == 'V') {
    for (int i = tempStart.y + 1; i < tempEnd.y; i++ ) {
      Position temp;
      temp.x = tempStart.x;
      temp.y = i;
      
      cout << "checking " << temp.x << " " << temp.y<< endl;//
      //cout << "piece is "<<board.getPiece(temp)->owner()<< endl;//
       
      if (board.getPiece(temp) != NULL) {
	Prompts::blocked();
	return MOVE_ERROR_BLOCKED;
      }
    }

  }

 if (dir == '/') {
    for (int i = 1; i < tempEnd.y - tempStart.y; i++ ) {
      Position temp;
      temp.x = tempStart.x + i;
      temp.y = tempStart.y + i;
      
      cout << "checking " << temp.x << " " << temp.y<< endl;//
      //cout << "piece is "<<board.getPiece(temp)->owner()<< endl;//
       
      if (board.getPiece(temp) != NULL) {
	Prompts::blocked();
	return MOVE_ERROR_BLOCKED;
      }
    }
  }

 if (dir == '\\') {
   cout << "thinks dir is " << dir << " and backwards is " << backwards<<endl;//
   cout << "starty and endy is " << tempStart.y << " " <<  tempEnd.y <<endl;//
   for (int i = 1; i < tempEnd.y - tempStart.y; i++ ) {
      Position temp;
      temp.x = tempStart.x + i;
      temp.y = tempStart.y - i;
      
      cout << "checking " << temp.x << " " << temp.y<< endl;//
      //cout << "piece is "<<board.getPiece(temp)->owner()<< endl;//
       
      if (board.getPiece(temp) != NULL) {
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
      if ((m_pieces.at(index(start)))->validMove(start, end, *this) > 0) {
	if (m_pieces.at(index(end)) != NULL) {
	    Prompts::capture(m_pieces.at(index(start))->owner());
	  }
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

int ChessGame::setUpSavedBoard(string filename) {
  string gameIdentifier; 
  unsigned int lastTurn;
  char P;
  char tempx;
  int tempy;
  int  idRead;
  char nextChar;
  Player playerTemp;
  
  ifstream input (filename);
  if (input.is_open()) {
    input >> gameIdentifier;
    input >> lastTurn;
    m_turn = lastTurn;
    //for (int i = 0; i!= EOF; i++) { //CHANGE THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    while (input >> P) {
      cout << P << endl; 
      
      if (P == '0') {
	//piece.owner = 0; for white player
	playerTemp = WHITE;
      }
      else {
	//piece.owner = 1; for black player
	playerTemp = BLACK;
      }
      input >> tempx;
      //some temp position: tempPosition.x = tempx + 97;
      cout << tolower(tempx) << endl;
      input >> tempy;
      cout << tempy << endl;
      //some temp position: tempPosition.y = tempy + 49;

      //index(tempPosition)

      input >> idRead;

      initPiece(idRead, playerTemp, Position(tolower(tempx)-97, tempy-1));

      // m_turns = 4;
    } // end of while loop
      // } for loop ends
  }
  else {
    Prompts::loadFailure();
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
