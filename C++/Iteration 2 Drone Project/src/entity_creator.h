/**
 * @file entity_creator.h
 */

#ifndef CSCI3081_ENTITY_CREATOR_H_
#define CSCI3081_ENTITY_CREATOR_H_

#include <EntityProject/entity.h>
#include <customer_creator.h>
#include <drone_creator.h>
#include <package_creator.h>

namespace csci3081 {

/**
 * @brief A class that creates a specific entity of type Drone, Customer, or Package
 */
class EntityCreator {
 public:
  /// Constructor
  EntityCreator();

  /// Destructor
  ~EntityCreator();

  /// Creates and returns entity object of type specified in JSON
  entity_project::Entity *CreateEntity(const picojson::object& val);

 private:
  CustomerCreator *customerCreator_;
  DroneCreator *droneCreator_;
  PackageCreator *packageCreator_;
};

}  // namespace csci3081

#endif  // CSCI3081_ENTITY_CREATOR_H_
