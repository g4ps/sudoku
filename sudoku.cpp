#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include "sudoku.hpp"

using namespace std;

sudoku::sudoku()
{
}

const char* sudoku::bad_file::what() const throw()
{
  return "bad file";
}

void sudoku::set_sudoku_from_file(string filename)
{
  ifstream is(filename);
  if (!is.good()) {
    cerr << "Couldn't open file " << filename << endl;
    throw bad_file();
  }
  set_sudoku_from_stream(is);
}

void sudoku::set_sudoku_from_stream(istream &os)
{
  for (int i = 0; i < 81; i++) {
    os >> field[i / 9] [i % 9];
    if (os.fail()) {
      cerr << "Couldn't read value at {" << i / 9 << ", "
	   << i % 9 << "}\n";
      throw bad_file();
    }
    int t = field[i / 9] [i % 9];
    if (t < 0 || t > 9) {
      cerr << "Bad number (" << t << ") at {"
	   << i % 9 << ", " << i / 9 << "}; number should be in range "
	   << "[1;9]\n";
      throw bad_file();
    }
  }
}

void sudoku::print() const
{
  int t;
  cout << "------------------\n";
  for (int i = 0; i < 81; i++) {
    t = field[i / 9][i % 9];
    if (t != 0)
      cout << t;
    else
      cout << " ";
    if (i % 3 == 2)
      cout << "|";
    else
      cout << " ";
    if (i % 9 == 8){
      cout << endl;
      if ((i / 9) % 3 == 2) {
	cout << "------------------\n";
      }
    }
  }
  cout << endl;
}

bool sudoku::solve() //returns true, if the puzzle is solved, false,
//if it cannot be solved
{
  //Setting pos to true
  for (int i = 0; i < 81; i++) {
    for (int j = 0; j < 9; j++) {
      pos[i][j] = true;
    }
  }
  //Pruning all, that are already set
  for (int i = 0; i < 81; i++) {
    if (field[i / 9][i % 9] != 0) {
      for (int j = 0; j < 9; j++) {
	pos[i][j] = false;
      }
      pos[i][field[i / 9][i % 9]] = true;
    }
  }
  bool ret = true;
  while (ret) { //doing this stuff, while we have progress
    ret = false;
    //Pruning all cells
    for (int i = 0; i < 81; i++) {
      if (prune_cell(i / 9, i % 9))
	ret = true;

      //if domain for the cell is empty, we return false
      if (field[i / 9][i % 9] == 0) {
	int size = 0;
	for (int j = 0; j < 9; j++) {
	  if (pos[i][j])
	    size++;
	}
	if (size == 0)
	  return false;
	
	//If we find cell with domain 1, set this number
	if (size == 1) {
	  for (int j = 0; j < 9; j++) {
	    if (pos[i][j]) {
	      field[i / 9][i % 9] = j + 1;
	      break;
	    }
	  }
	}
      }
    }
    
  }
  //if not solved, then we have more than one solution
  if (!is_solved())
    return false;
  return true;
}

bool sudoku::is_solved() const
{
  for (int i = 0; i < 81; i++) {
    if (field[i / 9][i % 9] == 0)
      return false;
  }
  return true;
}

bool sudoku::prune_cell(int y, int x) //Prunes cell in pos (y, x), according
//to constraints
//Returns true, if we changed domain of the cell
{
  bool ret = false;
  if (field[y][x] != 0) // if cell is already set, return
    return false;
  for (int i = 0; i < 9; i++) { //applying row constraints
    if (x == i)
      continue; //skipping the cell itself (redundant)
    if (field[y][i] != 0) {
      int t = field[y][i]; // needed for readability	
      //if we had hit a number
      if (pos[y * 9 + x][t - 1] == true)
	ret = true;
      pos[y * 9 + x][t - 1] = false;
    }
  }

  for (int i = 0; i < 9; i++) { //applying collumn constraints
    //idea is identical to pevious
    if (y == i)
      continue;
    if (field[i][x] != 0) {
      int t = field[i][x];
      if (pos[y * 9 + x][t - 1] == true)
	ret = true;
      pos[y * 9 + x][t - 1] = false;
    }
  }

  //applying square constraints
  for (int i = 0; i < 9; i++) {
    int posy = y - y % 3 + i / 3; // x pos of start of square
    int posx = x - x % 3 + i % 3; // y pos of start of square
    if (posx == x && posy == y) {
      continue;
    }
    if (field[posy][posx] != 0) {
      int t = field[posy][posx];
      if (pos[y * 9 + x][t - 1] == true)
	ret = true;
      pos[y * 9 + x][t - 1] = false;
    }
  }

  return ret;
}
