#include <cassert>
#include "Chess.h"
#include "Game.h"


/*
test_chess
runs as ./unittest

*/

//positions related to 3,3 for reuse in validmove checking
//to avoid compilation errors, adrianna told me to leave them as global variables
Position dur(4,4); //diagonal up right
Position dul(2,4); //diagonal up left
Position ddr(4,2); //diagonal down right
Position ddl(2,2); //diagonal down left

Position dur3(6,6); //diagonal up right
Position dul3(0,6); //diagonal up left

Position u(3,4); //up 1
Position u2(3,5); //up 2
Position d(3,2); //down 1
Position r(4,3); //right 1
Position l(2,3); //left 1

Position u3(3,6); //up 3
Position r3(6,3); //right 3
Position l3(0,3); //left 3

Position Lur(4,5); //knight up right
Position Lul(2,5); //knight up left
Position Lsur(5,4); //knight side up right
Position Lsdr(5,2); //knight side down right
Position Lsul(1,4); //knight side up left
Position Lsdl(1,2); //knight side down left
Position Ldr(4,1); //knight down right
Position Ldl(2,1); //knight down left


class test_chess {

public:
    ChessGame c;

    test_chess() {
        c.setupBoard();
        c.m_turn = 1;
    }

    void clearBoard() {
        for(int i = 0; i < (int)c.m_width* (int)c.m_height; i++) {
            delete(c.m_pieces.at(i));
            c.m_pieces.at(i) = NULL;
        }
    }

    void newGameIsNotOver() {
        assert(!c.gameOver());
        cout<<"passed newGameIsNotOver"<<endl<<endl;
    }


    void invalidNoPiece() {
        clearBoard();
        Position start(0,6);
        Position end(0,5);
        int i;
        i = c.makeMove(start, end);
        assert(i < 0);
        cout<<"passed invalidNoPiece\n"<<endl;
    }

    void offBoard() {
        Position istart(-1,-1);
        Position iend(9,9);
        Position valid(4,4);

        int i = c.makeMove(istart, valid);
        assert(i < 0);

        i = c.makeMove(valid, iend);
        assert(i < 0);
        cout<<"passed offBoard\n"<<endl;
    }

    void BlockedPiece() {
        clearBoard();
        Position start(3,1);
        Position end(4,2);
        int i;
        c.initPiece(3, WHITE, start);
        c.initPiece(3, WHITE, end);
        //white bishop at d2, e3
        i = c.getPiece(start)->validMove(start, end, c);
        assert(i<0);
        cout<<"passed BlockedPiece\n"<<endl;
    }

    void validMovesCapturePawn() {
        clearBoard();
        Position start(3,1);
        Position capture(4,2);
        int i;
        c.initPiece(0, WHITE, start);
        c.initPiece(0, BLACK, capture);
        //white pawn at d2, black pawn at e3
        i = c.getPiece(start)->validMove(start, capture, c);
        assert(i>0);
        cout<<"passed validMovesCapturePawn\n"<<endl;
    }

    void validMovesCaptureUsingAnythingElse() {
        clearBoard();
        Position start(3,1);
        Position capture(4,2);
        int i;
        c.initPiece(5, WHITE, start);
        c.initPiece(0, BLACK, capture);
        //white king at d2, black pawn at e3
        i = c.getPiece(start)->validMove(start, capture, c);
        assert(i>0);
        cout<<"passed validMovesCaptureUsingAnythingElse\n"<<endl;
    }

    void validMovesLonelyWhiteRook() {
        clearBoard();
        Position start(3,1);
        c.initPiece(1, WHITE, start);
        //white Rook only at d2
        validMovesLonelyRook();
        cout<<"passed validMovesLonelyWhiteRook\n"<<endl;
    }

    void validMovesLonelyWhiteKnight() {
        clearBoard();
        Position start(3,3);
        c.initPiece(2, WHITE, start);
        //white knight only at d4
        validMovesLonelyKnight();
        //tests valid and invalid movement patterns
        cout<<"passed validMovesLonelyWhiteKnight\n"<<endl;
    }

