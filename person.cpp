#include "person.h"
#include "helpers.h"
#include <iostream>

using namespace std;

set<string> Person::usedIds;

Person::Person(const string &n, const Date &d, const string &id, Gender g)
    : name(n), dob(d), id(id), gender(g)
{
  if (!id.empty())
    usedIds.insert(id);
}

void Person::input()
{
  name = getValidStringInput("Nhap ten: ", 50, false);
  string dobStr;
  string errorMsg;
  do
  {
    dobStr = getValidStringInput("Nhap ngay sinh (dd/mm/yyyy, vi du: 01/01/1980): ", 10, false);
    dob = Date::fromString(dobStr, errorMsg);
    if (!dob.isValid())
    {
      cout << "Loi: " << errorMsg << " Vui long nhap lai.\n";
    }
  } while (!dob.isValid());
  string id;
  do
  {
    id = getValidStringInput("Nhap ID (9 hoac 12 chu so, de trong neu khong co): ", 12, true);
    if (!id.empty() && !isValidIdNumber(id, usedIds, errorMsg))
    {
      cout << "Loi: " << errorMsg << " Vui long nhap lai.\n";
    }
    else
    {
      break;
    }
  } while (true);
  if (!id.empty())
  {
    usedIds.erase(this->id);
    this->id = id;
    usedIds.insert(this->id);
  }
  gender = Gender::Other;
  auto genderChoice = getValidIntegerInput("Nhap gioi tinh (1: Nam, 2: Nu, 3: Khac): ", 1, 3);
  if (genderChoice)
  {
    if (*genderChoice == 1)
      gender = Gender::Male;
    else if (*genderChoice == 2)
      gender = Gender::Female;
  }
}

void Person::inputWithoutId()
{
  name = getValidStringInput("Nhap ten: ", 50, false);
  string dobStr;
  string errorMsg;
  do
  {
    dobStr = getValidStringInput("Nhap ngay sinh (dd/mm/yyyy, vi du: 01/01/1980): ", 10, false);
    dob = Date::fromString(dobStr, errorMsg);
    if (!dob.isValid())
    {
      cout << "Loi: " << errorMsg << " Vui long nhap lai.\n";
    }
  } while (!dob.isValid());
  gender = Gender::Other;
  auto genderChoice = getValidIntegerInput("Nhap gioi tinh (1: Nam, 2: Nu, 3: Khac): ", 1, 3);
  if (genderChoice)
  {
    if (*genderChoice == 1)
      gender = Gender::Male;
    else if (*genderChoice == 2)
      gender = Gender::Female;
  }
}

void Person::display() const
{
  cout << "Ten: " << name << endl;
  cout << "Ngay sinh: " << dob.toString() << endl;
  if (!id.empty())
    cout << "ID: " << id << endl;
  cout << "Gioi tinh: ";
  switch (gender)
  {
  case Gender::Male:
    cout << "Nam";
    break;
  case Gender::Female:
    cout << "Nu";
    break;
  case Gender::Other:
    cout << "Khac";
    break;
  }
}

void Person::edit()
{
  cout << "Chinh sua thong tin (ID khong the thay doi):\n";
  inputWithoutId();
}

string Person::getId() const { return id; }

const Date &Person::getDob() const
{
  return dob;
}

const std::string &Person::getName() const
{
  return name;
}

Gender Person::getGender() const { return gender; }

HouseholdHead::HouseholdHead(const string &n, const Date &d, const string &id, Gender g)
    : Person(n, d, id, g) {}

void HouseholdHead::input()
{
  Person::input();
}

void HouseholdHead::display() const
{
  Person::display();
}

FamilyMember::FamilyMember(const string &n, const Date &d, const string &id, Gender g, Relationship rel, const string &headId)
    : Person(n, d, id, g), relationship(rel), headId(headId) {}

void FamilyMember::input()
{
  Person::input();
  headId = getValidStringInput("Nhap ID cua chu ho: ", 12, false);
  cout << "Nhap quan he voi chu ho:\n";
  cout << "1. Vo/Chong\n2. Con\n3. Cha/Me\n4. Anh/Chi/Em\n5. Khac\n";
  auto relChoice = getValidIntegerInput("Nhap lua chon: ", 1, 5);
  if (!relChoice)
    return;
  switch (*relChoice)
  {
  case 1:
    relationship = Relationship::Spouse;
    break;
  case 2:
    relationship = Relationship::Child;
    break;
  case 3:
    relationship = Relationship::Parent;
    break;
  case 4:
    relationship = Relationship::Sibling;
    break;
  default:
    relationship = Relationship::Other;
    break;
  }
}

void FamilyMember::edit()
{
  cout << "Chinh sua thong tin (ID va ID chu ho khong the thay doi):\n";
  inputWithoutId();
  cout << "Nhap quan he voi chu ho:\n";
  cout << "1. Vo/Chong\n2. Con\n3. Cha/Me\n4. Anh/Chi/Em\n5. Khac\n";
  auto relChoice = getValidIntegerInput("Nhap lua chon: ", 1, 5);
  if (!relChoice)
    return;
  switch (*relChoice)
  {
  case 1:
    relationship = Relationship::Spouse;
    break;
  case 2:
    relationship = Relationship::Child;
    break;
  case 3:
    relationship = Relationship::Parent;
    break;
  case 4:
    relationship = Relationship::Sibling;
    break;
  default:
    relationship = Relationship::Other;
    break;
  }
}

void FamilyMember::display() const
{
  Person::display();
  cout << endl;
  cout << "Quan he voi chu ho: ";
  switch (relationship)
  {
  case Relationship::Spouse:
    cout << "Vo/Chong";
    break;
  case Relationship::Child:
    cout << "Con";
    break;
  case Relationship::Parent:
    cout << "Cha/Me";
    break;
  case Relationship::Sibling:
    cout << "Anh/Chi/Em";
    break;
  case Relationship::Other:
    cout << "Khac";
    break;
  }
  cout << endl;
  cout << "ID chu ho: " << headId;
}

Relationship FamilyMember::getRelationship() const { return relationship; }

string FamilyMember::getHeadId() const { return headId; }