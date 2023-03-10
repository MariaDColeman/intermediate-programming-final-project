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
 int properAloneMove(Position start, Position end) const;
 int noPeopleInWay(Position start, Position end, const Board& board) const override;
 virtual int validMove(Position start, Position end, const Board& board) const override;
  char getDirection(Position start, Position end) const override;
  int getSpaces(Position start, Position end) const override;
  int hasMoved = 0;
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

    virtual int properDirection(char dir) const override {
      return (dir == 'V');
    }
    virtual int properSpaces(Position start, Position end) const override;
 
    int validMove(Position start, Position end,
		  const Board& board) const override;

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
       return (abs(getSpaces(start, end)) == 1);
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

    int getSetUp();
    Position findKing(Player pl);
    Position findRook(Player pl, int skipFirstRookCode);
    int isCheckedPosition(Position king);
    int isUnderCheckMate(Player pl); 
    //int inStalemate(Player pl);
    virtual int setUpSavedBoard(string filename) override; 
    //virtual int saveBoard(string filename) override;    
    // Whether the chess game is over
    virtual bool gameOver() const override;

    int isCastling(Position start, Position end);
    // Perform a move from the start Position to the end Position
    // The method returns an integer with the status
    // >= 0 is SUCCESS, < 0 is failure
    virtual int makeMove(Position start, Position end) override;

    //print the chess game board
    //virtual void printBoard() override;

    virtual void printMoveMessages(int code) override;
    virtual void run() override;

    friend class test_chess;
 protected:
    int over = 0;
};

#endif
