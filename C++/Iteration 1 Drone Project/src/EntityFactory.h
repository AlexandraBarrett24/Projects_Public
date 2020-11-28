#ifndef ENTITYFAC_H_
#define ENTITYFAC_H_

#include <EntityProject/ANVIL/customer.h>

namespace csci3081 {
/// This is the Abstract factory for Entities.
/**
 * Includes a build entity function and get type.
 */
class EntityFactory : public entity_project::EntityFactory {
 public:
    /// TODO: Add documentation.
    BuildEntity(value picojson::object&) { AddType<Customer>(); }
};

}  // namespace csci3081

#endif
