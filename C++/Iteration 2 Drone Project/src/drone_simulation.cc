#include <string>
#include <vector>
#include <drone_simulation.h>

#define DEBUG 0

namespace CS = csci3081;

CS::DroneSimulation::DroneSimulation() {
  this->teamName_ = "CSCI 3081W Section 020: Team 6";
  this->scheduler_ = new CS::Scheduler();
  this->entityCreator_ = new CS::EntityCreator();
}

CS::DroneSimulation::~DroneSimulation() {
  while (entities_.size() != 0) {
    entity_project::Entity *e = entities_.at(0);
    RemoveEntity(e);
    delete e;
  }
  delete scheduler_;
  delete entityCreator_;
}

/// Create an entity of type specified by JSON object.
entity_project::Entity *CS::DroneSimulation::CreateEntity(const picojson::object& val) {
#if DEBUG
  // Investigate json object that is passed in
  JsonHelper::PrintEntityDetails(val);
#endif
  return entityCreator_->CreateEntity(val);
}

/// Add an already created entity into the system.
void CS::DroneSimulation::AddEntity(entity_project::Entity* entity) {
  if (entity != nullptr) {
    static entity_project::EntityConsoleLogger logger;
    // Add entity into entity vector
    entities_.push_back(entity);
#if DEBUG
    std::cout << "entities_ size now " << entities_.size() << std::endl;
#endif
    // Add entity into type-specific vector in scheduler
    scheduler_->AddEntity(entity);

    // Add logger observer to entity
    AddObserver(entity, &logger);
  }
}

void CS::DroneSimulation::SetGraph(const entity_project::IGraph *graph) {
  graph_ = graph;
}

void CS::DroneSimulation::RemoveEntity(entity_project::Entity *entity) {
  for (int i = 0; i < entities_.size(); i++) {
    if (entity == entities_.at(i)) {
      entities_.erase(entities_.begin() + i);
      scheduler_->RemoveEntity(entity);
    }
  }
}

/// Schedule a drone delivery for a package to a customer. Handled by Scheduler class
void CS::DroneSimulation::ScheduleDelivery(entity_project::Package* package,
                                           entity_project::Customer* dest,
                                           const picojson::object& details) {
  // Checks if package and dest are added entities before scheduling
  int cnt = 0;
  for (int i = 0; i < entities_.size(); i++) {
    if (entities_.at(i) == package || entities_.at(i) == dest) {
      cnt++;
    }
  }

  // Check if package is close enough to customer to not need a drone
  // If so, remove package and customer right away
  CS::Vector3 packageLoc = Vector3(package->GetPosition());
  CS::Vector3 customerLoc = Vector3(dest->GetPosition());
  if (packageLoc.DistanceBetween(customerLoc) <= (package->GetRadius() + dest->GetRadius())) {
    RemoveEntity(package);
    RemoveEntity(dest);
    cnt = 0;  // Ensure final if statement isn't executed
  }

  // schedule if package and dest are entities in entities_ vector
  if (cnt == 2) {
    scheduler_->ScheduleDelivery(package, dest, graph_, details);
  }
}

/// Add an observer to a specific entity
void CS::DroneSimulation::AddObserver(entity_project::Entity* entity,
                                      entity_project::EntityObserver* observer) {
  std::string type;

  if (entity->GetDetails().find("type")->second.is<std::string>()) {
    type = entity->GetDetails().find("type")->second.get<std::string>();
  }

  if (type == "package") {
    CS::Package *package = entity->AsType<CS::Package>();
    package->AddObserver(observer);
  }
}

/// Remove an observer from a specific entity
void CS::DroneSimulation::RemoveObserver(entity_project::Entity* entity,
                                         entity_project::EntityObserver* observer) {
  std::string type;

  if (entity->GetDetails().find("type")->second.is<std::string>()) {
    type = entity->GetDetails().find("type")->second.get<std::string>();
  }

  if (type == "package") {
    CS::Package *package = entity->AsType<CS::Package>();
    package->RemoveObserver(observer);
  }
}

/// Updates the system time by dt
void CS::DroneSimulation::Update(float dt) {
#if DEBUG
  std::cout << "dt: " << dt << std::endl;
#endif
  std::string type;

  for (int i = 0; i < entities_.size(); i++) {
    // Get entity from vector
    entity_project::Entity *e = entities_.at(i);

    // find entity type
    if (e->GetDetails().find("type")->second.is<std::string>()) {
      type = e->GetDetails().find("type")->second.get<std::string>();
    }

    if (type == "drone") {
      CS::Drone *d = e->AsType<CS::Drone>();
      d->Update(dt);
      /*
	if (d->GetStatus() == "Waiting") {
        std::vector<Package *> packages = scheduler_->GetPackages();
        for (int i = 0; i < packages.size(); i++) {
          CS::Package *p = packages.at(i);
          // If package needs to be shipped to a customer and drone can
          // carry it, assigns drone to package
          if (p->GetStatus() == "Ready to ship" &&
              (d->GetRemainingCapacity() >= p->GetWeight())) {
            // Get route from drone to package
            CS::Vector3 packageLocationVector = Vector3(p->GetPosition());
            entity_project::SimpleUMNRoutePoint pLoc = packageLocationVector.ToRoutePoint();
            entity_project::SimpleUMNRoutePoint dLoc = d->GetPositionAsVector().ToRoutePoint();
            d->SetRoute(dLoc, pLoc);

            // Set first route waypoint as drone's nextWaypoint
            CS::Vector3 wpt = Vector3(d->GetFirstWaypoint());

            d->SetNextWaypoint(wpt);
            d->SetDestination(Vector3(p->GetPosition()));
            d->SetDestEntity(p);

            d->OnStatusChange("Travelling to package", p);
            p->OnStatusChange("Waiting on drone", d);
          }
        }
      }*/
    } else if (type == "customer") {
      CS::Customer *c = e->AsType<CS::Customer>();
      c->Update(dt);

    } else if (type == "package") {
      CS::Package *package = e->AsType<CS::Package>();
      package->Update(dt);
      if (package->GetStatus() == "Ready to ship") {
        ScheduleDelivery(package, package->GetCustomer(), package->GetDetails());
      } else if (package->GetStatus() == "Delivered") {
        // OBSERVER
        package->NotifyObservers("delivered");
        RemoveEntity(package);
#if DEBUG
        std::cout << "package removed from entities_" << std::endl;
#endif
      }
    }
  }
}
