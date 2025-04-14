#include "street_manager.h"
#include "helpers.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

bool StreetManager::isAddressUnique(const string &addr) const
{
  for (const auto &h : households)
  {
    if (h->getAddress() == addr)
    {
      return false;
    }
  }
  return true;
}

const HouseholdHead *StreetManager::getHead(const string &id) const
{
  auto it = heads.find(id);
  return it != heads.end() ? &it->second : nullptr;
}

const FamilyMember *StreetManager::getMember(const string &id) const
{
  auto it = familyMembers.find(id);
  return it != familyMembers.end() ? &it->second : nullptr;
}

void StreetManager::addHousehold()
{
  auto type = getValidIntegerInput("Chon loai ho (1: Thuong tru, 2: Tam tru): ", 1, 2);
  if (!type)
    return;

  unique_ptr<Household> h;
  string addr = getValidStringInput("Nhap dia chi ho: ", 100, false);
  if (!isAddressUnique(addr))
  {
    cout << "Loi: Dia chi " << addr << " da ton tai!\n";
    return;
  }

  HouseholdHead head;
  cout << "Nhap thong tin chu ho:\n";
  head.input();
  string headId = head.getId();
  if (heads.find(headId) != heads.end())
  {
    cout << "Loi: ID chu ho " << headId << " da ton tai!\n";
    return;
  }
  heads[headId] = head;

  string status = getValidStringInput("Nhap trang thai dac biet (Ngheo/Can ngheo, de trong neu khong co): ", 20, true);
  string errorMsg;
  if (!isValidSpecialStatus(status, errorMsg))
  {
    cout << "Loi: " << errorMsg << " Trang thai se duoc de trong.\n";
    status = "";
  }

  vector<string> memberIds;
  auto numMembers = getValidIntegerInput("Nhap so thanh vien (khong tinh chu ho): ", 0, 100);
  if (numMembers)
  {
    for (int i = 0; i < *numMembers; ++i)
    {
      string memId;
      do
      {
        memId = getValidStringInput("Nhap ID thanh vien " + to_string(i + 1) + ": ", 12, false);
        if (familyMembers.find(memId) == familyMembers.end())
        {
          cout << "Loi: ID thanh vien " << memId << " khong ton tai! Vui long them thanh vien truoc.\n";
        }
        else if (familyMembers[memId].getHeadId() != headId)
        {
          cout << "Loi: ID chu ho cua thanh vien (" << familyMembers[memId].getHeadId() << ") khong khop voi chu ho (" << headId << ").\n";
        }
        else
        {
          break;
        }
      } while (true);
      memberIds.push_back(memId);
    }
  }

  if (*type == 1)
  {
    h = make_unique<PermanentHousehold>(addr, headId, memberIds, status);
  }
  else
  {
    Date expiryDate;
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
    h = make_unique<TemporaryHousehold>(addr, headId, memberIds, status, expiryDate);
  }
  households.push_back(move(h));
  cout << "Them ho dan thanh cong!\n";
  saveToCsv();
}

void StreetManager::editHousehold()
{
  string searchTerm = getValidStringInput("Nhap dia chi hoac ID chu ho de tim kiem: ", 100, false);
  if (searchTerm.empty())
    return;
  for (auto &h : households)
  {
    if (h->getAddress() == searchTerm || h->getHeadId() == searchTerm)
    {
      string oldAddr = h->getAddress();
      h->edit();
      if (h->getAddress() != oldAddr && !isAddressUnique(h->getAddress()))
      {
        cout << "Loi: Dia chi moi " << h->getAddress() << " da ton tai! Quay lai dia chi cu.\n";
        h->edit();
      }
      cout << "Chinh sua ho dan thanh cong!\n";
      saveToCsv();
      return;
    }
  }
  cout << "Khong tim thay ho dan voi dia chi hoac ID: " << searchTerm << endl;
}

