#ifndef ICUSTOME_RDAO_H
#define ICUSTOME_RDAO_H


#include "Customer.h"
#include <vector>

class ICustomerDAO 
{
public:

    virtual ~ICustomerDAO() {}
    virtual void addCustomer(Customer* customer) = 0;
    virtual Customer* findCustomerById(int id) const = 0;
    virtual std::vector<Customer*> getAllCustomers() const = 0;
    virtual void updateWallet(int customerId, double newBalance) = 0;
    virtual void updateCustomerLevelAndPoints(int customerId, int points, const std::string& level) = 0;
    virtual void addMembershipHistory(int customerId, const std::string& oldLevel, const std::string& newLevel,
            const std::string& reason) = 0; 
    virtual void updateMonthlyCoupons(int customerId, int count) = 0;
    virtual void assignMonthlyCoupons() = 0;
    virtual void updateBadge(int customerId, const std::string& badge) = 0;
};
#endif