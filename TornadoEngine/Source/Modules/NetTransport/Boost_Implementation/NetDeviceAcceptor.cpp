/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "NetDeviceAcceptor.h"
#include "BL_Debug.h"
#include "HiTimer.h"

#include "Logger.h"
#include "INetTransport.h"
#include "ResolverSelf_IP_v4.h"
#include "NetControlTCP.h"
#include "EnumMMO.h"

using namespace boost::asio;
using namespace std;


TNetDeviceAcceptor::TNetDeviceAcceptor( boost::asio::io_service& io_service ) :
  mSocket( io_service )
{

}
//--------------------------------------------------------------------------------
TNetDeviceAcceptor::~TNetDeviceAcceptor()
{

}
//--------------------------------------------------------------------------------
bool TNetDeviceAcceptor::Open( unsigned short port, unsigned char numNetWork )
{
  bool res = false;

  std::string sLocalHost;
  TResolverSelf_IP_v4 resolver;
  if( resolver.Get( sLocalHost, numNetWork ) == false )
    return false;
  try
  {
    const ip::address_v4 ipv4_address_Local = ip::address_v4::from_string( sLocalHost );
    const ip::address addr_Local( ipv4_address_Local );
    const ip::tcp::endpoint endpoint_Local( addr_Local, port );
    mSocket.open( endpoint_Local.protocol() );

    SetReUse();
    OffNagl();
    Set_HardClose();

    mSocket.bind( endpoint_Local );
    // listen
    mSocket.listen( eMaxCountListen );

    mIP_Port.Set( ipv4_address_Local.to_ulong(), port );
    res = true;
  }
  catch( std::exception& e )
  {
    GetLogger( STR_NAME_NET_TRANSPORT )->
      WriteF_time( "Open Acceptor (%d,%d) FAIL: %s.\n", port, numNetWork, e.what() );
  }
  return res;
}
//--------------------------------------------------------------------------------
void TNetDeviceAcceptor::Close()
{
  if( mSocket.is_open() == false ) return;
  try
  {
    mSocket.close();
  }
  catch( std::exception& e )
  {
    GetLogger( STR_NAME_NET_TRANSPORT )->
      WriteF_time( "Close Acceptor FAIL: %s.\n", e.what() );
  }
}
//--------------------------------------------------------------------------------
void TNetDeviceAcceptor::OffNagl()
{
  ip::tcp::no_delay option( true );
  mSocket.set_option( option );
}
//--------------------------------------------------------------------------------
void TNetDeviceAcceptor::SetReUse()
{
  mSocket.set_option( ip::tcp::acceptor::reuse_address( true ) );
}
//--------------------------------------------------------------------------------
void TNetDeviceAcceptor::Set_HardClose()
{
  boost::asio::socket_base::linger option( true, 0 );
  mSocket.set_option( option );
}
//--------------------------------------------------------------------------------
bool TNetDeviceAcceptor::SetRecvBuffer( unsigned int size )
{
  boost::system::error_code ec;
  boost::asio::socket_base::receive_buffer_size option( size );
  mSocket.set_option( option, ec );
  return (ec == 0);
}
//--------------------------------------------------------------------------------
bool TNetDeviceAcceptor::SetSendBuffer( unsigned int size )
{
  boost::system::error_code ec;
  boost::asio::socket_base::send_buffer_size option( size );
  mSocket.set_option( option, ec );
  return (ec == 0);
}
//--------------------------------------------------------------------------------
