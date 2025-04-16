#ifndef HOUSEHOLD_H
#define HOUSEHOLD_H

#include "person.h"
#include <vector>
#include <string>

enum class SpecialStatus
{
  None,
  NearPoor,
  Poor
};

class Household
{
protected:
  std::string address;
  std::string headId;
  std::vector<std::string> memberIds;
  SpecialStatus specialStatus;
  static std::set<std::string> usedAddresses;

public:
  Household(const std::string &addr = "", const std::string &hId = "",
            const std::vector<std::string> &memIds = {}, SpecialStatus status = SpecialStatus::None);
  virtual ~Household() = default;
  virtual void input();
  virtual void display() const = 0;
  virtual void edit();
  virtual std::string getType() const = 0;
  std::string getAddress() const;
  std::string getHeadId() const;
  SpecialStatus getSpecialStatus() const;
  const std::vector<std::string> &getMemberIds() const;
};

class PermanentHousehold : public Household
{
public:
  PermanentHousehold(const std::string &addr = "", const std::string &hId = "",
                     const std::vector<std::string> &memIds = {}, SpecialStatus status = SpecialStatus::None);
  void display() const override;
  std::string getType() const override;
};

class TemporaryHousehold : public Household
{
private:
  Date expiryDate;

public:
  TemporaryHousehold(const std::string &addr = "", const std::string &hId = "",
                     const std::vector<std::string> &memIds = {}, SpecialStatus status = SpecialStatus::None,
                     const Date &exp = {1, 1, 1900});
  void input() override;
  void display() const override;
  void edit() override;
  std::string getType() const override;
  Date getExpiryDate() const;
};

#endif // HOUSEHOLD_H