void StreetManager::deleteHousehold()
{
  string searchTerm = getValidStringInput("Nhap dia chi hoac ID chu ho de xoa: ", 100, false);
  if (searchTerm.empty())
    return;
  for (auto it = households.begin(); it != households.end(); ++it)
  {
    if ((*it)->getAddress() == searchTerm || (*it)->getHeadId() == searchTerm)
    {
      auto confirm = getValidIntegerInput("Ban co chac muon xoa ho dan nay? (1: Co, 0: Khong): ", 0, 1);
      if (confirm && *confirm == 1)
      {
        heads.erase((*it)->getHeadId());
        households.erase(it);
        cout << "Xoa ho dan thanh cong!\n";
        saveToCsv();
      }
      else
      {
        cout << "Huy thao tac xoa.\n";
      }
      return;
    }
  }
  cout << "Khong tim thay ho dan voi dia chi hoac ID: " << searchTerm << endl;
}

void StreetManager::manageHouseholdMembers()
{
  string searchTerm = getValidStringInput("Nhap dia chi hoac ID chu ho de tim kiem: ", 100, false);
  if (searchTerm.empty())
    return;
  for (auto &h : households)
  {
    if (h->getAddress() == searchTerm || h->getHeadId() == searchTerm)
    {
      while (true)
      {
        cout << "\nQUAN LY THANH VIEN HO\n";
        cout << "1. Them thanh vien vao ho\n";
        cout << "2. Xoa thanh vien khoi ho\n";
        cout << "0. Quay lai\n";
        auto choice = getValidIntegerInput("Nhap lua chon: ", 0, 2);
        if (!choice || *choice == 0)
          break;
        switch (*choice)
        {
        case 1:
        {
          string memId;
          do
          {
            memId = getValidStringInput("Nhap ID thanh vien: ", 12, false);
            if (familyMembers.find(memId) == familyMembers.end())
            {
              cout << "Loi: ID thanh vien " << memId << " khong ton tai! Vui long them thanh vien truoc.\n";
            }
            else if (familyMembers[memId].getHeadId() != h->getHeadId())
            {
              cout << "Loi: ID chu ho cua thanh vien (" << familyMembers[memId].getHeadId() << ") khong khop voi chu ho (" << h->getHeadId() << ").\n";
            }
            else
            {
              break;
            }
          } while (true);
          auto &memIds = const_cast<vector<string> &>(h->getMemberIds());
          memIds.push_back(memId);
          cout << "Them thanh vien vao ho thanh cong!\n";
          break;
        }
        case 2:
        {
          if (h->getMemberIds().empty())
          {
            cout << "Ho dan khong co thanh vien de xoa.\n";
            break;
          }
          cout << "Danh sach ID thanh vien:\n";
          for (size_t i = 0; i < h->getMemberIds().size(); ++i)
          {
            cout << i + 1 << ": " << h->getMemberIds()[i] << endl;
          }
          auto index = getValidIntegerInput("Nhap so thu tu thanh vien can xoa: ", 1, h->getMemberIds().size());
          if (!index)
            break;
          auto &memIds = const_cast<vector<string> &>(h->getMemberIds());
          memIds.erase(memIds.begin() + *index - 1);
          cout << "Xoa thanh vien khoi ho thanh cong!\n";
          break;
        }
        }
      }
      saveToCsv();
      return;
    }
  }
  cout << "Khong tim thay ho dan voi dia chi hoac ID: " << searchTerm << endl;
}

void StreetManager::manageMembers()
{
  while (true)
  {
    cout << "\nQUAN LY THANH VIEN\n";
    cout << "1. Them thanh vien\n";
    cout << "2. Chinh sua thanh vien\n";
    cout << "3. Xoa thanh vien\n";
    cout << "0. Quay lai\n";
    auto choice = getValidIntegerInput("Nhap lua chon: ", 0, 3);
    if (!choice || *choice == 0)
      break;
    switch (*choice)
    {
    case 1:
      addMember();
      break;
    case 2:
      editMember();
      break;
    case 3:
      deleteMember();
      break;
    }
  }
}