    void validMovesLonelyWhiteBishop() {
        clearBoard();
        Position start(3,3);
        c.initPiece(3, WHITE, start);
        //white bishop only at d4
        validMovesLonelyBishop();
        //tests valid and invalid movement patterns
        cout<<"passed validMovesLonelyWhiteBishop\n"<<endl;
    }

    void validMovesLonelyWhiteQueen() {
        clearBoard();
        Position start(3,3);
        c.initPiece(4, WHITE, start);
        //white queen only at d4
        validMovesLonelyQueen();
        //tests valid and invalid movement patterns
        cout<<"passed validMovesLonelyWhiteQueen\n"<<endl;
    }

    void validMovesLonelyWhiteKing() {
        clearBoard();
        Position start(3,3);
        c.initPiece(5, WHITE, start);
        //white king only at d4
        validMovesLonelyKing();
        //tests valid and invalid movement patterns
        cout<<"passed validMovesLonelyWhiteKing\n"<<endl;
    }


    void validMovesLonelyBlackRook() {
        clearBoard();
        Position start(3,1);
        c.initPiece(1, BLACK, start);
        //black Rook only at d4
        validMovesLonelyRook();
        //tests valid and invalid movement patterns
        cout<<"passed validMovesLonelyBlackRook\n"<<endl;
    }

    void validMovesLonelyBlackKnight() {
        clearBoard();
        Position start(3,3);
        c.initPiece(2, BLACK, start);
        //black knight only at d4
        validMovesLonelyKnight();
        //tests valid and invalid movement patterns
        cout<<"passed validMovesLonelyBlackKnight\n"<<endl;
    }

    void validMovesLonelyBlackBishop() {
        clearBoard();
        Position start(3,3);
        c.initPiece(3, BLACK, start);
        //black bishop only at d4
        validMovesLonelyBishop();
        //tests valid and invalid movement patterns
        cout<<"passed validMovesLonelyBlackBishop\n"<<endl;
    }

    void validMovesLonelyBlackQueen() {
        clearBoard();
        Position start(3,3);
        c.initPiece(4, BLACK, start);
        //black queen only at d4
        validMovesLonelyQueen();
        //tests valid and invalid movement patterns
        cout<<"passed validMovesLonelyBlackQueen\n"<<endl;
    }

    void validMovesLonelyBlackKing() {
        clearBoard();
        Position start(3,3);
        c.initPiece(5, BLACK, start);
        //Black king only at d4
        validMovesLonelyKing();
        //tests valid and invalid movement patterns
        cout<<"passed validMovesLonelyBlackKing\n"<<endl;
    }

    void validMovesLonelyWhitePawn() {
        clearBoard();
        Position start(3,1);
        c.initPiece(0, WHITE, start);
        //white pawn only at d2
        Position dur(4,2); //diagonal up right
        Position dul(2,2); //diagonal up left
        Position ddr(4,0); //diagonal down right
        Position ddl(2,0); //diagonal down left

        Position u(3,2); //up 1
        Position u2(3,3); //up 2
        Position d(3,0); //down 1
        Position r(4,1); //right 1
        Position l(2,1); //left 1
        int i;

        //invalid diagonal up right
        i = c.getPiece(start)->validMove(start, dur, c);
        assert(i < 0);

        //invalid diagonal up left
        i = c.getPiece(start)->validMove(start, dul, c);
        assert(i < 0);

        //invalid diagonal down right
        i = c.getPiece(start)->validMove(start, ddr, c);
        assert(i < 0);

        //invalid diagonal down left
        i = c.getPiece(start)->validMove(start, ddl, c);
        assert(i < 0);

        //invalid down
        i = c.getPiece(start)->validMove(start, d, c);
        assert(i < 0);

        //invalid right
        i = c.getPiece(start)->validMove(start, r, c);
        assert(i < 0);

        //invalid left
        i = c.getPiece(start)->validMove(start, l, c);
        assert(i < 0);

        //valid up
        i = c.getPiece(start)->validMove(start, u, c);
        assert(i > 0);

        //valid up2
        i = c.getPiece(start)->validMove(start, u2, c);
        assert(i > 0);

        cout<<"passed validMovesLonelyWhitePawn"<<endl<<endl;
    }

