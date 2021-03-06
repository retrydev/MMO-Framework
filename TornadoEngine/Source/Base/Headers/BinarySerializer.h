/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#pragma once

#include "PushMaster.h"
#include "PopMaster.h"
#include "ContainerRise.h"
#include "TypeDef.h"

class DllExport TBinarySerializer
{
protected:
  TPushMaster mPushMaster;
  TPopMaster mPopMaster;
public:
  // Requirement: Type have to have 2 methods: Serialize(TPushMaster* p), Deserialize(TPopMaster* p)
  template<typename Type>
  inline void Serialize( Type* pObj, TContainerRise& c, int offset = 0 );
  template<typename Type>
  inline void Deserialize( Type* pObj, TContainerRise& c, int offset = 0 );
};
//--------------------------------------------------------------------
template<typename Type>
void TBinarySerializer::Serialize( Type* pObj, TContainerRise& c, int offset )
{
  mPushMaster.Clear();
  pObj->Serialize( &mPushMaster );
  mPushMaster.CopyInBuffer( c, offset );
}
//--------------------------------------------------------------------
template<typename Type>
void TBinarySerializer::Deserialize( Type* pObj, TContainerRise& c, int offset )
{
  mPopMaster.SetBuffer( &c, offset );
  pObj->Deserialize( &mPopMaster );
}
//--------------------------------------------------------------------
