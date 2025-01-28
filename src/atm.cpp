#include "atm.hpp"
#include "db.hpp"
#include "utils.hpp"
#include <cstdint>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

namespace EqbalAtm {

void Atm::run() {
  while (true) {
    start();
  }
}

Atm::InputVal Atm::readInput(const InputType t, bool &ok) {
  printLine("(enter c to cancel)");
  skipWhiteSpace();
  ok = true;
  switch (t) {
  case InputType::String: {
    std::string inp;
    std::cin >> inp;
    if (inp == "c") {
      break;
    }
    return inp;
  }
  case InputType::Integer: {
    auto firstChar = std::cin.peek();
    if (firstChar == 'c') {
      std::cin.ignore(1000, '\n');
      break;
    }
    auto inp = readInt();
    return inp;
  }
  }
  ok = false;
  return 0;
}

std::optional<Card *> Atm::readCard() {
  bool ok = true;
  auto inp = readInput(InputType::String, ok);
  if (!ok) {
    return std::nullopt;
  }
  auto cardID = std::get<std::string>(inp);
  if (!db.cardExists(cardID)) {
    printLine("Invalid Card ID.");
    return std::nullopt;
  }
  auto card = &db.getCard(cardID);
  return std::make_optional<Card *>(card);
}

void Atm::start() {
  printLine("Welcome To EqbalATM, Please Enter Your Card ID:");
  auto cardOpt = readCard();
  if (cardOpt) {
    selectedCard = cardOpt.value();
    selectAction();
  }
  printLine("Thank you. Goodbye!");
  selectedCard = nullptr;
}
void Atm::printCardInfo(const Card &c) const {
  printLine("\tCard ID:\t" + c.getID());
  printLine("\tCard Owner:\t" + c.getOwner());
}

void Atm::selectAction() {
  printLine("Entered Card Info:");
  printCardInfo(*selectedCard);
  printLine("Please Choose one of the following actions:\n"
            "1) Withdraw Money\n"
            "2) Deposit Money\n"
            "3) Transfer Money\n"
            "4) Get Current Balance");
  int64_t action = 0;
  while (action < 1 || action > 4) {
    printLine("Enter a Number Between 1-4");
    bool ok = false;
    auto ans = readInput(InputType::Integer, ok);
    if (!ok) {
      return;
    }
    action = std::get<int64_t>(ans);
  }

  switch (action) {
  case 1: {
    withdraw();
    break;
  }
  case 2: {
    deposit();
    break;
  }
  case 3: {
    transfer();
    break;
  }
  case 4: {
    getBalance();
    break;
  }
  }
}

bool Atm::confirm(const std::string &msg) {
  printLine(msg);
  printLine("Confirm? (y/n)");
  skipWhiteSpace();
  std::string ans;
  std::cin >> ans;
  return ans[0] == 'y';
}
void Atm::withdraw() {
  printLine("Please Enter Withdraw Amount:");
  int64_t amount = 0;
  bool ok;
  InputVal ans;
  while (amount < 100 || amount > 100000) {
    printLine("Enter a Number Between 100-100000");
    ok = false;
    ans = readInput(InputType::Integer, ok);
    if (!ok) {
      return;
    }
    amount = std::get<int64_t>(ans);
  }
  printLine("Enter Card Password:");
  ans = readInput(InputType::String, ok);
  if (!ok) {
    return;
  }
  std::string password = std::get<std::string>(ans);
  if (selectedCard->getPassword() != password) {
    printLine("Incorrect Password.");
    return;
  }
  std::stringstream st;
  st << "Withdraw " << amount << " ?";
  ok = confirm(st.str());
  if (ok && selectedCard->getBalance() >= amount) {
    selectedCard->withdraw(amount);
    st.str(std::string());
    st << "Current Balance: " << selectedCard->getBalance();
    printLine(st.str());
    return;
  }
  if (ok) {
    printLine("Insufficient Balance");
  }
}
void Atm::deposit() {
  printLine("Please Enter Deposit Amount:");
  int64_t amount = 0;
  bool ok;
  InputVal ans;
  while (amount < 100 || amount > 100000) {
    printLine("Enter a Number Between 100-100000");
    ok = false;
    ans = readInput(InputType::Integer, ok);
    if (!ok) {
      return;
    }
    amount = std::get<int64_t>(ans);
  }
  printLine("Enter Card Password:");
  ans = readInput(InputType::String, ok);
  if (!ok) {
    return;
  }
  std::string password = std::get<std::string>(ans);
  if (selectedCard->getPassword() != password) {
    printLine("Incorrect Password.");
    return;
  }
  std::stringstream st;
  st << "Deposit " << amount << " ?";
  ok = confirm(st.str());
  if (ok) {
    selectedCard->deposit(amount);
    st.str(std::string());
    st << "Current Balance: " << selectedCard->getBalance();
    printLine(st.str());
  }
}
void Atm::transfer() {
  printLine("Please Enter Destination Card ID:");
  auto cardOpt = readCard();
  if (!cardOpt) {
    printLine("Invalid Destination Card.");
    return;
  }
  auto destCard = cardOpt.value();
  printLine("Please Enter Transfer Amount:");
  int64_t amount = 0;
  bool ok;
  InputVal ans;
  while (amount < 100 || amount > 100000) {
    printLine("Enter a Number Between 100-100000");
    ok = false;
    ans = readInput(InputType::Integer, ok);
    if (!ok) {
      return;
    }
    amount = std::get<int64_t>(ans);
  }
  printLine("Enter Card Password:");
  ans = readInput(InputType::String, ok);
  if (!ok) {
    return;
  }
  std::string password = std::get<std::string>(ans);
  if (selectedCard->getPassword() != password) {
    printLine("Incorrect Password.");
    return;
  }
  std::stringstream st;
  st << "Transfer " << amount << " From";
  printLine(st.str());
  printCardInfo(*selectedCard);
  printLine("To");
  printCardInfo(*destCard);
  ok = confirm(std::string());
  if (ok && selectedCard->getBalance() >= amount) {
    selectedCard->withdraw(amount);
    destCard->deposit(amount);
    st.str(std::string());
    st << "Current Balance: " << selectedCard->getBalance();
    printLine(st.str());
    return;
  }
  if (ok) {
    printLine("Insufficient Balance");
  }
}

void Atm::getBalance() {
  printLine("Enter Card Password:");
  bool ok;
  auto ans = readInput(InputType::String, ok);
  if (!ok) {
    return;
  }
  std::string password = std::get<std::string>(ans);
  if (selectedCard->getPassword() != password) {
    printLine("Incorrect Password.");
    return;
  }

  std::stringstream st;
  st << "Current Balance: " << selectedCard->getBalance();
  printLine(st.str());
}

} // namespace EqbalAtm
