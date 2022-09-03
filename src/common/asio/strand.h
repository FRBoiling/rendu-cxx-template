/*
* Created by boil on 2022/8/20.
*/
#ifndef RENDU_STRAND_H_
#define RENDU_STRAND_H_

#include "io_context.h"
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/strand.hpp>

namespace rendu::asio {
  /**
    Hack to make it possible to forward declare strand (which is a inner class)
  */
  class Strand : public boost::asio::io_context::strand {
  public:
    Strand(IoContext &ioContext) : boost::asio::io_context::strand(ioContext) {}
  };

  using boost::asio::bind_executor;
}

#endif // RENDU_STRAND_H_
