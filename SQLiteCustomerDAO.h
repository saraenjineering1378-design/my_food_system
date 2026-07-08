#ifndef SQLITECUSTOMERDAO_H
#define SQLITECUSTOMERDAO_H

#include "ICustomerDAO.h"
#include "DatabaseManager.h"
#include "Customer.h"
#include <vector>
#include <string>

class SQLiteCustomerDAO : public ICustomerDAO 
{
private:
    DatabaseManager& dbManager;

public:
    SQLiteCustomerDAO(DatabaseManager& db) : dbManager(db) {}

    void addCustomer(Customer* customer) override;
    Customer* findCustomerById(int id) const override; 
    std::vector<Customer*> getAllCustomers() const override; 
    void updateWallet(int customerId, double newBalance) override;
    
    //faz dovom
    void updateCustomerLevelAndPoints(int customerId, int points, const std::string& level);
    void addMembershipHistory(int customerId, const std::string& oldLevel, const std::string& newLevel, const std::string& reason);

    void updateMonthlyCoupons(int customerId, int count);
    void assignMonthlyCoupons();  // takhsis coupon haye mahane
    void updateBadge(int customerId, const std::string& badge);

};
#endif
