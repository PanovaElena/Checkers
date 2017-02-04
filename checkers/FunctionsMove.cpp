#include "FunctionsMove.h"

Cache cache(100);
Board board;

int color; //� ���������� ��������� � Generate
int coord;
int num;
int coordEaten;
int eaten[12]; int nEaten = 0;
int type_bool;

//��������������� ������� - �������
inline int OnBoard(int cell)
{
	return (cell >> 6 == 0);
}

inline int IsBlack(int cell)
{
	return (((cell >> 3) ^ cell) & 1);
}

inline int Inside(int cell)
{
	return (OnBoard(cell) && IsBlack(cell));
}

inline int OnLastRow(int cell) 
{
	return !(Inside(cell + forwardRight[color]) || Inside(cell + forwardLeft[color]));
}

//���������� ���������������� ����������� � ���������
inline int Perp(int route) //7<->9, -7<->-9, �.�. ������ ��������� �������������� �����������
{
	return (route ^ (7<<1));
}
//����� �� ��������� �� ������ � �����������
inline int CanMove(int _coord)	
{
	return (Inside(_coord) && board.IsEmpty(_coord));
}

//����� �� ���� � �������� �����������
inline int CanEatChecker(int route)	
{
	return (Inside(coord + 2 * route) && !board.IsEmpty(coord+route)&&board.IsEmpty(coord+2*route)&&(board[coord+route]->GetColor()!=color));
}

int temp_i;
inline int CanEatDamka(int route) //��������� ���������� �����, ������� ����� ������
{
	for (temp_i = 1; CanMove(coord + temp_i*route); temp_i++);
	if (!Inside(coord + temp_i*route) || (board[coord + temp_i*route]->GetColor() == color)) return 0;
	if (CanMove(coord + (temp_i + 1)*route)) {
		coordEaten= coord + temp_i*route;
		return 1;
	}
	return 0;
}

//������ ����
Move temp_move;
inline Move GetMove(int finalCoord) { //������ ��� ������� �����
	temp_move.SetColor(color);
	temp_move.SetNum(num);
	temp_move.SetCoord(finalCoord);
	temp_move.SetNEaten(nEaten);
	temp_move.SetEaten(eaten);
	temp_move.SetType(type_bool);
	return temp_move;
}


int SearchMoveChecker(Checker *ch)
{
	color = ch->GetColor(); //� ���������� �������� � Generate
	coord = ch->GetCoord(); //�� ���� ���������
	num = ch->GetNum();//� ���������� �������� � Generate
	if (CanEatChecker(backRight[color])) return 1;
	if (CanEatChecker(backLeft[color])) return 1;
	if (CanEatChecker(forwardLeft[color])) return 1;
	if (CanEatChecker(forwardRight[color])) return 1;

	if (CanMove(coord+forwardRight[color])) { 
		if (OnLastRow(coord + forwardRight[color])) type_bool = 1;
		cache.Push(GetMove(coord + forwardRight[color]));
		type_bool = 0;
	}
	if (CanMove(coord + forwardLeft[color])) {
		if (OnLastRow(coord + forwardLeft[color])) type_bool = 1;
		cache.Push(GetMove(coord + forwardLeft[color]));
		type_bool = 0;
	}

	return 0;
}

int SearchMoveDamka(Checker *ch)
{
	int i;
	color = ch->GetColor(); //� ���������� �������� � Generate
	coord = ch->GetCoord(); //�� ���� ���������
	num = ch->GetNum();//� ���������� �������� � Generate
	if (CanEatDamka(backRight[color])) return 1;
	if (CanEatDamka(backLeft[color])) return 1;
	if (CanEatDamka(forwardLeft[color])) return 1;
	if (CanEatDamka(forwardRight[color])) return 1;

	for (i = 1; CanMove(coord + i*backRight[color]); i++) cache.Push(GetMove(coord + i*backRight[color]));
	for (i = 1; CanMove(coord + i*backLeft[color]); i++) cache.Push(GetMove(coord + i*backLeft[color]));
	for (i = 1; CanMove(coord + i*forwardLeft[color]); i++) cache.Push(GetMove(coord + i*forwardLeft[color]));
	for (i = 1; CanMove(coord + i*forwardRight[color]); i++) cache.Push(GetMove(coord + i*forwardRight[color]));
	return 0;
}

