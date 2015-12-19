#include "../../include/utils/survival_time.h"
#include "../../include/logging.h"

namespace collie {
namespace utils {

SurvivalTime::SurvivalTime(const std::string& object_name)
    : kObjectName(object_name) {}

SurvivalTime::~SurvivalTime() {
  auto end_time = std::chrono::steady_clock::now();

  LOG(INFO) << "The survival time of" << kObjectName << ":"
            << std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                                     kBornTime)
                   .count()
            << "ms";
}
}
}
