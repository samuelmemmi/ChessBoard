#define TRUE 1
#define FALSE 0

#include "ChessBoard.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

/***********************
* Memmi Samuel
* 342677358
* Group 01 - CS
* Ass4
***********************/

// PGN characters
const char PAWN = 'P';
const char ROOK = 'R';
const char KNIGHT = 'N';
const char BISHOP = 'B';
const char QUEEN = 'Q';
const char KING = 'K';
const char CAPTURE = 'x';
const char PROMOTION = '=';
const char CHECK = '+';
const char MATE = '#';
const char FIRST_COL = 'a';

// FEN & Board characters
const char WHITE_PAWN = 'P';
const char WHITE_ROOK = 'R';
const char WHITE_KNIGHT = 'N';
const char WHITE_BISHOP = 'B';
const char WHITE_QUEEN = 'Q';
const char WHITE_KING = 'K';
const char BLACK_PAWN = 'p';
const char BLACK_ROOK = 'r';
const char BLACK_KNIGHT = 'n';
const char BLACK_BISHOP = 'b';
const char BLACK_QUEEN = 'q';
const char BLACK_KING = 'k';

// FEN separator for strtok()
const char SEP = '/';

// Board characters
const char EMPTY = ' ';

// Move logical representation
typedef struct {
	char srcPiece, srcRow, srcCol, destPiece, destRow, destCol, piecePromotion;
	int iSrc, jSrc, iDest, jDest;
	int isWhite, isCapture, isPromotion, isCheck, isMate, isLegal;
} Move;
// char = jeu d echec comment on voit , int = dans notre tableau , isrc dou je vien , destrow e, destcol 4



/**************************
 * convert char to digit
 **************************/
int toDigit(char c) {
	if (!('0' <= c && c <= '9'))
		printf("%c\n", c);
	assert('0' <= c && c <= '9');
	return c - '0';
}

void createBoard(char board[][SIZE], char fen[])
{
	int i = 0, row = 0, col = 0;
	int lenString = strlen(fen);

	for (i; i < lenString; i++) //we repeat this are we are not yet arrived to the end of the fen
	{
		/*if we arrived to the separator we jump into the next line of the array 2D
		and we are in the first column*/
		if (fen[i] == SEP)
		{
			row++;
			col = 0;

		}
		//if in the array fen we have a digit we want to convert this number of empty spaces
		else if (isdigit(fen[i]))
		{
			int spaces = toDigit(fen[i]);
			for (int j = 0; j < spaces; j++)
			{
				board[row][col] = EMPTY;
				col++;
			}
		}
		/*if there is no digit and we are not in the separator yet we want to put every caracter
		in the array fen to the board 2D */
		else
		{
			board[row][col] = fen[i];
			col++;
		}
	}
}


/*here we want to print a line with *| to the beginning and letter of the number of the size
after and finish with the same thing |**/
void printColumns() {
	char column = toupper(FIRST_COL);
	printf("* |");
	for (int i = 0; i < SIZE; i++) {
		if (i) {
			printf(" ");
		}
		printf("%c", column);
		column++;
	}
	printf("| *\n");
}

void printSpacers() { //here we want to print a line with * and - SIZE times and *
	printf("* -");
	for (int i = 0; i < SIZE; i++) {
		printf("--");
	}
	printf(" *\n");
}

void printBoard(char board[][SIZE])//here we want to print the board that we receive in the createBoard
{
	printColumns();
	printSpacers();

	for (int i = 0; i < SIZE; i++)
	{
		printf("%d ", SIZE - i);

		for (int j = 0; j < SIZE; j++)
			printf("|%c", board[i][j]);


		printf("| %d\n", SIZE - i);

	}
	printSpacers();
	printColumns();
}

// Transforme la lettre en minuscule
char letterToLower(char c)
{
	if (c >= 'A' && c <= 'Z')
		c += 'a' - 'A';

	return c;
}

