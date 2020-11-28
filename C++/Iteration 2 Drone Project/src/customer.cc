#include <customer.h>
#include <ctime>
#include <string>
namespace CS = csci3081;

CS::Customer::Customer(const picojson::object& val) {
  AddType<csci3081::Customer>();

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
    // Assign unique ID to each customer
  static int customerId_ = 0;
  unsigned int seed = 2;
  this->id_ = (rand_r(&seed) % 100000) + customerId_++;
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
  this->is_dynamic_ = false;
  this->destination_ = ZERO_VECTOR;
  this->status_ = "Existing";
  this->statuslist_ = {"Existing",  // Customer in world, but has no package assigned
                       "Waiting for package",  // Package assigned to Customer
                       "Received package"};  // Customer received package
}

void CS::Customer::OnStatusChange(std::string status, entity_project::Entity *e = nullptr) {
  status_ = status;
  if (e != nullptr) {
    if (status == "Waiting for package") {
      package_ = e;
    }
  }
}

void CS::Customer::Update(float dt) {}
