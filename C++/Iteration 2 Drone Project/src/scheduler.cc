#include <scheduler.h>
#include <vector>

#define DEBUG 0

namespace CS = csci3081;

void CS::Scheduler::ScheduleDelivery(entity_project::Package* package,
                                     entity_project::Customer* customer,
                                     const entity_project::IGraph *graph,
                                     const picojson::object& details) {
  // Cast to csci3081::<type>
  CS::Package *p = package->AsType<CS::Package>();
  CS::Customer *c = customer->AsType<CS::Customer>();
  CS::Drone *d;

  CS::PathFinder pf;
  pf.SetGraph(graph);


  // assign customer to package, update statuses
  p->SetCustomer(c);
  p->OnStatusChange("Ready to ship", c);
  c->OnStatusChange("Waiting for package", p);

  for (int i = 0; i < drones_.size(); i++) {
    d = drones_.at(i);

#if DEBUG
    std::cout << "Drone status: " << d->GetStatus() << std::endl;
    std::cout << "Drone capacity: " << d->GetRemainingCapacity() << std::endl;
    std::cout << "Package weight: " << p->GetWeight() << std::endl;
#endif

    if (d->GetStatus() == "Waiting" &&
        (d->GetRemainingCapacity() >= p->GetWeight())) {
#if DEBUG
      std::cout << "Drone found" << std::endl;
#endif
      // Get route from drone to package
      CS::Vector3 packageLocationVector = Vector3(p->GetPosition());

      CS::Vector3 customerLocationVector = Vector3(c->GetPosition());
      std::string closestPackagePoint = pf.findClosestPoint(packageLocationVector);
      std::string closestDronePoint = pf.findClosestPoint(d->GetPositionAsVector());
      std::string closestCustomerPoint = pf.findClosestPoint(customerLocationVector);
      // Find shortest path from drone to package
      std::vector<Vector3> d_to_p_route = pf.algorithm(closestDronePoint, closestPackagePoint);
      std::vector<std::string> str_d_to_p = pf.GetStrRoute();
      // str_d_to_p.push_back(closestPackagePoint);
      d->SetStrRoute(str_d_to_p, 0);
      /*
	std::cout << str_d_to_p.size() << std::endl;
      for (int i = 0; i < str_d_to_p.size(); i++) {
        std::cout << str_d_to_p.at(i) << std::endl;
      } */

      // Find shortest path from package to customer
      std::vector<Vector3> p_to_c_route = pf.algorithm(closestPackagePoint, closestCustomerPoint);
      std::vector<std::string> str_p_to_c = pf.GetStrRoute();
      // str_p_to_c.push_back(closestCustomerPoint);
      d->SetStrRoute(str_p_to_c, 1);
      // Add package location to end of drone->package route
      std::vector<float> p_pos = p->GetPosition();
      d_to_p_route.push_back(Vector3(p_pos));
      // Add customer location to end of package->customer route
      std::vector<float> c_pos = c->GetPosition();
      p_to_c_route.push_back(Vector3(c_pos));

#if DEBUG
      // Print out each vector3 in route
      std::cout << "FROM DRONE TO PACKAGE" << std::endl;
      for (int i = 0; i < d_to_p_route.size(); i++) {
        std::cout << d_to_p_route.at(i).toString() << std::endl;
      }
      std::cout << std::endl << "FROM PACKAGE TO CUSTOMER" << std::endl;;
      for (int i = 0; i < p_to_c_route.size(); i++) {
        std::cout << p_to_c_route.at(i).toString() << std::endl;
      }
      std::cout << std::endl;
#endif

      d->SetRoute(d_to_p_route, p_to_c_route);
      // Set first route waypoint as drone's nextWaypoint
      CS::Vector3 wpt = d_to_p_route.at(0);
      d->SetNextWaypoint(wpt);
      d->SetDestination(Vector3(p->GetPosition()));
      d->SetDestEntity(p);



      // Set first route waypoint as drone's nextWaypoint
      // CS::Vector3 wpt = Vector3(d->GetFirstWaypoint());
      // d->SetNextWaypoint(wpt);
      d->SetDestination(Vector3(p->GetPosition()));
      d->SetDestEntity(p);

      d->OnStatusChange("Travelling to package", p);
      break;
    }
  }
}

void CS::Scheduler::AddEntity(entity_project::Entity *e) {
  std::string type;
  if (e->GetDetails().find("type")->second.is<std::string>()) {
    type = e->GetDetails().find("type")->second.get<std::string>();

    if (type == "drone") {
      Drone *d = e->AsType<Drone>();
      drones_.push_back(d);
#if DEBUG
      std::cout << "drones_ size now " << drones_.size() << std::endl;
#endif
    } else if (type == "customer") {
      Customer *c = e->AsType<Customer>();
      customers_.push_back(c);
#if DEBUG
      std::cout << "customers_ size now " << customers_.size() << std::endl;
#endif
    } else if (type == "package") {
      Package *p = e->AsType<Package>();
      packages_.push_back(p);
#if DEBUG
      std::cout << "packages_ size now " << packages_.size() << std::endl;
#endif
    }
  }
}

void CS::Scheduler::RemoveEntity(entity_project::Entity *e) {
  for (int i = 0; i < drones_.size(); i++) {
    if (e == drones_.at(i)) {
      drones_.erase(drones_.begin() + i);
    }
  }
  for (int i = 0; i < customers_.size(); i++) {
    if (e == customers_.at(i)) {
      customers_.erase(customers_.begin() + i);
    }
  }
  for (int i = 0; i < packages_.size(); i++) {
    if (e == packages_.at(i)) {
      packages_.erase(packages_.begin() + i);
    }
  }
}
