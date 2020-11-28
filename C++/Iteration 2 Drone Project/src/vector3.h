/**
 * @file vector3.h
 */

#ifndef CSCI3081_VECTOR3_H_
#define CSCI3081_VECTOR3_H_

#include <cmath>
#include <stdlib.h>
#include <string>

#include <vector>
#include <EntityProject/simple_UMN_route_manager.h>

#define ZERO_VECTOR Vector3()

namespace csci3081 {

/**
 * @brief A class implementing a simple 3-dimensional (mathematical) vector, with
 * x, y, and z values
 *
 * This class offers multiple ways of constructing a Vector3 object, as well as
 * useful functions (length, normalize, distance between),
 * and overloaded operations +,-,==,!= to aid in implementing the Drone Simulation.
 * Also offers a conversion to SimpleUMNRoutePoint for usage in calculating routes
 * for drones and other movable entities.
 */
class Vector3 {
 public:
  /**
   * @brief Vector3 Constructor
   *
   * Can pass in no values, an x value, x and y value, or all three values.
   * Any values not passed in default to 0.0
   */
  explicit Vector3(float x = 0.0, float y = 0.0, float z = 0.0) {
    x_ = x;
    y_ = y;
    z_ = z;
  }

  /// Constructor that takes in a vector<float> of size <= 3
  explicit Vector3(std::vector<float> vec) {
    int size = vec.size();
    if (size == 0) {
      (*this) = ZERO_VECTOR;
    } else if (size == 1) {
      (*this) = Vector3(vec.at(0));
    } else if (size == 2) {
      (*this) = Vector3(vec.at(0), vec.at(1));
    } else {
      (*this) = Vector3(vec.at(0), vec.at(1), vec.at(2));
    }
  }

  /// Constructor that takes in an array of three floats
  explicit Vector3(float arr[3]) {
    x_ = arr[0];
    y_ = arr[1];
    z_ = arr[2];
  }

  /// Constructor that takes in a const array of three floats
  explicit Vector3(const float arr[3]) {
    x_ = arr[0];
    y_ = arr[1];
    z_ = arr[2];
  }

  /// Constructor that takes in a SimpleUMNRoutePoint
  explicit Vector3(entity_project::SimpleUMNRoutePoint p) {
    x_ = p[0];
    y_ = p[1];
    z_ = p[2];
  }

  /// Gets x value
  float GetX() { return x_; }
  /// Gets y value
  float GetY() { return y_; }
  /// Gets z value
  float GetZ() { return z_; }
  /// Sets x value
  void SetX(float x) { x_ = x; }
  /// Sets y value
  void SetY(float y) { y_ = y; }
  /// Sets z value
  void SetZ(float z) { z_ = z; }

  void Clear() {
    x_ = 0.0;
    y_ = 0.0;
    z_ = 0.0;
  }

  /// Overloaded addition operator
  Vector3 operator+(Vector3 rhs) { return Vector3(x_ + rhs.GetX(),
                                                   y_ + rhs.GetY(),
                                                   z_ + rhs.GetZ()); }
  /// Overloaded subtraction operator
  Vector3 operator-(Vector3 rhs) { return Vector3(x_ - rhs.GetX(),
                                                   y_ - rhs.GetY(),
                                                   z_ - rhs.GetZ()); }
  /// Overloaded multiplication operator (usage: (scalar) c * Vector3)
  Vector3 operator*(float c) { return Vector3(x_ * c,
                                              y_ * c,
                                              z_ * c); }

  /// Overloaded equality operator (equal if corresponding values differ by 0.01 or less)
  bool operator==(Vector3 o) {
    return (abs(x_ - o.GetX()) <= 0.01 &&
            abs(y_ - o.GetY()) <= 0.01 &&
            abs(z_ - o.GetZ()) <= 0.01);
  }

  /// Overloaded not equal operator
  bool operator!=(Vector3 o) {
    return !(*this == o);
  }

  /// Returns length (magnitude) of vector
  float Length() { return static_cast<float>(sqrt(pow(x_, 2) + pow(y_, 2) + pow(z_, 2))); }

  /// Returns a normalized vector (length 1) in the same direction as the one passed in
  Vector3 Normalize() {
    // checks to see if it is not a zero vector
    if (*this != ZERO_VECTOR) {
      return *this * (1.0 / this->Length());
    }
    return *this;
  }

  /// Returns the distance between two Vector3 points
  float DistanceBetween(Vector3 o) { return (*this - o).Length(); }

  /// Converts Vector3 object to a SimpleUMNRoutePoint, for use in route storage
  entity_project::SimpleUMNRoutePoint ToRoutePoint() {
    entity_project::SimpleUMNRoutePoint p = entity_project::SimpleUMNRoutePoint(x_, y_, z_);
    return p;
  }

  /// Prints each index to stdout, separated by commas. Does not print
  /// newline or any leading/trailing spaces
  std::string toString() {
    std::string out;
    out = std::to_string(x_) + "," + std::to_string(y_) + "," + std::to_string(z_);
    return out;
  }

 private:
  float x_;
  float y_;
  float z_;
};
}  // namespace csci3081

#endif  // CSCI3081_VECTOR3_H
