#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <regex>
#include <ctime>
using namespace std;

// Enum class cho giới tính
enum class Gender
{
  Male,
  Female,
  Other
};

// Enum class cho quan hệ với chủ hộ
enum class Relationship
{
  Spouse,  // Vợ/Chồng
  Child,   // Con
  Parent,  // Cha/Mẹ
  Sibling, // Anh/Chị/Em
  Other    // Khác
};

// Hàm lấy ngày hiện tại dưới dạng dd/mm/yyyy
string getCurrentDate()
{
  time_t now = time(nullptr);
  tm *localTime = localtime(&now);
  char buffer[11];
  strftime(buffer, sizeof(buffer), "%d/%m/%Y", localTime);
  return string(buffer);
}

// Hàm kiểm tra ngày sinh
bool isValidDate(const string &dob, string &errorMsg)
{
  // Kiểm tra định dạng dd/mm/yyyy
  regex datePattern(R"(\d{2}/\d{2}/\d{4})");
  if (!regex_match(dob, datePattern))
  {
    errorMsg = "Định dạng phải là dd/mm/yyyy.";
    return false;
  }

  // Tách ngày, tháng, năm
  int day, month, year;
  try
  {
    day = stoi(dob.substr(0, 2));
    month = stoi(dob.substr(3, 2));
    year = stoi(dob.substr(6, 4));
  }
  catch (...)
  {
    errorMsg = "Không thể phân tích ngày, tháng, năm.";
    return false;
  }

  // Kiểm tra năm (1900–hiện tại + 100 năm)
  time_t now = time(nullptr);
  tm *localTime = localtime(&now);
  int currentYear = 1900 + localTime->tm_year;
  if (year < 1900 || year > currentYear + 100)
  {
    errorMsg = "Năm phải từ 1900 đến " + to_string(currentYear + 100) + ".";
    return false;
  }

  // Kiểm tra tháng
  if (month < 1 || month > 12)
  {
    errorMsg = "Tháng phải từ 1 đến 12.";
    return false;
  }

  // Kiểm tra ngày theo tháng
  int maxDays;
  switch (month)
  {
  case 2: // Tháng 2
    maxDays = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0) ? 29 : 28;
    break;
  case 4:
  case 6:
  case 9:
  case 11: // Tháng 30 ngày
    maxDays = 30;
    break;
  default: // Tháng 31 ngày
    maxDays = 31;
    break;
  }

  if (day < 1 || day > maxDays)
  {
    errorMsg = "Ngày không hợp lệ: Tháng " + to_string(month) + " chỉ có tối đa " + to_string(maxDays) + " ngày.";
    return false;
  }

  errorMsg = "";
  return true;
}

// Hàm kiểm tra ngày ở tương lai (cho ngày hết hạn tạm trú)
bool isFutureDate(const string &date, string &errorMsg)
{
  if (!isValidDate(date, errorMsg))
  {
    return false;
  }

  // Lấy ngày hiện tại
  time_t now = time(nullptr);
  tm *localTime = localtime(&now);
  int currentDay = localTime->tm_mday;
  int currentMonth = localTime->tm_mon + 1;
  int currentYear = 1900 + localTime->tm_year;

  // Tách ngày, tháng, năm từ ngày nhập
  int day = stoi(date.substr(0, 2));
  int month = stoi(date.substr(3, 2));
  int year = stoi(date.substr(6, 4));

  // So sánh
  if (year < currentYear)
  {
    errorMsg = "Ngày phải ở tương lai so với ngày hiện tại (" + getCurrentDate() + ").";
    return false;
  }
  else if (year == currentYear)
  {
    if (month < currentMonth)
    {
      errorMsg = "Ngày phải ở tương lai so với ngày hiện tại (" + getCurrentDate() + ").";
      return false;
    }
    else if (month == currentMonth && day <= currentDay)
    {
      errorMsg = "Ngày phải ở tương lai so với ngày hiện tại (" + getCurrentDate() + ").";
      return false;
    }
  }

  errorMsg = "";
  return true;
}

