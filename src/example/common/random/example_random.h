/*
* Created by boil on 2022/9/12.
*/
#include <iostream>
#include "random.h"
#include "log.h"
#include "enum.h"

using namespace rendu;

void example_random() {
//  sfmt_t state;
//  std::random_device dev;
//  if (dev.entropy() > 0) {
//    std::array<uint32, SFMT_N32> seed{};
//    std::generate(seed.begin(), seed.end(), std::ref(dev));
//
//    sfmt_init_by_array(&state, seed.data(), seed.size());
//  } else
//  sfmt_init_gen_rand(&state, uint32(time(nullptr)));
//
//  std::this_thread::sleep_for(std::chrono::milliseconds(123));
//  for (int i = 0; i < 100; ++i) {
//    auto a = sfmt_genrand_uint32(&state);
//    spdlog::info("--->{}", a);
//  }

  RD_STOPWATCH sw;
  spdlog::info("-------0------");
  for (int i = 0; i < 100; ++i) {
    auto a = rand32();
    spdlog::info("--->{}", a);
  }


  spdlog::info("-------1------");
  for (int i = 0; i < 100; ++i) {
    auto a = irand(0,1);
    spdlog::info("--->{}", a);
  }


  spdlog::info("-------3------");
  for (int i = 0; i < 100; ++i) {
    auto a = urand(0,1);
    spdlog::info("--->{}", a);
  }

  spdlog::info("Stopwatch: {} seconds", sw);
}

