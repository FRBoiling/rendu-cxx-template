/**********************************
**  Created by boil on 2022/8/14.
***********************************/

#ifndef ProtobufJSON_h__
#define ProtobufJSON_h__

#include "define.h"
#include <string>

namespace google::protobuf {
  class Message;
}

namespace JSON {
  RD_SHARED_API std::string Serialize(google::protobuf::Message const &message);

  RD_SHARED_API bool Deserialize(std::string const &json, google::protobuf::Message *message);
}

#endif // ProtobufJSON_h__