// Hàm kiểm tra ngày đã hết hạn
bool isExpired(const string &date, string &errorMsg)
{
  if (!isValidDate(date, errorMsg))
  {
    return false;
  }

  // Lấy ngày hiện tại
  time_t now = time(nullptr);
  tm *localTime = localtime(&now);
  int currentDay = localTime->tm_mday;
  int currentMonth = localTime->tm_mon + 1;
  int currentYear = 1900 + localTime->tm_year;

  // Tách ngày, tháng, năm từ ngày nhập
  int day = stoi(date.substr(0, 2));
  int month = stoi(date.substr(3, 2));
  int year = stoi(date.substr(6, 4));

  // So sánh: Hết hạn nếu ngày ≤ ngày hiện tại
  if (year > currentYear)
  {
    return false;
  }
  else if (year == currentYear)
  {
    if (month > currentMonth)
    {
      return false;
    }
    else if (month == currentMonth && day > currentDay)
    {
      return false;
    }
  }

  errorMsg = "";
  return true;
}

// Hàm kiểm tra số CMND/CCCD
bool isValidIdNumber(const string &idNumber, const set<string> &usedIdNumbers, string &errorMsg)
{
  // Kiểm tra độ dài (9 cho CMND, 12 cho CCCD)
  if (idNumber.length() != 9 && idNumber.length() != 12)
  {
    errorMsg = "Số CMND phải có 9 chữ số, CCCD phải có 12 chữ số.";
    return false;
  }

  // Kiểm tra chỉ chứa số
  regex numberPattern(R"(\d+)");
  if (!regex_match(idNumber, numberPattern))
  {
    errorMsg = "Số CMND/CCCD chỉ được chứa chữ số.";
    return false;
  }

  // Kiểm tra trùng lặp
  if (usedIdNumbers.find(idNumber) != usedIdNumbers.end())
  {
    errorMsg = "Số CMND/CCCD đã tồn tại.";
    return false;
  }

  errorMsg = "";
  return true;
}

// Hàm kiểm tra trạng thái đặc biệt
bool isValidSpecialStatus(const string &status, string &errorMsg)
{
  if (status.empty() || status == "Ngheo" || status == "Can ngheo")
  {
    errorMsg = "";
    return true;
  }
  errorMsg = "Trạng thái đặc biệt chỉ được là 'Ngheo', 'Can ngheo', hoặc rỗng.";
  return false;
}

// Lớp đại diện cho một cá nhân
class Person
{
protected:
  string name;
  string dob;                       // Ngày sinh
  string idNumber;                  // Số CMND/CCCD
  Gender gender;                    // Giới tính
  static set<string> usedIdNumbers; // Lưu trữ các số CMND/CCCD đã sử dụng
public:
  Person(string n = "", string d = "", string id = "", Gender g = Gender::Other)
      : name(n), dob(d), idNumber(id), gender(g) {}

  virtual void input()
  {
    cout << "Nhap ho ten: ";
    getline(cin, name);

    // Kiểm tra ngày sinh
    string errorMsg;
    do
    {
      cout << "Nhap ngay sinh (dd/mm/yyyy): ";
      getline(cin, dob);
      if (!isValidDate(dob, errorMsg))
      {
        cout << "Loi: " << errorMsg << " Vui long nhap lai.\n";
      }
    } while (!isValidDate(dob, errorMsg));

    // Kiểm tra số CMND/CCCD
    do
    {
      cout << "Nhap so CMND/CCCD (9 hoac 12 so): ";
      getline(cin, idNumber);
      if (!isValidIdNumber(idNumber, usedIdNumbers, errorMsg))
      {
        cout << "Loi: " << errorMsg << " Vui long nhap lai.\n";
      }
      else
      {
        usedIdNumbers.insert(idNumber);
        break;
      }
    } while (true);

    cout << "Nhap gioi tinh (1: Nam, 2: Nu, 3: Khac): ";
    int g;
    cin >> g;
    cin.ignore();
    switch (g)
    {
    case 1:
      gender = Gender::Male;
      break;
    case 2:
      gender = Gender::Female;
      break;
    default:
      gender = Gender::Other;
      break;
    }
  }

