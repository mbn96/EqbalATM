#include "atm.hpp"
#include "db.hpp"

int main() {
  EqbalAtm::DataBase db;
  db.addCard(EqbalAtm::Card{"fc3385ee-e554-4f2a-acf3-9ac9623296e6",
                            "Melanie Watson", "Mq8Xj7Ud", 520000});
  db.addCard(EqbalAtm::Card{"0aa413aa-2410-459a-a659-9ad5536d08b9",
                            "Stephen Jones", "Sb5PzVup", 654000});
  db.addCard(EqbalAtm::Card{"841624c9-9c2f-4456-95cc-cd59f95a5394",
                            "Dean Jensen", "p4utfMzq", 231000});
  db.addCard(EqbalAtm::Card{"99a60c48-edd0-4ec0-bb1b-4c3071761d8e",
                            "Brendon Esparza", "kQs2zSNv", 843000});
  db.addCard(EqbalAtm::Card{"bca7f845-c9e3-4c29-8a10-d1b906bf796a",
                            "Myrtle Fleming", "VNH6pYKX", 451000});

  EqbalAtm::Atm atm(db);
  atm.run();
  return 0;
}
