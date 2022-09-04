/*
* Created by boil on 2022/8/28.
*/

#ifndef RENDU_EVENT_SYSTEM_H_
#define RENDU_EVENT_SYSTEM_H_

#include "system.h"
#include "singleton.h"

class EventSystem : public Singleton<EventSystem>, public ISystem {

public:
  void Register() override;

  void Destroy() override;

  const std::type_info &GetType() override;
};


#endif //RENDU_EVENT_SYSTEM_H_