void StreetManager::addMember()
{
  FamilyMember fm;
  cout << "Nhap thong tin thanh vien:\n";
  fm.input();
  string memId = fm.getId();
  if (familyMembers.find(memId) != familyMembers.end())
  {
    cout << "Loi: ID thanh vien " << memId << " da ton tai!\n";
    return;
  }
  string headId = fm.getHeadId();
  if (heads.find(headId) == heads.end())
  {
    cout << "Loi: ID chu ho " << headId << " khong ton tai!\n";
    return;
  }
  familyMembers[memId] = fm;
  cout << "Them thanh vien thanh cong!\n";
  saveToCsv();
}

void StreetManager::editMember()
{
  string memId = getValidStringInput("Nhap ID thanh vien can chinh sua: ", 12, false);
  if (familyMembers.find(memId) == familyMembers.end())
  {
    cout << "Khong tim thay thanh vien voi ID: " << memId << endl;
    return;
  }
  cout << "Chinh sua thong tin thanh vien:\n";
  familyMembers[memId].edit();
  cout << "Chinh sua thanh vien thanh cong!\n";
  saveToCsv();
}

void StreetManager::deleteMember()
{
  string memId = getValidStringInput("Nhap ID thanh vien can xoa: ", 12, false);
  if (familyMembers.find(memId) == familyMembers.end())
  {
    cout << "Khong tim thay thanh vien voi ID: " << memId << endl;
    return;
  }
  auto confirm = getValidIntegerInput("Ban co chac muon xoa thanh vien nay? (1: Co, 0: Khong): ", 0, 1);
  if (confirm && *confirm == 1)
  {
    for (auto &h : households)
    {
      auto &memIds = const_cast<vector<string> &>(h->getMemberIds());
      memIds.erase(remove(memIds.begin(), memIds.end(), memId), memIds.end());
    }
    familyMembers.erase(memId);
    cout << "Xoa thanh vien thanh cong!\n";
    saveToCsv();
  }
  else
  {
    cout << "Huy thao tac xoa.\n";
  }
}

void StreetManager::findExpiredTemporaryHouseholds()
{
  bool found = false;
  vector<size_t> expiredIndices;
  cout << "Danh sach ho tam tru het han tinh den " << Date::currentDate().toString() << ":\n";
  for (size_t i = 0; i < households.size(); ++i)
  {
    if (households[i]->getType() == "Temporary")
    {
      auto tempH = dynamic_cast<TemporaryHousehold *>(households[i].get());
      if (tempH && tempH->getExpiryDate().isExpired())
      {
        households[i]->display();
        cout << "-------------------\n";
        expiredIndices.push_back(i);
        found = true;
      }
    }
  }
  if (!found)
  {
    cout << "Khong co ho tam tru nao het han.\n";
    return;
  }
  auto confirm = getValidIntegerInput("Ban co chac muon xoa tat ca ho tam tru het han? (1: Co, 0: Khong): ", 0, 1);
  if (confirm && *confirm == 1)
  {
    for (auto it = expiredIndices.rbegin(); it != expiredIndices.rend(); ++it)
    {
      heads.erase(households[*it]->getHeadId());
      households.erase(households.begin() + *it);
    }
    cout << "Da xoa tat ca ho tam tru het han.\n";
    saveToCsv();
  }
}

void StreetManager::displayAll() const
{
  if (households.empty())
  {
    cout << "Khong co ho dan nao!\n";
    return;
  }
  for (const auto &h : households)
  {
    h->display();
    cout << "Chu ho:\n";
    if (auto head = getHead(h->getHeadId()))
    {
      head->display();
    }
    else
    {
      cout << "Khong tim thay thong tin chu ho voi ID: " << h->getHeadId() << endl;
    }
    if (!h->getMemberIds().empty())
    {
      cout << "Thanh vien:\n";
      for (const auto &memId : h->getMemberIds())
      {
        if (auto fm = getMember(memId))
        {
          fm->display();
          cout << endl;
        }
        else
        {
          cout << "Khong tim thay thanh vien voi ID: " << memId << endl;
        }
      }
    }
    cout << "-------------------\n";
  }
}

