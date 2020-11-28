#include <stdlib.h>
#include <ctime>
#include <time.h>
#include <string>
#include <vector>
#include <drone.h>

#define DEBUG 0

namespace CS = csci3081;

CS::Drone::Drone(const picojson::object& val, CS::DroneType model) {
  AddType<Drone>();

  picojson::array pos;
  picojson::array dir;

  // initialize position_ and direction_ arrays
  if (val.find("position")->second.is<picojson::array>()) {
    pos = val.find("position")->second.get<picojson::array>();
    if (pos.size() == 0) {
      this->position_[0] = 0.0;
      this->position_[1] = 0.0;
      this->position_[2] = 0.0;
    } else {
      for (int i = 0; i < 3; i++) {
        if (pos[i].is<double>()) {
          this->position_[i] = pos[i].get<double>();
        } else {
          this->direction_[i] = 0.0;
        }
      }
    }
  }

  if (val.find("direction")->second.is<picojson::array>()) {
    dir = val.find("direction")->second.get<picojson::array>();
    if (dir.size() == 0) {
      this->direction_[0] = 1.0;
      this->direction_[1] = 0.0;
      this->direction_[2] = 0.0;
    } else {
      for (int i = 0; i < 3; i++) {
        if (dir[i].is<double>()) {
          this->direction_[i] = dir[i].get<double>();
        } else {
          if (i == 0) {
            this->direction_[i] = 1.0;
          } else {
            this->direction_[i] = 0.0;
          }
        }
      }
    }
  }

  // initialize entity_project::Entity attributes
    // Assign unique ID to each drone
  static int droneId_ = 0;
  unsigned int seed = 0;
  this->id_ = (rand_r(&seed) % 100000) + droneId_++;
  if (val.find("name")->second.is<std::string>()) {
    this->name_ = val.find("name")->second.get<std::string>();
  }
  if (val.find("radius")->second.is<double>()) {
    this->radius_ = val.find("radius")->second.get<double>();
  }
  this->details_ = val;

  // initialize EntityBase attributes
  if (val.find("type")->second.is<std::string>()) {
    this->type_ = val.find("type")->second.get<std::string>();
  }
  this->is_dynamic_ = true;
  this->destination_ = ZERO_VECTOR;
  this->status_ = "Waiting";
  this->statuslist_ = {"Waiting",  // Waiting for a delivery
                       "Travelling to package",
                       "Picking up package",
                       "Travelling to customer",
                       "Dropping off package",
                       "Idle"};  // out of battery

  // initialize type-specific attributes
  this->model_ = model.GetModel();
  this->currSpeed_ = 0.0;
  this->maxSpeed_ = model.GetMaxSpeed();
  this->maxCapacity_ = model.GetWeightCapacity();
  this->currLoadWeight_ = 0.0;
  this->destEntity_ = nullptr;
  this->destEntityType_ = "";
  this->massNoLoad_ = model.GetMass();
  this->baseAcceleration_ = model.GetAcceleration();
  this->batteryCapacity_ = model.GetBatteryCapacity();
  this->batteryLeft_ = batteryCapacity_;
  // Calculate the inital force of the drone.
  this->forceDrone_ = massNoLoad_ * baseAcceleration_;
  // When initialized, the total mass is the same as massNoLoad_
  this->massTotal_ = massNoLoad_;
  // Calculate inital acceleration of the drone
  this->accelerationDrone_ = forceDrone_/currLoadWeight_;

#if DEBUG
  // print statments for debugging
  std::cout << "Drone " << id_ << ": " << name_ << std::endl;
  std::cout << "Model: \t\t" << model_ << std::endl;
  std::cout << "Mass: \t\t" << massNoLoad_ << " kg\n";
  std::cout << "Max Speed: \t" << maxSpeed_ << " km/h\n";
  std::cout << "Base Accel: \t" << baseAcceleration_ << " m/s^2\n";
  std::cout << "Weight Cap: \t" << maxCapacity_ << " kg\n";
  std::cout << "Battery: \t" << batteryCapacity_ << " s\n";
  std::cout << std::endl;
#endif
}

void CS::Drone::SetRoute(std::vector<Vector3> route1, std::vector<Vector3> route2) {
  ClearRoute();
  route_to_package = route1;
  route_to_customer = route2;
  route_ = route_to_package;
  strRoute_ = PStrRoute_;
}

void CS::Drone::SetDestEntity(entity_project::Entity *e) {
  destEntity_ = e;
  if (e->GetDetails().find("type")->second.is<std::string>()) {
    destEntityType_ = e->GetDetails().find("type")->second.get<std::string>();
  } else {
    destEntityType_ = "entity";
  }
}

/// Removes the specified package from packageList, if possible.
void CS::Drone::RemovePackage(CS::Package *p) {
  for (int i = 0; i < packageList_.size(); i++) {
    if (p == packageList_.at(i)) {
      currLoadWeight_ -= p->GetWeight();
      packageList_.erase(packageList_.begin() + i);
    }
  }
}

