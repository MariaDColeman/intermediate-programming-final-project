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



//king is the position of the king to be checked
//returns movecheck if true, -99 if not in check
  int ChessGame::isCheckedPosition(Position king) {
    //    cout << "in isCheckedPosition" << endl;
    Board& board = *this;
    Player ourOwner = board.getPiece(king)->owner();
    Player theirOwner = Player(!ourOwner);
    Position start;

  //go through all of the m pieces
  for (int i = 0; i < (int)board.width() * (int)board.height(); i++) {
    start.x = i % board.width();
    start.y = i / board.height();
    //piece is not null, and it's the opponent's piece
    if (board.getPiece(start) != NULL) {
      if (board.getPiece(start)->owner() == theirOwner) {
	//cout << "piece its looking at is: " << board.getPiece(start)->id() << " " << start.x << " " << start.y << endl;//
	//check to see if it can get to player's king
	//	cout<< "validMove is returning " << ((board.getPiece(start)->validMove(start,king, board))) <<endl;
		if ((board.getPiece(start)->validMove(start,king, board)) > 0) {
	  //Prompts::check(theirOwner);
		  //	  cout << "returning move_check" << endl;
	return MOVE_CHECK; 
	}
      }//else not opp piece
    }//else it's a null pointer
  }//went through all of the pieces
  return -99;
} 

//pl the player to check if they are under checkmate
//returns move_ checkmate if under checkmate, -11 if not 
int ChessGame::isUnderCheckMate(Player pl) {   
  // Position King = this->findKing(pl);
  Position start;
  Position end;
  

  for (int i = 0; i < (int)this->width() * (int)this->height(); i++) {
    start.x = i % this->width();
    start.y = i / this->height();
    //only look at things that are not null pointers
    //and only look at pieces that are the player's
    if (this->getPiece(start) != NULL) {
      if (this->getPiece(start)->owner() == pl) {

	//loop through all of the positions on the board
	for (int j = 0; j < (int)this->width() * (int)this->height(); j++) {
	  end.x = j % (this->width());
	  end.y = j / (this->height());

	  //if valid move, move it, and see if takes the player out of check
	  if ((this->getPiece(start)->validMove(start,end, *this)) > 0) {
	    Piece* captured = m_pieces.at(index(end));
	    m_pieces.at(index(end)) = m_pieces.at(index(start));
            m_pieces.at(index(start)) = NULL;
	    //if no longer incheck,undo, and the player is not under check mate
	    if (isCheckedPosition((this->findKing(pl))) < 0) {
	      m_pieces.at(index(start)) = m_pieces.at(index(end));
              m_pieces.at(index(end)) = captured;
	      return -11;
	    }
	    //else, still undo and keep searching
	     m_pieces.at(index(start)) = m_pieces.at(index(end));
             m_pieces.at(index(end)) = captured;
	  } //else, not a valid move from start to end
	}//went through all positions on board
      }//else, is not the player's piece
    }//else, is a null pointer
  }//went through all the pieces in m pieces
  return MOVE_CHECKMATE;
}

//pl the player whose king we want to find
//returns the found king's position, (-1,-1) if not found
Position ChessGame::findKing(Player pl) {
    Position king;
      for (int i = 0; i < (int)this->width() * (int)this->height(); i++) {
        king.x = i % this->width();
        king.y = i / this->height();
        if (this->getPiece(king) != NULL) {
          if ((this->getPiece(king)->id() == KING_ENUM)&&(this->getPiece(king)-\
>owner() == pl)) {
            return king;
          }
        }
      }
      return Position(-1,-1);
}