/*here we do every step to read every pgn that we receive, and to put every information
on the struct Move*/
void initMove(char pgn[], Move* my_move, int isWhiteTurn, char board[][SIZE])
{
	//we initialize all the value at the begining
	int i = 0;
	int j = 0;
	my_move->srcPiece = 'P';
	my_move->isWhite = isWhiteTurn;
	my_move->isCapture = FALSE;
	my_move->isPromotion = FALSE;
	my_move->isCheck = FALSE;
	my_move->isMate = FALSE;
	my_move->iSrc = -1;
	my_move->jSrc = -1;
	my_move->piecePromotion = FALSE;
	my_move->isLegal = FALSE;
	my_move->destPiece = FALSE;
	my_move->srcRow = FALSE;
	my_move->srcCol = FALSE;
	my_move->destCol = -2;
	my_move->destRow = -2;

	for (j; j < SIZE; j++)
	{
		/*here we put the first letter that we receive in the srcPiece and we progress of one if this is one
		of this letter, else the letter is P*/
		if ((j == 0) && (pgn[i] == 'K' ||
			pgn[i] == 'Q' ||
			pgn[i] == 'B' ||
			pgn[i] == 'N' ||
			pgn[i] == 'R'))
		{
			my_move->srcPiece = pgn[i];
			i++;
		}

		//so here if we are the second place and this is a letter [a-h] we put in to the destcol and progress 
		else if ((j == 1) && ('a' <= pgn[i] && pgn[i] <= 'h'))
		{
			my_move->destCol = pgn[i];
			//our board don't know what is the value of a, for him a is the first column
			my_move->jDest = my_move->destCol - 'a';
			i++;

		}
		//so here if we are the third place and this is a number [1-8] we put in to the destrow and progress
		else if ((j == 2) && ('1' <= pgn[i] && pgn[i] <= '8'))
		{

			my_move->destRow = pgn[i];
			/*our line of the board begin at 0 and the line of the pgn begin at 8 so we have to make our
			board understand the difference*/
			my_move->iDest = SIZE - toDigit(my_move->destRow);
			i++;

		}
		else if ((j == 3) && (pgn[i] == 'x'))//here if we have 'x' we want to capture
		{
			my_move->isCapture = TRUE;
			i++;
		}
		/*if we are in the fourth place and we have letter[a-h] we put into the srccol
		but if also already give us the destcol we must change it into what now in the pgn
		and the srccol become what we receive in the destcol first*/
		else if ((j == 4) && (('a' <= pgn[i]) && (pgn[i] <= 'h')))
		{
			if (my_move->destCol != -2)
			{
				my_move->srcCol = my_move->destCol;
				my_move->destCol = pgn[i];
				//our board don't know what is the value of a, for him a is the first column
				my_move->jSrc = (my_move->srcCol) - 'a';
				my_move->jDest = my_move->destCol - 'a';
				i++;
			}
			else
			{
				my_move->destCol = pgn[i];
				my_move->jDest = my_move->destCol - 'a';
				i++;
			}

		}
		/*if we are in the fifth place and we have number[1-8] we put into the srcrow
	but if also already give us the destrow we must change it into what now in the pgn
	and the srcrow become what we receive in the destrow first*/
		else if ((j == 5) && (('1' <= pgn[i]) && (pgn[i] <= '8')))
		{
			if (my_move->destRow != -2)
			{
				my_move->srcRow = my_move->destRow;
				my_move->destRow = pgn[i];
				my_move->iSrc = SIZE - toDigit(my_move->srcRow);
				my_move->iDest = SIZE - toDigit(my_move->destRow);
				i++;
			}
			else
			{
				my_move->destRow = pgn[i];
				my_move->iDest = SIZE - toDigit(my_move->destRow);
				i++;
			}

		}
		/*if give us the caracter '=' that mean that the pawn arrive in the last line
		and have to change into something else that he want but not a king*/
		else if ((j == 6) && (pgn[i] == '='))
		{
			i++;
			if ((pgn[i] == 'N'))
			{
				my_move->isPromotion = TRUE;
				my_move->piecePromotion = pgn[i];
				i++;
			}
			else if ((pgn[i] == 'Q'))
			{
				my_move->isPromotion = TRUE;
				my_move->piecePromotion = pgn[i];
				i++;
			}
			else if ((pgn[i] == 'R'))
			{
				my_move->isPromotion = TRUE;
				my_move->piecePromotion = pgn[i];
				i++;
			}
			else if ((pgn[i] == 'B'))
			{
				my_move->isPromotion = TRUE;
				my_move->piecePromotion = pgn[i];
				i++;
			}
		}
		else if ((j == 7) && (pgn[i] == '+'))//this caracter mean that we are in check
			my_move->isCheck = TRUE;
		else if ((j == 7) && pgn[i] == '#')
			my_move->isMate = TRUE;
	}
	my_move->destPiece = board[my_move->iDest][my_move->jDest];

	if (isWhiteTurn == 0)//if we are in the black turn we change the letter to a small one
	{
		my_move->srcPiece = letterToLower(my_move->srcPiece);
		my_move->piecePromotion = letterToLower(my_move->piecePromotion);
	}

	if ((my_move->iSrc != -1) && (my_move->jSrc != -1))
		return;
	//here we have just the source of the row so we want to find the source of the column
	else if (my_move->iSrc != -1)
	{
		for (int j = 0; j < SIZE; j++)
		{
			my_move->jSrc = j;
			if ((board[my_move->iSrc][j] == my_move->srcPiece) &&
				(isMoveOfPiece(board, my_move->iSrc, my_move->iDest, j, my_move->jDest, *my_move) == 1))
			{
				return;
			}

		}
	}
	//here we have just the source of the column so we want to find the source of the row
	else if (my_move->jSrc != -1)
	{
		for (int i = 0; i < SIZE; i++)
		{
			my_move->iSrc = i;
			if ((board[i][my_move->jSrc] == my_move->srcPiece) &&
				(isMoveOfPiece(board, i, my_move->iDest, my_move->jSrc, my_move->jDest, *my_move) == 1))
			{
				return;
			}

		}
	}
	else //we don't have any source and we have to find both of them
	{
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++)
			{
				my_move->iSrc = i;
				my_move->jSrc = j;

				if ((board[i][j] == my_move->srcPiece) &&
					(isMoveOfPiece(board, i, my_move->iDest, j, my_move->jDest, *my_move) == 1))
				{
					return;
				}

			}
	}

}

