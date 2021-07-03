#ifndef SUDOKU_HPP
#define SUDOKU_HPP
#include <string>
#include <iostream>
#include <exception>
#include <fstream>

using namespace std;

class sudoku
{
  int field[9][9];
  bool pos[81][9];
public:
  sudoku();

  class bad_file: public exception
  {
    const char* what() const throw();
  };
  
  void set_sudoku_from_file(string filename);
    
  void set_sudoku_from_stream(istream &os);

  void print() const;

  bool solve();

  bool is_solved() const;

  bool prune_cell(int y, int x);
};

#endif
