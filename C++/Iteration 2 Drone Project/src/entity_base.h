/**
 * @file entity_base.h
 */

#ifndef CSCI3081_ENTITY_BASE_H_
#define CSCI3081_ENTITY_BASE_H_

#include <vector3.h>
#include <string>
#include <vector>
namespace csci3081 {

/**
 * @brief An abstract base class for csci3081::<type> entities which implements common methods
 * and attributes.
 */
class EntityBase {
 public:
  /// Default constructor
  EntityBase() {}

  /// Controls if the entity can move within the simulation
  void SetDynamic(bool b) { is_dynamic_ = b; }

  /// Returns the type (drone, customer, package) of the entity
  std::string GetType() { return type_; }

  /// Returns entity's current status
  std::string GetStatus() { return status_; }

  /**
   * @brief Alter's entity's behavior/attributes depending on status
   *
   * Entity argument used to pass in a pointer to a specific entity, if required
   * for the callee entity to successfully change it's attributes (e.g. if a Drone
   * has to add a specific package into it's Package vector). Statuses are specific
   * to the entity type and stored in statusList_ upon instantiation.
   */
  virtual void OnStatusChange(std::string status, entity_project::Entity *e) = 0;

  /// Gets the final destination (the point to which the entity is navigating to)
  Vector3 GetDestination() { return destination_; }

  /// Sets the final destination
  void SetDestination(Vector3 d) { destination_ = d; }

  /// When traveling along a route, gets the next waypoint
  Vector3 GetNextWaypoint() { return nextWaypoint_; }

  /// Sets the position which the entity travels towards on its way to the destination
  void SetNextWaypoint(Vector3 wpt) { nextWaypoint_ = wpt; }

  /// Alter's entity's state upon each timestep
  virtual void Update(float dt) = 0;

 protected:
  std::string type_;
  bool is_dynamic_;
  std::string status_;
  std::vector<std::string> statuslist_;
  Vector3 nextWaypoint_;
  Vector3 destination_;
};
}  // namespace csci3081


#endif  // CSCI3081_ENTITY_BASE_H_
