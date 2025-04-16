#ifndef STREET_MANAGER_H
#define STREET_MANAGER_H

#include "household.h"
#include <vector>
#include <memory>
#include <map>

class StreetManager
{
private:
  std::vector<std::unique_ptr<Household>> households;
  std::map<std::string, HouseholdHead> heads;
  std::map<std::string, FamilyMember> familyMembers;
  bool isAddressUnique(const std::string &addr) const;

public:
  void addHousehold();
  void editHousehold();
  void deleteHousehold();
  void manageHouseholdMembers();
  void manageMembers();
  void findExpiredTemporaryHouseholds();
  void displayAll() const;
  void searchByAddress(const std::string &addr) const;
  void searchById(const std::string &id) const;
  void reportByType() const;
  bool saveToCsv() const;
  bool loadFromCsv();
  const HouseholdHead *getHead(const std::string &id) const;
  const FamilyMember *getMember(const std::string &id) const;
  void addMember();
  void addMemberWithOutId(std::string id, std::string headId);
  void editMember();
  void deleteMember();
};

#endif // STREET_MANAGER_H