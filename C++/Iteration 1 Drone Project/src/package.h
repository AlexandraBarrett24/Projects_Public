#ifndef PACKAGE_H_
#define PACKAGE_H_

#include <EntityProject/ANVIL/package.h>

namespace csci3081 {

/// TODO: Add documentation. You may edit this class including adding members or methods.
/**
 * TODO: Add detailed documentation.
 */
class Package : public entity_project::Package {
  public:
    /// TODO: Add documentation.
    Package() { AddType<Package>(); }
    //Package(name : std::string, location:: position, radius:: float);
    //~Package();
    void setDetails(const picojson::object& val){
    	details_= val;
    }
 private: 
    std::string sender;
    std::string recipiant;
    //position location;
    float radius;
   
};

}  // namespace csci3081
#endif
