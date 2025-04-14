#include "helpers.h"
#include <sstream>
#include <iostream>
#include <ctime>
#include <regex>

using namespace std;

Date::Date(int d, int m, int y) : day(d), month(m), year(y) {}

string Date::toString() const
{
  char buffer[11];
  snprintf(buffer, sizeof(buffer), "%02d/%02d/%04d", day, month, year);
  return string(buffer);
}

Date Date::fromString(const string &str, string &errorMsg)
{
  regex datePattern(R"((\d{2})/(\d{2})/(\d{4}))");
  smatch match;
  if (!regex_match(str, match, datePattern))
  {
    errorMsg = "Dinh dang ngay phai la dd/mm/yyyy";
    return Date();
  }
  int d = stoi(match[1]);
  int m = stoi(match[2]);
  int y = stoi(match[3]);
  Date date(d, m, y);
  if (!date.isValid())
  {
    errorMsg = "Ngay khong hop le";
    return Date();
  }
  errorMsg = "";
  return date;
}

Date Date::currentDate()
{
  time_t now = time(nullptr);
  tm *ltm = localtime(&now);
  return Date(ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
}

bool Date::isValid() const
{
  if (month < 1 || month > 12 || day < 1 || year < 1900)
    return false;
  int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
  {
    daysInMonth[1] = 29;
  }
  return day <= daysInMonth[month - 1];
}

bool Date::isFuture() const
{
  Date current = currentDate();
  if (year > current.year)
    return true;
  if (year < current.year)
    return false;
  if (month > current.month)
    return true;
  if (month < current.month)
    return false;
  return day > current.day;
}

bool Date::isExpired() const
{
  return !isFuture();
}

vector<string> split(const string &str, char delimiter)
{
  vector<string> result;
  stringstream ss(str);
  string item;
  while (getline(ss, item, delimiter))
  {
    result.push_back(item);
  }
  return result;
}

bool isValidIdNumber(const string &id, const set<string> &usedIds, string &errorMsg)
{
  if (id.empty())
    return true;
  if (id.length() != 9 && id.length() != 12)
  {
    errorMsg = "ID phai co 9 hoac 12 chu so";
    return false;
  }
  if (!regex_match(id, regex("\\d+")))
  {
    errorMsg = "ID chi duoc chua chu so";
    return false;
  }
  if (usedIds.count(id))
  {
    errorMsg = "ID da duoc su dung";
    return false;
  }
  errorMsg = "";
  return true;
}

bool isValidSpecialStatus(const string &status, string &errorMsg)
{
  if (status.empty())
    return true;
  if (status != "Ngheo" && status != "Can ngheo")
  {
    errorMsg = "Trang thai dac biet chi duoc la 'Ngheo' hoac 'Can ngheo'";
    return false;
  }
  errorMsg = "";
  return true;
}

string getValidStringInput(const string &prompt, size_t maxLength, bool allowEmpty)
{
  string input;
  while (true)
  {
    cout << prompt;
    getline(cin, input);
    if (input.empty() && allowEmpty)
      return "";
    if (input.empty())
    {
      cout << "Loi: Khong duoc de trong!\n";
      continue;
    }
    if (input.length() > maxLength)
    {
      cout << "Loi: Do dai toi da la " << maxLength << " ky tu!\n";
      continue;
    }
    break;
  }
  return input;
}

optional<int> getValidIntegerInput(const string &prompt, int min, int max)
{
  string input;
  while (true)
  {
    cout << prompt;
    getline(cin, input);
    try
    {
      size_t pos;
      int value = stoi(input, &pos);
      if (pos != input.length())
        throw invalid_argument("Khong phai so nguyen");
      if (value < min || value > max)
      {
        cout << "Loi: Gia tri phai tu " << min << " den " << max << "!\n";
        continue;
      }
      return value;
    }
    catch (...)
    {
      cout << "Loi: Vui long nhap so nguyen hop le!\n";
    }
  }
}