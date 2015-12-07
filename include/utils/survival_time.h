#ifndef COLLIE_UTILS_SURVIVIAL_TIME_H
#define COLLIE_UTILS_SURVIVIAL_TIME_H

#include <memory>
#include <chrono>

namespace collie {
namespace utils {

class SurvivalTime {
 public:
  explicit SurvivalTime(const std::string& object_name);
  virtual ~SurvivalTime();
  const std::chrono::steady_clock::time_point kBornTime;
  const std::string kObjectName;
};
}
}

#endif /* COLLIE_UTILS_SURVIVAL_TIME_H */
