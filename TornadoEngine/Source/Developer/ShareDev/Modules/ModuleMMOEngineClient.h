/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ModuleMMOEngineClientH
#define ModuleMMOEngineClientH

#include "ModuleMMOEngine.h"
#include "Client.h"

class DllExport TModuleMMOEngineClient : 
  public TModuleMMOEngine<nsMMOEngine::TClient>
{
};

#endif
