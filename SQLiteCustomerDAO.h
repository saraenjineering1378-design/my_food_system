#ifndef SQLITECUSTOMERDAO_H
#define SQLITECUSTOMERDAO_H

#include "ICustomerDAO.h"
#include "DatabaseManager.h"
#include "Customer.h"
#include <vector>

class SQLiteCustomerDAO : public ICustomerDAO {
private:
    DatabaseManager& dbManager;

public:
    SQLiteCustomerDAO(DatabaseManager& db) : dbManager(db) {}

    void addCustomer(Customer* customer) override;
    Customer* findCustomerById(int id) const override; 
    std::vector<Customer*> getAllCustomers() const override; 
    void updateWallet(int customerId, double newBalance) override;
};

#endif
