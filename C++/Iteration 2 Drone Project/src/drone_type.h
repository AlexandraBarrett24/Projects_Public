/**
 * @file drone_type.h
 */

#ifndef CSCI3081_DRONE_TYPE_H
#define CSCI3081_DRONE_TYPE_H

#include <string>
namespace csci3081 {

/**
 * @brief A class that records a type of drone specified in project/data/planet-x.csv
 */
class DroneType {
 public:
  /// Constructor from data
  DroneType(std::string model,
            float mass,
            float max_speed,
            float acceleration,
            float weight_cap,
            float battery_cap) {
    model_ = model;
    mass_ = mass;
    max_speed_ = max_speed;
    acceleration_ = acceleration;
    weight_cap_ = weight_cap;
    battery_cap_ = battery_cap;
  }

  /// Returns name of model
  std::string GetModel() { return model_; }
  /// Returns mass of this model of drone, in kg
  float GetMass() { return mass_; }
  /// Returns the max speed of this model of drone, in m/s
  float GetMaxSpeed() { return max_speed_; }
  /// Returns acceleration this model of drone can generate, in m/s^2
  float GetAcceleration() { return acceleration_; }
  /// Returns the maximum amount of weight this model can carry, in kg
  float GetWeightCapacity() { return weight_cap_; }
  /// Returns how long this drone can operate before running of battery, in seconds
  float GetBatteryCapacity() { return battery_cap_; }

 private:
  std::string model_;
  float mass_;
  float max_speed_;
  float acceleration_;
  float weight_cap_;
  float battery_cap_;
};

}  // namespace csci3081

#endif  // CSCI3081_DRONE_TYPE_H
