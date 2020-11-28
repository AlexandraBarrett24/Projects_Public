/**
 * @file customer_creator.h
 */

#ifndef CSCI3081_CUSTOMER_CREATOR_H_
#define CSCI3081_SRC_CUSTOMER_CREATOR_H_

#include <customer.h>
namespace csci3081 {

/**
 * @brief A class that creates a csci3081::Customer object
 */
class CustomerCreator {
 public:
  /// Creates new Customer object
  Customer *CreateEntity(const picojson::object& val) { return new Customer(val); }
};

}  // namespace csci3081

#endif  // CSCI3081_SRC_CUSTOMER_CREATOR_H_
