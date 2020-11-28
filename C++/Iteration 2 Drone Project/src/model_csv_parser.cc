#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <model_csv_parser.h>

namespace CS = csci3081;

std::map<std::string, CS::DroneType> CS::ModelCSVParser::CreateSet() {
  // open csv file
  std::ifstream csv("data/planet-x.csv");
  std::string modelInfo;
  std::map<std::string, CS::DroneType> attributes;
  std::map<int, std::string> columns;

  // init storage attributes
  std::string model;
  float mass;
  float spd;
  float a;
  float weight;
  float battery;

  if (csv.is_open()) {
    // Get titles
    getline(csv, modelInfo);
    int i = 1;
    while (1) {
      std::string header = modelInfo.substr(0, modelInfo.find(','));
      if (header == "Model #") {
        columns.insert(std::pair<int, std::string>(i, "Model"));
      } else if (header == "Mass (kg)") {
        columns.insert(std::pair<int, std::string>(i, "Mass"));
      } else if (header == "Max Speed (km/h)") {
        columns.insert(std::pair<int, std::string>(i, "Speed"));
      } else if (header == "Base Acceleration (m/s^2)") {
        columns.insert(std::pair<int, std::string>(i, "Accel"));
      } else if (header == "WeightCapacity (kg)") {
        columns.insert(std::pair<int, std::string>(i, "Weight"));
      } else if (header == "Base Battery Capacity (seconds)") {
        columns.insert(std::pair<int, std::string>(i, "Battery"));
      } else {
        break;
      }
      if (i < 6) {
        modelInfo.erase(0, modelInfo.find(',')+1);
      } else {
        break;
      }
      i++;
    }
    while (getline(csv, modelInfo)) {
      i = 1;
      while (columns.find(i) != columns.end()) {
        // Get attribute stored in col i
        std::string att = columns.find(i)->second;

        if (att == "Model") {
          // parse model
          model = modelInfo.substr(0, modelInfo.find(','));
        } else if (att == "Mass") {
          // parse mass
          mass = std::stof(modelInfo.substr(0, modelInfo.find(',')));
        } else if (att == "Speed") {
          // parse max speed
          spd = std::stof(modelInfo.substr(0, modelInfo.find(',')));
        } else if (att == "Accel") {
          // parse acceleration
          a = std::stof(modelInfo.substr(0, modelInfo.find(',')));
        } else if (att == "Weight") {
          // parse weight capacity
          weight = std::stof(modelInfo.substr(0, modelInfo.find(',')));
        } else if (att == "Battery") {
          // parse battery
          battery = std::stof(modelInfo.substr(0, modelInfo.find(',')));
        }
        // move to next column
        modelInfo.erase(0, modelInfo.find(',')+1);
        i++;
      }

      // if model not already in attributes, create DroneType and add it
      if (attributes.find(model) == attributes.end()) {
        CS::DroneType *dtype = new DroneType(model, mass, spd, a, weight, battery);
        attributes.insert(std::pair<std::string, CS::DroneType>(model, *dtype));
        delete dtype;
      }
    }
    csv.close();
    return attributes;
  } else {
    std::cout << "ERROR: Cannot open planet-x.csv" << std::endl;
  }
  return attributes;
}
