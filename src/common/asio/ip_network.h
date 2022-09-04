/*
* Created by boil on 2022/8/20.
*/

#ifndef RENDU_IP_NETWORK_H_
#define RENDU_IP_NETWORK_H_

#include "define.h"
#include "ip_address.h"
#include <boost/asio/ip/network_v4.hpp>
#include <boost/asio/ip/network_v6.hpp>

namespace rendu::net {
    inline bool IsInNetwork(boost::asio::ip::address_v4 const &networkAddress, boost::asio::ip::address_v4 const &mask,
                            boost::asio::ip::address_v4 const &clientAddress) {
      boost::asio::ip::network_v4 network = boost::asio::ip::make_network_v4(networkAddress, mask);
      boost::asio::ip::address_v4_range hosts = network.hosts();
      return hosts.find(clientAddress) != hosts.end();
    }

    inline boost::asio::ip::address_v4 GetDefaultNetmaskV4(boost::asio::ip::address_v4 const &networkAddress) {
      if ((address_to_uint(networkAddress) & 0x80000000) == 0)
        return boost::asio::ip::address_v4(0xFF000000);
      if ((address_to_uint(networkAddress) & 0xC0000000) == 0x80000000)
        return boost::asio::ip::address_v4(0xFFFF0000);
      if ((address_to_uint(networkAddress) & 0xE0000000) == 0xC0000000)
        return boost::asio::ip::address_v4(0xFFFFFF00);
      return boost::asio::ip::address_v4(0xFFFFFFFF);
    }

    inline bool IsInNetwork(boost::asio::ip::address_v6 const &networkAddress, uint16 prefixLength,
                            boost::asio::ip::address_v6 const &clientAddress) {
      boost::asio::ip::network_v6 network = boost::asio::ip::make_network_v6(networkAddress, prefixLength);
      boost::asio::ip::address_v6_range hosts = network.hosts();
      return hosts.find(clientAddress) != hosts.end();
    }
  }

#endif // RENDU_IP_NETWORK_H_