    void validMovesLonelyRook() {
        //white rook only at d2
        Position start(3,1);
        Position dur(4,2); //diagonal up right
        Position dul(2,2); //diagonal up left
        Position ddr(4,0); //diagonal down right
        Position ddl(2,0); //diagonal down left

        Position dur3(6,4); //diagonal up right
        Position dul3(0,4); //diagonal up left

        Position u(3,2); //up 1
        Position u2(3,3); //up 2
        Position d(3,0); //down 1
        Position r(4,1); //right 1
        Position l(2,1); //left 1

        Position u3(3,4); //up 3
        Position r3(6,1); //right 3
        Position l3(0,1); //left 3
        int i;

        //invalid diagonal up right
        i = c.getPiece(start)->validMove(start, dur, c);
        assert(i < 0);

        //invalid diagonal up left
        i = c.getPiece(start)->validMove(start, dul, c);
        assert(i < 0);

        //invalid diagonal down right
        i = c.getPiece(start)->validMove(start, ddr, c);
        assert(i < 0);

        //invalid diagonal down left
        i = c.getPiece(start)->validMove(start, ddl, c);
        assert(i < 0);

        //invalid diagonal up right
        i = c.getPiece(start)->validMove(start, dur3, c);
        assert(i < 0);

        //invalid diagonal up left
        i = c.getPiece(start)->validMove(start, dul3, c);
        assert(i < 0);

        //valid down
        i = c.getPiece(start)->validMove(start, d, c);
        assert(i > 0);

        //valid right
        i = c.getPiece(start)->validMove(start, r, c);
        assert(i > 0);

        //valid left
        i = c.getPiece(start)->validMove(start, l, c);
        assert(i > 0);

        //valid up
        i = c.getPiece(start)->validMove(start, u, c);
        assert(i > 0);

        //valid up2
        i = c.getPiece(start)->validMove(start, u2, c);
        assert(i > 0);

        //valid right 3
        i = c.getPiece(start)->validMove(start, r3, c);
        assert(i > 0);

        //valid left 3
        i = c.getPiece(start)->validMove(start, l3, c);
        assert(i > 0);

        //valid up3
        i = c.getPiece(start)->validMove(start, u3, c);
        assert(i > 0);
    }



