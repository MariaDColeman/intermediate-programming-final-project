#undef FOR_RELEASE

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ostream>
#include <fstream>
#include <cmath>
#include <string>
#include "Game.h"
#include "Chess.h"
#include "Prompts.h"
#include "Terminal.h"

using std::cout;
using std::endl;
using std::abs;
using std::ifstream;
using std::ostream;
using std::string;



//int ChessPiece::isCheckedPosition(Position position, const Board& board) const {
  int ChessGame::isCheckedPosition(Position position) const {
  //assuming position is position of the king right now or in future
  //call for every board state. put back (in valid move) and print error if this says it will be in checked position
  //go through m_pieces for the other team and see if its a valid move from where the piece is currently to "position"
  //if any can get to "position", then king is in check so Prompts::check(pass a player probably "other team")
    cout << "in isCheckedPosition" << endl;
    const Board& board = *this;
  Player ourOwner = board.getPiece(position)->owner();
  Player theirOwner = Player(!ourOwner);
  Position temp;
  
  for (int i = 0; i < (int)board.width() * (int)board.height(); i++) {
    cout << "in for loop in chededpos" << endl;
    temp.x = i % board.width();
    temp.y = i / board.height();
    if (board.getPiece(temp) != NULL) {
      cout << "in not null" << endl;
      if (board.getPiece(temp)->owner() == theirOwner) {
      //       if (validMove(temp ,position, board) > 0) {
	cout << "in their owner" << endl;
	cout << "valid move is: " << board.getPiece(temp)->validMove(temp, position, board) << endl;
      if ((board.getPiece(temp)->validMove(temp ,position, board)) > 0) {
	Prompts::check(theirOwner);
	return MOVE_CHECK; 
      }
    }
    }
  }
  return -1;
  
} 

int ChessPiece::isCheckMate(Board& board) const {

  //loop through m_pieces i
  //if m_pieces.at(i)->owner() == positionOfKing->owner()

  //loop through all board spots as "end" positions.
  //make the move: makeMove(i, end, board);
  //have it return the id of the piece at end position or an error code; The error code will be less than 0 and the piece id will be greater than   >= 0;
  //call isCheckedPosition(positionOfKing(may be updated), board)
  //if its not checked, then put back piece into "end" position. use initPieces and stick it in end position and break out of loop. and return 0;
  //if we go through all of the pieces, return 1;
  return 1;
}


