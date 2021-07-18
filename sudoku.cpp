#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include "sudoku.hpp"

using namespace std;

sudoku::sudoku()
{
}

void sudoku::clear()
{
  //Setting pos to true
  for (int i = 0; i < 81; i++) {
    for (int j = 0; j < 9; j++) {
      pos[i][j] = true;
      field[i / 9][i % 9] = 0;
    }
  }
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
  set_domain();
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

void sudoku::set_domain()
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
      pos[i][field[i / 9][i % 9] - 1] = true;
    }
  }
}

bool sudoku::infer () //returns true, if no cell domain got below 1
//false otherwise
{
  bool ret = true;
  while (ret) { //doing this stuff, while we have progress
    ret = false;
    //Pruning all cells
    for (int i = 0; i < 81; i++) {
      if (arch_cons_cell(i / 9, i % 9))
	ret = true;
      // print_domain(i / 9, i % 9);
      if (get_cell_domain(i / 9, i % 9) == 0) {
	// cout << "Bad cell: " << i / 9 + 1 << " " << i % 9 << endl;
	return false;
      }
    }    
  }
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

bool sudoku::arch_cons_cell(int y, int x)
//set domain in the cell accodring to its domain
//using arch-consistensy to do this, therefore the name
{
  bool ret = false;
  //  if (field[y][x] != 0) // if cell is already set, return
  //    return false;
  for (int i = 0; i < 9; i++) { //applying row constraints
    if (x == i)
      continue; //skipping the cell itself (redundant)
    if (field[y][i] != 0) {
      int t = field[y][i];	
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

  //if we got single domain, that we sould set the field
  if (get_cell_domain(y, x) == 1) {
    for (int i = 0; i < 9; i++) {
      if (pos[y * 9 + x][i]) {
	field[y][x] = i + 1;
	break;
      }
    }
  }
  
  return ret;
}

int sudoku::get_cell_domain(int y, int x)
{
  int p = y * 9 + x;
  int ret = 0;
  for (int i = 0; i < 9; i++) {
    if (pos[p][i])
      ret++;
  }
  return ret;
}

int sudoku::get_unsolved_cell()
{
  int min = -1;
  int mpos = 0;
  for (int i = 0; i < 81; i++) {
    int temp = get_cell_domain(i / 9, i % 9);
    if ((min == -1 || temp < min) && temp != 1) {
      min = temp;
      mpos = i;
    }
  }
  return mpos;
}

bool sudoku::solve()
{
  if (!infer())
    return false;
  if (is_solved())
    return true;
  int next = get_unsolved_cell();
  for (int i = 0; i < 9; i++) {
    if (pos[next][i]) {
      sudoku temp = *this;
      temp.field[next / 9][next % 9] = i + 1;
      for (int j = 0; j < 9; j++) {
	temp.pos[next][j] = false;
	if (j == i)
	  temp.pos[next][j] = true;
      }
      if (temp.solve()) {
	*this = temp;
	return true; 
      }
    }
  }
  return false;
}


void sudoku::print_domain(int y, int x)
{
  for (int i = 0; i < 9; i++) {
    if (pos[y * 9 + x][i])
      cout << i + 1 << " ";
  }
  cout << endl;
}

void sudoku::create(int k)
{
  clear();
  sudoku ret;
  ret.clear();
  for (int i = 0; i < k; i++) {
    while (1) {
      sudoku iter = ret;
      int npos = rand() % 81;
      if (iter.field[npos / 9][npos % 9] != 0) {
	continue;
      }
      iter.field[npos / 9][npos % 9] = rand() % 9 + 1;
      sudoku iter2 = iter;
      iter2.set_domain();
      if (!iter2.solve())
	continue;
      ret = iter;
      break;
    }
  }
  *this = ret;
}
