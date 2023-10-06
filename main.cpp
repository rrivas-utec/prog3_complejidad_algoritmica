#include <deque>
#include <list>
#include <unordered_map>
#include "benchmarker.h"

void general_test() {
  bench_marker<int, 400000, 1000,
               std::vector,
               std::deque,
               std::list,
               std::map> bm("test_result.csv");
  bm.execute();
}

int main() {
  general_test();
  return 0;
}