  virtual void display() const
  {
    cout << "Ho ten: " << name << ", Ngay sinh: " << dob
         << ", CMND/CCCD: " << idNumber << ", Gioi tinh: ";
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

  string getIdNumber() const { return idNumber; }
  virtual void edit()
  {
    // Xóa số CMND/CCCD cũ khỏi danh sách
    usedIdNumbers.erase(idNumber);

    cout << "Chinh sua thong tin:\n";
    input();
  }

  virtual ~Person()
  {
    usedIdNumbers.erase(idNumber); // Xóa số CMND/CCCD khi đối tượng bị hủy
  }
};

// Khởi tạo set tĩnh
set<string> Person::usedIdNumbers;

// Lớp chủ hộ
class HouseholdHead : public Person
{
public:
  HouseholdHead(string n = "", string d = "", string id = "", Gender g = Gender::Other)
      : Person(n, d, id, g) {}

  void input() override
  {
    cout << "Nhap thong tin chu ho:\n";
    Person::input();
  }

  void display() const override
  {
    cout << "Chu ho: ";
    Person::display();
  }
};

// Lớp thành viên gia đình
class FamilyMember : public Person
{
private:
  Relationship relationship; // Quan hệ với chủ hộ
public:
  FamilyMember(string n = "", string d = "", string id = "", Gender g = Gender::Other,
               Relationship rel = Relationship::Other)
      : Person(n, d, id, g), relationship(rel) {}

  void input() override
  {
    cout << "Nhap thong tin thanh vien:\n";
    Person::input();
    cout << "Nhap quan he voi chu ho (1: Vo/Chong, 2: Con, 3: Cha/Me, 4: Anh/Chi/Em, 5: Khac): ";
    int rel;
    cin >> rel;
    cin.ignore();
    switch (rel)
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

  void display() const override
  {
    Person::display();
    cout << ", Quan he: ";
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
  }
};

// Lớp cơ sở trừu tượng cho hộ dân
class Household
{
protected:
  string address;               // Địa chỉ của hộ
  HouseholdHead head;           // Chủ hộ
  vector<FamilyMember> members; // Danh sách thành viên
  string specialStatus;         // Trạng thái đặc biệt
public:
  Household(string addr = "", HouseholdHead h = HouseholdHead(), string status = "")
      : address(addr), head(h), specialStatus(status) {}

  virtual void input()
  {
    cout << "Nhap dia chi ho: ";
    getline(cin, address);
    head.input();
    // Kiểm tra trạng thái đặc biệt
    string errorMsg;
    do
    {
      cout << "Nhap trang thai dac biet (Ngheo/Can ngheo, de trong neu khong co): ";
      getline(cin, specialStatus);
      if (!isValidSpecialStatus(specialStatus, errorMsg))
      {
        cout << "Loi: " << errorMsg << " Vui long nhap lai.\n";
      }
    } while (!isValidSpecialStatus(specialStatus, errorMsg));
    cout << "Nhap so thanh vien (khong tinh chu ho): ";
    int numMembers;
    cin >> numMembers;
    cin.ignore();
    members.clear();
    for (int i = 0; i < numMembers; i++)
    {
      cout << "Nhap thong tin thanh vien " << i + 1 << ":\n";
      FamilyMember fm;
      fm.input();
      members.push_back(fm);
    }
  }

  virtual void display() const = 0;
  virtual string getType() const = 0;

  string getAddress() const { return address; }
  string getHeadId() const { return head.getIdNumber(); }
  string getSpecialStatus() const { return specialStatus; }

  void addMember()
  {
    cout << "Them thanh vien moi:\n";
    FamilyMember fm;
    fm.input();
    members.push_back(fm);
    cout << "Them thanh vien thanh cong!\n";
  }

  void editMember()
  {
    if (members.empty())
    {
      cout << "Ho khong co thanh vien nao!\n";
      return;
    }
    cout << "Danh sach thanh vien:\n";
    for (size_t i = 0; i < members.size(); i++)
    {
      cout << i + 1 << ". ";
      members[i].display();
      cout << endl;
    }
    cout << "Nhap so thu tu thanh vien can chinh sua (1-" << members.size() << "): ";
    int index;
    cin >> index;
    cin.ignore();
    if (index >= 1 && index <= static_cast<int>(members.size()))
    {
      members[index - 1].edit();
      cout << "Chinh sua thanh vien thanh cong!\n";
    }
    else
    {
      cout << "So thu tu khong hop le!\n";
    }
  }

