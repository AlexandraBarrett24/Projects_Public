/**
 * @file scheduler.h
 */

#ifndef CSCI3081_SCHEDULER_H_
#define CSCI3081_SCHEDULER_H_

#include <drone.h>
#include <customer.h>
#include <package.h>
#include "pathFinder.h"
#include <EntityProject/osm_graph_parser.h>
#include <EntityProject/graph.h>
#include <string>
#include <vector>

namespace csci3081 {

/**
 * @brief A class that stores and handles the entities in the DroneSimulation system
 *
 * Contains three vectors, each storing all entities of a certain type currently active
 * in the system. Also handles the assigning of which drone to pick up which package.
 */
class Scheduler {
 public:
  /**
   * @brief Schedules a package delivery to a customer
   *
   * Checks each drone in the system and assigns the first "Waiting" drone to
   * pick up the package and deliver it to the customer. Takes into
   * account the weight of the package and the current capacity of the
   * drones as well, so a drone doesn't get assigned to a package that
   * makes the drone exceed it's weight limit.
   */
  void ScheduleDelivery(entity_project::Package* package,
                        entity_project::Customer* customer,
                        const entity_project::IGraph *graph,
                        const picojson::object& details);

  /// Adds an entity into the proper vector, depending on its type
  void AddEntity(entity_project::Entity *e);
  /// Removes an entity from its vector
  void RemoveEntity(entity_project::Entity *e);

  /// Returns the package vector
  std::vector<Package*> GetPackages() { return packages_; }
  /// Returns the customer vector
  std::vector<Customer*> GetCustomers() { return customers_; }
  /// Returns the drone vector
  std::vector<Drone*> GetDrones() { return drones_; }

 private:
  std::vector<Package*> packages_;
  std::vector<Customer*> customers_;
  std::vector<Drone*> drones_;
  std::vector<entity_project::IGraphNode *> nodes_;
};

}  // namespace csci3081


#endif  // CSCI3081_SCHEDULER_H_
