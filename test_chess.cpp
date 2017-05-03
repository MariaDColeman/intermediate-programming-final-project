#include <cassert>
#include "Chess.h"
#include "Game.h"


/*
test_chess

*/

class test_chess {

public:
  ChessGame c1;
 
  test_chess() {
    cout<<"new"<<endl;
    //c1 = ChessGame();
    //cout<<"c1"<<endl;
    c1.setupBoard();
    cout<<"setuo"<<endl;
    c1.m_turn = 1;
    cout<<"turn"<< endl;
  }

  void newGameIsNotOver() {
    //cout<<c1.gameOver()<<endl;
    assert(!c1.gameOver());
    cout<<"passed newGameIsNotOver"<<endl;
  }

  void validPawn() {
    Position start(0,1);
    Position end(0,2);
    int i = c1.makeMove(start, end);
    cout<<"passed" <<i<<endl;
  }
 

};

int main() {
  test_chess t;
  t.newGameIsNotOver();
  t.validPawn();
  
  cout<<"about to return"<<endl;
  return 0;
}