//the letter is upper so white?
int isUpperCase(char c)
{
	int value = FALSE;
	if (c >= 65 && c <= 90)
		value = TRUE;
	return value;
}
int isLowerCase(char c)//the letter is lower so black?
{
	int value = FALSE;
	if (c >= 97 && c <= 122)
		value = TRUE;
	return value;
}
int isSameCase(char c1, char c2) //we compare two letter to know if they are in the same size or not
{
	int value = FALSE;
	if ((isLowerCase(c1) == TRUE && isUpperCase(c2) == TRUE) ||
		(isLowerCase(c2) == TRUE && isUpperCase(c1) == TRUE))
		value = TRUE;
	return value;
}
//we want to make sure that the row is free and we haven't someone on our path
int isRowFree(char board[][SIZE], int iSrc, int jSrc, int iDest, int jDest)
{
	int temp = 1;
	for (int i = iSrc < iDest ? iSrc + 1 : iSrc - 1; iSrc < iDest ? i<iDest : i>iDest;
		iSrc < iDest ? i++ : i--)
	{
		if (board[i][jDest] != EMPTY)
		{
			temp = 0;
			break;
		}
		else
			temp = 1;

	}
	return temp;
}
//we want to make sure that the column is free and we haven't someone on our path
int isColumnFree(char board[][SIZE], int iSrc, int jSrc, int iDest, int jDest)
{
	int temp = 1;
	for (int j = jSrc < jDest ? jSrc + 1 : jSrc - 1; jSrc < jDest ? j<jDest : j>jDest;
		jSrc < jDest ? j++ : j--)
	{
		if (board[iDest][j] != EMPTY)
		{
			temp = 0;
			break;
		}
		else
			temp = 1;
	}
	return temp;

}

