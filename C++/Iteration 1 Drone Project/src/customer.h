#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <EntityProject/ANVIL/customer.h>

namespace csci3081 {
/// TODO: Add documentation. You may edit this class including adding members or methods.
/**
 * TODO: Add detailed documentation.
 */
class Customer : public entity_project::Customer {
 public:
    /// TODO: Add documentation.
    Customer() { AddType<Customer>(); }
    //Customer(name : std::string, location:: position, radius:: float);
    //~Customer();
    void setDetails(const picojson::object& val){
    	details_= val;
    }
 private: 
    std::string name;
    //position location;
    float radius;
      
};

}  // namespace csci3081

#endif
