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


};
#endif