void StreetManager::searchByAddress(const string &addr) const
{
  bool found = false;
  for (const auto &h : households)
  {
    if (h->getAddress() == addr)
    {
      h->display();
      cout << "Chu ho:\n";
      if (auto head = getHead(h->getHeadId()))
      {
        head->display();
      }
      else
      {
        cout << "Khong tim thay thong tin chu ho voi ID: " << h->getHeadId() << endl;
      }
      if (!h->getMemberIds().empty())
      {
        cout << "Thanh vien:\n";
        for (const auto &memId : h->getMemberIds())
        {
          if (auto fm = getMember(memId))
          {
            fm->display();
            cout << endl;
          }
          else
          {
            cout << "Khong tim thay thanh vien voi ID: " << memId << endl;
          }
        }
      }
      found = true;
    }
  }
  if (!found)
    cout << "Khong tim thay ho dan voi so nha: " << addr << endl;
}

void StreetManager::searchById(const string &id) const
{
  bool found = false;
  for (const auto &h : households)
  {
    if (h->getHeadId() == id)
    {
      h->display();
      cout << "Chu ho:\n";
      if (auto head = getHead(h->getHeadId()))
      {
        head->display();
      }
      if (!h->getMemberIds().empty())
      {
        cout << "Thanh vien:\n";
        for (const auto &memId : h->getMemberIds())
        {
          if (auto fm = getMember(memId))
          {
            fm->display();
            cout << endl;
          }
        }
      }
      found = true;
    }
    else
    {
      for (const auto &memId : h->getMemberIds())
      {
        if (memId == id)
        {
          h->display();
          cout << "Chu ho:\n";
          if (auto head = getHead(h->getHeadId()))
          {
            head->display();
          }
          cout << "Thanh vien:\n";
          if (auto fm = getMember(memId))
          {
            fm->display();
            cout << endl;
          }
          found = true;
          break;
        }
      }
    }
  }
  if (!found)
    cout << "Khong tim thay ho dan voi ID: " << id << endl;
}

void StreetManager::reportByType() const
{
  int permCount = 0, tempCount = 0, poorCount = 0, nearPoorCount = 0;
  for (const auto &h : households)
  {
    if (h->getType() == "Permanent")
      permCount++;
    else if (h->getType() == "Temporary")
      tempCount++;
    if (h->getSpecialStatus() == "Ngheo")
      poorCount++;
    else if (h->getSpecialStatus() == "Can ngheo")
      nearPoorCount++;
  }
  cout << "Thong ke:\n";
  cout << "Ho thuong tru: " << permCount << endl;
  cout << "Ho tam tru: " << tempCount << endl;
  cout << "Ho ngheo: " << poorCount << endl;
  cout << "Ho can ngheo: " << nearPoorCount << endl;
}

