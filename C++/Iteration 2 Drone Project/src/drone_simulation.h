/**
 * @file drone_simulation.h
 */

#ifndef CSCI3081_DRONE_SIMULATION_H_
#define CSCI3081_DRONE_SIMULATION_H_


#include <EntityProject/ANVIL/drone_delivery_system.h>
#include <EntityProject/entity_console_logger.h>
#include "src/json_helper.h"
#include <scheduler.h>
#include <entity_creator.h>
#include <vector3.h>
#include <pathFinder.h>
#include <EntityProject/osm_graph_parser.h>
#include <EntityProject/graph.h>
#include <string>
#include <vector>


namespace csci3081 {

/**
 * @brief A facade class that implements the Drone Delivery System.
 *
 * Inherits from the entity_project::DroneDeliverySystem. Currently
 * does not add any new methods, but merely implements
 * the functions described in the entity_project::DroneDeliverySystem
 * superclass. Contains three factories, each creates a specific entity
 * type (drone, customer, package), and a scheduler, which handles assigning
 * packages to drones.
 */
class DroneSimulation : public entity_project::DroneDeliverySystem {
 public:
  DroneSimulation();

  ~DroneSimulation();

  /// Return name of the project team
  const std::string& GetTeamName() const { return teamName_; }

  /// Create an entity based off of a picojson::object
  entity_project::Entity *CreateEntity(const picojson::object& val);

  Scheduler *GetScheduler() { return scheduler_; }

  /**
   * @brief Adds entity into the system
   *
   * Logs the entity into the entities_ vector and
   * the vector for that entity type in the scheduler
   */
  void AddEntity(entity_project::Entity* entity);

  /**
   * @brief Removes an entity from the system
   *
   * Removes entity from both entities_ vector and
   * type-specific vector in scheduler
   *
   */
  void RemoveEntity(entity_project::Entity* entity);

#ifdef ANVIL2
  /// TODO: Add documentation.
  void SetGraph(const entity_project::IGraph* graph);
#endif

  /// Schedule a drone delivery for a package to a customer. Handled by Scheduler class
  void ScheduleDelivery(entity_project::Package* package,
                        entity_project::Customer* dest,
                        const picojson::object& details);

  /**
   * @brief Adds an Observer to the given Entity
   *
   * The observer is added if the entity type
   * is a package. The observer is added in the 
   * package class.
   *
   */
  void AddObserver(entity_project::Entity* entity, entity_project::EntityObserver* observer);

  /**
   * @brief Removes an Observer from the given Entity
   *
   * The observer is removed if the entity type
   * is a package. The observer is removed in the 
   * package class.
   *
   */
  void RemoveObserver(entity_project::Entity* entity, entity_project::EntityObserver* observer);

  /// Return complete entities_vector
  const std::vector<entity_project::Entity*>& GetEntities() const { return entities_; }

  /**
   * @brief Updates system time by dt
   *
   * For each entity in entities_, calls their Update() function. Also causes
   * "Waiting" drones to search for and assign itself to any packages that are
   * "Ready to ship" (statuses are described in more detail in drone.h, package.h,
   * customer.h). Customers and packages are also removed from the system if they
   * have status "Received package" or "Delivered", respectively.
   */
  void Update(float dt);

 private:
  std::string teamName_;
  std::vector<entity_project::Entity*> entities_;
  Scheduler *scheduler_;
  EntityCreator *entityCreator_;
  const entity_project::IGraph *graph_;
};

}  // namespace csci3081


#endif  // CSCI3081_DRONE_SIMULATION_H_
