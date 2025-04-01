/*
* Created by boil on 25-2-26.
*/

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

namespace core
{
    // 系统接口
    class ISystem {
    public:
        virtual ~ISystem() = default;
        virtual void configure() = 0;
        virtual void update(double delta_time) = 0;
        virtual bool is_parallel_safe() const = 0;
    };

} // namespace rendu

#endif //SYSTEM_HPP