int ChessPiece::properAloneMove(Position start, Position end) const {

   
  //cout << "Proper dir" << (properDirection(getDirection(start,end)) > 0)  << endl;
  //cout << "proper spaces" << (properSpaces(start,end) > 0) << endl;
  //cout << "both" << ((properDirection(getDirection(start,end) > 0) && (properSpaces(start, end) > 0))) << endl;
 
  cout << "proper dir: " << (properDirection(getDirection(start,end)) > 0) << endl;
  cout << "proper spaces " << (properSpaces(start,end) > 0) << endl;
  cout << "both" << (((properDirection(getDirection(start,end))) > 0) && ((properSpaces(start, end)) > 0)) << endl;
  if ((((properDirection(getDirection(start,end))) > 0) && ((properSpaces(start, end)) > 0))) {
    //Prompts::illegalMove();
    cout << "in loooooop" << endl;
    return SUCCESS;   
  }
  else {
    return MOVE_ERROR_ILLEGAL;
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
  int backwards = 0;

  //cout << "dir is ahhh " << dir << endl;

  
  if (spaces < 0) {
    //going in the negative x or the negative y
    backwards = 1;
    //cout << "made backwards 1" << endl;
  }

  if (backwards) {
    tempStart = end;
    tempEnd = start;
  }

  //cout << "tempStart.y = "<<  tempStart.y << endl;
  //cout << "tempEnd.y = " << tempEnd.y << endl;
  

  
  if (dir == 'H') {
    for (int i = (int) tempStart.x + 1; i < (int) tempEnd.x; i++ ) {
      Position temp;
      temp.x = i;
      temp.y = tempStart.y;
      
      //cout << "checking " << temp.x << " " << temp.y<< endl;//
      //cout << "piece is "<<board.getPiece(temp)->owner()<< endl;//
       
      if (board.getPiece(temp) != NULL) {
	//Prompts::blocked();
	return MOVE_ERROR_BLOCKED;
      }
    }
  }

 if (dir == 'V') {
   for (int i = (int) tempStart.y + 1; i < (int) tempEnd.y; i++ ) {
      Position temp;
      temp.x = tempStart.x;
      temp.y = i;
      
      //cout << "checking " << temp.x << " " << temp.y<< endl;//
      //cout << "piece is "<<board.getPiece(temp)->owner()<< endl;//
       
      if (board.getPiece(temp) != NULL) {
	//Prompts::blocked();
	return MOVE_ERROR_BLOCKED;
      }
    }

  }

 if (dir == '/') {
   for (int i = 1; i < (int) (tempEnd.y) - (int) (tempStart.y); i++ ) {
      Position temp;
      temp.x = tempStart.x + i;
      temp.y = tempStart.y + i;
      
      cout << "checking " << temp.x << " " << temp.y<< endl;//
      //cout << "piece is "<<board.getPiece(temp)->owner()<< endl;//
       
      if (board.getPiece(temp) != NULL) {
	//Prompts::blocked();
	return MOVE_ERROR_BLOCKED;
      }
    }
  }

 if (dir == '\\') {
   //cout << "thinks dir is " << dir << " and backwards is " << backwards<<endl;//
   //cout << "starty and endy is " << tempStart.y << " " <<  tempEnd.y <<endl;//
   //cout << " in \\" << endl;
   //   for (int i = 1; i < (int) (tempEnd.y) - (int) (tempStart.y); i++ ) {
     for (int i = 1; i < (int) (tempStart.y) - (int) (tempEnd.y); i++ ) {
       //  cout << "in for loop" << endl;
      Position temp;
      temp.x = tempStart.x + i;
      temp.y = tempStart.y - i;
      
      //cout << "checking in \\" << temp.x << " " << temp.y<< endl;//
      //cout << "piece is "<<board.getPiece(temp)->owner()<< endl;//
       
      if (board.getPiece(temp) != NULL) {
	//Prompts::blocked();
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
  int tempCode;
  if (retCode==1) {
    tempCode = m_pieces.at(index(start))->validMove(start, end, *this);
  }


    
  int checkedCode = 0; 
      //if ((m_pieces.at(index(start)))->validMove(start, end, *this) > 0) {
    //    cout << "tempCode is: " << tempCode << endl;
      //if it's a valid move
  //int capturedID = -1;
  Piece* captured;
  if (tempCode > 0) {

      //if (!checkedCode) {
	if (m_pieces.at(index(end)) != NULL) {
	    Prompts::capture(m_pieces.at(index(start))->owner());
	    // capturedID = m_pieces.at(index(end));
	    captured = m_pieces.at(index(end));
	}
       
      m_pieces.at(index(end)) = m_pieces.at(index(start));
      m_pieces.at(index(start)) = NULL;


      
    Position ourKing;
    int kingFound = 0;
    //get the kings position
    while (!kingFound) {
      for (int i = 0; i < (int)this->width() * (int)this->height(); i++) {
    ourKing.x = i % this->width();
    ourKing.y = i / this->height();
    if (this->getPiece(ourKing) != NULL) {
      if ((this->getPiece(ourKing)->id() == KING_ENUM)&&(this->getPiece(ourKing)->owner() == ((this->m_turn + 1) % 2))) {
	cout << "king found" << endl;
	kingFound = 1;
      }
    }
      }
    }

    //     checkedCode = ChessPiece::isCheckedPosition(ourKing, *this);
       checkedCode = this->isCheckedPosition(ourKing);
       cout << "checked code is: " << checkedCode;
  if (checkedCode == MOVE_CHECK) {
    
    Prompts::cantExposeCheck();
    m_pieces.at(index(start)) = m_pieces.at(index(end));
    m_pieces.at(index(end)) = captured;
    return MOVE_ERROR_CANT_EXPOSE_CHECK;
  }

      return SUCCESS;
  } else {
	switch (tempCode) {
	    case MOVE_ERROR_ILLEGAL:
	      Prompts::illegalMove();
	      return MOVE_ERROR_ILLEGAL;
	      break;
	    case MOVE_ERROR_BLOCKED:
	      Prompts::blocked();
	      return MOVE_ERROR_BLOCKED;
	      break;
	    default:
	      return retCode;
	      break;
          }

	
  }
  
  
    /*
      else {
	return MOVE_CHECK;
      }
    */   
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

  Player playerTemp;
  
  ifstream input (filename);
  if (input.is_open()) {
    input >> gameIdentifier;
    input >> lastTurn;
    m_turn = lastTurn;
    //for (int i = 0; i!= EOF; i++) { //CHANGE THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    while (input >> P) {
      //cout << P << endl; 
      
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
      //cout << tolower(tempx) << endl;
      input >> tempy;
      //cout << tempy << endl;
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
    return LOAD_FAILURE;
  }
  return 1;
}

int Board::saveBoard(string filename) {
  cout<<"saving! " << filename<<endl;//
  std::ofstream output;
  output.open(filename);
  output << "chess" <<endl;
  output << m_turn <<endl;

  for (unsigned int i = 0; i < m_height * m_width; i++) {
    if (m_pieces.at(i) != NULL) {
      output << m_pieces.at(i)->owner()<< " ";
      output << ((char) (i % m_width + 'a'));
      output << (i / m_height + 1) << " ";
      output << m_pieces.at(i)->id() <<endl;
    }
  }
  output.close();
  return SUCCESS;
}

//print the chess board
void Board::printBoard() {
  Terminal::colorBg(Terminal::BLACK);
Terminal::colorFg(1, Terminal::BLACK);
  
  string kg = "\u265A";
  string q = "\u265B";
  string r = "\u265C";
  string b = "\u265D";
  string kt = "\u265E";
  string p = "\u265F";

  //prints one in circle for unknown pieces
  string u = "\u2776";
  
  //cout << "This is a single character: " << kg << endl; 

  string sym;

  for (int i = m_height - 1; i >= 0; i--) {
    cout << i + 1<< " ";
    for (int j=0; j < (int) m_width; j++) {

      //alternates background for checkerboard
      if ((i + j) % 2) {
        Terminal::colorBg(Terminal::WHITE);
      }
      else {
        Terminal::colorBg(Terminal::BLACK);
      }
  
      
      if (m_pieces.at((i*m_width) + j) != NULL) {
	//print correct piece rep
	switch (m_pieces[(i*m_width) + j]->id()) {
	case KING_ENUM: sym = kg;
	  break;
	case QUEEN_ENUM: sym = q;
	  break;
        case BISHOP_ENUM: sym = b;
	  break;
	case KNIGHT_ENUM: sym = kt;
	  break;
	case ROOK_ENUM: sym = r;
	  break;
	case PAWN_ENUM: sym = p;
	  break;
	default: sym = u;
	  break;
	}

	//change color for black vs white
	if (m_pieces[(i*m_width) + j]->owner()) {
	  Terminal::colorFg(1, Terminal::BLACK);
	}
	else {
	  Terminal::colorFg(1, Terminal::WHITE);
	}
	cout  << sym << " ";
    }
      else {
	cout << "  ";
      }
  }
    //reset colors
    Terminal::colorBg(Terminal::BLACK);
    Terminal::colorFg(1, Terminal::BLACK);
    cout << endl;
  }
  
  cout << "  A B C D E F G H"<<endl;

  /*  for (int i = m_height - 1; i >= 0; i--) {
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
  */
  Terminal::colorFg(1, Terminal::WHITE);
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
