#include "menu.h"
#include "street_manager.h"

int main()
{
  StreetManager manager;
  Menu menu(manager);
  menu.run();
  return 0;
}