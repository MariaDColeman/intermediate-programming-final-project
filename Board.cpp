#include <assert.h>
#include <cctype>
#include <iostream>
#include <string>
#include <sstream>


#include "Game.h"
#include "Prompts.h"

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;

///////////////
// Board //
///////////////

Board::~Board() {
    // Delete all pointer-based resources
    for (unsigned int i=0; i < m_width * m_height; i++)
        delete m_pieces[i];
    for (size_t i=0; i < m_registeredFactories.size(); i++)
        delete m_registeredFactories[i];
}

// Get the Piece at a specific Position, or nullptr if there is no
// Piece there or if out of bounds.
Piece* Board::getPiece(Position position) const {
    if (validPosition(position))
        return m_pieces[index(position)];
    else {
        Prompts::outOfBounds();
        return nullptr;
    }
}

// Create a piece on the board using the factory.
// Returns true if the piece was successfully placed on the board
bool Board::initPiece(int id, Player owner, Position position) {
    Piece* piece = newPiece(id, owner);
    if (!piece) return false;

    // Fail if the position is out of bounds
    if (!validPosition(position)) {
        Prompts::outOfBounds();
        return false;
    }
    // Fail if the position is occupied
    if (getPiece(position)) {
        Prompts::blocked();
        return false;
    }
    m_pieces[index(position)] = piece;
    return true;
}

// Add a factory to the Board to enable producing
// a certain type of piece
bool Board::addFactory(AbstractPieceFactory* pGen) {
    // Temporary piece to get the ID
    Piece* p = pGen->newPiece(WHITE);
    int id = p->id();
    delete p;

    PieceGenMap::iterator it = m_registeredFactories.find(id);
    if (it == m_registeredFactories.end()) { // not found
        m_registeredFactories[id] = pGen;
        return true;
    } else {
        std::cout << "Id " << id << " already has a generator\n";
        return false;
    }
}

// Search the factories to find a factory that can translate `id' to
// a Piece, and use it to create the Piece. Returns nullptr if not found.
Piece* Board::newPiece(int id, Player owner) {
    PieceGenMap::iterator it = m_registeredFactories.find(id);
    if (it == m_registeredFactories.end()) { // not found
        std::cout << "Id " << id << " has no generator\n";
        return nullptr;
    } else {
        return it->second->newPiece(owner);
    }
}

int Board::makeMove(Position start, Position end) {
  
  if (!(validPosition(start) && validPosition(end))) {
    Prompts::outOfBounds();
    return -7;
  }
  else if ((m_pieces.at(index(start)) == NULL) ||(m_pieces.at(index(start))->owner() != playerTurn())) {
    Prompts::noPiece();
    return -6;
    }
  else {
    return 1;
  }
}

void Board::run() {
  int initialInput;
  string filename;
  
  do {
      Prompts::menu();
      cin >> initialInput;
  if (initialInput == 1) {
      this->setupBoard();
      m_turn = 1;
  }
  else if (initialInput == 2) {
      Prompts::loadGame();
      cin >> filename;
      
      setUpSavedBoard(filename);
  }
  } while ((initialInput != 1)&&(initialInput !=2));


 string line;
 std::getline(cin, line);
	
  do {
    
  
  //for (int round = 0; round < 10; round++) {  //allows for 5 rounds
    Position start;
    Position end;
    //int currPlayer;
    //int correctPlayer;
      char startx;
      char starty;
      char endx;
      char endy;
    
    do {
        if ((m_turn%2)==1) {
	  cout << endl;
	  Prompts::playerPrompt(WHITE, ((m_turn)/2)+1);
	}
        else {
	  cout << endl;
	Prompts::playerPrompt(BLACK, ((m_turn)/2));
	}

        printBoard(); //will need to toggle

        std::getline(cin, line);
	//need to convert to lower
	if (!line.compare("q")) {  //opposite b/c compare returns 0 if same
	  cout << "handle quit later" << endl;
	  break;
	}
	if (!line.compare("board")) {
	  cout << "handle board later" << endl;
	  std::getline(cin, line); //get move input
	}
        if (!line.compare("save")) {
	  cout <<"handle save later" << endl;
	  std::getline(cin, line); //get move input
	}
        if (!line.compare("forfeit")) {
	  cout << "check turn # later" <<endl;
	  Prompts::win(Player(m_turn%2), m_turn); //turn input might not be correct
	  line = "q";
	  break;
	}
	
	std::stringstream lineReader(line);

	
        lineReader >> startx;
        lineReader >> starty;
        lineReader >> endx;
        lineReader >> endy;

	startx = tolower(startx);
        endx = tolower(endx);
	//Position start(startx - 97, starty - 49);
        //Position end(endx - 97, endy - 49);
	start.x = startx-97;
	start.y = starty-49;
	end.x = endx-97;
	end.y = endy-49;
	
	//        cout << start.x << " " << start.y <<endl;// error checking
	//        cout << end.x << " " << end.y <<endl;//

	//cout << makeMove(start,end) << endl;

    } while (!line.compare("q") || makeMove(start, end) < 0);
   
  
        m_turn++;
  } while (!(gameOver() || !line.compare("q")));


}
