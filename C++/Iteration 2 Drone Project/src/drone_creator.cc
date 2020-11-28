#include <drone_creator.h>
#include <map>
#include <string>

#define DEBUG 0

namespace CS = csci3081;

CS::DroneCreator::DroneCreator() {
  csvParser_ = new ModelCSVParser;
  models_ = csvParser_->CreateSet();
}

CS::DroneCreator::~DroneCreator() {
  delete csvParser_;
}

CS::Drone *CS::DroneCreator::CreateEntity(const picojson::object& val) {
  models_ = csvParser_->CreateSet();
  std::string m;
  // Parse JSON to find model type
  if (val.find("model") != val.end()) {
    m = val.find("model")->second.get<std::string>();
#if DEBUG
    std::cout << "Drone CreateEntity: found model type " << m << std::endl;
#endif
    // if valid model type
    if (models_.find(m) != models_.end()) {
      DroneType dtype = models_.find(m)->second;
      return new Drone(val, dtype);
    } else {
#if DEBUG
      std::cout << "ERROR: picojson contains invalid model, setting defaults" << std::endl;
#endif
    }
  } else {
    // if no model type is specified
#if DEBUG
    std::cout << "No specific drone model specified, setting defaults" << std::endl;
#endif
  }
    DroneType dtype = DroneType("default", 10, 30, 5, 10, 30);
    return new Drone(val, dtype);
}
