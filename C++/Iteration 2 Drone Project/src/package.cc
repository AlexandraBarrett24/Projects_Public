#include <string>
#include <ctime>
#include <vector>
#include <package.h>

#define DEBUG 0

namespace CS = csci3081;

/// Generates a package from a picojson::object
CS::Package::Package(const picojson::object& val) {
  AddType<Package>();

  picojson::array pos;
  picojson::array dir;

  std::vector<entity_project::EntityObserver*> observers_;

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
    // Assign unique ID to each package
  static int packageId_ = 0;
  unsigned int seed = 1;
  this->id_ = (rand_r(&seed) % 100000) + packageId_++;
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
  this->statuslist_ = {
    "Existing",  // Package has not been assigned to a customer
    "Ready to ship",  // Package assigned to customer, waiting for drone assignment
    "Waiting on drone",  // Package assigned to drone
    "In transit",  // Package is being delivered by drone
    "Delivered"
  };

  // initialize type_specific attributes
  this->weight_ = 5.0;
  this->customer_ = nullptr;
  this->drone_ = nullptr;
}

void CS::Package::OnStatusChange(std::string status, entity_project::Entity *e = nullptr) {
  status_ = status;
#if DEBUG
  int id = GetId();
  std::cout << "Package #" << id << " status now " << status_ << std::endl;
#endif
  if (e != nullptr) {
    if (status == "Ready to ship") {
      CS::Customer *c = e->AsType<CS::Customer>();
      SetCustomer(c);
    } else if (status == "Waiting on drone") {
      SetDynamic(true);
      SetDrone(e);
      NotifyObservers("Travelling to package");
    }
  } else {
    if (status == "Existing") {
      SetCustomer(nullptr);
    } else if (status == "In transit") {
      NotifyObservers("In transit");
    }
  }
}

void CS::Package::Update(float dt) {
  // If package has been picked up by drone, move along with drone
  if (is_dynamic_ == true && GetStatus() == "In transit") {
    position_[0] = drone_->GetPosition()[0];
    position_[1] = drone_->GetPosition()[1];
    position_[2] = drone_->GetPosition()[2];
  }
}


void CS::Package::NotifyObservers(std::string status) {
#if DEBUG
  std::cout << "Observers notified: " << status << std::endl;
#endif
  picojson::object obj;
  obj["type"] = picojson::value("notify");


  // no observers to notify
  if (observers_.size() == 0) {
    return;
  }

  if (status == "Travelling to package") {
    // if status = scheduled
    obj["value"] = picojson::value("scheduled");
    picojson::value val(obj);
    for (int i = 0; i < observers_.size() ; i++) {
      observers_[i]->OnEvent(val, *this);
    }
  } else if (status == "In transit") {
    // if status = en route
    obj["value"] = picojson::value("en route");
    picojson::value val(obj);
    for (int i = 0; i < observers_.size() ; i++) {
      observers_[i]->OnEvent(val, *this);
    }

  } else if (status == "delivered") {
    // if delivered
    obj["value"] = picojson::value("delivered");
    picojson::value val(obj);
    for (int i = 0; i < observers_.size() ; i++) {
      observers_[i]->OnEvent(val, *this);
    }
  }
}

void CS::Package::AddObserver(entity_project::EntityObserver* entity) {
  observers_.push_back(entity);
}

void CS::Package::RemoveObserver(entity_project::EntityObserver* entity) {
  for (int i = 0; i < observers_.size(); i++) {
    if (entity == observers_.at(i)) {
      observers_.erase(observers_.begin() + i);
    }
  }
}
