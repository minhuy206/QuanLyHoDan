#include "household.h"
#include "helpers.h"
#include <iostream>

using namespace std;

Household::Household(const string &addr, const string &hId, const vector<string> &memIds, const string &status)
    : address(addr), headId(hId), memberIds(memIds), specialStatus(status) {}
Household::~Household()
{
}
void Household::input()
{
  address = getValidStringInput("Nhap dia chi ho: ", 100, false);
  headId = getValidStringInput("Nhap ID chu ho: ", 12, false);
  specialStatus = getValidStringInput("Nhap trang thai dac biet (Ngheo/Can ngheo, de trong neu khong co): ", 20, true);
  string errorMsg;
  if (!isValidSpecialStatus(specialStatus, errorMsg))
  {
    cout << "Loi: " << errorMsg << " Trang thai se duoc de trong.\n";
    specialStatus = "";
  }
  auto numMembers = getValidIntegerInput("Nhap so thanh vien (khong tinh chu ho): ", 0, 100);
  if (!numMembers)
    return;
  memberIds.clear();
  for (int i = 0; i < *numMembers; ++i)
  {
    string memId = getValidStringInput("Nhap ID thanh vien " + to_string(i + 1) + ": ", 12, false);
    memberIds.push_back(memId);
  }
}

void Household::edit()
{
  cout << "Chinh sua thong tin ho dan (ID chu ho va thanh vien khong the thay doi):\n";
  address = getValidStringInput("Nhap dia chi ho: ", 100, false);
  specialStatus = getValidStringInput("Nhap trang thai dac biet (Ngheo/Can ngheo, de trong neu khong co): ", 20, true);
  string errorMsg;
  if (!isValidSpecialStatus(specialStatus, errorMsg))
  {
    cout << "Loi: " << errorMsg << " Trang thai se duoc de trong.\n";
    specialStatus = "";
  }
}

string Household::getAddress() const { return address; }

string Household::getHeadId() const { return headId; }

string Household::getSpecialStatus() const { return specialStatus; }

const vector<string> &Household::getMemberIds() const { return memberIds; }

PermanentHousehold::PermanentHousehold(const string &addr, const string &hId,
                                       const vector<string> &memIds, const string &status)
    : Household(addr, hId, memIds, status) {}

void PermanentHousehold::display() const
{
  cout << "Ho thuong tru - So nha: " << address << endl;
  cout << "Chu ho (ID): " << headId << endl;
  cout << "So thanh vien: " << memberIds.size() << endl;
  if (!memberIds.empty())
  {
    cout << "Danh sach ID thanh vien: ";
    for (size_t i = 0; i < memberIds.size(); ++i)
    {
      cout << memberIds[i];
      if (i < memberIds.size() - 1)
        cout << ", ";
    }
    cout << endl;
  }
  if (!specialStatus.empty())
  {
    cout << "Trang thai dac biet: " << specialStatus << endl;
  }
}

string PermanentHousehold::getType() const { return "Permanent"; }

TemporaryHousehold::TemporaryHousehold(const string &addr, const string &hId,
                                       const vector<string> &memIds, const string &status, const Date &exp)
    : Household(addr, hId, memIds, status), expiryDate(exp) {}

void TemporaryHousehold::input()
{
  address = getValidStringInput("Nhap dia chi ho: ", 100, false);
  headId = getValidStringInput("Nhap ID chu ho: ", 12, false);
  specialStatus = getValidStringInput("Nhap trang thai dac biet (Ngheo/Can ngheo, de trong neu khong co): ", 20, true);
  string errorMsg;
  if (!isValidSpecialStatus(specialStatus, errorMsg))
  {
    cout << "Loi: " << errorMsg << " Trang thai se duoc de trong.\n";
    specialStatus = "";
  }
  auto numMembers = getValidIntegerInput("Nhap so thanh vien (khong tinh chu ho): ", 0, 100);
  if (!numMembers)
    return;
  memberIds.clear();
  for (int i = 0; i < *numMembers; ++i)
  {
    string memId = getValidStringInput("Nhap ID thanh vien " + to_string(i + 1) + ": ", 12, false);
    memberIds.push_back(memId);
  }
  do
  {
    string expStr = getValidStringInput("Nhap ngay het han tam tru (dd/mm/yyyy, sau " +
                                            Date::currentDate().toString() + ", vi du: 01/12/2023): ",
                                        10, false);
    expiryDate = Date::fromString(expStr, errorMsg);
    if (!expiryDate.isValid())
    {
      cout << "Loi: " << errorMsg << " Vui long nhap lai.\n";
    }
    else if (!expiryDate.isFuture())
    {
      cout << "Loi: Ngay het han phai sau ngay hien tai. Vui long nhap lai.\n";
    }
    else
    {
      break;
    }
  } while (true);
}

void TemporaryHousehold::edit()
{
  cout << "Chinh sua thong tin ho tam tru (ID chu ho va thanh vien khong the thay doi):\n";
  address = getValidStringInput("Nhap dia chi ho: ", 100, false);
  specialStatus = getValidStringInput("Nhap trang thai dac biet (Ngheo/Can ngheo, de trong neu khong co): ", 20, true);
  string errorMsg;
  if (!isValidSpecialStatus(specialStatus, errorMsg))
  {
    cout << "Loi: " << errorMsg << " Trang thai se duoc de trong.\n";
    specialStatus = "";
  }
  do
  {
    string expStr = getValidStringInput("Nhap ngay het han tam tru (dd/mm/yyyy, sau " +
                                            Date::currentDate().toString() + ", vi du: 01/12/2023): ",
                                        10, false);
    expiryDate = Date::fromString(expStr, errorMsg);
    if (!expiryDate.isValid())
    {
      cout << "Loi: " << errorMsg << " Vui long nhap lai.\n";
    }
    else if (!expiryDate.isFuture())
    {
      cout << "Loi: Ngay het han phai sau ngay hien tai. Vui long nhap lai.\n";
    }
    else
    {
      break;
    }
  } while (true);
}

void TemporaryHousehold::display() const
{
  cout << "Ho tam tru - So nha: " << address << endl;
  cout << "Chu ho (ID): " << headId << endl;
  cout << "So thanh vien: " << memberIds.size() << endl;
  if (!memberIds.empty())
  {
    cout << "Danh sach ID thanh vien: ";
    for (size_t i = 0; i < memberIds.size(); ++i)
    {
      cout << memberIds[i];
      if (i < memberIds.size() - 1)
        cout << ", ";
    }
    cout << endl;
  }
  cout << "Ngay het han tam tru: " << expiryDate.toString() << endl;
  if (!specialStatus.empty())
  {
    cout << "Trang thai dac biet: " << specialStatus << endl;
  }
}

string TemporaryHousehold::getType() const { return "Temporary"; }

Date TemporaryHousehold::getExpiryDate() const { return expiryDate; }