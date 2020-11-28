#ifndef DRONE_H_
#define DRONE_H_

#include <EntityProject/ANVIL/drone.h>

namespace csci3081 {

/// TODO: Add documentation. You may edit this class including adding members or methods.
/**
 * TODO: Add detailed documentation.
 */
class Drone : public entity_project::Drone {
  public:
    /// TODO: Add documentation.
    Drone() { 
    	AddType<Drone>(); 
    }
    //Drone(name, location, radius);
    //~Drone();
    void setDetails(const picojson::object& val){
    	details_= val;
    }
    
 private: 
    int id;
    std::string name;
    //position location;
    //position direction;
    float radius;
    float speed;
   
};

}  // namespace csci3081
#endif