int MovePawn(int iSrc, int iDest, int jSrc, int jDest, char board[][SIZE], Move* my_move)
{
	int value = TRUE;

	if (board[iSrc][jSrc] == 'P')//this is the pawn white
	{
		/*so if the pawn arrive in the first line of the board and he want to do promotion
		and also his mouvement is good that mean that he is on the same column
		and he advance of one line*/
		if ((iDest == 0) && (my_move->isPromotion == TRUE) && (jSrc == jDest) && (iSrc - 1 == iDest))
		{

			my_move->srcPiece = my_move->piecePromotion;
			value = TRUE;
		}
		/*if he is in the same column,he don't want to capture he has two options:
		he can decline one row or if he is the penultimate place he can if we want to decline
		of two line*/
		else if ((jSrc == jDest) && (my_move->isCapture == FALSE))
		{
			if (iSrc - 1 == iDest)
				value = TRUE;
			else if ((iSrc - 2 == iDest) && (iSrc == SIZE - 2) && isRowFree(board, iSrc, jSrc, iDest, jDest))
				value = TRUE;
			else
				value = FALSE;
		}
		/*if he want to capture and the arrival case isn't the same and also he move of one line but
		he step back of one and one column of which side of the column*/

		else if ((iSrc - 1 == iDest) && (jSrc + 1 == jDest)
			&& (isSameCase(my_move->srcPiece, my_move->destPiece) == TRUE) && (my_move->isCapture == TRUE)) {

			value = TRUE;
		}
		else if ((iSrc - 1 == iDest) && (jSrc - 1 == jDest)
			&& (isSameCase(my_move->srcPiece, my_move->destPiece) == TRUE) && (my_move->isCapture == TRUE)) {

			value = TRUE;
		}
		else
			value = FALSE;
	}
	/*the black pawn has the same rule of the white but in the reverse direction that mean that
	he have to arrive in the penultimate place to promotion,he progress and not decline of one or two row
	and he capture the same way but progress not decline*/
	if (board[iSrc][jSrc] == 'p')
	{
		if ((iDest == SIZE - 1) && (my_move->isPromotion == TRUE) && (jSrc == jDest) && (iSrc + 1 == iDest))
		{

			my_move->srcPiece = my_move->piecePromotion;
			value = TRUE;
		}
		else if ((jSrc == jDest) && (my_move->isCapture == FALSE))
		{
			if (iSrc + 1 == iDest)
				value = TRUE;

			else if ((iSrc + 2 == iDest) && (iSrc == 1) && isRowFree(board, iSrc, jSrc, iDest, jDest))
				value = TRUE;
			else
				value = FALSE;
		}
		else if ((iSrc + 1 == iDest) && (jSrc + 1 == jDest)
			&& (isSameCase(my_move->srcPiece, my_move->destPiece) == TRUE) && (my_move->isCapture == TRUE))
		{

			value = TRUE;
		}
		else if ((jSrc - 1 == jDest) && (iSrc + 1 == iDest) &&
			(isSameCase(my_move->srcPiece, my_move->destPiece) == TRUE) && (my_move->isCapture == TRUE))
			value = TRUE;
		else
			value = FALSE;
	}
	return value;
}

/*the knight have 8 options he can progress or step back of two lines and progress
	or step back of one column , or he can step back or progress of one line
	and step back or progress of two columns*/