  void deleteMember()
  {
    if (members.empty())
    {
      cout << "Ho khong co thanh vien nao!\n";
      return;
    }
    cout << "Danh sach thanh vien:\n";
    for (size_t i = 0; i < members.size(); i++)
    {
      cout << i + 1 << ". ";
      members[i].display();
      cout << endl;
    }
    cout << "Nhap so thu tu thanh vien can xoa (1-" << members.size() << "): ";
    int index;
    cin >> index;
    cin.ignore();
    if (index >= 1 && index <= static_cast<int>(members.size()))
    {
      members.erase(members.begin() + (index - 1));
      cout << "Xoa thanh vien thanh cong!\n";
    }
    else
    {
      cout << "So thu tu khong hop le!\n";
    }
  }

  virtual void edit()
  {
    cout << "Chinh sua thong tin ho dan:\n";
    cout << "1. Chinh sua dia chi\n";
    cout << "2. Chinh sua thong tin chu ho\n";
    cout << "3. Quan ly thanh vien\n";
    cout << "4. Chinh sua trang thai dac biet\n";
    cout << "Nhap lua chon: ";
    int choice;
    cin >> choice;
    cin.ignore();
    string errorMsg;
    switch (choice)
    {
    case 1:
      cout << "Nhap dia chi moi: ";
      getline(cin, address);
      break;
    case 2:
      head.edit();
      break;
    case 3:
      manageMembers();
      break;
    case 4:
      do
      {
        cout << "Nhap trang thai dac biet moi (Ngheo/Can ngheo, de trong neu khong co): ";
        getline(cin, specialStatus);
        if (!isValidSpecialStatus(specialStatus, errorMsg))
        {
          cout << "Loi: " << errorMsg << " Vui long nhap lai.\n";
        }
      } while (!isValidSpecialStatus(specialStatus, errorMsg));
      break;
    default:
      cout << "Lua chon khong hop le!\n";
    }
  }

  void manageMembers()
  {
    int choice;
    do
    {
      cout << "\nQUAN LY THANH VIEN\n";
      cout << "1. Them thanh vien\n";
      cout << "2. Chinh sua thanh vien\n";
      cout << "3. Xoa thanh vien\n";
      cout << "0. Quay lai\n";
      cout << "Nhap lua chon: ";
      cin >> choice;
      cin.ignore();
      switch (choice)
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
      case 0:
        break;
      default:
        cout << "Lua chon khong hop le!\n";
      }
    } while (choice != 0);
  }

  const vector<FamilyMember> &getMembers() const { return members; }

  virtual ~Household() {}
};

// Lớp hộ thường trú
class PermanentHousehold : public Household
{
public:
  PermanentHousehold(string addr = "", HouseholdHead h = HouseholdHead(), string status = "")
      : Household(addr, h, status) {}

  void display() const override
  {
    cout << "Ho thuong tru - So nha: " << address << endl;
    head.display();
    cout << endl;
    cout << "So thanh vien: " << members.size() << endl;
    if (!members.empty())
    {
      cout << "Danh sach thanh vien:\n";
      for (size_t i = 0; i < members.size(); i++)
      {
        cout << "Thanh vien " << i + 1 << ": ";
        members[i].display();
        cout << endl;
      }
    }
    if (!specialStatus.empty())
      cout << "Trang thai dac biet: " << specialStatus << endl;
  }

  string getType() const override { return "Thuong tru"; }
};

// Lớp hộ tạm trú
class TemporaryHousehold : public Household
{
private:
  string expiryDate; // Ngày hết hạn tạm trú
public:
  TemporaryHousehold(string addr = "", HouseholdHead h = HouseholdHead(), string status = "", string exp = "")
      : Household(addr, h, status), expiryDate(exp) {}

