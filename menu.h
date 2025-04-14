#ifndef MENU_H
#define MENU_H

#include "street_manager.h"

class Menu
{
private:
  StreetManager &manager;

public:
  Menu(StreetManager &mgr);
  void displayMenu() const;
  void run();
};

#endif // MENU_H