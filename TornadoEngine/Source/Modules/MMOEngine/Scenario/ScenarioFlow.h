/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#pragma once

#include "IScenario.h"
#include "ContextScFlow.h"
#include "MakerScenario.h"
#include "SrcEvent_ex.h"
#include "ScenarioBaseHeader.h"

#ifdef WIN32
#pragma pack(push, 1)
#endif

namespace nsMMOEngine
{
  class TScenarioFlow : public IScenario
  {
    enum{ eUp, eDown };
    struct THeaderFlow : public TScenarioBaseHeader
    {
      THeaderFlow(){ type = TMakerScenario::eFlow; }
    }_PACKED;
    //-------------------------------------------------
    struct THeaderSendUp : public THeaderFlow
    {
      THeaderSendUp(){ subType = eUp; }
    }_PACKED;
    //-------------------------------------------------
    struct THeaderSendDown : public THeaderFlow
    {
      THeaderSendDown(){ subType = eDown; }
    }_PACKED;
    //-------------------------------------------------
  public:
    TScenarioFlow();
    virtual ~TScenarioFlow();

    void SendUp( TBreakPacket& bp, bool check );
    void SendDown( TBreakPacket& bp, bool check );

    virtual void Recv( TDescRecvSession* pDesc );
  protected:
    virtual void DelayBegin();
  private:
    TContextScFlow* Context(){ return (TContextScFlow*) mCurContext; }
    // для Send
    void HandlePacket( TBreakPacket& bp, bool check );
    // для Recv
    template <class TypeSrc>
    void Recv( TDescRecvSession* pDesc )
    {
      // защита от хака, могут прислать пакет меньшего размера, сервер выйдет за границы памяти и упадет
      if( pDesc->dataSize < sizeof( THeaderFlow ) )
        return;
      // создать событие
      TypeSrc* pEvent = new TypeSrc;
      // отцепиться от памяти, в которой содержится пакет
      // отдать память под контроль события
      pEvent->c = pDesc->c;
      pEvent->SetShift( sizeof( THeaderFlow ) );

      // откуда пришел пакет - сессия
      pEvent->sessionID = pDesc->sessionID;
      // добавить событие без копирования и указать истинное время создания события в транспорте
      Context()->GetSE()->AddEventWithoutCopy<TypeSrc>( pEvent, pDesc->time_ms );
    }
  };
  //-------------------------------------------------------------------
}

#ifdef WIN32
#pragma pack(pop)
#endif