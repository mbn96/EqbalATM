#include "utils.hpp"
#include <iostream>

int readInt() {
  using namespace std;
  int res;
  cin >> res;
  while (!cin.good()) {
    cin.clear();
    cin.ignore(1000, '\n');
    cout << "Please Enter a Number" << endl;
    cin >> res;
  }

  return res;
}
