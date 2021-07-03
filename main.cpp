#include <iostream>
#include <exception>
#include <string>
#include <fstream>
#include "sudoku.hpp"

using namespace std;



int main(int argc, char **argv)
{
  sudoku s1;
  string k;
  if (argc == 2) {
    k = argv[1];
  }
  try {
    if (k.length() == 0)
      s1.set_sudoku_from_stream(cin);
    else
      s1.set_sudoku_from_file(k);
  }
  catch(exception &e) {
    if (k.length() != 0)
      cerr << "Invalid file " << k << endl;
    else
      cerr << "Invalid input\n";
    exit(0);
  }
  s1.print();
  s1.solve();
  s1.print();
  return 0;
}
