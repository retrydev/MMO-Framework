/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ManagedMMOEngineWrapperWSuperServerH
#define ManagedMMOEngineWrapperWSuperServerH

#include "WBaseServer.h"
#include "ISuperServer.h"

namespace ManagedMMOEngineWrapper
{
  public ref class WDescDownSuperServer
  {
    unsigned int id_session;
    int countClient;
  };

  public ref class WSuperServer : public WBaseServer
  {
    NativeMMOEngineWrapper::ISuperServer* mSuperServer;
  public:
    WSuperServer();
  };
}

#endif