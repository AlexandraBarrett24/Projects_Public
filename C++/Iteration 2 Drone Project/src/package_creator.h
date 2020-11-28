/**
 * @file package_creator.h
 */

#ifndef CSCI3081_PACKAGE_CREATOR_H_
#define CSCI3081_PACKAGE_CREATOR_H_

#include <package.h>
namespace csci3081 {

/**
 * @brief A class that creates a Package object
 */
class PackageCreator {
 public:
  /// Creates new instance of package
  Package *CreateEntity(const picojson::object& val) { return new Package(val); }
};

}  // namespace csci3081

#endif  // CSCI3081_PACKAGE_CREATOR_H_
