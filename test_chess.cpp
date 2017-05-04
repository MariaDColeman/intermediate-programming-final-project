#include <cassert>
#include "Chess.h"
#include "Game.h"


/*
test_chess

*/

class test_chess {

public:
  ChessGame c1;
  //Pawn *  p = new Pawn(Player(WHITE), 0);
  
 
  test_chess() {
    c1.setupBoard();
    c1.m_turn = 1;
  }

  /*  ChessGame* before(){
    cout<<"before"<<endl;
    ChessGame * c2 = new ChessGame();
    //c2.setupBoard();
    //c1 =c2;
    c2->setupBoard();
    c2->m_turn = 1;
    return c2;
    }*/

  void newGameIsNotOver() {
    assert(!c1.gameOver());
    cout<<"passed newGameIsNotOver"<<endl<<endl;
  }
  
  void invalidDiagonalURPawn() {
    Position start(1,1);
    Position end(3,2);
    int i = c1.makeMove(start, end);
    assert(i == MOVE_ERROR_ILLEGAL);
    c1.printMoveMessages(i);
    cout<<"passed invalidDiagonalURPawn"<<endl<<endl;
  }

  
  void invalidDiagonalULPawn() {
    Position start(1,1);
    Position end(0,2);
    int i = c1.makeMove(start, end);
    assert(i == MOVE_ERROR_ILLEGAL);
    c1.printMoveMessages(i);
    cout<<"passed invalidDiagonalULPawn\n"<<endl;
  }

  
  void invalidDownPawn() {
    Position start(1,1);
    Position end(0,1);
    int i = c1.makeMove(start, end);
    assert(i < 0);
    c1.printMoveMessages(i);
    cout<<"passed invalidDownPawn\n"<<endl;
  }

  void invalidNoPiece(){
    Position start(0,6);
    Position end(0,5);
    int i = c1.makeMove(start, end);
    assert(i < 0);
    cout<<"passed invalidNoPiece\n"<<endl;
  }

  void offBoard(){
    Position start(-1,-1);
    Position end(9,9);
    int i = c1.makeMove(start, end);
    assert(i < 0);
    cout<<"passed offBoard\n"<<endl;
  }

  
  
 

};

int main() {
  test_chess t;
  
  t.newGameIsNotOver();
  t.invalidDiagonalURPawn();
  t.invalidDiagonalULPawn();
  t.invalidDownPawn();
  t.invalidNoPiece();
  t.offBoard();
  cout<<"about to return"<<endl;
  return 0;
}
