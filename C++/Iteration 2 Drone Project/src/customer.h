/**
 * @file customer.h
 */

#ifndef CSCI3081_CUSTOMER_H_
#define CSCI3081_CUSTOMER_H_

#include <EntityProject/ANVIL/customer.h>
#include <entity_base.h>
#include <string>

namespace csci3081 {

/**
 * @brief Stores data about Customer entities.
 *
 * Each Customer object contains a pointer to the package that needs to be
 * delivered to the customer (if such an entity has been set). As with all
 * entities of a specific type, inherits from both entity_project::Entity and
 * csci3081::EntityBase classes.
 */
class Customer : public entity_project::Customer, public EntityBase {
 public:
  /// Default constructor
  Customer() { AddType<Customer>(); }

  /**
   * @brief Generates customer from picojson::object.
   *
   * Initializes position, direction, id, name, radius of detection, from
   * picojson::object file, as well as all attributes from EntityBase 
   */
  explicit Customer(const picojson::object& val);

  /// Returns whether customer can move in simulation
  bool IsDynamic() const { return is_dynamic_; }

  /// Returns the package assigned to customer (default nullptr)
  entity_project::Entity *GetPackage() { return package_; }
  /// Sets the customer's package
  void SetPackage(entity_project::Entity *p) { package_ = p; }

  /**
   * @brief Updates behavior of Customer depending on which status is passed in.
   *
   * Statuses:
   *    Existing (nullptr) : Customer created, but has no package assigned to it
   *    Waiting for package (Package) : Customer is assigned a package, package_
   *        attribute is updated to reference the package object passed in.
   *    Received package (nullptr) : Package successfully delivered to customer,
   *        customer is removed from the simulation upon this status change.
   */
  void OnStatusChange(std::string status, entity_project::Entity *e);

  /**
   * @brief (Does nothing this iteration) Performs any actions that must be
   * undertaken by the Customer on each timestep.
   */
  void Update(float dt);

 private:
  /// Holds the package that needs to be delivered to the customer
  entity_project::Entity *package_;
};
}  // namespace csci3081

#endif  // CSCI3081_CUSTOMER_H_
