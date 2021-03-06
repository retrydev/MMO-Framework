/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ���������
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "CommonParam.h"
#include "Logger.h"
#include "EnumMMO.h"

void InitLogger( const char* loggerName )
{
  // ����������� ���������������� ���
  GetLogger()->Register( STR_NAME_MMO_ENGINE );
  GetLogger()->Register( STR_NAME_NET_TRANSPORT );
  GetLogger()->Register( loggerName );

  GetLogger()->Init( loggerName );
  GetLogger()->SetPrintf( false );
  GetLogger()->SetEnable( false );
//#ifdef _DEBUG
  GetLogger()->Get( loggerName )->SetEnable( true );
  GetLogger()->Get( loggerName )->SetPrintf( true );

  //GetLogger()->Get( STR_NAME_MMO_ENGINE )->SetEnable( true );
  //GetLogger()->Get( STR_NAME_MMO_ENGINE )->SetPrintf( true );
  //GetLogger()->Get( STR_NAME_NET_TRANSPORT )->SetEnable( true );
  //GetLogger()->Get( STR_NAME_NET_TRANSPORT )->SetPrintf( true );
  //#endif
}

