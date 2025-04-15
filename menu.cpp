#include "menu.h"
#include "helpers.h"
#include <iostream>

using namespace std;

Menu::Menu(StreetManager &mgr) : manager(mgr) {}

void Menu::displayMenu() const
{
  cout << "\n=== QUAN LY HO DAN ===\n";
  cout << "1. Them ho dan\n";
  cout << "2. Chinh sua ho dan\n";
  cout << "3. Xoa ho dan\n";
  cout << "4. Quan ly thanh vien ho\n";
  cout << "5. Quan ly thanh vien\n";
  cout << "6. Tim ho tam tru het han\n";
  cout << "7. Hien thi tat ca ho dan\n";
  cout << "8. Tim kiem theo dia chi\n";
  cout << "9. Tim kiem theo ID\n";
  cout << "10. Thong ke theo loai\n";
  cout << "11. Doc du lieu tu file CSV\n";
  cout << "12. Ghi du lieu ra file CSV\n";
  cout << "0. Thoat\n";
  cout << "======================\n";
}

void Menu::run()
{
  while (true)
  {
    displayMenu();
    auto choice = getValidIntegerInput("Nhap lua chon: ", 0, 12);
    if (!choice)
      continue;
    switch (*choice)
    {
    case 0:
      cout << "Tam biet!\n";
      return;
    case 1:
      manager.addHousehold();
      break;
    case 2:
      manager.editHousehold();
      break;
    case 3:
      manager.deleteHousehold();
      break;
    case 4:
      manager.manageHouseholdMembers();
      break;
    case 5:
      manager.manageMembers();
      break;
    case 6:
      manager.findExpiredTemporaryHouseholds();
      break;
    case 7:
      manager.displayAll();
      break;
    case 8:
    {
      string addr = getValidStringInput("Nhap dia chi can tim: ", 100, false);
      manager.searchByAddress(addr);
      break;
    }
    case 9:
    {
      string id = getValidStringInput("Nhap ID can tim: ", 12, false);
      manager.searchById(id);
      break;
    }
    case 10:
      manager.reportByType();
      break;
    case 11:
      if (manager.loadFromCsv())
        cout << "Doc du lieu thanh cong!\n";
      else
        cout << "Doc du lieu that bai!\n";
      break;
    case 12:
      if (manager.saveToCsv())
        cout << "Ghi du lieu thanh cong!\n";
      else
        cout << "Ghi du lieu that bai!\n";
      break;
    }
  }
}