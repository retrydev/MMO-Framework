/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#pragma once

#include <map>
#include <boost/bimap/bimap.hpp>
#include "DelegateManagerContextSc.h"

namespace nsMMOEngine
{
  class TContainerContextSc;
  class TManagerContextClientLogining : public TDelegateManagerContextSc
  {
    typedef std::map<unsigned int, TContainerContextSc*> TMapUintPtr;
    typedef TMapUintPtr::iterator TMapUintPtrIt;

    typedef boost::bimaps::bimap<unsigned int,unsigned int> bmUintUint;

    TMapUintPtr  mMapSessionContext;
    bmUintUint   mMapSessionKey;
    
  public:
    TManagerContextClientLogining(TBase* pBase);
    ~TManagerContextClientLogining();
    
    // навигация
    bool FindSessionByClientKey(unsigned int id, unsigned int& sessionID);
    bool FindClientKeyBySession(unsigned int sessionID, unsigned int& id);
    TContainerContextSc* FindContextBySession(unsigned int sessionID);
    
    // добавление/удаление
    TContainerContextSc* AddContext(unsigned int sessionID);
    bool AddKeyBySession(unsigned int sessionID, unsigned int id_client);
    // удаление
    void DeleteBySession(unsigned int sessionID);
    // отцепиться
    void UnlinkContextBySession(unsigned int sessionID);
  private:
    void Clear();
  };
}
