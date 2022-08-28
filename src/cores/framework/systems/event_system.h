/*
* Created by boil on 2022/8/28.
*/

#ifndef RENDU_EVENT_SYSTEM_H_
#define RENDU_EVENT_SYSTEM_H_

#include "system.h"

class EventSystem :public System<EventSystem>,public ISystem{
public:
  int m_id;
public:
  void Register() override;

  void Destroy() override;

  std::string ToString() override;
};


#endif //RENDU_EVENT_SYSTEM_H_