bool StreetManager::saveToCsv() const
{
  {
    ofstream file("household_heads.csv");
    if (!file.is_open())
    {
      cout << "Loi: Khong the mo file household_heads.csv de ghi.\n";
      return false;
    }
    file << "Name;Dob;Id;Gender\n";
    for (const auto &[id, head] : heads)
    {
      file << head.getName() << ";";
      file << head.getDob().toString() << ";";
      file << id << ";";
      switch (head.getGender())
      {
      case Gender::Male:
        file << "Male";
        break;
      case Gender::Female:
        file << "Female";
        break;
      default:
        file << "Other";
        break;
      }
      file << "\n";
    }
    file.close();
  }

  {
    ofstream file("family_members.csv");
    if (!file.is_open())
    {
      cout << "Loi: Khong the mo file family_members.csv de ghi.\n";
      return false;
    }
    file << "Name;Dob;Id;Gender;Relationship;HeadId\n";
    for (const auto &[id, fm] : familyMembers)
    {
      file << fm.getName() << ";";
      file << fm.getDob().toString() << ";";
      file << id << ";";
      switch (fm.getGender())
      {
      case Gender::Male:
        file << "Male";
        break;
      case Gender::Female:
        file << "Female";
        break;
      default:
        file << "Other";
        break;
      }
      file << ";";
      switch (fm.getRelationship())
      {
      case Relationship::Spouse:
        file << "Spouse";
        break;
      case Relationship::Child:
        file << "Child";
        break;
      case Relationship::Parent:
        file << "Parent";
        break;
      case Relationship::Sibling:
        file << "Sibling";
        break;
      default:
        file << "Other";
        break;
      }
      file << ";";
      file << fm.getHeadId() << "\n";
    }
    file.close();
  }

  {
    ofstream file("households.csv");
    if (!file.is_open())
    {
      cout << "Loi: Khong the mo file households.csv de ghi.\n";
      return false;
    }
    file << "Address;Type;SpecialStatus;ExpiryDate;HeadId;MemberIds\n";
    for (const auto &h : households)
    {
      file << h->getAddress() << ";";
      file << h->getType() << ";";
      file << h->getSpecialStatus() << ";";
      if (h->getType() == "Temporary")
      {
        auto tempH = dynamic_cast<const TemporaryHousehold *>(h.get());
        file << tempH->getExpiryDate().toString();
      }
      file << ";";
      file << h->getHeadId() << ";";
      const auto &memIds = h->getMemberIds();
      for (size_t i = 0; i < memIds.size(); ++i)
      {
        file << memIds[i];
        if (i < memIds.size() - 1)
          file << ",";
      }
      file << "\n";
    }
    file.close();
  }

  cout << "Da luu du lieu vao households.csv, family_members.csv, household_heads.csv.\n";
  return true;
}