int MoveKnight(int iSrc, int iDest, int jSrc, int jDest, char board[][SIZE])
{
	int value = TRUE;
	if ((board[iSrc][jSrc] == 'N') || (board[iSrc][jSrc] == 'n'))
	{
		if (iSrc == iDest && jSrc == jDest)
			value = FALSE;
		else if ((iSrc + 2 == iDest) && (jSrc + 1 == jDest))
			value = TRUE;
		else if ((iSrc + 2 == iDest) && (jSrc - 1 == jDest))
			value = TRUE;
		else if ((iSrc - 2 == iDest) && (jSrc + 1 == jDest))
			value = TRUE;
		else if ((iSrc - 2 == iDest) && (jSrc - 1 == jDest))
			value = TRUE;
		else if ((jSrc + 2 == jDest) && (iSrc + 1 == iDest))
			value = TRUE;
		else if ((jSrc - 2 == jDest) && (iSrc + 1 == iDest))
			value = TRUE;
		else if ((jSrc + 2 == jDest) && (iSrc - 1 == iDest))
			value = TRUE;
		else if ((jSrc - 2 == jDest) && (iSrc - 1 == iDest))
			value = TRUE;
		else
			value = FALSE;
	}
	return value;
}


int sign(int n)
{
	if (n >= 0)
		return 1;
	else
		return -1;
}


int MoveRook(int iSrc, int iDest, int jSrc, int jDest, char board[][SIZE])
{
	int value = TRUE;
	if ((board[iSrc][jSrc] == 'R') || (board[iSrc][jSrc] == 'r')
		|| (board[iSrc][jSrc] == 'Q') || (board[iSrc][jSrc] == 'q'))
	{
		// the rook move horizontale or vertiacle so he can't have the same row or column
		if (iSrc == iDest && jSrc == jDest)
			value = FALSE;
		/*if the row are the same that mean that he move verticale,so he have to know the column is free
		 or he can move ad there is someone in his path*/
		else if (iSrc == iDest)
		{
			if (isColumnFree(board, iSrc, jSrc, iDest, jDest) == 1)
				value = TRUE;
			else if (jSrc + 1 == jDest)
				value = TRUE;
			else if (jSrc - 1 == jDest)
				value = TRUE;
			else
				value = FALSE;
		}
		/*if the column are the same that mean that he move horizontale,so he have to know the row is free
		 or he can move ad there is someone in his path*/
		else if (jSrc == jDest)
		{
			if (isRowFree(board, iSrc, jSrc, iDest, jDest) == 1)//haut et bas
				value = TRUE;
			else if (iSrc + 1 == iDest)
				value = TRUE;
			else if (iSrc - 1 == iDest)
				value = TRUE;
			else
				value = FALSE;
		}
		else
			value = FALSE;

	}
	return value;
}

