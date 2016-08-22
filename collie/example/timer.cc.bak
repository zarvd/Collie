#include "../inc/timer.h"
#include <iostream>

using namespace collie;

int main(void) {
  auto &q = TimerQueue::GetInstance();
  q.Start();
  TimerQueue::TimeUnit t[] = {
      TimerQueue::TimeUnit(100),
      TimerQueue::TimeUnit(50),
      TimerQueue::TimeUnit(30),
      TimerQueue::TimeUnit(120)};
  for (int i = 0; i < 4; ++i) {
    q.Insert(t[i], [i] { std::cout << "t" << i << std::endl; });
  }
  std::this_thread::sleep_for(TimerQueue::TimeUnit(3000));
  q.Stop();
  return 0;
}