  void input() override
  {
    Household::input();
    string errorMsg;
    cout << "Nhap ngay het han tam tru (dd/mm/yyyy, sau " << getCurrentDate() << "): ";
    do
    {
      getline(cin, expiryDate);
      if (!isFutureDate(expiryDate, errorMsg))
      {
        cout << "Loi: " << errorMsg << " Vui long nhap lai.\n";
      }
    } while (!isFutureDate(expiryDate, errorMsg));
  }

  void display() const override
  {
    cout << "Ho tam tru - So nha: " << address << endl;
    head.display();
    cout << endl;
    cout << "So thanh vien: " << members.size() << endl;
    if (!members.empty())
    {
      cout << "Danh sach thanh vien:\n";
      for (size_t i = 0; i < members.size(); i++)
      {
        cout << "Thanh vien " << i + 1 << ": ";
        members[i].display();
        cout << endl;
      }
    }
    cout << "Ngay het han tam tru: " << expiryDate << endl;
    if (!specialStatus.empty())
      cout << "Trang thai dac biet: " << specialStatus << endl;
  }

  void edit() override
  {
    cout << "Chinh sua thong tin ho dan:\n";
    cout << "1. Chinh sua dia chi\n";
    cout << "2. Chinh sua thong tin chu ho\n";
    cout << "3. Quan ly thanh vien\n";
    cout << "4. Chinh sua trang thai dac biet\n";
    cout << "5. Chinh sua ngay het han tam tru\n";
    cout << "Nhap lua chon: ";
    int choice;
    cin >> choice;
    cin.ignore();
    string errorMsg;
    if (choice == 5)
    {
      cout << "Nhap ngay het han tam tru moi (dd/mm/yyyy, sau " << getCurrentDate() << "): ";
      do
      {
        getline(cin, expiryDate);
        if (!isFutureDate(expiryDate, errorMsg))
        {
          cout << "Loi: " << errorMsg << " Vui long nhap lai.\n";
        }
      } while (!isFutureDate(expiryDate, errorMsg));
    }
    else
    {
      Household::edit();
      if (choice < 1 || choice > 4)
      {
        cout << "Lua chon khong hop le!\n";
      }
    }
  }

  string getType() const override { return "Tam tru"; }

  string getExpiryDate() const { return expiryDate; }
};

// Lớp quản lý khu phố
class StreetManager
{
private:
  vector<Household *> households;

public:
  ~StreetManager()
  {
    for (Household *h : households)
      delete h;
  }

  void addHousehold()
  {
    int type;
    cout << "Chon loai ho (1: Thuong tru, 2: Tam tru): ";
    cin >> type;
    cin.ignore();

    Household *h = nullptr;
    switch (type)
    {
    case 1:
      h = new PermanentHousehold();
      break;
    case 2:
      h = new TemporaryHousehold();
      break;
    default:
      cout << "Loai ho khong hop le!\n";
      return;
    }

    h->input();
    households.push_back(h);
    cout << "Them ho dan thanh cong!\n";
  }

  void editHousehold()
  {
    string searchTerm;
    cout << "Nhap dia chi hoac so CMND/CCCD chu ho de tim kiem: ";
    getline(cin, searchTerm);
    bool found = false;
    for (Household *h : households)
    {
      if (h->getAddress() == searchTerm || h->getHeadId() == searchTerm)
      {
        h->edit();
        found = true;
        cout << "Chinh sua ho dan thanh cong!\n";
        break;
      }
    }
    if (!found)
    {
      cout << "Khong tim thay ho dan voi dia chi hoac CMND/CCCD: " << searchTerm << endl;
    }
  }

  void deleteHousehold()
  {
    string searchTerm;
    cout << "Nhap dia chi hoac so CMND/CCCD chu ho de xoa: ";
    getline(cin, searchTerm);
    bool found = false;
    for (auto it = households.begin(); it != households.end(); ++it)
    {
      if ((*it)->getAddress() == searchTerm || (*it)->getHeadId() == searchTerm)
      {
        delete *it;
        households.erase(it);
        found = true;
        cout << "Xoa ho dan thanh cong!\n";
        break;
      }
    }
    if (!found)
    {
      cout << "Khong tim thay ho dan voi dia chi hoac CMND/CCCD: " << searchTerm << endl;
    }
  }

