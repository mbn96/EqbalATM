#include "utils.hpp"
#include <iostream>

int64_t readInt() {
  using namespace std;
  int64_t res;
  cin >> res;
  while (!cin.good()) {
    cin.clear();
    cin.ignore(1000, '\n');
    cout << "Please Enter a Number" << endl;
    cin >> res;
  }

  return res;
}


void skipWhiteSpace() {
  static const std::string wps = "\n\t ";
  while (wps.find(static_cast<char>(std::cin.peek())) != std::string::npos) {
    std::cin.ignore(1);
  }
}