void CS::Drone::OnStatusChange(std::string status, entity_project::Entity *e = nullptr) {
  status_ = status;
#if DEBUG
  int id = GetId();
  std::cout << "Drone #"<< id << " status now " << status_ << std::endl;
#endif
  if (e != nullptr) {
    CS::Package *p = e->AsType<CS::Package>();
    if (status == "Travelling to package") {
      p->OnStatusChange("Waiting on drone", this);
    } else if (status == "Picking up package") {
      AddPackage(p);
      p->OnStatusChange("In transit", nullptr);
      currLoadWeight_ += p->GetWeight();

    } else if (status == "Travelling to customer") {
    } else if (status == "Dropping off package") {
      RemovePackage(p);
      p->OnStatusChange("Delivered", nullptr);
      Customer *c = p->GetCustomer();
      c->OnStatusChange("Received package", nullptr);
    }
  } else if (status == "Idle") {
    // if packageList is non-empty
    while (packageList_.size() > 0) {
      // change status of first package in packageList_ to "Ready to ship"
      Package *p = packageList_.at(0);
      p->OnStatusChange("Ready to ship", p->GetCustomer());
      // erase first package
      RemovePackage(p);
#if DEBUG
      std::cout << "Package dropped! packagesList_ size: " << packageList_.size() << std::endl;
#endif
    }
    // if dest_entity is a package
    if (destEntityType_ == "package") {
      // change status of destination package to "Ready to ship"
      Package *destPack = destEntity_->AsType<CS::Package>();
      destPack->OnStatusChange("Ready to ship", destPack->GetCustomer());
      // remove destination package
      destEntity_ == nullptr;
      destEntityType_ == "";
    }

  } else if (status == "Waiting") {
    // remove any stored route, destination, or waypoints
    // Drone does not move when "Waiting"
    destEntity_ == nullptr;
    destEntityType_ == "";
    route_.clear();
    nextWaypoint_.Clear();
  }
}

void CS::Drone::Update(float dt) {
#if DEBUG
  // std::cout << "Battery left: " << batteryLeft_ << " seconds" << std::endl;
#endif
  if (status_ == "Idle") {  // Do nothing if Idle
    // else if Drone has a route to travel along, move along route
  } else if (IsDynamic() == true && nextWaypoint_ != ZERO_VECTOR) {
    CS::Vector3 currPos = Vector3(position_);
    CS::Vector3 direction;
    CS::Vector3 newPos;
    CS::Vector3 velocity;

    // Get normalized direction vector, set to direction_
    direction = nextWaypoint_ - currPos;
    direction = direction.Normalize();

    // Calculate acceleration
    accelerationDrone_ = forceDrone_/massTotal_;
    velocity = velocity + direction * (accelerationDrone_*dt);
    direction_[0] = direction.GetX();
    direction_[1] = direction.GetY();
    direction_[2] = direction.GetZ();  // Allie TODO: might need to do same for velocity?

    // Convert maxSpeed to m/s
    float metersPerSecond = maxSpeed_ / 3.6;

    // Update drone's position
    newPos = currPos + (velocity * metersPerSecond);
    position_[0] = newPos.GetX();
    position_[1] = newPos.GetY();
    position_[2] = newPos.GetZ();
    currPos = Vector3(position_);

    // Update drone's battery life
    batteryLeft_ -= dt;
    if (batteryLeft_ <= 0.0) {
#if DEBUG
      std::cout << "Out of battery!" << std::endl;
#endif
      OnStatusChange("Idle", nullptr);
    }

    // If drone is within a radius length of waypoint, remove waypoint
    // from route and set next waypoint as destination
    if (nextWaypoint_.ToRoutePoint().DistanceBetween(currPos.ToRoutePoint()) < radius_) {
      route_.erase(route_.begin());
      if (!strRoute_.empty()) {
        strRoute_.erase(strRoute_.begin());
      }
      if (route_.size() > 0) {
        SetNextWaypoint(route_.at(0));
      } else {
        SetNextWaypoint(ZERO_VECTOR);
      }
    }
    // If drone's radius intersects with radius of destination entity
    if (destination_.ToRoutePoint().DistanceBetween(currPos.ToRoutePoint())
        < (radius_ + destEntity_->GetRadius())) {
      // package case
      if (destEntityType_ == "package") {
        CS::Package *p = destEntity_->AsType<Package>();
        OnStatusChange("Picking up package", p);

        // Get route to customer, and load into route_
        destination_ = Vector3(p->GetCustomer()->GetPosition());
        SetDestEntity(p->GetCustomer());

        route_ = route_to_customer;
        strRoute_ = CStrRoute_;

        // Set first route waypoint as drone's nextWaypoint
        CS::Vector3 wpt = Vector3(route_.at(0));
        SetNextWaypoint(wpt);

        OnStatusChange("Travelling to customer", nullptr);

        // customer case
      } else if (destEntityType_ == "customer") {
        CS::Customer *c = destEntity_->AsType<CS::Customer>();
        CS::Package *p = c->GetPackage()->AsType<CS::Package>();
        OnStatusChange("Dropping off package", p);
      }
    }
  }
}
