#include <cassert>
#include "Chess.h"
#include "Terminal.h"



/*
test_chess

*/

class test_chess {
  ChessGame new;

public:
  test_chess () {
    new = ChessGame();
    new.setupBoard();
    m_turn=1;
  }




  

};

int main() {
  test_chess t;


  
  return 0;
}
