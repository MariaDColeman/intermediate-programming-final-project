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
using std::isdigit;
using std::isalpha;

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
	
 do { // while game is not over
    Position start;
    Position end;
      char startx;
      char starty;
      char endx;
      char endy;
      int nonMoveInput = 0; //a controlling factor for getting player input
      int tryingToMove = 0; //a controlling factor for parse error
      int wantsBoard = 1; //should be 0;
    
    do { //white the current player needs to make a move
      //prints current player prompt
        if ((m_turn%2)==1) {
	  cout << endl;
	  Prompts::playerPrompt(WHITE, ((m_turn)/2)+1);
	}
        else {
	  cout << endl;
	Prompts::playerPrompt(BLACK, ((m_turn)/2));
	}

	//prints board
	if (wantsBoard) {
        printBoard(); //will need to toggle
	}

	//gets user input for movement or other, converts to lower
        std::getline(cin, line);
     	for (unsigned int i = 0; i < line.length(); i++) {
	  line[i] = tolower(line[i]);
	}
	
	//handles non movement input
	if (!line.compare("q")) {  //opposite b/c compare returns 0 if same
	  cout << "handle quit later" << endl;
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
	  cout <<"handle save later" << endl;
	  nonMoveInput = 1;
	}
        if (!line.compare("forfeit")) {
	  cout << "check turn # later" <<endl;
	  Prompts::win(Player(m_turn%2), m_turn);
	  //turn input might not be correct
	  line = "q";
	  break;
	}

	//handles movement input
	if (!nonMoveInput) {
	  tryingToMove = 1;
	  //if correct movement input format, try to make the move
	  if (isalpha(line[0]) && isalpha(line[3]) && isdigit(line[1]) && isdigit(line[4])) {

	    //std::stringstream lineReader(line);
	    /*lineReader >> startx;
        lineReader >> starty;
        lineReader >> endx;
        lineReader >> endy;
	    */
	startx = tolower(line[0]);
        endx = tolower(line[3]);
	starty = line[1];
	endy = line[4];

	//converts to our positional coordinates
	start.x = startx-97;
	start.y = starty-49;
	end.x = endx-97;
	end.y = endy-49;
	  } else { //incorrect moving input format
	    Prompts::parseError();
	    nonMoveInput = 1;
	  }
	//        cout << start.x << " " << start.y <<endl;// error checking
	//        cout << end.x << " " << end.y <<endl;//

	//	cout << makeMove(start,end) << endl;
	} else if (!tryingToMove) { //incorrect nonmove input format
	    Prompts::parseError();
	    nonMoveInput = 1;
        }
    } while (!line.compare("q") || nonMoveInput || makeMove(start, end) < 0);
    //while curr player needs to make a move
    //if not quitting or hasn't inputed movement places or move was invalid
   
  
     m_turn++;
  } while (!(gameOver() || !line.compare("q")));
 //while the game is not over or they don't want to quit


}
