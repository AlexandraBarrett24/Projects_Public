#include "gtest/gtest.h"
#include "EntityProject/ANVIL/drone_delivery_system.h"

#include <drone.h>
#include <package.h>
#include <customer.h>
#include <EntityProject/project_settings.h>

#include <iostream>

namespace csci3081 {

using entity_project::DroneDeliverySystem;
using entity_project::Entity;

class ObserverTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    system = GetDroneDeliverySystem("default");
  }
  virtual void TearDown() {}

  DroneDeliverySystem* system;
};
/*******************************************************************************
 * Test Cases
 ******************************************************************************/

Drone *MakeDrones(DroneDeliverySystem *system,
                   std::string model,
                   float posx = 0.0,
                   float posy = 0.0,
                   float posz = 0.0) {
    picojson::object obj;
    picojson::array pos;
    pos.push_back(picojson::value(posx));
    pos.push_back(picojson::value(posy));
    pos.push_back(picojson::value(posz));

    Entity *entity;
    obj["type"] = picojson::value("drone");
    obj["model"] = picojson::value(model);
    obj["position"] = (picojson::value) pos;
    obj["radius"] = picojson::value(1.0);
    entity = system->CreateEntity(obj);
    Drone *d = entity->AsType<Drone>();
    return d;
  }

  Package *MakePackages(DroneDeliverySystem *system,
                       float posx = 0.0,
                       float posy = 0.0,
                       float posz = 0.0,
                       float weight = 5.0) {
    picojson::object obj;
    picojson::array pos;
    pos.push_back(picojson::value(posx));
    pos.push_back(picojson::value(posy));
    pos.push_back(picojson::value(posz));

    Entity *entity;
    obj["type"] = picojson::value("package");
    obj["position"] = (picojson::value) pos;
    entity = system->CreateEntity(obj);
    Package *p = entity->AsType<Package>();
    p->SetWeight(weight);
    return p;
  }

  Customer *MakeCustomers(DroneDeliverySystem *system,
                         float posx = 0.0,
                         float posy = 0.0,
                         float posz = 0.0) {
    picojson::object obj;
    picojson::array pos;
    pos.push_back(picojson::value(posx));
    pos.push_back(picojson::value(posy));
    pos.push_back(picojson::value(posz));

    Entity *entity;
    obj["type"] = picojson::value("customer");
    obj["position"] = (picojson::value) pos;
    entity = system->CreateEntity(obj);
    return entity->AsType<Customer>();
  }


TEST_F(ObserverTest, AddingObservers) {
    Drone *d = MakeDrones(system, "Q-36-01");
    Package *p = MakePackages(system);
    entity_project::EntityObserver* observer;
    entity_project::EntityObserver* observer1;
    entity_project::EntityObserver* observer2;

    system->AddObserver(p, observer);
    system->AddObserver(p, observer1);
    system->AddObserver(p, observer2);

    int size = p->GetObservers().size();

    ASSERT_EQ(size, 3);
}


TEST_F(ObserverTest, RemovingObservers) {
    Drone *d = MakeDrones(system, "Q-36-01");
    Package *p = MakePackages(system);
    entity_project::EntityObserver* observer;
    entity_project::EntityObserver* observer1;
    entity_project::EntityObserver* observer2;

    system->AddObserver(p, observer);
    system->AddObserver(p, observer1);
    system->AddObserver(p, observer2);

    system->RemoveObserver(p, observer1);

    int size = p->GetObservers().size();

    ASSERT_EQ(size, 2);
}

class observer : public entity_project::EntityObserver{
 public:
  int log = 0;

  void OnEvent(const picojson::value& event, const Entity& entity) {
    log = 1;
  }

  int GetLog(){return log;}
};

TEST_F(ObserverTest, observer_not_added) {
    Package *p = MakePackages(system);
    observer *obs = new observer();
    system->AddObserver(p, obs);
    Customer *c = MakeCustomers(system);
    picojson::object obj;
    obj["dest_index"] = picojson::value("1");
    obj["pkg_index"] = picojson::value("0");

    picojson::value val(obj);
    // picojson::object obj;

    system->ScheduleDelivery(p, c, obj);
    for (int i = 0; i < 1000; i++) {
        system->Update(0.1);
    }
    int check = obs->GetLog();
    ASSERT_EQ(check, 1);
}

TEST_F(ObserverTest, entity_radius_is_wrong) {
  Package *p = MakePackages(system);
  Drone *d = MakeDrones(system, "Q-36-01");
  Customer *c = MakeCustomers(system);
  float pradius = p->GetRadius();
  float cradius = c->GetRadius();

  float dradius = d->GetRadius();
  float pcompare = 1.0;
  float dcompare = 1.0;
  float ccompare = 1.0;

  ASSERT_EQ(pradius , pcompare);
  ASSERT_EQ(dradius , dcompare);
  ASSERT_EQ(cradius , ccompare);
}

TEST_F(ObserverTest, entity_id_not_unique) {
  Package *p = MakePackages(system);
  Customer *c = MakeCustomers(system);

  int pID =  p->GetId();
  int cID = c->GetId();
ASSERT_NE(pID , cID);
}
}  // namespace csci3081