bool StreetManager::loadFromCsv()
{
  households.clear();
  heads.clear();
  familyMembers.clear();
  set<string> usedIds;
  set<string> usedAddresses;

  {
    ifstream file("household_heads.csv");
    if (!file.is_open())
    {
      cout << "Canh bao: Khong tim thay file household_heads.csv. Tao danh sach rong.\n";
    }
    else
    {
      string line;
      bool firstLine = true;
      while (getline(file, line))
      {
        if (firstLine)
        {
          firstLine = false;
          continue;
        }
        if (line.empty())
          continue;
        vector<string> fields = split(line, ';');
        if (fields.size() < 4)
        {
          cout << "Canh bao: Dong khong hop le trong household_heads.csv: " << line << "\n";
          continue;
        }
        string name = fields[0];
        string dobStr = fields[1];
        string id = fields[2];
        string genderStr = fields[3];

        string errorMsg;
        if (!isValidIdNumber(id, usedIds, errorMsg))
        {
          cout << "Canh bao: ID chu ho khong hop le: " << errorMsg << "\n";
          continue;
        }
        usedIds.insert(id);

        Date dob = Date::fromString(dobStr, errorMsg);
        if (!dob.isValid())
        {
          cout << "Canh bao: Ngay sinh khong hop le: " << errorMsg << "\n";
          continue;
        }

        Gender gender;
        if (genderStr == "Male")
          gender = Gender::Male;
        else if (genderStr == "Female")
          gender = Gender::Female;
        else
          gender = Gender::Other;

        heads[id] = HouseholdHead(name, dob, id, gender);
      }
      file.close();
    }
  }

  {
    ifstream file("family_members.csv");
    if (!file.is_open())
    {
      cout << "Canh bao: Khong tim thay file family_members.csv. Tao danh sach rong.\n";
    }
    else
    {
      string line;
      bool firstLine = true;
      while (getline(file, line))
      {
        if (firstLine)
        {
          firstLine = false;
          continue;
        }
        if (line.empty())
          continue;
        vector<string> fields = split(line, ';');
        if (fields.size() < 6)
        {
          cout << "Canh bao: Dong khong hop le trong family_members.csv: " << line << "\n";
          continue;
        }
        string name = fields[0];
        string dobStr = fields[1];
        string id = fields[2];
        string genderStr = fields[3];
        string relStr = fields[4];
        string headId = fields[5];

        string errorMsg;
        if (!isValidIdNumber(id, usedIds, errorMsg))
        {
          cout << "Canh bao: ID thanh vien khong hop le: " << errorMsg << "\n";
          continue;
        }
        usedIds.insert(id);

        Date dob = Date::fromString(dobStr, errorMsg);
        if (!dob.isValid())
        {
          cout << "Canh bao: Ngay sinh khong hop le: " << errorMsg << "\n";
          continue;
        }

        Gender gender;
        if (genderStr == "Male")
          gender = Gender::Male;
        else if (genderStr == "Female")
          gender = Gender::Female;
        else
          gender = Gender::Other;

        Relationship rel;
        if (relStr == "Spouse")
          rel = Relationship::Spouse;
        else if (relStr == "Child")
          rel = Relationship::Child;
        else if (relStr == "Parent")
          rel = Relationship::Parent;
        else if (relStr == "Sibling")
          rel = Relationship::Sibling;
        else
          rel = Relationship::Other;

        if (heads.find(headId) == heads.end())
        {
          cout << "Canh bao: ID chu ho " << headId << " khong ton tai cho thanh vien " << id << "\n";
          continue;
        }

        familyMembers[id] = FamilyMember(name, dob, id, gender, rel, headId);
      }
      file.close();
    }
  }

  {
    ifstream file("households.csv");
    if (!file.is_open())
    {
      cout << "Canh bao: Khong tim thay file households.csv. Tao danh sach rong.\n";
      return false;
    }
    string line;
    bool firstLine = true;
    while (getline(file, line))
    {
      if (firstLine)
      {
        firstLine = false;
        continue;
      }
      if (line.empty())
        continue;
      vector<string> fields = split(line, ';');
      if (fields.size() < 5)
      {
        cout << "Canh bao: Dong khong hop le trong households.csv: " << line << "\n";
        continue;
      }
      string address = fields[0];
      string type = fields[1];
      string specialStatus = fields[2];
      string expiryDateStr = fields[3];
      string headId = fields[4];
      string memberIdsStr = fields.size() > 5 ? fields[5] : "";

      string errorMsg;
      if (!isValidSpecialStatus(specialStatus, errorMsg))
      {
        cout << "Canh bao: Trang thai dac biet khong hop le cho ho tai " << address << ": " << errorMsg << "\n";
        continue;
      }

      if (usedAddresses.count(address))
      {
        cout << "Canh bao: Dia chi " << address << " da ton tai!\n";
        continue;
      }
      usedAddresses.insert(address);

      if (heads.find(headId) == heads.end())
      {
        cout << "Canh bao: ID chu ho " << headId << " khong ton tai cho ho tai " << address << "\n";
        continue;
      }

      vector<string> memberIds;
      if (!memberIdsStr.empty())
      {
        memberIds = split(memberIdsStr, ',');
        for (const auto &memId : memberIds)
        {
          if (familyMembers.find(memId) == familyMembers.end())
          {
            cout << "Canh bao: ID thanh vien " << memId << " khong ton tai cho ho tai " << address << "\n";
          }
          else if (familyMembers[memId].getHeadId() != headId)
          {
            cout << "Canh bao: ID chu ho cua thanh vien (" << familyMembers[memId].getHeadId() << ") khong khop voi chu ho (" << headId << ") cho ho tai " << address << "\n";
          }
        }
      }

      unique_ptr<Household> h;
      if (type == "Permanent")
      {
        h = make_unique<PermanentHousehold>(address, headId, memberIds, specialStatus);
      }
      else if (type == "Temporary")
      {
        Date expiryDate = Date::fromString(expiryDateStr, errorMsg);
        if (!expiryDate.isValid() || !expiryDate.isFuture())
        {
          cout << "Canh bao: Ngay het han tam tru khong hop le cho ho tai " << address << ": " << errorMsg << "\n";
          continue;
        }
        h = make_unique<TemporaryHousehold>(address, headId, memberIds, specialStatus, expiryDate);
      }
      else
      {
        cout << "Canh bao: Loai ho khong hop le cho ho tai " << address << "\n";
        continue;
      }

      households.push_back(move(h));
    }
    file.close();
  }

  cout << "Da doc du lieu tu households.csv, family_members.csv, household_heads.csv.\n";
  return true;
}