int ChessPiece::properAloneMove(Position start, Position end) const {
   
  if ((((properDirection(getDirection(start,end))) > 0) && ((properSpaces(start, end)) > 0))) {
    //Prompts::illegalMove();
    //cout << "in loooooop" << endl;
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
  if (((int)end.x - (int)start.x) == ((int)end.y - (int)start.y)) {
    return '/';
  }
  if (((int)end.x - (int)start.x) == -((int)end.y - (int)start.y)) {
    return '\\';
  }
  if (((end.x - start.x)*(end.x - start.x) + (end.y - start.y)*(end.y - start.y)) == 5) {
    return 'L';
  }
  else {
    //cout << "get dir Startx: " << start.x << "endx: " << end.x <<endl;//
    //cout << "get dir Starty: " << start.y << "endy: " << end.y <<endl;//
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

//returns error codes or success
int ChessPiece::noPeopleInWay(Position start, Position end, const Board& board) const  {
  Position tempStart = start;
  Position tempEnd = end;
  
  char dir = getDirection(start, end);
  int spaces = getSpaces(start, end);
  int backwards = 0;

  if (spaces < 0) {
    //going in the negative x or the negative y
    backwards = 1;
  }

  if (backwards) {
    tempStart = end;
    tempEnd = start;
  }

  if (dir == 'H') {
    for (int i = (int) tempStart.x + 1; i < (int) tempEnd.x; i++ ) {
      Position temp;
      temp.x = i;
      temp.y = tempStart.y;
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
      
      // cout << "checking " << temp.x << " " << temp.y<< endl;//
      //cout << "piece is "<<board.getPiece(temp)->owner()<< endl;//
       
      if (board.getPiece(temp) != NULL) {
	//Prompts::blocked();
	return MOVE_ERROR_BLOCKED;
      }
    }
  }

 if (dir == '\\') {
     for (int i = 1; i < (int) (tempStart.y) - (int) (tempEnd.y); i++ ) {
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

bool ChessGame::gameOver() const{
    return over;
}




// Make a move on the board. Return an int, with < 0 being failure
int ChessGame::makeMove(Position start, Position end){
  //cout<<"making move from " <<start.x<<","<<start.y<< " " <<end.x<<","<<end.y<<endl;
  
  //negative if out of bounds
  int retCode = Board::makeMove(start, end);
  int validMoveCode; //code from check if it's a valid move
  Position whitepawn;
  Position blackpawn;
  if (retCode==1) {
    retCode = m_pieces.at(index(start))->validMove(start, end, *this);
    //was valid move code
  }
  int checkedCode = 0; 
  Piece* captured = NULL;
  //if it's a valid move from gen board and piece move patterns
  if (retCode > 0) { 
    //was valid move code
     captured = m_pieces.at(index(end));
     Position ourKing = this->findKing(this->playerTurn());
     
     //cout<<"ourking is at " <<ourKing.x<<","<<ourKing.y<<endl;//
     //cout << "is checked position of ourKing " << this->isCheckedPosition(ourKing) << endl;
     int underCheck = this->isCheckedPosition(ourKing) == MOVE_CHECK;//
     
     //cout <<" is our king already under check?" << underCheck<<endl;//
     
     //if its already in check before moving
     if (this->isCheckedPosition(ourKing)== MOVE_CHECK) {
       // cout<<"must handle check" <<endl;//
       //actually move the pieces
       m_pieces.at(index(end)) = m_pieces.at(index(start));
       m_pieces.at(index(start)) = NULL;

       //if doesn't handle check, undo and return error
       if (this->isCheckedPosition(this->findKing(this->playerTurn()))== MOVE_CHECK) {
	 m_pieces.at(index(start)) = m_pieces.at(index(end));
	 m_pieces.at(index(end)) = captured;
	 return MOVE_ERROR_MUST_HANDLE_CHECK;
       }
     }
     else { //it wasn't in check before the move so keep going
       //actually move the pieces
       int control = 1;
       if ((m_pieces.at(index(start))->id())== PAWN_ENUM) {
	 if (((m_pieces.at(index(start))->owner())== WHITE) && (end.y == 7)) {
	   whitepawn.x = end.x;
	   whitepawn.y = 7;
	   m_pieces.at(index(end)) = NULL;
	   initPiece(QUEEN_ENUM, m_pieces.at(index(start))->owner(), whitepawn);
	   m_pieces.at(index(start)) = NULL;
	   control = 0;
	 }
	 else if (((m_pieces.at(index(start))->owner())== BLACK) && (end.y == 0)) {
	   blackpawn.x = end.x;
	   blackpawn.y = 0;
	   m_pieces.at(index(end)) = NULL;
	   initPiece(QUEEN_ENUM, m_pieces.at(index(start))->owner(), blackpawn);
	   m_pieces.at(index(start)) = NULL;
	   control = 0;
	 }
      }
       if (control) {
       m_pieces.at(index(end)) = m_pieces.at(index(start));
       m_pieces.at(index(start)) = NULL;
       }
     }
     ourKing= this->findKing(this->playerTurn());

      //determines if under check for the first time
     if (this->isCheckedPosition(ourKing) == MOVE_CHECK) {
       checkedCode = MOVE_ERROR_CANT_EXPOSE_CHECK;
       //Prompts::cantExposeCheck();
       m_pieces.at(index(start)) = m_pieces.at(index(end));
       m_pieces.at(index(end)) = captured;

       if(isUnderCheckMate(Player(!(this->playerTurn()))) == MOVE_CHECKMATE) {
	 cout << "checkmate" << endl;
	 over = 1;
	 return MOVE_CHECKMATE;
       }


       
       return checkedCode;
     } //end cases under check

      //det captured
     if (captured != NULL) {
       //Prompts::capture(m_pieces.at(index(end))->owner());
       retCode = MOVE_CAPTURE;
     }

      //come back and do when curr player checkmates someone else

     if(isUnderCheckMate(Player(!(this->playerTurn()))) == MOVE_CHECKMATE) {
	 cout << "stalemate" << endl;
	 over = 1;
	 return MOVE_STALEMATE;
       }
   

      return retCode;
  } //else was invalid already
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
    m_turn = lastTurn + 1;
    //cout << "lastTurn " << lastTurn << " mturn: " << m_turn << endl;
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
  output << m_turn-1 <<endl;

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
   //reset
  Terminal::colorFg(1, Terminal::WHITE);
}

int ChessGame::getSetUp() {
  int initialInput = 0;
  string line;
  string filename;
  do {
      Prompts::menu();
      std::getline(cin, line);
  if (!line.compare("1")) {  //opposite b/c compare returns 0 if same
      this->setupBoard();
      m_turn = 1;
      initialInput = 1;
      return SUCCESS;
  }
  else if (!line.compare("2")) {  //opposite b/c compare returns 0 if same
    initialInput = 1;
    Prompts::loadGame();
      std::getline(cin, line);
      std::stringstream lineReader(line);
      lineReader>> filename;
      return setUpSavedBoard(filename);
  }
 } while (!initialInput);
  return SUCCESS;
}



void ChessGame::run() {
  string line = "";
  string filename;
  int nonMoveInput = 0; //a controlling factor for getting player input
  int wantsBoard = 1; //default should be 0, change before submitting
  int moveCode = 0;
  int counter = 0;
  
  if (this->getSetUp() == LOAD_FAILURE) {
    return;
  }

  while (!(gameOver() || !line.compare("q") || moveCode == MOVE_STALEMATE)) { // while game is not over
   counter++;
   Position start;
   Position end;

    do { //while the current player needs to make a move
      char startx = 0;
      char starty = 'a';
      char endx = 0;
      char endy = 'a';
      moveCode = 0;
      nonMoveInput = 0; //a controlling factor for getting player input to make a move

      //prints current player prompt
      if ((m_turn%2)==1) {
        cout << endl;//spacing for now, remove later?
          Prompts::playerPrompt(WHITE, ((m_turn)/2)+1);
        }
        else {
          cout << endl;
        Prompts::playerPrompt(BLACK, ((m_turn)/2));
        }

        //prints board
        if (wantsBoard) {
        printBoard(); //will need to toggle default
        }

	//gets user input for movement or other, converts to lower
        std::getline(cin, line);
        //cout << "line above is " << line << endl;
	for (unsigned int i = 0; i < line.length(); i++) {
          line[i] = tolower(line[i]);
        }
        //cout << "line below is " << line << endl;


	 //handles non movement input
        if (!line.compare("q")) {  //opposite b/c compare returns 0 if same
          cout << "handle quit later" << endl;
          nonMoveInput = 1;
          break;
        }
        if (!line.compare("board")) { //toggles board
          wantsBoard = !wantsBoard;
          nonMoveInput = 1;
          if (wantsBoard) {
            printBoard();
          }
        }
        if (!line.compare("save")) {
          cout <<"handle save later" << endl; //
          Prompts::saveGame();
          std::getline(cin, line);
           std::stringstream lineReader(line);
           lineReader>> filename;
          this->saveBoard(filename); //not dynamic b/c in board
          nonMoveInput = 1;
        }
	 if (!line.compare("forfeit")) {
          cout << "check turn # later" <<endl; //
          Prompts::win(Player(m_turn%2), m_turn);
          //turn input might not be correct
          line = "q";
          nonMoveInput = 1;
          break;
        }
	 
        //handles movement input
        if (!nonMoveInput) {

        std::stringstream lineReader(line);
        //cout << "line is" << line << endl;//
        lineReader >> startx;
        lineReader >> starty;
        lineReader >> endx;
        lineReader >> endy;

        startx = tolower(startx);
        endx = tolower(endx);

        //converts to our positional coordinates
        start.x = startx-97;
        start.y = starty-49;
        end.x = endx-97;
        end.y = endy-49;

	if (!isalpha((char)startx) || !isalpha((char)endx) || !isdigit((char)starty) || !isdigit((char)endy)) {
            //incorrect moving input format
            Prompts::parseError();
            nonMoveInput = 1;
	}

	if (!nonMoveInput) {
	moveCode = makeMove(start,end);
	printMoveMessages(moveCode);
	}
        }

        //cout << startx << " " << starty <<endl;// error checking
        //cout << endx << " " << endy <<endl;//
        //      cout << makeMove(start,end) << endl;
        
    } while (!line.compare("q") || nonMoveInput || moveCode < 0);
//while curr player needs to make a move
    //if not quitting or hasn't inputed movement places or move was invalid

     m_turn++;
     //cout << "move code is " << moveCode << endl;
  }

 if (counter != 0) {
   m_turn--;
 }

 //cout << moveCode << endl;
 if ((line.compare("q") != 0) && (moveCode != MOVE_STALEMATE)) {
   Prompts::win(this->playerTurn(), (m_turn + 1) / 2);
 }

 
 Prompts::gameOver();
 //while the game is not over or they don't want to quit
}

void ChessGame::printMoveMessages(int code) {
  switch (code) {
  case MOVE_ERROR_OUT_OF_BOUNDS:
    Prompts::outOfBounds();
    return;
  case MOVE_ERROR_NO_PIECE:
    Prompts::noPiece();
    return;
  case MOVE_ERROR_BLOCKED:
    Prompts::blocked();
    return;
  case MOVE_ERROR_CANT_CASTLE:
    Prompts::cantCastle();
    return;
  case MOVE_ERROR_MUST_HANDLE_CHECK:
    Prompts::mustHandleCheck();
    return;
  case MOVE_ERROR_CANT_EXPOSE_CHECK:
    Prompts::cantExposeCheck();
    return;
  case MOVE_ERROR_ILLEGAL:
    Prompts::illegalMove();
    return;
  case MOVE_CHECK:
    Prompts::check(this->playerTurn());
    return;
  case MOVE_CAPTURE:
    Prompts::capture(this->playerTurn());
    return;
  case MOVE_CHECKMATE:
    Prompts::checkMate(this->playerTurn());
    return;
  case MOVE_STALEMATE:
    Prompts::staleMate();
    return;
  case GAME_WIN:
    Prompts::win(this->playerTurn(),this->turn());
    return;
  case GAME_OVER:
    Prompts::gameOver();
    return;
  default:
    return;
  } 
}

