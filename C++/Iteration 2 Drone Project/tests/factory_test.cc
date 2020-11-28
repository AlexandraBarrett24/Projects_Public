#include "gtest/gtest.h"
#include "EntityProject/ANVIL/drone_delivery_system.h"

#include <drone.h>
#include <package.h>
#include <customer.h>
#include <EntityProject/project_settings.h>
#include <EntityProject/osm_graph_parser.h>
#include <EntityProject/graph.h>

#include <iostream>

namespace csci3081 {

using entity_project::DroneDeliverySystem;
using entity_project::Entity;

class FactoryTest : public ::testing::Test {
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

  Drone *MakeDrone(DroneDeliverySystem *system,
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

  Package *MakePackage(DroneDeliverySystem *system,
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

  Customer *MakeCustomer(DroneDeliverySystem *system,
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

  void initGraph(DroneDeliverySystem *system) {
    entity_project::OSMGraphParser parser;
    const entity_project::IGraph *graph = parser.CreateGraph("data/umn.osm",
                                                             "data/umn.height.csv");
    system->SetGraph(graph);
  }

  // Test 1
  TEST_F(FactoryTest, DroneCreated) {
    picojson::object obj;
    obj["type"] = picojson::value("drone");
    Entity* entity = system->CreateEntity(obj);
    ASSERT_NE(entity, nullptr);
    Drone *drone = entity->AsType<Drone>();
    ASSERT_NE(drone, nullptr);
    ASSERT_EQ(system->GetEntities().size(), 0);
    system->AddEntity(drone);
    ASSERT_EQ(system->GetEntities().size(), 1);
    ASSERT_EQ(picojson::value(system->GetEntities()[0]->GetDetails()).serialize(),
              picojson::value(obj).serialize());
  }

  // Test 2
  TEST_F(FactoryTest, PackageCreated) {
    picojson::object pkg;
    pkg["type"] = picojson::value("package");
    Entity* entity = system->CreateEntity(pkg);
    Package *package = entity->AsType<Package>();
    ASSERT_EQ(system->GetEntities().size(), 0);
    system->AddEntity(package);
    ASSERT_EQ(system->GetEntities().size(), 1);
  }

  // Test 3
  TEST_F(FactoryTest, CustomerCreated) {
    picojson::object obj;
    obj["type"] = picojson::value("customer");
    Entity* entity = system->CreateEntity(obj);
    Customer *customer = entity->AsType<Customer>();
    ASSERT_EQ(system->GetEntities().size(), 0);
    system->AddEntity(customer);
    ASSERT_EQ(system->GetEntities().size(), 1);
  }

  // Test 4
  TEST_F(FactoryTest, PackageAddedRemovedFromDrone) {
    Drone *d = MakeDrone(system, "Q-36-01");
    Package *p = MakePackage(system);
    ASSERT_EQ(d->GetPackages().size(), 0);
    d->AddPackage(p);
    ASSERT_EQ(d->GetPackages().size(), 1);
    d->RemovePackage(p);
    ASSERT_EQ(d->GetPackages().size(), 0);
  }

  // Test 5
  TEST_F(FactoryTest, PackageRemovedOnDeliveredStatus) {
    Package *p = MakePackage(system);
    system->AddEntity(p);
    ASSERT_EQ(system->GetEntities().size(), 1);
    p->OnStatusChange("Delivered", nullptr);
    for (int i = 0; i < 5; i++) {
      system->Update(0.01);
    }
    ASSERT_EQ(system->GetEntities().size(), 0);
  }

  // Test 6
  TEST_F(FactoryTest, CustomerNotRemovedOnReceivedStatus) {
    Customer *c = MakeCustomer(system);
    system->AddEntity(c);
    ASSERT_EQ(system->GetEntities().size(), 1);
    c->OnStatusChange("Received package", nullptr);
    for (int i = 0; i < 5; i++) {
      system->Update(0.01);
    }
    ASSERT_EQ(system->GetEntities().size(), 1);
  }

  // Test 7
  TEST_F(FactoryTest, DestEntitySetOnScheduling) {
    Drone *d = MakeDrone(system, "Q-36-01");
    Package *p = MakePackage(system, 5.0, 5.0, 5.0);
    Customer *c = MakeCustomer(system);
    system->AddEntity(d);
    system->AddEntity(p);
    system->AddEntity(c);

    initGraph(system);
    system->ScheduleDelivery(p, c, p->GetDetails());
    ASSERT_EQ(d->GetDestEntity(), p);
  }

  // Test 8
  TEST_F(FactoryTest, DroneDoesNotPickUpTooHeavyPackage) {
    Drone *d1 = MakeDrone(system, "Q-36-01", 5.0, 5.0, 5.0);
    Drone *d2 = MakeDrone(system, "Q-36-02", -5.0, -5.0, -5.0);
    Package *p = MakePackage(system, 10, 10, 10, 15.0);
    Customer *c = MakeCustomer(system);
    system->AddEntity(d1);
    system->AddEntity(d2);
    system->AddEntity(c);
    system->AddEntity(p);
    ASSERT_EQ(system->GetEntities().size(), 4);
    ASSERT_EQ(d1->GetDestEntity(), nullptr);
    ASSERT_EQ(d2->GetDestEntity(), nullptr);

    initGraph(system);
    system->ScheduleDelivery(p, c, p->GetDetails());
    ASSERT_EQ(d1->GetDestEntity(), nullptr);
    ASSERT_EQ(d2->GetDestEntity(), p);
  }

  // Test 9
  TEST_F(FactoryTest, IdleDroneDoesNotPickUpPackage) {
    Drone *d1 = MakeDrone(system, "Q-36-01", 5.0, 5.0, 5.0);
    Drone *d2 = MakeDrone(system, "Q-36-01", -5.0, -5.0, -5.0);
    Package *p = MakePackage(system, 10, 10, 10);
    Customer *c = MakeCustomer(system);
    system->AddEntity(d1);
    system->AddEntity(d2);
    system->AddEntity(c);
    system->AddEntity(p);
    d1->OnStatusChange("Idle", nullptr);
    ASSERT_EQ(system->GetEntities().size(), 4);
    ASSERT_EQ(d1->GetDestEntity(), nullptr);
    ASSERT_EQ(d2->GetDestEntity(), nullptr);

    initGraph(system);
    system->ScheduleDelivery(p, c, p->GetDetails());
    ASSERT_EQ(d1->GetDestEntity(), nullptr);
    ASSERT_EQ(d2->GetDestEntity(), p);
  }


  // Test 10
  TEST_F(FactoryTest, PackageAssignedtoCustomerAndNoDronesAvailable) {
    Package *p = MakePackage(system, 10, 10, 10);
    Customer *c = MakeCustomer(system);
    system->AddEntity(c);
    system->AddEntity(p);
    ASSERT_EQ(system->GetEntities().size(), 2);

    initGraph(system);
    system->ScheduleDelivery(p, c, p->GetDetails());
    ASSERT_EQ(p->GetCustomer(), c);
    ASSERT_EQ(p->GetDrone(), nullptr);
  }


  // Test 11
  TEST_F(FactoryTest, DestEntitySetOnScheduling2) {
    Package *p = MakePackage(system, 10, 10, 10);
    Customer *c = MakeCustomer(system);
    system->AddEntity(c);
    system->AddEntity(p);
    ASSERT_EQ(system->GetEntities().size(), 2);

    initGraph(system);
    system->ScheduleDelivery(p, c, p->GetDetails());
    Drone *d = MakeDrone(system, "Q-36-01", 50.0, 50.0, 50.0);
    system->AddEntity(d);
    ASSERT_EQ(d->GetStatus(), "Waiting");
    ASSERT_EQ(d->GetDestEntity(), nullptr);
    system->ScheduleDelivery(p, c, p->GetDetails());
    system->Update(0.01);
    ASSERT_EQ(d->GetDestEntity(), p);
  }


  // Test 12
  TEST_F(FactoryTest, PackageRemovedAfterDelivery) {
    Drone *d = MakeDrone(system, "Q-36-01", 498.29, 253.88, -228.62);
    Customer *c = MakeCustomer(system, -951.41, 254.66, 298.270996);
    Package *p = MakePackage(system, -255.99, 257.80, -296.86);
    system->AddEntity(d);
    system->AddEntity(c);
    system->AddEntity(p);
    ASSERT_EQ(system->GetEntities().size(), 3);

    initGraph(system);
    system->ScheduleDelivery(p, c, p->GetDetails());
    for (int i = 0; i < 5000; i++) {
      system->Update(0.01);
      Vector3 pos = d->GetPositionAsVector();
    }
    ASSERT_EQ(system->GetEntities().size(), 2);
  }
}  // namespace csci3081