    void validMovesLonelyKnight() {
        Position start(3,3);
        int i;

        //valid knight shaped moves
        i = c.getPiece(start)->validMove(start, Lur, c);
        assert(i > 0);
        i = c.getPiece(start)->validMove(start, Lul, c);
        assert(i > 0);
        i = c.getPiece(start)->validMove(start, Lsur, c);
        assert(i > 0);
        i = c.getPiece(start)->validMove(start, Lsdr, c);
        assert(i > 0);
        i = c.getPiece(start)->validMove(start, Lsul, c);
        assert(i > 0);
        i = c.getPiece(start)->validMove(start, Lsdl, c);
        assert(i > 0);
        i = c.getPiece(start)->validMove(start, Ldr, c);
        assert(i > 0);
        i = c.getPiece(start)->validMove(start, Ldl, c);
        assert(i > 0);

        //invalid diagonal up right
        i = c.getPiece(start)->validMove(start, dur, c);
        assert(i < 0);

        //invalid diagonal up left
        i = c.getPiece(start)->validMove(start, dul, c);
        assert(i < 0);

        //invalid diagonal down right
        i = c.getPiece(start)->validMove(start, ddr, c);
        assert(i < 0);

        //invalid diagonal down left
        i = c.getPiece(start)->validMove(start, ddl, c);
        assert(i < 0);


        //invalid diagonal up right
        i = c.getPiece(start)->validMove(start, dur3, c);
        assert(i < 0);

        //invalid diagonal up left
        i = c.getPiece(start)->validMove(start, dul3, c);
        assert(i < 0);

        //invalid down
        i = c.getPiece(start)->validMove(start, d, c);
        assert(i < 0);

        //invalid right
        i = c.getPiece(start)->validMove(start, r, c);
        assert(i < 0);

        //invalid left
        i = c.getPiece(start)->validMove(start, l, c);
        assert(i < 0);

        //invalid up
        i = c.getPiece(start)->validMove(start, u, c);
        assert(i < 0);

        //invalid up2
        i = c.getPiece(start)->validMove(start, u2, c);
        assert(i < 0);

        //invalid right 3
        i = c.getPiece(start)->validMove(start, r3, c);
        assert(i < 0);

        //invalid left 3
        i = c.getPiece(start)->validMove(start, l3, c);
        assert(i < 0);

        //invalid up3
        i = c.getPiece(start)->validMove(start, u3, c);
        assert(i < 0);
    }


    void validMovesLonelyBishop() {
        Position start(3,3);
        int i;

        //invalid knight shaped moves
        i = c.getPiece(start)->validMove(start, Lur, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Lul, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Lsur, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Lsdr, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Lsul, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Lsdl, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Ldr, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Ldl, c);
        assert(i < 0);

        //valid diagonal up right
        i = c.getPiece(start)->validMove(start, dur, c);
        assert(i > 0);

        //valid diagonal up left
        i = c.getPiece(start)->validMove(start, dul, c);
        assert(i > 0);

        //valid diagonal down right
        i = c.getPiece(start)->validMove(start, ddr, c);
        assert(i > 0);

        //valid diagonal down left
        i = c.getPiece(start)->validMove(start, ddl, c);
        assert(i > 0);


        //valid diagonal up right
        i = c.getPiece(start)->validMove(start, dur3, c);
        assert(i > 0);

        //valid diagonal up left
        i = c.getPiece(start)->validMove(start, dul3, c);
        assert(i > 0);

        //invalid down
        i = c.getPiece(start)->validMove(start, d, c);
        assert(i < 0);

        //invalid right
        i = c.getPiece(start)->validMove(start, r, c);
        assert(i < 0);

        //invalid left
        i = c.getPiece(start)->validMove(start, l, c);
        assert(i < 0);

        //invalid up
        i = c.getPiece(start)->validMove(start, u, c);
        assert(i < 0);

        //invalid up2
        i = c.getPiece(start)->validMove(start, u2, c);
        assert(i < 0);

        //invalid right 3
        i = c.getPiece(start)->validMove(start, r3, c);
        assert(i < 0);

        //invalid left 3
        i = c.getPiece(start)->validMove(start, l3, c);
        assert(i < 0);

        //invalid up3
        i = c.getPiece(start)->validMove(start, u3, c);
        assert(i < 0);
    }