/*the bishop move in diagonale on every side he want so we have to make sure that the diagonale is free
so he can go to is destination or if he want he can just move of one in diagonale*/
int MoveBishop(int iSrc, int iDest, int jSrc, int jDest, char board[][SIZE])
{
	int value = TRUE;
	int iDirection = 0;
	int jDirection = 0;
	int x = 0, y = 0;

	if ((board[iSrc][jSrc] == 'B') || (board[iSrc][jSrc] == 'b')
		|| (board[iSrc][jSrc] == 'Q') || (board[iSrc][jSrc] == 'q'))
	{
		if ((iSrc == iDest) && (jSrc == jDest))//so he can't be in the same row or column in the destination
			value = FALSE;
		else if ((iSrc != iDest) && (jDest != jSrc) && (abs(iSrc - iDest) == abs(jSrc - jDest)))
		{
			/*here we want to know if the diagonale is free and for that we progress of one every time
			and we have to know the sign of the movement*/
			iDirection = sign(iDest - iSrc);
			jDirection = sign(jDest - jSrc);
			x = jSrc + jDirection;
			y = iSrc + iDirection;
			while ((x != jDest) && (y != iDest))
			{
				if (board[y][x] != EMPTY)
					return 0;
				x += jDirection;
				y += iDirection;
			}
			value = TRUE;
		}
		else if ((iSrc - 1 == iDest) && (jSrc - 1 == jDest))
			value = TRUE;
		else if ((iSrc - 1 == iDest) && (jSrc + 1 == jDest))
			value = TRUE;
		else if ((iSrc + 1 == iDest) && (jSrc - 1 == jDest))
			value = TRUE;
		else if ((iSrc + 1 == iDest) && (jSrc + 1 == jDest))
			value = TRUE;
		else
			value = FALSE;
	}
	return value;
}
/*the queen can move to every place that mean verticale,horizontale or diagonale and
for that we check the move of the rook or the move of the bishop*/
int MoveQueen(int iSrc, int iDest, int jSrc, int jDest, char board[][SIZE])
{
	int value = TRUE;
	if (MoveBishop(iSrc, iDest, jSrc, jDest, board) == TRUE)
		value = TRUE;
	else if (MoveRook(iSrc, iDest, jSrc, jDest, board) == TRUE)
		value = TRUE;
	else
		value = FALSE;
	return value;
}
//the king can move to every direction but progress or decline of just one
int MoveKing(int iSrc, int iDest, int jSrc, int jDest, char board[][SIZE])
{
	int value = TRUE;
	if ((board[iSrc][jSrc] == 'K') || (board[iSrc][jSrc] == 'k'))
	{
		if (iSrc == iDest && jSrc == jDest)
			value = FALSE;
		else if ((iSrc + 1 == iDest) && (jSrc == jDest))
			value = TRUE;
		else if ((iSrc - 1 == iDest) && (jSrc == jDest))
			value = TRUE;
		else if ((iSrc == iDest) && (jSrc + 1 == jDest))
			value = TRUE;
		else if ((iSrc == iDest) && (jSrc - 1 == jDest))
			value = TRUE;
		else if ((iSrc - 1 == iDest) && (jSrc + 1 == jDest))
			value = TRUE;
		else if ((iSrc - 1 == iDest) && (jSrc - 1 == jDest))
			value = TRUE;
		else if ((iSrc + 1 == iDest) && (jSrc + 1 == jDest))
			value = TRUE;
		else if ((iSrc + 1 == iDest) && (jSrc - 1 == jDest))
			value = TRUE;
		else
			value = FALSE;
	}
	return value;
}


//here we check every case of the piece black or white
int isMoveOfPiece(char board[][SIZE], int iSrc, int iDest, int jSrc, int jDest, Move my_move) {
	int value = 0;
	switch (my_move.srcPiece)
	{
	case 'P':value = MovePawn(iSrc, iDest, jSrc, jDest, board, &my_move); break;
	case 'p':value = MovePawn(iSrc, iDest, jSrc, jDest, board, &my_move); break;
	case 'K':value = MoveKing(iSrc, iDest, jSrc, jDest, board); break;
	case 'k':value = MoveKing(iSrc, iDest, jSrc, jDest, board); break;
	case 'Q':value = MoveQueen(iSrc, iDest, jSrc, jDest, board); break;
	case 'q':value = MoveQueen(iSrc, iDest, jSrc, jDest, board); break;
	case 'B':value = MoveBishop(iSrc, iDest, jSrc, jDest, board); break;
	case 'b':value = MoveBishop(iSrc, iDest, jSrc, jDest, board); break;
	case 'N':value = MoveKnight(iSrc, iDest, jSrc, jDest, board); break;
	case 'n':value = MoveKnight(iSrc, iDest, jSrc, jDest, board); break;
	case 'R':value = MoveRook(iSrc, iDest, jSrc, jDest, board); break;
	case 'r':value = MoveRook(iSrc, iDest, jSrc, jDest, board); break;
	}

	if (value == 0)
		return 0;
	else
		return 1;
}

