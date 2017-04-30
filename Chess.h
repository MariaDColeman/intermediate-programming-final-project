#ifndef CHESS_H
#define CHESS_H

#include "Game.h"
#include "Prompts.h"
#include <cmath>
#include <string>

using std::cout;
using std::endl;
using std::abs;
using std::string;

// Game status codes
// -----------------
// These enumerations are optional. You can choose to use them,
// or you can decide they're not needed. They *could* be used, for
// example, to return values from validMove() and makeMove(), and
// any other methods you want. But so long as you follow the conventions
// of those methods (>0 is success, <0 is failure), you're free to
// do things your own way.
enum status {
    LOAD_FAILURE = -10,
    SAVE_FAILURE,
    PARSE_ERROR,
    MOVE_ERROR_OUT_OF_BOUNDS,
    MOVE_ERROR_NO_PIECE,
    MOVE_ERROR_BLOCKED,
    MOVE_ERROR_CANT_CASTLE,
    MOVE_ERROR_MUST_HANDLE_CHECK,
    MOVE_ERROR_CANT_EXPOSE_CHECK,
    MOVE_ERROR_ILLEGAL,
    SUCCESS = 1,
    MOVE_CHECK,
    MOVE_CAPTURE,
    MOVE_CHECKMATE,
    MOVE_STALEMATE,
    GAME_WIN,
    GAME_OVER
};

// Possible pieces
enum PieceEnum {
    PAWN_ENUM = 0,
    ROOK_ENUM,
    KNIGHT_ENUM,
    BISHOP_ENUM,
    QUEEN_ENUM,
    KING_ENUM
};

class ChessPiece : public Piece {
 protected:
 ChessPiece(Player owner, int id) : Piece(owner, id) {}

 public:
  //peopleInWay() ? function that goes throough desired path to see if anything is in the path . for all excep knight
  //returns a 2 if no people are in the way. returns a -2 if there are people in the way
  //virtual int noPeopleInWay(Position start, Position end, const Board& board); 
 int properAloneMove(Position start, Position end) const;
 int noPeopleInWay(Position start, Position end, const Board& board) const;
 int isCheckedPosition(Position position, const Board& board) const;
 virtual int validMove(Position start, Position end, const Board& board) const override {
    //properDirection() ? //if empty board would it be allowed just moving itself. rewritten in every type of ChessPiece
    //getDirection()
    //noPeopleInWay()?
    //if whats caling this is a pawn, call get direction() if its white them get direction has to be / with +1 space or \ with -1 space and reversed if black (s0 -1 space and +1 space) then return SUCCESS

    Position position;
    int check = isCheckedPosition(position, board);

    
    if(board.getPiece(start)->id() == PAWN_ENUM) {
      char tempDir = getDirection(start,end);
      char tempSpaces = getSpaces(start,end);

      if (board.getPiece(end) != NULL) {
	if (board.getPiece(end)->owner() == board.getPiece(start)->owner() ) {
	  // Prompts::blocked();
	  return MOVE_ERROR_BLOCKED;
      }

      if (board.getPiece(start)->owner() == WHITE) {
	if ((tempDir == '/') && (tempSpaces == 1)) {
	  return SUCCESS;
	}
	if ((tempDir == '\\') && (tempSpaces == -1)) {
	  return SUCCESS;
	}
	
	else {
	  // Prompts::illegalMove();
	  return MOVE_ERROR_ILLEGAL;
	}
	
      }
      if (board.getPiece(start)->owner() == BLACK) {
	if ((tempDir == '/') && (tempSpaces == -1)) {
	  return SUCCESS;
	}
	if ((tempDir == '\\') && (tempSpaces == 1)) {
	  return SUCCESS;
	}
	else {
	  //Prompts::illegalMove();
	  return MOVE_ERROR_ILLEGAL;
	}
      }
    }
    
    }
    
    if (properAloneMove(start,end) >= 0) {
      cout << "is proper alone move, but is it no people in way?" <<endl;//
      return noPeopleInWay(start, end, board); //actually if noPeopleInWay() >= 0
      //personInDestination() 
      //if m_pieces.at(position end) != NULL
      // remove m_pieces.at(end)
    }
    else {
      return MOVE_ERROR_ILLEGAL;
    }
  }

  char getDirection(Position start, Position end) const;
  int getSpaces(Position start, Position end) const;
  
  virtual int properDirection(char) const {return 0;}
  virtual int properSpaces(Position, Position) const {return 0;}

  
};

