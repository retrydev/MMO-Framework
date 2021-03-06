/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef PSM_TypePacketH
#define PSM_TypePacketH

#include "TypeDef.h"

#ifdef WIN32
#pragma pack(push, 1)
#endif

namespace nsPSM
{
  enum
  { 
    ePacket_ClientTryEnterRoom,// указать id_client, тип боя - от этого зависит кол-во игроков
    ePacket_ClientTryEnterGarage,

    ePacket_ClientEnterGarage,
    ePacket_ClientEnterQueue,
    ePacket_ClientEnterRoom,// указать id_room
    
    // уникальный для кластера id_room
    ePacket_AddRoom,
    ePacket_DeleteRoom,

    ePacket_GameImpl,
  };
  
  struct DllExport TBasePacket_PSM
  {
    TBasePacket_PSM(unsigned char _type){type = _type;}
    unsigned char type;
  }_PACKED;
}

#ifdef WIN32
#pragma pack(pop)
#endif

#endif
