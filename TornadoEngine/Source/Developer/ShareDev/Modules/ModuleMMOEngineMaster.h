/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ModuleMMOEngineMasterH
#define ModuleMMOEngineMasterH

#include "ModuleMMOEngine.h"
#include "Master.h"

class DllExport TModuleMMOEngineMaster : 
  public TModuleMMOEngine<nsMMOEngine::TMaster>
{
protected:
  virtual void Input(int id_sender, void* p, int size);
  virtual bool Output(void* p, int size);
};

#endif
