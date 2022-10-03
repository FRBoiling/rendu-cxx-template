/*
* Created by boil on 2022/9/25.
*/

#ifndef RENDU_RANDOM_H_
#define RENDU_RANDOM_H_

#include "define.h"
#include "duration.h"
#include <limits>

namespace rendu {
//  返回min..max(inclusive)范围内的随机数。
  RD_COMMON_API int32 irand(int32 min, int32 max);

//  返回min..max(inclusive)范围内的随机数。
  RD_COMMON_API uint32 urand(uint32 min, uint32 max);

//  返回最小和最大秒之间的随机毫秒值。函数等价于urand(min*IN_MILLISECONDS, max*IN_MILLISECONDS)
  RD_COMMON_API uint32 urandms(uint32 min, uint32 max);

//  返回0 ..UINT32_MAX。
  RD_COMMON_API uint32 rand32();

//  返回min..max(最多毫秒精度)范围内的随机时间。仅适用于毫秒差为有效uint32的值
  RD_COMMON_API Milliseconds randtime(Milliseconds min, Milliseconds max);

//  返回min..max范围内的随机浮点数。
  RD_COMMON_API float frand(float min, float max);

//  返回一个从0.0到1.0(exclusive)的随机双精度浮点值。
  RD_COMMON_API double rand_norm();

//  返回一个从0.0到100.0(exclusive)的随机双精度值。
  RD_COMMON_API double rand_chance();

//  返回0..计数(exclusive)，每个值有不同的发生几率
  RD_COMMON_API uint32 urandweighted(size_t count, double const *chances);

//  如果随机滚动符合指定的机会(范围0-100)则返回true
  inline bool roll_chance_f(float chance) {
    return chance > rand_chance();
  }

//  如果随机滚动符合指定的机会(范围0-100)则返回true
  inline bool roll_chance_i(int chance) {
    return chance > irand(0, 99);
  }

/*
* Wrapper satisfying UniformRandomNumberGenerator concept for use in <random> algorithms
*/
  class RD_COMMON_API RandomEngine {
  public:
    typedef uint32 result_type;

    static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }

    static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }

    result_type operator()() const { return rand32(); }

    static RandomEngine &Instance();
  };

}//namespace rendu

#endif // RENDU_RANDOM_H_
