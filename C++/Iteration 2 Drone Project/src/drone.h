/**
 * @file drone.h
 */

#ifndef CSCI3081_DRONE_H_
#define CSCI3081_DRONE_H_

#include <EntityProject/ANVIL/drone.h>
#include <EntityProject/simple_UMN_route_manager.h>
#include <drone_type.h>
#include <entity_base.h>
#include <package.h>
#include <vector3.h>
#include <vector>
#include <string>

namespace csci3081 {

/**
 * @brief Stores data about Drone entities.
 *
 * Drones are made to pick up and drop off multiple packages (in future iterations).
 * Drones each have a Package vector to store packages currently being delivered and
 * a maximum capacity (weight limit). Also contains a route_ vector to store the
 * drone's route to their destination, and attributes to store the current entity they
 * are heading towards (either a package to pick up or a customer to deliver to).
 */
class Drone : public entity_project::Drone, public EntityBase {
 public:
  Drone() { AddType<Drone>(); }

  /// Generates a drone from a picojson::object and a DroneType template
  Drone(const picojson::object& val, DroneType model);

  /// Returns drone's current position
  Vector3 GetPositionAsVector() { return Vector3(position_); }
  /// returns the drone's current direction.
  Vector3 GetDirectionAsVector() { return Vector3(direction_); }

  /**
   * @brief Creates a set of waypoints from one location to another
   *
   * Follows roads and paths to avoid collisions with buildings and other tall obstacles
   * Stores waypoints in route_ vector.
   */
  std::vector<Vector3> GetRoute() { return route_; }

  /// Sets the route in point form.
  void SetRoute(std::vector<Vector3> route1, std::vector<Vector3> route2);

  /// Completely clears route_ vector
  void ClearRoute() { route_.clear(); }

  /// Returns the type of the destination entity (default nullptr)
  std::string GetDestEntityType() { return destEntityType_; }
  /// Returns destination entity (default nullptr)
  entity_project::Entity *GetDestEntity() { return destEntity_; }
  /// Sets destination entity
  void SetDestEntity(entity_project::Entity *e);

  /// Returns base acceleration of drone
  float GetBaseAcceleration() const { return baseAcceleration_;}

  /// Returns current weight of all packages being carried
  float GetCurrLoadWeight() { return currLoadWeight_; }
  /// Returns the remaining amount of weight the drone can carry
  float GetRemainingCapacity() const { return maxCapacity_ - currLoadWeight_; }
  /// Returns the max amount of weight the drone can safely carry
  float GetMaxCapacity() const { return maxCapacity_; }
  /// Modifies the maximum weight limit
  void SetMaxCapacity(double d) { maxCapacity_ = d; }

  /// Returns the maximum battery capacity (seconds)
  float GetBatteryCapacity() const { return batteryCapacity_; }
  /// Returns the amount of seconds left the drone can fly for
  float GetRemainingBattery() const { return batteryLeft_;}

  /// Returns maximum speed of drone.
  float GetMaxSpeed() const { return maxSpeed_; }
  /// Returns current speed of drone.
  float GetCurrentSpeed() const { return currSpeed_; }

  /// Adds package to packageList
  void AddPackage(Package *p) { packageList_.push_back(p); }

  /// Removes the specified package from packageList
  void RemovePackage(Package *p);

  /// Returns entire list of packages being carried by drone
  std::vector<Package*> GetPackages() { return packageList_; }

  /**
   * @brief Updates behavior of Drone depending on which status is passed in.
   *
   * Statuses:
   *    Waiting (nullptr) : Currently holding no packages, waiting for a delivery
   *    Travelling to package (Package) : Drone navigating to package location,
   *        assigns package's status to "Waiting on drone"
   *    Picking up package (Package) : Adds package to packageList
   *    Travelling to customer (nullptr) : Drone navigating to customer
   *    Dropping off package (Package) : Removes package from packageList, changes
   *        package's status to "Delivered"
   *    Idle (nullptr) : Drone out of battery, will drop all packages and not be
   *        assigned any new ones.
   */
  void OnStatusChange(std::string status, entity_project::Entity *e);

  /**
   * @brief Update's drone's position in simulation according to route_
   *
   * If drone has a route loaded in, will follow that route. When drone reaches
   * the location of a package it has been assigned, it will pick it up and then
   * load a route to the customer. Drops off package once drone reaches customer's
   * position.
   */
  void Update(float dt);

  /// Saves the string format of the route.
  void SetStrRoute(std::vector<std::string> route, int which) {
    if (which == 0) {
      PStrRoute_ = route;
    } else {
      CStrRoute_ = route;
    }
  }

  /// Gets the current route in string form.
  const std::vector<std::string>& GetCurrentRoute() const {
    const std::vector<std::string>& r = strRoute_;
    return r;
  }


 private:
  std::vector<Package*> packageList_;
  std::vector<std::string> PStrRoute_;
  std::vector<std::string> CStrRoute_;
  std::vector<std::string> strRoute_;
  std::vector<Vector3> route_to_package;
  std::vector<Vector3> route_to_customer;
  std::vector<Vector3> route_;
  std::string model_;
  float currSpeed_;
  float maxSpeed_;
  float currLoadWeight_;
  float maxCapacity_;
  float batteryCapacity_;
  float batteryLeft_;
  entity_project::Entity *destEntity_;
  std::string destEntityType_;
  // allie's additions
  float forceDrone_ = 1;
  float accelerationDrone_ = 1;
  float massNoLoad_ = 1;
  float massTotal_ = 1;
  float baseAcceleration_ = 1;
};
}  // namespace csci3081

#endif  // CSCI3081_DRONE_H_
