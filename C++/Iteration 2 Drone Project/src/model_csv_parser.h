/**
 * @file model_csv_parser.h
 */

#ifndef CSCI3081_MODEL_CSV_PARSER
#define CSCI3081_MODEL_CSV_PARSER

#include <drone_type.h>
#include <map>
#include <string>

namespace csci3081 {

/**
 * @brief A class that converts project/data/planet-x.csv into a map format
 *
 * The unordered setmap created by this class is referenced by the
 * DroneCreator to create a drone of a specific model.
 */
class ModelCSVParser {
 public:
  /**
   * @brief Parses project/data/planet-x.csv
   *
   * @return A map associating a model name (e.g. "Q-36-01") with a
   * set of attributes, stored in a DroneType object.
   */
  std::map<std::string, DroneType> CreateSet();
};

}  // namespace csci3081

#endif  // CSCI3081_MODEL_CSV_PARSER
