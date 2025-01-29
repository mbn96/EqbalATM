#pragma once

#include "db.hpp"
#include <cstdint>
#include <optional>
#include <string>
#include <variant>
namespace EqbalAtm {

class Atm {
private:
  using InputVal = std::variant<std::string, int64_t>;
  enum class InputType { String, Integer };

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
  void start();
  void selectAction();
  void withdraw();
  void deposit();
  void transfer();
  void getBalance();
  std::optional<InputVal> readInput(const InputType t);
  std::optional<Card *> readCard();
  bool confirm(const std::string &msg);
  void popStage();
  void printCardInfo(const Card &c) const;
};

} // namespace EqbalAtm
