#include "db.hpp"
#include <utility>

namespace EqbalAtm {
bool DataBase::cardExists(const std::string &cardID) const {
  return cards.find(cardID) != cards.end();
}
void DataBase::addCard(Card &&c) {
  cards.insert(std::make_pair(c.getID(), std::move(c)));
}

Card &DataBase::getCard(const std::string &cardID) { return cards[cardID]; }

} // namespace EqbalAtm
