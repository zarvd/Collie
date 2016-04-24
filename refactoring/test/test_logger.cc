#include "../inc/logger.h"

using namespace collie;

int main(void) {
  std::string hi = "Hi;";
  LOG(INFO) << hi;
  LOG(DEBUG) << "Hello, world";
  LOG(INFO) << "Hello, world";
  LOG(ERROR) << "1";
  LOG(INFO) << 1;
  LOG(INFO) << 1.2;
  int a = 1;
  LOG(INFO) << a;
  LOG(INFO) << 1.4124124;
  return 0;
}
