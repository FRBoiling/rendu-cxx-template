/*
* Created by boil on 2022/8/20.
*/
#ifndef RENDU_IO_CONTEXT_H_
#define RENDU_IO_CONTEXT_H_

#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>

namespace rendu
{
    namespace asio
    {
        class IoContext
        {
        public:
            IoContext() : _impl() { }
            explicit IoContext(int concurrency_hint) : _impl(concurrency_hint) { }

            operator boost::asio::io_context&() { return _impl; }
            operator boost::asio::io_context const&() const { return _impl; }

            std::size_t run() { return _impl.run(); }
            void stop() { _impl.stop(); }

            boost::asio::io_context::executor_type get_executor() noexcept { return _impl.get_executor(); }

        private:
            boost::asio::io_context _impl;
        };

        template<typename T>
        inline decltype(auto) post(boost::asio::io_context& ioContext, T&& t)
        {
            return boost::asio::post(ioContext, std::forward<T>(t));
        }

        template<typename T>
        inline decltype(auto) get_io_context(T&& ioObject)
        {
            return ioObject.get_executor().context();
        }
    }
}

#endif // RENDU_IO_CONTEXT_H_
