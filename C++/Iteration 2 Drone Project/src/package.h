/**
 * @file package.h
 */

#ifndef CSCI3081_PACKAGE_H_
#define CSCI3081_PACKAGE_H_

#include <EntityProject/ANVIL/package.h>
#include <EntityProject/entity_observer.h>
#include <entity_base.h>
#include <customer.h>
#include <string>
#include <vector>

namespace csci3081 {

/**
 * @brief Stores data about Package entities.
 *
 * Packages are initially constructed without a customer, but is assigned
 * one by the Scheduler. Also includes an attribute to store a pointer to
 * the drone that is delivering the package.
 */
class Package : public entity_project::Package, public EntityBase {
 public:
  /// Default constructor
  Package() { AddType<Package>(); }

  /**
   * @brief Generates package from picojson::object.
   *
   * Initializes position, direction, id, name, radius of detection, from
   * picojson::object file, as well as all attributes from EntityBase 
   */
  explicit Package(const picojson::object& val);

  /// Returns whether package can move within simulation
  bool IsDynamic() const { return is_dynamic_; }

  /// Returns weight of package (default 5.0)
  float GetWeight() const { return weight_; }
  /// Sets weight of package
  void SetWeight(double w) { weight_ = w; }

  /// Returns a pointer to the package's customer (default nullptr)
  Customer *GetCustomer() { return customer_; }
  /// Sets the package's customer
  void SetCustomer(Customer* c) { customer_ = c; }

  /// Returns the drone assigned to pick up the package (default nullptr)
  entity_project::Entity *GetDrone() { return drone_; }
  void SetDrone(entity_project::Entity *d) { drone_ = d; }

  /**
   * @brief Updates behavior of Package depending on status passed in
   *
   * Statuses:
   *    Existing (nullptr) : Package has not been assigned to a customer yet
   *    Ready to ship (Customer) : Package has been assigned customer, waiting
   *        on drone assignment
   *    Waiting on drone (Drone) : Drone has been assigned and will pick package up soon
   *    In transit (nullptr) : Package is being delivered by drone
   *    Delivered (nullptr) : Package has been delivered. Package is removed from
   *        simulation upon this status change.
   */
  void OnStatusChange(std::string status, entity_project::Entity *e);

  /**
   * @brief Performs any actions that must be undertaken by the Customer on each timestep.
   *
   * If package has status "In transit", it's position is updated in accordance with it's
   * assigned drone's location
   */
  void Update(float dt);

  /**
   * @brief Notifies observers of status changes regarding the package.
   *
   * If package has status "Travelling to package", then we notify the observers that the delivery has been "scheduled".
   * If package has status "In transit", then we notify the observers that the package is "en route".
   * If package has status "delivered", then we notify the observers that the package has been "delivered".
   */
  void NotifyObservers(std::string status);

  /**
   * @brief Adds new observers to the package for updates.
   */
  void AddObserver(entity_project::EntityObserver* entity);

  /**
   * @brief Removes observers from the package.
   */
  void RemoveObserver(entity_project::EntityObserver* entity);

  // returns the observers of the package
  std::vector<entity_project::EntityObserver*> GetObservers() {return observers_ ;}



 private:
  float weight_;
  Customer *customer_;
  entity_project::Entity *drone_;
  std::vector<entity_project::EntityObserver*> observers_;
};
}  // namespace csci3081
#endif
