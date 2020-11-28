/**
 * @file drone_creator.h
 */

#ifndef CSCI3081_DRONE_CREATOR_H_
#define CSCI3081_DRONE_CREATOR_H_

#include <drone.h>
#include <model_csv_parser.h>
#include <map>
#include <string>
namespace csci3081 {

/**
 * @brief A class that creates a Drone object
 */
class DroneCreator {
 public:
  /// Constructor
  DroneCreator();
  /// Destructor
  ~DroneCreator();

  /**
   * @brief Creates new instance of drone.
   *
   * If a model entry is found, create Drone according to the
   * specifications for that model in planet-x.csv. Otherwise
   * use defaults
   *    - Mass = 10 kg
   *    - MaxSpeed = 30 m/s
   *    - Acceleration = 1 m/s^2
   *    - Max Capacity = 10 kg
   *    - Battery Capacity = 30 seconds
   */
  Drone *CreateEntity(const picojson::object& val);

 private:
  /// Stores csv parser that translates planet-x.csv into DroneTypes
  ModelCSVParser *csvParser_;
  /// List of DroneTypes stored in planet-x.csv
  std::map<std::string, DroneType> models_;
};

}  // namespace csci3081

#endif  // CSCI3081_DRONE_CREATOR_H_
