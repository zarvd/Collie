#include "../inc/logger.h"

using namespace collie;

int main(void) {
  Logger::Init();
  std::string hi = "Hi;";
  Logger::SetLogLevel(DEBUG);
  LOG(INFO) << hi;
  LOG(DEBUG) << "Hello, world";
  LOG(INFO) << "Hello, world";
  LOG(WARN) << "1";
  LOG(ERROR) << "1";
  LOG(INFO) << 1;
  LOG(INFO) << 1.2;
  int a = 1;
  LOG(INFO) << a;
  LOG(INFO) << 1.4124124;
  return 0;
}
