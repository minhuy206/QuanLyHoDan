#ifndef HOUSEHOLD_H
#define HOUSEHOLD_H

#include "person.h"
#include <vector>
#include <string>

class Household
{
protected:
  std::string address;
  std::string headId;
  std::vector<std::string> memberIds;
  std::string specialStatus;

public:
  Household(const std::string &addr = "", const std::string &hId = "",
            const std::vector<std::string> &memIds = {}, const std::string &status = "");
  virtual ~Household() = default; // Add virtual destructor
  virtual void input();
  virtual void display() const = 0;
  virtual void edit();
  virtual std::string getType() const = 0;
  std::string getAddress() const;
  std::string getHeadId() const;
  std::string getSpecialStatus() const;
  const std::vector<std::string> &getMemberIds() const;
};

class PermanentHousehold : public Household
{
public:
  PermanentHousehold(const std::string &addr = "", const std::string &hId = "",
                     const std::vector<std::string> &memIds = {}, const std::string &status = "");
  void display() const override;
  std::string getType() const override;
};

class TemporaryHousehold : public Household
{
private:
  Date expiryDate;

public:
  TemporaryHousehold(const std::string &addr = "", const std::string &hId = "",
                     const std::vector<std::string> &memIds = {}, const std::string &status = "",
                     const Date &exp = {1, 1, 1900});
  void input() override;
  void display() const override;
  void edit() override;
  std::string getType() const override;
  Date getExpiryDate() const;
};

#endif // HOUSEHOLD_H