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
    c1 = ChessGame();
    cout<<"c1"<<endl;
    c1.setupBoard();
    cout<<"setuo"<<endl;
    c1.m_turn = 1;
    cout<<"turn"<<endl;
  }

  void newGameIsNotOver() {
    assert(!c1.gameOver());
  }

 

};

int main() {
  cout<<"begining"<<endl;
  test_chess t;
  t.newGameIsNotOver();
  
  return 0;
}