//here we want to find the position of the king in the board 
void findKing(char board[][SIZE], int array[2], int isWhiteTurn)
{
	int line = 0;
	int column = 0;
	for (int ld = 0; ld <= SIZE; ld++)
		for (int cd = 0; cd <= SIZE; cd++)
		{
			if (isWhiteTurn != TRUE)
			{
				if (board[ld][cd] == 'K')
				{
					line = ld;
					column = cd;
					break;
				}
			}
			else if (isWhiteTurn != FALSE)
			{
				if (board[ld][cd] == 'k')
				{
					line = ld;
					column = cd;
					break;
				}
			}
		}
	array[0] = line;
	array[1] = column;
}

int isCheck(char board[][SIZE], int iDest, int jDest, Move my_move, int isWhiteTurn)
{
	/*here we want to know if we can move our piece and not put the king in position that
	we can eat him because its check and its illegal,so for that we use his position to know if we can move */
	Move newMove;
	newMove.srcPiece = my_move.srcPiece;
	newMove.iSrc = my_move.iDest;
	newMove.jSrc = my_move.jDest;
	newMove.isWhite = my_move.isWhite;
	int kingLocation[2];
	findKing(board, kingLocation, newMove.isWhite);
	newMove.iDest = kingLocation[0];
	newMove.jDest = kingLocation[1];
	if (!isMoveOfPiece(board, newMove.iSrc, newMove.iDest, newMove.jSrc, newMove.jDest, newMove))
	{
		return 0;
	}
	else
		return 1;
}

//here we want to check if the move is legal
int isLegalMove(char board[][SIZE], char pgn[], Move* my_move, int isWhiteTurn) {

	//if he is not in the board this is not legal
	if (my_move->jDest > SIZE || my_move->iDest > SIZE ||
		my_move->jDest < 0 || my_move->iDest < 0)
		return 0;
	//we check if he can move
	if (!isMoveOfPiece(board, my_move->iSrc, my_move->iDest, my_move->jSrc, my_move->jDest, *my_move)) {
		return 0;
	}
	//we verify if my_move is check is true and also if he can move and not be in check
	if (my_move->isCheck == 1)
	{
		if (!isCheck(board, my_move->iDest, my_move->jDest, *my_move, isWhiteTurn))
			return 0;
	}
	//we check if the capture is true so verify if we can go there and the letter is not the same
	if (my_move->isCapture == 1)
	{
		if (!MovePawn(my_move->iSrc, my_move->iDest, my_move->jSrc, my_move->jDest, board, my_move))
			return 0;
		else if (isMoveOfPiece(board, my_move->iSrc, my_move->iDest, my_move->jSrc, my_move->jDest, *my_move)
			&& (isSameCase(my_move->srcPiece, my_move->destPiece) == FALSE))
			return 0;
	}
	//we check ispromotion with the condition we put in the pawn
	if (my_move->isPromotion == 1)
	{
		if (!MovePawn(my_move->iSrc, my_move->iDest, my_move->jSrc, my_move->jDest, board, my_move))
			return 0;
	}
	if (my_move->isMate == 1)
	{
		if (!isCheck(board, my_move->iDest, my_move->jDest, *my_move, isWhiteTurn))
			return 0;
	}
	return 1;
}


int makeMove(char board[][SIZE], char pgn[], int isWhiteTurn)
{
	Move my_move;
	initMove(pgn, &my_move, isWhiteTurn, board);
	int legal = isLegalMove(board, pgn, &my_move, isWhiteTurn);
	//we check all the condition and if the move is legal we do it on the board
	if (legal == 1) {
		board[my_move.iSrc][my_move.jSrc] = EMPTY;
		board[my_move.iDest][my_move.jDest] = my_move.srcPiece;
		return 1;
	}
	//else we print the move is illegal
	else {
		return 0;
	}
}