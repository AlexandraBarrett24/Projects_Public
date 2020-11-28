#include <string>
#include <entity_creator.h>

namespace CS = csci3081;

CS::EntityCreator::EntityCreator() {
  this->customerCreator_ = new CS::CustomerCreator();
  this->droneCreator_ = new CS::DroneCreator();
  this->packageCreator_ = new CS::PackageCreator();
}

CS::EntityCreator::~EntityCreator() {
  delete customerCreator_;
  delete droneCreator_;
  delete packageCreator_;
}

entity_project::Entity *CS::EntityCreator::CreateEntity(const picojson::object& val) {
  std::string type;
  if (val.find("type")->second.is<std::string>()) {
    type = val.find("type")->second.get<std::string>();
  }
  if (type == "package") {
    return packageCreator_->CreateEntity(val);
  } else if (type == "customer") {
    return customerCreator_->CreateEntity(val);
  } else if (type == "drone") {
    return droneCreator_->CreateEntity(val);
  } else {
    return nullptr;
  }
}
