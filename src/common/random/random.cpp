/*
* Created by boil on 2022/9/25.
*/


#include "random.h"
#include "sfmt_rand.h"
#include <memory>
#include <random>
#include "random_define.h"

namespace rendu {

  static thread_local std::unique_ptr<SFMTRand> sfmtRand;
  static RandomEngine engine;

  static SFMTRand *GetRng() {
    if (!sfmtRand)
      sfmtRand = std::make_unique<SFMTRand>();

    return sfmtRand.get();
  }

  int32 irand(int32 min, int32 max) {
        assert(max >= min);
    std::uniform_int_distribution<int32> uid(min, max);
    return uid(engine);
  }

  uint32 urand(uint32 min, uint32 max) {
    assert(max >= min);
    std::uniform_int_distribution<uint32> uid(min, max);
    return uid(engine);
  }

  uint32 urandms(uint32 min, uint32 max) {
    assert(std::numeric_limits<uint32>::max() / Milliseconds::period::den >= max);
    return urand(min * Milliseconds::period::den, max * Milliseconds::period::den);
  }

  float frand(float min, float max) {
    assert(max >= min);
    std::uniform_real_distribution<float> urd(min, max);
    return urd(engine);
  }

  Milliseconds randtime(Milliseconds min, Milliseconds max) {
    long long diff = max.count() - min.count();
    assert(diff >= 0);
    assert(diff <= (uint32) -1);
    return min + Milliseconds(urand(0, diff));
  }

  uint32 rand32() {
    return GetRng()->RandomUInt32();
  }

  double rand_norm() {
    std::uniform_real_distribution<double> urd;
    return urd(engine);
  }

  double rand_chance() {
    std::uniform_real_distribution<double> urd(0.0, 100.0);
    return urd(engine);
  }

  uint32 urandweighted(size_t count, double const *chances) {
    std::discrete_distribution<uint32> dd(chances, chances + count);
    return dd(engine);
  }

  RandomEngine &RandomEngine::Instance() {
    return engine;
  }
}//namespace rendu
