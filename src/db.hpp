#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
namespace EqbalAtm {

struct Card {
private:
  std::string id;
  std::string password;
  std::string owner;
  int64_t balance;

public:
  Card() = default;
  Card(const std::string &p_id, const std::string &p_owner,
       const std::string &p_password, int64_t p_balance)
      : id{p_id}, password{p_password}, owner{p_owner}, balance{p_balance} {}
  Card(Card &&crd) = default;
  Card &operator=(Card &&name) = default;
  ~Card() = default;

  Card(const Card &crd) = delete;
  Card &operator=(const Card &name) = delete;

  int64_t getBalance() const noexcept { return balance; }

  int64_t withdraw(int64_t amount) {
    balance -= amount;
    return balance;
  }

  int64_t deposit(int64_t amount) {
    balance += amount;
    return balance;
  }

  const std::string &getID() const noexcept { return id; }
  const std::string &getOwner() const noexcept { return owner; }
  const std::string &getPassword() const noexcept { return password; }
};

class DataBase {
private:
  std::unordered_map<std::string, Card> cards;

public:
  DataBase() = default;
  DataBase(DataBase &&) = delete;
  DataBase(const DataBase &) = delete;
  DataBase &operator=(DataBase &&) = delete;
  DataBase &operator=(const DataBase &) = delete;
  ~DataBase() = default;

  bool cardExists(const std::string &cardID) const;
  void addCard(Card &&c);
  Card &getCard(const std::string &cardID);
};

} // namespace EqbalAtm
