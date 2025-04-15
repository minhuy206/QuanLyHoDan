#include "household.h"
#include "helpers.h"
#include <iostream>

using namespace std;

Household::Household(const string &addr, const string &hId, const vector<string> &memIds, SpecialStatus status)
    : address(addr), headId(hId), memberIds(memIds), specialStatus(status) {}

void Household::input()
{
  address = getValidStringInput("Nhap dia chi ho: ", 100, false);
  headId = getValidStringInput("Nhap ID chu ho: ", 12, false);
  cout << "Nhap trang thai dac biet:\n";
  cout << "0. Khong co\n1. Can ngheo\n2. Ngheo\n";
  auto statusChoice = getValidIntegerInput("Nhap lua chon: ", 0, 2);
  if (statusChoice)
  {
    switch (*statusChoice)
    {
    case 0:
      specialStatus = SpecialStatus::None;
      break;
    case 1:
      specialStatus = SpecialStatus::NearPoor;
      break;
    case 2:
      specialStatus = SpecialStatus::Poor;
      break;
    }
  }
  else
  {
    specialStatus = SpecialStatus::None;
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
  cout << "Nhap trang thai dac biet:\n";
  cout << "0. Khong co\n1. Can ngheo\n2. Ngheo\n";
  auto statusChoice = getValidIntegerInput("Nhap lua chon: ", 0, 2);
  if (statusChoice)
  {
    switch (*statusChoice)
    {
    case 0:
      specialStatus = SpecialStatus::None;
      break;
    case 1:
      specialStatus = SpecialStatus::NearPoor;
      break;
    case 2:
      specialStatus = SpecialStatus::Poor;
      break;
    }
  }
  else
  {
    specialStatus = SpecialStatus::None;
  }
}

string Household::getAddress() const { return address; }

string Household::getHeadId() const { return headId; }

SpecialStatus Household::getSpecialStatus() const { return specialStatus; }

const vector<string> &Household::getMemberIds() const { return memberIds; }

PermanentHousehold::PermanentHousehold(const string &addr, const string &hId,
                                       const vector<string> &memIds, SpecialStatus status)
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
  cout << "Trang thai dac biet: ";
  switch (specialStatus)
  {
  case SpecialStatus::None:
    cout << "Khong co";
    break;
  case SpecialStatus::NearPoor:
    cout << "Can ngheo";
    break;
  case SpecialStatus::Poor:
    cout << "Ngheo";
    break;
  }
  cout << endl;
}

string PermanentHousehold::getType() const { return "Permanent"; }

TemporaryHousehold::TemporaryHousehold(const string &addr, const string &hId,
                                       const vector<string> &memIds, SpecialStatus status, const Date &exp)
    : Household(addr, hId, memIds, status), expiryDate(exp) {}

void TemporaryHousehold::input()
{
  address = getValidStringInput("Nhap dia chi ho: ", 100, false);
  headId = getValidStringInput("Nhap ID chu ho: ", 12, false);
  cout << "Nhap trang thai dac biet:\n";
  cout << "0. Khong co\n1. Can ngheo\n2. Ngheo\n";
  auto statusChoice = getValidIntegerInput("Nhap lua chon: ", 0, 2);
  if (statusChoice)
  {
    switch (*statusChoice)
    {
    case 0:
      specialStatus = SpecialStatus::None;
      break;
    case 1:
      specialStatus = SpecialStatus::NearPoor;
      break;
    case 2:
      specialStatus = SpecialStatus::Poor;
      break;
    }
  }
  else
  {
    specialStatus = SpecialStatus::None;
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
  string errorMsg;
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
  cout << "Nhap trang thai dac biet:\n";
  cout << "0. Khong co\n1. Can ngheo\n2. Ngheo\n";
  auto statusChoice = getValidIntegerInput("Nhap lua chon: ", 0, 2);
  if (statusChoice)
  {
    switch (*statusChoice)
    {
    case 0:
      specialStatus = SpecialStatus::None;
      break;
    case 1:
      specialStatus = SpecialStatus::NearPoor;
      break;
    case 2:
      specialStatus = SpecialStatus::Poor;
      break;
    }
  }
  else
  {
    specialStatus = SpecialStatus::None;
  }
  string errorMsg;
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
  cout << "Trang thai dac biet: ";
  switch (specialStatus)
  {
  case SpecialStatus::None:
    cout << "Khong co";
    break;
  case SpecialStatus::NearPoor:
    cout << "Can ngheo";
    break;
  case SpecialStatus::Poor:
    cout << "Ngheo";
    break;
  }
  cout << endl;
}

string TemporaryHousehold::getType() const { return "Temporary"; }

Date TemporaryHousehold::getExpiryDate() const { return expiryDate; }