  void manageHouseholdMembers()
  {
    string searchTerm;
    cout << "Nhap dia chi hoac so CMND/CCCD chu ho de tim kiem: ";
    getline(cin, searchTerm);
    bool found = false;
    for (Household *h : households)
    {
      if (h->getAddress() == searchTerm || h->getHeadId() == searchTerm)
      {
        h->manageMembers();
        found = true;
        break;
      }
    }
    if (!found)
    {
      cout << "Khong tim thay ho dan voi dia chi hoac CMND/CCCD: " << searchTerm << endl;
    }
  }

  void findExpiredTemporaryHouseholds() const
  {
    bool found = false;
    cout << "Danh sach ho tam tru het han tam tru tinh den " << getCurrentDate() << ":\n";
    for (const Household *h : households)
    {
      if (h->getType() == "Tam tru")
      {
        const TemporaryHousehold *tempH = dynamic_cast<const TemporaryHousehold *>(h);
        string errorMsg;
        if (tempH && isExpired(tempH->getExpiryDate(), errorMsg))
        {
          h->display();
          cout << "-------------------\n";
          found = true;
        }
      }
    }
    if (!found)
    {
      cout << "Khong co ho tam tru nao het han.\n";
    }
  }

  void displayAll() const
  {
    if (households.empty())
    {
      cout << "Khong co ho dan nao!\n";
      return;
    }
    for (const Household *h : households)
    {
      h->display();
      cout << "-------------------\n";
    }
  }

  void searchByAddress(const string &addr) const
  {
    bool found = false;
    for (const Household *h : households)
    {
      if (h->getAddress() == addr)
      {
        h->display();
        found = true;
      }
    }
    if (!found)
      cout << "Khong tim thay ho dan voi so nha: " << addr << endl;
  }

  void searchByIdNumber(const string &id) const
  {
    bool found = false;
    for (const Household *h : households)
    {
      if (h->getHeadId() == id)
      {
        h->display();
        found = true;
      }
      else
      {
        for (const FamilyMember &fm : h->getMembers())
        {
          if (fm.getIdNumber() == id)
          {
            h->display();
            found = true;
            break;
          }
        }
      }
    }
    if (!found)
      cout << "Khong tim thay ho dan voi CMND/CCCD: " << id << endl;
  }

  void reportByType() const
  {
    int permCount = 0, tempCount = 0, poorCount = 0, nearPoorCount = 0;
    for (const Household *h : households)
    {
      if (h->getType() == "Thuong tru")
        permCount++;
      else if (h->getType() == "Tam tru")
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
};

// Hàm chính
int main()
{
  StreetManager manager;
  int choice;

  do
  {
    cout << "\nQUAN LY HO DAN KHU PHO\n";
    cout << "1. Them ho dan\n";
    cout << "2. Chinh sua ho dan\n";
    cout << "3. Xoa ho dan\n";
    cout << "4. Quan ly thanh vien\n";
    cout << "5. Tim ho tam tru het han\n";
    cout << "6. Hien thi tat ca ho dan\n";
    cout << "7. Tim kiem theo so nha\n";
    cout << "8. Tim kiem theo CMND/CCCD\n";
    cout << "9. Thong ke theo loai ho\n";
    cout << "0. Thoat\n";
    cout << "Nhap lua chon: ";
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
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
      manager.findExpiredTemporaryHouseholds();
      break;
    case 6:
      manager.displayAll();
      break;
    case 7:
    {
      string addr;
      cout << "Nhap so nha can tim: ";
      getline(cin, addr);
      manager.searchByAddress(addr);
      break;
    }
    case 8:
    {
      string id;
      cout << "Nhap so CMND/CCCD can tim: ";
      getline(cin, id);
      manager.searchByIdNumber(id);
      break;
    }
    case 9:
      manager.reportByType();
      break;
    case 0:
      cout << "Tam biet!\n";
      break;
    default:
      cout << "Lua chon khong hop le!\n";
    }
  } while (choice != 0);

  return 0;
}