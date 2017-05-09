#include "statistics_func.h"
#define _CRT_SECURE_NO_WARNINGS


#include <string>
#include <iostream>
#include <stdio.h>
#include <algorithm>

#include <iostream>
#include <fstream>
using namespace std;

double averSizeMovies = 0;
double averNumCaptureMoves = 0;

int maxDeepForc = 0;
int curMaxForc = 0;

int numMovies = 0;
int numForc = 0;
int maxSizeCache = 0;
long long numNodes = 0;

void ClearStatistics()
{
	averSizeMovies = 0;
	averNumCaptureMoves = 0;
	numMovies = 0;
	numForc = 0;
	maxSizeCache = 0;
	numNodes = 0;
	maxDeepForc = 0;
	curMaxForc = 0;
}
void CalculateAverSizeOfMovie(int sizeMove)
{
	averSizeMovies = (averSizeMovies * numMovies + sizeMove) / (numMovies + 1);
	numMovies++;
}
void CalculateAverNumCaptureMoves(int sizeMove)
{
	averNumCaptureMoves = (averNumCaptureMoves  * numForc + sizeMove) / (numForc + 1);
	numForc++;
	//if (sizeMove > 1) averNumCaptures = sizeMove;
}
void CalculateMaxSize(int n)
{
	maxSizeCache = (n > maxSizeCache ? n : maxSizeCache);
}
void CalculateNumNodes(int n)
{
	numNodes += n;
}
void UpdateMaxForcing()
{
	if (curMaxForc)
		maxDeepForc++;
	else
	{
		curMaxForc = 1;
		maxDeepForc = 1;
	}
}
void ClearForcing()
{
	curMaxForc = 0;
}
string IntToStr(int n)
{
	string s;
	if (n == 0)
		s.push_back('0');
	while (n)
	{
		s.push_back((n % 10) + '0');
		n /= 10;
	}
	for (int i = 0; i < s.length()/2; i++)
		swap(s[i], s[s.length() - i - 1]);
	return s;
}
void PrintStatistics(int numGame, int numStep, int depth, int typeSearch, int typeEvalute, int color)
{
	char *col[] = { {" white"}, {" black"} };
	char *search[] = { { " search" },{ " A_B" }, {" forcing"} };
	char *ev[] = { { " simple_ev" },{ " smart_ev" } };
	string s = "statistics " + IntToStr(numGame) + search[typeSearch] + ev[typeEvalute]+ IntToStr(depth) + col[color]+".txt";
	ofstream f(s, fstream::app);

	if (!f.is_open())
		f = ofstream(s);

	f << "\nnum_step_" << numStep<<" max_size_of_cache_" << maxSizeCache;
	f << " aver_size_of_move_" <<averSizeMovies<<" num_nodes_"<<numNodes;
	f << " aver_num_of_capture's_moves_" << averNumCaptureMoves << " max_deep_forc_"<<maxDeepForc;

	f.close();
	ClearStatistics();
}