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
  // enum class AtmStage { Start, SelectAction, Withdraw, Deposit, Transfer };

  DataBase &db;
  Card *selectedCard;
  // std::stack<std::pair<AtmStage, int32_t>> stageStack;

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
  InputVal readInput(const InputType t, bool &ok);
  std::optional<Card *> readCard();
  bool confirm(const std::string &msg);
  void popStage();
  void printCardInfo(const Card &c) const;
};

} // namespace EqbalAtm