    void validMovesLonelyQueen() {
        Position start(3,3);
        int i;

        //invalid knight shaped moves
        i = c.getPiece(start)->validMove(start, Lur, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Lul, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Lsur, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Lsdr, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Lsul, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Lsdl, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Ldr, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Ldl, c);
        assert(i < 0);

        //valid diagonal up right
        i = c.getPiece(start)->validMove(start, dur, c);
        assert(i > 0);

        //valid diagonal up left
        i = c.getPiece(start)->validMove(start, dul, c);
        assert(i > 0);

        //valid diagonal down right
        i = c.getPiece(start)->validMove(start, ddr, c);
        assert(i > 0);

        //valid diagonal down left
        i = c.getPiece(start)->validMove(start, ddl, c);
        assert(i > 0);


        //valid diagonal up right
        i = c.getPiece(start)->validMove(start, dur3, c);
        assert(i > 0);

        //valid diagonal up left
        i = c.getPiece(start)->validMove(start, dul3, c);
        assert(i > 0);


        //valid down
        i = c.getPiece(start)->validMove(start, d, c);
        assert(i > 0);

        //valid right
        i = c.getPiece(start)->validMove(start, r, c);
        assert(i > 0);

        //valid left
        i = c.getPiece(start)->validMove(start, l, c);
        assert(i > 0);

        //valid up
        i = c.getPiece(start)->validMove(start, u, c);
        assert(i > 0);

        //valid up2
        i = c.getPiece(start)->validMove(start, u2, c);
        assert(i > 0);

        //valid right 3
        i = c.getPiece(start)->validMove(start, r3, c);
        assert(i > 0);

        //valid left 3
        i = c.getPiece(start)->validMove(start, l3, c);
        assert(i > 0);

        //valid up3
        i = c.getPiece(start)->validMove(start, u3, c);
        assert(i > 0);
    }


    void validMovesLonelyKing() {
        Position start(3,3);
        int i;

        //invalid knight shaped moves
        i = c.getPiece(start)->validMove(start, Lur, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Lul, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Lsur, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Lsdr, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Lsul, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Lsdl, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Ldr, c);
        assert(i < 0);
        i = c.getPiece(start)->validMove(start, Ldl, c);
        assert(i < 0);

        //valid diagonal up right
        i = c.getPiece(start)->validMove(start, dur, c);
        assert(i > 0);

        //valid diagonal up left
        i = c.getPiece(start)->validMove(start, dul, c);
        assert(i > 0);

        //valid diagonal down right
        i = c.getPiece(start)->validMove(start, ddr, c);
        assert(i > 0);

        //valid diagonal down left
        i = c.getPiece(start)->validMove(start, ddl, c);
        assert(i > 0);


        //invalid diagonal up right3
        i = c.getPiece(start)->validMove(start, dur3, c);
        assert(i < 0);

        //invalid diagonal up left3
        i = c.getPiece(start)->validMove(start, dul3, c);
        assert(i < 0);


        //valid down
        i = c.getPiece(start)->validMove(start, d, c);
        assert(i > 0);

        //valid right
        i = c.getPiece(start)->validMove(start, r, c);
        assert(i > 0);

        //valid left
        i = c.getPiece(start)->validMove(start, l, c);
        assert(i > 0);

        //valid up
        i = c.getPiece(start)->validMove(start, u, c);
        assert(i > 0);

        //invalid up2
        i = c.getPiece(start)->validMove(start, u2, c);
        assert(i < 0);

        //invalid right 3
        i = c.getPiece(start)->validMove(start, r3, c);
        assert(i < 0);

        //invalid left 3
        i = c.getPiece(start)->validMove(start, l3, c);
        assert(i < 0);

        //invalid up3
        i = c.getPiece(start)->validMove(start, u3, c);
        assert(i < 0);
    }



};

int main() {
    test_chess t;

    t.newGameIsNotOver();
    t.invalidNoPiece();
    t.offBoard();
    t.BlockedPiece();


    t.validMovesCapturePawn();
    t.validMovesCaptureUsingAnythingElse();

    t.validMovesLonelyWhitePawn();
    t.validMovesLonelyWhiteRook();
    t.validMovesLonelyWhiteKnight();
    t.validMovesLonelyWhiteBishop();
    t.validMovesLonelyWhiteQueen();
    t.validMovesLonelyWhiteKing();
    t.validMovesLonelyBlackRook();
    t.validMovesLonelyBlackKnight();
    t.validMovesLonelyBlackBishop();
    t.validMovesLonelyBlackQueen();
    t.validMovesLonelyBlackKing();

    cout<<"passed all unit tests!"<<endl;
    return 0;
}
