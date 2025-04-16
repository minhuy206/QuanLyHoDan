#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <set>
#include "helpers.h"

enum class Gender
{
  Male,
  Female,
  Other
};

class Person
{

protected:
  std::string name;
  Date dob;
  std::string id;
  Gender gender;
  void inputWithoutId();
  static std::set<std::string> usedIds;

public:
  Person(const std::string &n = "", const Date &d = {1, 1, 1900},
         const std::string &id = "", Gender g = Gender::Other);
  virtual void input();
  virtual void display() const;
  virtual void edit();
  std::string getId() const;
  const Date &getDob() const;
  const std::string &getName() const;
  Gender getGender() const;
  virtual ~Person() {}
};

class HouseholdHead : public Person
{
public:
  HouseholdHead(const std::string &n = "", const Date &d = {1, 1, 1900},
                const std::string &id = "", Gender g = Gender::Other);
  void input() override;
  void display() const override;
};

enum class Relationship
{
  Spouse,
  Child,
  Parent,
  Sibling,
  None
};

class FamilyMember : public Person
{
private:
  Relationship relationship;
  std::string headId;

public:
  FamilyMember(const std::string &n = "", const Date &d = {1, 1, 1900},
               const std::string &id = "", Gender g = Gender::Other,
               Relationship rel = Relationship::None,
               const std::string &headId = "");
  void input() override;
  void inputHeadIdAndRelationship(std::string headId);
  void inputWithoutId(std::string id, std::string headId);
  void display() const override;
  void edit() override;
  Relationship getRelationship() const;
  std::string getHeadId() const;
};

#endif // PERSON_H