class Pawn : public ChessPiece {
protected:
    friend PieceFactory<Pawn>;
    Pawn(Player owner, int id) : ChessPiece(owner, id) {}
public:
    // This method will have piece-specific logic for checking valid moves
    // It may also call the generic Piece::validMove for common logic
    int validMove(Position start, Position end,
        const Board& board) const override {
      return ChessPiece::validMove(start, end, board);
      //return SUCCESS;
    }
    virtual int properDirection(char dir) const override {
      //   cout << "pawn checking proper direction" << endl;
      return (dir == 'V');
    }
    virtual int properSpaces(Position start, Position end) const override{
      //MAKE SURE TO COME BACK AND DEAL WITH FIRST MOVE

      if ((m_owner == BLACK) && (start.y == 6)) {
	return ((getSpaces(start, end) == -2) || (getSpaces(start,end) == -1));
      }
      else if ((m_owner == WHITE) && (start.y == 1)) {
	return ((getSpaces(start, end) == 2) || (getSpaces(start,end) == 1));
      }
      
      if (m_owner == BLACK) {
	return (getSpaces(start, end) == -1);
      } else {
	return (getSpaces(start, end) == 1);
      }
    }
};

class Rook : public ChessPiece {
protected:
    friend PieceFactory<Rook>;
    Rook(Player owner, int id) : ChessPiece(owner, id) {}
public:
    // This method will have piece-specific logic for checking valid moves
    // It may also call the generic Piece::validMove for common logic
    int validMove(Position start, Position end,
		  const Board& board) const override {return ChessPiece::validMove(start, end, board); }
    virtual int properDirection(char dir) const override {
      return ((dir == 'V')||(dir == 'H'));
    }
    virtual int properSpaces(Position, Position) const override{
      return 1;
    }
};


class Knight : public ChessPiece {
  //remember to return a 2 for noPeopleInWay() since it doesn't matter
protected:
    friend PieceFactory<Knight>;
    Knight(Player owner, int id) : ChessPiece(owner, id) {}
public:
    // This method will have piece-specific logic for checking valid moves
    // It may also call the generic Piece::validMove for common logic
    int validMove(Position start, Position end,
		  const Board& board) const override { return ChessPiece::validMove(start, end, board); }
    virtual int properDirection(char dir) const override {
      cout << dir << endl;
      return (dir == 'L');
    }
    virtual int properSpaces(Position start, Position end) const override{
      return (getSpaces(start,end) == 5);
    }
};


class Bishop : public ChessPiece {
protected:
    friend PieceFactory<Bishop>;
    Bishop(Player owner, int id) : ChessPiece(owner, id) {}
public:
    // This method will have piece-specific logic for checking valid moves
    // It may also call the generic Piece::validMove for common logic
    int validMove(Position start, Position end,
        const Board& board) const override { return ChessPiece::validMove(start, end, board); }
    virtual int properDirection(char dir) const override {
      return ((dir == '/')||(dir == '\\'));
    }
    virtual int properSpaces(Position, Position) const override{
      return 1;
    }
};


class Queen : public ChessPiece {
protected:
    friend PieceFactory<Queen>;
    Queen(Player owner, int id) : ChessPiece(owner , id) {}
public:
    // This method will have piece-specific logic for checking valid moves
    // It may also call the generic Piece::validMove for common logic
    int validMove(Position start, Position end,
        const Board& board) const override {return ChessPiece::validMove(start, end, board); }
    virtual int properDirection(char dir) const override {
      return (dir != 'L' && dir != '0');
    }
    virtual int properSpaces(Position, Position) const override{
      return 1;
    }
};


class King : public ChessPiece {
protected:
    friend PieceFactory<King>;
    King(Player owner, int id) : ChessPiece(owner, id) {}
public:
    // This method will have piece-specific logic for checking valid moves
    // It may also call the generic Piece::validMove for common logic
    int validMove(Position start, Position end,
        const Board& board) const override {return ChessPiece::validMove(start, end, board); }
    virtual int properDirection(char dir) const override {
      return ((dir != 'L')&&(dir != '0'));
    }
    virtual int properSpaces(Position start, Position end) const override{
      return (getSpaces(start, end) == 1);
    }
};

class ChessGame : public Board {
public:
    ChessGame() : Board(8, 8) {
        // Add all factories needed to create Piece subclasses
        addFactory(new PieceFactory<Pawn>(PAWN_ENUM));
        addFactory(new PieceFactory<Rook>(ROOK_ENUM));
        addFactory(new PieceFactory<Knight>(KNIGHT_ENUM));
        addFactory(new PieceFactory<Bishop>(BISHOP_ENUM));
        addFactory(new PieceFactory<Queen>(QUEEN_ENUM));
        addFactory(new PieceFactory<King>(KING_ENUM));
    }

    // Setup the chess board with its initial pieces
    virtual void setupBoard() override;


    virtual int setUpSavedBoard(string filename) override; 
    
    // Whether the chess game is over
    virtual bool gameOver() const override { return false; }

    // Perform a move from the start Position to the end Position
    // The method returns an integer with the status
    // >= 0 is SUCCESS, < 0 is failure
    virtual int makeMove(Position start, Position end) override;

    //print the chess game board
    //virtual void printBoard() override;

    //virtual int run() override;
};

#endif
