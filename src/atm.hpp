#pragma once

#include "db.hpp"
namespace EqbalAtm {

class Atm {
private:
  DataBase &db;
  Card *selectedCard;

public:
  Atm(DataBase &p_db) : db(p_db) {}
  ~Atm() = default;
  Atm(Atm &&) = delete;
  Atm(const Atm &) = delete;
  Atm &operator=(Atm &&) = delete;
  Atm &operator=(const Atm &) = delete;

  void run();

private:
};

} // namespace EqbalAtm