void SearchEatDamkaInRay(int);

inline void MakeMoveChecker(int route) {
	eaten[nEaten++] = board[coord + route]->GetNum();
	board[coord + route]->ChangeColor();
	coord +=2 * route;
	if (OnLastRow(coord)) type_bool = 1;
}
inline void UnMakeMoveChecker(int route){
	coord -= 2 * route;
	board[coord + route]->ChangeColor();
	nEaten--;
	type_bool = 0;
}
void SearchEatCheckerInRay(int route)
{
	int f = 0;

	MakeMoveChecker(route);//������ �� ��������� ������

	if (type_bool) {
		if (CanEatDamka(Perp(route))) {f++; SearchEatDamkaInRay(Perp(route));}
	} //���������� ����� � ���������� ����������� �� ������ �����
	else {
		if (CanEatChecker(route)) { f++; SearchEatCheckerInRay(route); }
		if (CanEatChecker(Perp(route))) { f++; SearchEatCheckerInRay(Perp(route)); }
		if (CanEatChecker(-Perp(route))) { f++; SearchEatCheckerInRay(-Perp(route)); }
	}

	//���������� ���a, ���� ������ ������ ����
	if (f == 0) cache.Push(GetMove(coord));

	UnMakeMoveChecker(route);
}

inline void MakeMoveDamka(int& savedCoord, int& savedCoordEaten) {
	savedCoord = coord; savedCoordEaten = coordEaten;
	eaten[nEaten++] = board[coordEaten]->GetNum();
	coord = coordEaten;
	board[coordEaten]->ChangeColor();
}
inline void UnMakeMoveDamka(int savedCoord, int savedCoordEaten) {
	board[savedCoordEaten]->ChangeColor();
	coord =savedCoord;
	nEaten--;
}
void SearchEatDamkaInRay(int route)
{
	int f = 0, savedCoord, savedCoordEaten;

	MakeMoveDamka(savedCoord, savedCoordEaten);//������ �� ��������� �����

	//���� ������ � ������� �����-������
	do {
		coord += route;
		if (CanEatDamka(Perp(route))) { f++; SearchEatDamkaInRay(Perp(route)); }
		if (CanEatDamka(-Perp(route))) { f++; SearchEatDamkaInRay(-Perp(route)); }
	} while (CanMove(coord + route));
	//���� ������� ����������, �������, ������ �� ����
	if (CanEatDamka(route)) { f++; SearchEatDamkaInRay(route); }

	//���������� ���� �����, ���� ������ ������ ����
	if (f == 0) do {
		cache.Push(GetMove(coord));
		coord -= route;
	} while (CanMove(coord));

	UnMakeMoveDamka(savedCoord, savedCoordEaten);
}

int SearchEatChecker(Checker* ch) 
{
	color = ch->GetColor(); //� ���������� �������� � Generate
	coord = ch->GetCoord(); //�� ���� ���������
	num = ch->GetNum();//� ���������� �������� � Generate
	if (CanEatChecker(backRight[color])) SearchEatCheckerInRay(backRight[color]);
	if (CanEatChecker(backLeft[color])) SearchEatCheckerInRay(backLeft[color]);
	if (CanEatChecker(forwardLeft[color])) SearchEatCheckerInRay(forwardLeft[color]);
	if (CanEatChecker(forwardRight[color])) SearchEatCheckerInRay(forwardRight[color]);
	return 0;
}

int SearchEatDamka(Checker* ch)
{
	color = ch->GetColor(); //� ���������� �������� � Generate
	coord = ch->GetCoord(); //�� ���� ���������
	num = ch->GetNum();//� ���������� �������� � Generate
	if (CanEatDamka(backRight[color])) SearchEatDamkaInRay(backRight[color]);
	if (CanEatDamka(backLeft[color])) SearchEatDamkaInRay(backLeft[color]);
	if (CanEatDamka(forwardLeft[color])) SearchEatDamkaInRay(forwardLeft[color]);
	if (CanEatDamka(forwardRight[color])) SearchEatDamkaInRay(forwardRight[color]);
	return 0;
}