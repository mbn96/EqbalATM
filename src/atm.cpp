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

std::optional<Atm::InputVal> Atm::readInput(const InputType t) {
  std::cout << "(enter c to cancel)" << std::endl;
  skipWhiteSpace();
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
  return std::nullopt;
}

std::optional<Card *> Atm::readCard() {
  auto inp = readInput(InputType::String);
  if (!inp) {
    return std::nullopt;
  }
  auto cardID = std::get<std::string>(inp.value());
  if (!db.cardExists(cardID)) {
    std::cout << "Invalid Card ID." << std::endl;
    return std::nullopt;
  }
  auto card = &db.getCard(cardID);
  return std::make_optional<Card *>(card);
}

void Atm::start() {
  std::cout << "Welcome To EqbalATM, Please Enter Your Card ID:" << std::endl;
  auto cardOpt = readCard();
  if (cardOpt) {
    selectedCard = cardOpt.value();
    selectAction();
  }
  std::cout << "Thank you. Goodbye!" << std::endl;
  selectedCard = nullptr;
}
void Atm::printCardInfo(const Card &c) const {
  std::cout << "\tCard ID:\t" + c.getID() << std::endl;
  std::cout << "\tCard Owner:\t" + c.getOwner() << std::endl;
}

void Atm::selectAction() {
  std::cout << "Entered Card Info:" << std::endl;
  printCardInfo(*selectedCard);
  std::cout << "Please Choose one of the following actions:\n"
               "1) Withdraw Funds\n"
               "2) Deposit Funds\n"
               "3) Transfer Funds\n"
               "4) Check Balance"
            << std::endl;
  int64_t action = 0;
  while (action < 1 || action > 4) {
    std::cout << "Enter a Number Between 1-4" << std::endl;
    auto ans = readInput(InputType::Integer);
    if (!ans) {
      return;
    }
    action = std::get<int64_t>(ans.value());
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
  std::cout << msg << std::endl;
  std::cout << "Confirm? (y/n)" << std::endl;
  skipWhiteSpace();
  std::string ans;
  std::cin >> ans;
  return ans[0] == 'y';
}
void Atm::withdraw() {
  std::cout << "Please Enter Withdraw Amount:" << std::endl;
  int64_t amount = 0;
  std::optional<InputVal> ans;
  while (amount < 100 || amount > 100000) {
    std::cout << "Enter a Number Between 100-100000" << std::endl;
    ans = readInput(InputType::Integer);
    if (!ans) {
      return;
    }
    amount = std::get<int64_t>(ans.value());
  }
  std::cout << "Enter Card Password:" << std::endl;
  ans = readInput(InputType::String);
  if (!ans) {
    return;
  }
  std::string password = std::get<std::string>(ans.value());
  if (selectedCard->getPassword() != password) {
    std::cout << "Incorrect Password." << std::endl;
    return;
  }
  std::stringstream st;
  st << "Withdraw " << amount << " ?";
  bool ok;
  ok = confirm(st.str());
  if (ok && selectedCard->getBalance() >= amount) {
    selectedCard->withdraw(amount);
    st.str(std::string());
    st << "Current Balance: " << selectedCard->getBalance();
    std::cout << st.str() << std::endl;
    return;
  }
  if (ok) {
    std::cout << "Insufficient Balance" << std::endl;
  }
}
void Atm::deposit() {
  std::cout << "Please Enter Deposit Amount:" << std::endl;
  int64_t amount = 0;
  std::optional<InputVal> ans;
  while (amount < 100 || amount > 100000) {
    std::cout << "Enter a Number Between 100-100000" << std::endl;
    ans = readInput(InputType::Integer);
    if (!ans) {
      return;
    }
    amount = std::get<int64_t>(ans.value());
  }
  std::cout << "Enter Card Password:" << std::endl;
  ans = readInput(InputType::String);
  if (!ans) {
    return;
  }
  std::string password = std::get<std::string>(ans.value());
  if (selectedCard->getPassword() != password) {
    std::cout << "Incorrect Password." << std::endl;
    return;
  }
  std::stringstream st;
  st << "Deposit " << amount << " ?";
  bool ok;
  ok = confirm(st.str());
  if (ok) {
    selectedCard->deposit(amount);
    st.str(std::string());
    st << "Current Balance: " << selectedCard->getBalance();
    std::cout << st.str() << std::endl;
  }
}
void Atm::transfer() {
  std::cout << "Please Enter Destination Card ID:" << std::endl;
  auto cardOpt = readCard();
  if (!cardOpt) {
    std::cout << "Invalid Destination Card." << std::endl;
    return;
  }
  auto destCard = cardOpt.value();
  if (destCard->getID() == selectedCard->getID()) {
    std::cout << "Cannot Transfer funds To the same card." << std::endl;
    return;
  }
  std::cout << "Please Enter Transfer Amount:" << std::endl;
  int64_t amount = 0;
  std::optional<InputVal> ans;
  while (amount < 100 || amount > 100000) {
    std::cout << "Enter a Number Between 100-100000" << std::endl;
    ans = readInput(InputType::Integer);
    if (!ans) {
      return;
    }
    amount = std::get<int64_t>(ans.value());
  }
  std::cout << "Enter Card Password:" << std::endl;
  ans = readInput(InputType::String);
  if (!ans) {
    return;
  }
  std::string password = std::get<std::string>(ans.value());
  if (selectedCard->getPassword() != password) {
    std::cout << "Incorrect Password." << std::endl;
    return;
  }
  std::stringstream st;
  st << "Transfer " << amount << " From";
  std::cout << st.str() << std::endl;
  printCardInfo(*selectedCard);
  std::cout << "To" << std::endl;
  printCardInfo(*destCard);
  bool ok;
  ok = confirm(std::string());
  if (ok && selectedCard->getBalance() >= amount) {
    selectedCard->withdraw(amount);
    destCard->deposit(amount);
    st.str(std::string());
    st << "Current Balance: " << selectedCard->getBalance();
    std::cout << st.str() << std::endl;
    return;
  }
  if (ok) {
    std::cout << "Insufficient Balance" << std::endl;
  }
}

void Atm::getBalance() {
  std::cout << "Enter Card Password:" << std::endl;
  auto ans = readInput(InputType::String);
  if (!ans) {
    return;
  }
  std::string password = std::get<std::string>(ans.value());
  if (selectedCard->getPassword() != password) {
    std::cout << "Incorrect Password." << std::endl;
    return;
  }

  std::stringstream st;
  st << "Current Balance: " << selectedCard->getBalance();
  std::cout << st.str() << std::endl;
}

} // namespace EqbalAtm
