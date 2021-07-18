#include <iostream>
#include <exception>
#include <string>
#include <fstream>
#include <time.h>
#include <sys/time.h>
#include "sudoku.hpp"

using namespace std;



int main(int argc, char **argv)
{
  struct timeval t1;
  gettimeofday(&t1, NULL);
  srand(t1.tv_sec * 1000000 + t1.tv_usec);
  sudoku s1;
  string k;
  string filename;
  if (argc >= 2) {
    k = argv[1];
    if (k == "-c" && argc == 2) {
      s1.create(25);
      s1.print();
      exit(0);
    }
    else if (k == "-c" && argc == 3) {
      string diff = argv[2];
      if (diff == "easy") {
	s1.create(36);
      }
      else if (diff == "medium") {
	s1.create(25);
      }
      else if (diff == "hard") {
	s1.create(20);
      }
      else if (diff == "expert") {
	s1.create(10);
      }
      else {
	cerr << "ERROR: usage " << argv[0] << " {[-s filename] [-c ['easy' 'medium' 'hard']]}" << endl;
	cerr << "-c -- creates puzzle with specified level of difficulty" << endl;
	cerr << "-s -- solves puzzle from specfied filename" << endl;
	exit(1);
      }
      s1.print();
      exit(0);
    }
    else if (k == "-s" && argc == 3) {
      filename = argv[2];
    }
    else if (argc == 2) {
    }
    else {
      cerr << "ERROR: usage " << argv[0] << " {[-s filename] [-c ['easy' 'medium' 'hard']]}" << endl;
      cerr << "-c -- creates puzzle with specified level of difficulty" << endl;
      cerr << "-s -- solves puzzle from specfied filename" << endl;
      exit(1);
    }
  }
  try {
    if (filename.length() == 0)
      s1.set_sudoku_from_stream(cin);
    else
      s1.set_sudoku_from_file(filename);
  }
  catch(exception &e) {
    if (filename.length() != 0)
      cerr << "Invalid file " << k << endl;
    else
      cerr << "Invalid input\n";
    exit(1);
  }
  s1.print();
  if (s1.solve()) {
    s1.print();
  }
  else {
    cout << "Puzzle cannot be solved\n";
    exit(1);
  }
  return 0;
}
