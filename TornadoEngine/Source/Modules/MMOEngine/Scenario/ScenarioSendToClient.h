/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#pragma once

#include "IScenario.h"
#include "ContextScSendToClient.h"
#include "MakerScenario.h"
#include "SessionManager.h"
#include "ScenarioBaseHeader.h"

#ifdef WIN32
#pragma pack(push, 1)
#endif

namespace nsMMOEngine
{
  class TScenarioSendToClient : public IScenario
  {
    enum
    {
      eSuperServer,
      eMaster,
      eSlave,
    };
    //-------------------------------------------------
    struct THeader : public TScenarioBaseHeader
    {
      THeader(){ type = TMakerScenario::eSendToClient; id_client = 0; }
      unsigned int id_client;
    }_PACKED;
    //==================================================================
    struct THeaderSuperServer : public THeader
    {
      THeaderSuperServer(){ subType = eSuperServer; }
    }_PACKED;
    //-------------------------------------------------
    struct THeaderMaster : public THeader
    {
      THeaderMaster(){ subType = eMaster; }
    }_PACKED;
    //-------------------------------------------------
    struct THeaderSlave : public THeader
    {
      THeaderSlave(){ subType = eSlave; }
    }_PACKED;
    //-------------------------------------------------
  public:
    TScenarioSendToClient();
    virtual ~TScenarioSendToClient();

    virtual void Recv( TDescRecvSession* pDesc );
  public:
    void SendFromSuperServer( std::list<unsigned int>& lKey, TBreakPacket& bp );
    void SendFromMaster( std::list<unsigned int>& lKey, TBreakPacket& bp );
    void SendFromSlave( std::list<unsigned int>& lKey, TBreakPacket& bp );
  protected:

    void RecvFromSuperServer( TDescRecvSession* pDesc );
    void RecvFromMaster( TDescRecvSession* pDesc );
    void RecvFromSlave( TDescRecvSession* pDesc );

  protected:
    TContextScSendToClient * Context(){ return (TContextScSendToClient*) mCurContext; }
    virtual void DelayBegin();
  private:
    template <class T>
    void SendAll( std::list<unsigned int>& lKey, TBreakPacket& bp );

    template <class T>
    void Send( unsigned int id_client, TBreakPacket& bp );
  };
  //------------------------------------------------------------------------------
  template <class T>
  void TScenarioSendToClient::Send( unsigned int id_client, TBreakPacket& bp )
  {
    T h;
    h.id_client = id_client;
    bp.PushFront( (char*) &h, sizeof( h ) );

    unsigned int sessionID = Context()->GetSessionID();
    Context()->GetMS()->Send( sessionID, bp );
  }
  //------------------------------------------------------------------------------
  template <class T>
  void TScenarioSendToClient::SendAll( std::list<unsigned int>& lKey, TBreakPacket& bp )
  {
    if( bp.GetSize() == 0 )
      return;// нельзя передавать нулевые пакеты
    for( unsigned int id_client : lKey )
    {
      NeedContextByClientKey( id_client );
      if( Context() )
      {
        if( Begin() == false )
        {
          mBP = bp;
          T h;
          h.id_client = id_client;
          mBP.PushFront( (char*) &h, sizeof( h ) );
          // пока отослать нельзя, сохранить пакет до момента возможности
          Context()->SaveBreakPacket( mBP );
          continue;
        }
        Send<T>( id_client, bp );
        End();
      }
    }
  }
  //-------------------------------------------------------------------
}

#ifdef WIN32
#pragma pack(pop)
#endif
