/**********************************
**  Created by boil on 2022/8/14.
***********************************/

#ifndef RENDU_PROTOBUF_JSON_H_
#define RENDU_PROTOBUF_JSON_H_

#include "define.h"
#include <string>

namespace google::protobuf {
  class Message;
}

namespace JSON {
  RD_SHARED_API std::string Serialize(google::protobuf::Message const &message);

  RD_SHARED_API bool Deserialize(std::string const &json, google::protobuf::Message *message);
}

#endif // RENDU_PROTOBUF_JSON_H_
