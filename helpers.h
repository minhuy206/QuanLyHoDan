#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <vector>
#include <set>
#include <optional>

class Date
{
private:
  int day, month, year;

public:
  Date(int d = 1, int m = 1, int y = 1900);
  std::string toString() const;
  static Date fromString(const std::string &str, std::string &errorMsg);
  static Date currentDate();
  bool isValid() const;
  bool isFuture() const;
  bool isExpired() const;
};

std::vector<std::string> split(const std::string &str, char delimiter);
bool isValidIdNumber(const std::string &id, const std::set<std::string> &usedIds, std::string &errorMsg);
bool isValidSpecialStatus(const std::string &status, std::string &errorMsg);
std::string getValidStringInput(const std::string &prompt, size_t maxLength, bool allowEmpty);
std::optional<int> getValidIntegerInput(const std::string &prompt, int min, int max);

#endif // HELPERS_H