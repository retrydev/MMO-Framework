/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ClientLogic.h"

#include <Ogre.h>

#include "ListModules.h"
#include "Logger.h"

#include "ModuleTimer.h"
#include "ModuleGraphicEngine.h"
#include "ModulePhysicEngine.h"

#include "ClientMain.h"
#include "DebugPanel.h"

TClientLogic::TClientLogic()
{
  mClientMain = NULL;
  mDebugPanel = NULL;
  fallMotionState = NULL;
}
//----------------------------------------------------------
TClientLogic::~TClientLogic()
{

}
//----------------------------------------------------------
bool TClientLogic::WorkClient()
{
  return true;
}
//----------------------------------------------------------
void TClientLogic::Input(int id, void* p, int size)
{
  switch(id)
  {
    case nsListModules::GraphicEngine:
      break;
    case nsListModules::MMOEngineClient:
      HandleFromMMOEngine((nsMMOEngine::TBaseEvent*)p);
      break;
    case nsListModules::PhysicEngine:
      break;
    case nsListModules::SoundEngine:
      break;
    case nsListModules::Timer:
      CallBackModule(nsListModules::GraphicEngine, &TClientLogic::ShowFPS);
      break;
    default:BL_FIX_BUG();
  }
}
//----------------------------------------------------------
void TClientLogic::StartEvent()
{
  CallBackModule(nsListModules::Timer, &TClientLogic::StartTimer);
  CallBackModule(nsListModules::GraphicEngine, &TClientLogic::InitForms);
  CallBackModule(nsListModules::GraphicEngine, &TClientLogic::ShowTanks);

  CallBackModule(nsListModules::PhysicEngine, &TClientLogic::InitPhysic);

  mComp.pGraphicEngine->GetCBBeginWork()->Register( &TClientLogic::GraphicBeginWork ,this);
  
  mComp.pGraphicEngine->GetCBStopEvent()->Register( &TClientLogic::FreeGraphicResource,this);

  mComp.pPhysicEngine->GetCBBeginWork()->Register( &TClientLogic::PhysicBeginWork ,this);
  mComp.pPhysicEngine->GetCBEndWork()->Register( &TClientLogic::PhysicEndWork ,this);
}
//----------------------------------------------------------
void TClientLogic::StartTimer()
{
  // вызовется из потока таймера
  unsigned int mID_Timer = mComp.pTimer->New(10);
}
//----------------------------------------------------------
void TClientLogic::InitForms()
{
  //mClientMain = new TClientMain;
  //mClientMain->Show();

  mDebugPanel = new TDebugPanel;
  mDebugPanel->Show();
}
//----------------------------------------------------------
void TClientLogic::InitLog()
{
  GetLogger()->Register("Inner");// для логирования внутренних событий
  GetLogger()->Init("Client");
}
//---------------------------------------------------------------------------------------------
void TClientLogic::HandleFromMMOEngine(nsMMOEngine::TBaseEvent* pBE)
{
  std::string sEvent;  
  switch(pBE->mType)
  {
    case nsMMOEngine::eDisconnectUp:
      sEvent = "DisconnectUp";
      break;
    case nsMMOEngine::eError:
      sEvent = "Error";
      break;
    case nsMMOEngine::eEnterInQueue:
      sEvent = "EnterInQueue";
      break;
    case nsMMOEngine::eLeaveQueue:
      sEvent = "LeaveQueue";
      break;
    case nsMMOEngine::eRecvFromUp:
      sEvent = "RecvFromUp";
      break;
    case nsMMOEngine::eResultLogin:
      sEvent = "ResultLogin";
      CallBackModule(nsListModules::GraphicEngine, &TClientLogic::LoginOnServer);
      break;
    default:BL_FIX_BUG();
  }
  GetLogger("Inner")->WriteF_time("MMOEngine: %s.\n",sEvent.data());
}
//---------------------------------------------------------------------------------------------
struct TOrient
{
  const Ogre::Vector3 axisRotate;
  const Ogre::Radian  angleRotate;
  const Ogre::Vector3 scale;
  const Ogre::Vector3 pos;
  TOrient(Ogre::Vector3 _axisRotate, Ogre::Radian _angleRotate, 
    Ogre::Vector3 _scale, Ogre::Vector3 _pos):
  axisRotate(_axisRotate), angleRotate(_angleRotate),
    scale(_scale),pos(_pos){}
};
//---------------------------------------------------------------------------------------------
void AddEntity(Ogre::Entity* pEnt, TOrient& orient, Ogre::SceneManager* pSM)
{
  pEnt->setCastShadows(false);
  Ogre::SceneNode* pNode = pSM->getRootSceneNode()->createChildSceneNode();
  pNode->attachObject(pEnt);

  pNode->setScale(orient.scale);
  pNode->rotate(orient.axisRotate, orient.angleRotate);
  pNode->setPosition(orient.pos);
}
//---------------------------------------------------------------------------------------------
void TClientLogic::LoginOnServer()
{
  mClientMain->Hide();
  // вставка Танка (временно), прототип модели
  ShowTank(0, Ogre::Vector3(0,0,0));

  Ogre::Camera* pCamera = mComp.pGraphicEngine->GetGE()->GetCamera();
  pCamera->setPosition(60,60,60);
  pCamera->lookAt(0,0,0);
}
//---------------------------------------------------------------------------------------------
void TClientLogic::ShowFPS()
{
  float fps = mComp.pGraphicEngine->GetGE()->GetWindow()->getAverageFPS();
  mDebugPanel->SetFPS(fps);

  mDebugPanel->SetX(x);
  mDebugPanel->SetY(y);
  mDebugPanel->SetZ(z);
}
//---------------------------------------------------------------------------------------------
void TClientLogic::ShowTank(int index, Ogre::Vector3& pos)
{
  // вставка Танка (временно), прототип модели
  //###
  Ogre::SceneManager* pSM = mComp.pGraphicEngine->GetGE()->GetSceneManager();
  pSM->setAmbientLight(Ogre::ColourValue(1, 1, 1));
  TOrient orGun     (Ogre::Vector3(1,0,0), Ogre::Radian( 3.14f/2), Ogre::Vector3(10,-10,10), Ogre::Vector3(0,   3,  4) + pos);
  TOrient orHull    (Ogre::Vector3(1,0,0), Ogre::Radian(-3.14f/2), Ogre::Vector3(10,-10,10), Ogre::Vector3(0, -10,-10) + pos);
  TOrient orChassisL(Ogre::Vector3(1,0,0), Ogre::Radian(-3.14f/2), Ogre::Vector3(10, 10,10), Ogre::Vector3(0, -20,-10) + pos);
  TOrient orChassisR(Ogre::Vector3(1,0,0), Ogre::Radian(-3.14f/2), Ogre::Vector3(10, 10,10), Ogre::Vector3(0, -20,-10) + pos);
  TOrient orTurret  (Ogre::Vector3(1,0,0), Ogre::Radian(-3.14f/2), Ogre::Vector3(10,-10,10), Ogre::Vector3(0,   0,-10) + pos);
  TOrient orTrackL  (Ogre::Vector3(1,0,0), Ogre::Radian(-3.14f/2), Ogre::Vector3(10, 10,10), Ogre::Vector3(0, -20,-10) + pos);
  TOrient orTrackR  (Ogre::Vector3(1,0,0), Ogre::Radian(-3.14f/2), Ogre::Vector3(10, 10,10), Ogre::Vector3(0, -20,-10) + pos);

  char sHull[50], sTurret[50], sChassisL[50], sChassisR[50], sGun[50], sTrackL[50], sTrackR[50];
  sprintf(sHull,     "Hull%d",     index); 
  sprintf(sTurret,   "Turret%d",   index); 
  sprintf(sChassisL, "ChassisL%d", index); 
  sprintf(sChassisR, "ChassisR%d", index); 
  sprintf(sGun,      "Gun%d",      index); 
  sprintf(sTrackL,   "TrackL%d",   index); 
  sprintf(sTrackR,   "TrackR%d",   index); 

  AddEntity(pSM->createEntity(sHull,     "KingTiger/Hull.mesh"),    orHull,     pSM);
  AddEntity(pSM->createEntity(sTurret,   "KingTiger/Turret.mesh"),  orTurret,   pSM);
  AddEntity(pSM->createEntity(sChassisL, "KingTiger/ChassisL.mesh"),orChassisL, pSM);
  AddEntity(pSM->createEntity(sChassisR, "KingTiger/ChassisR.mesh"),orChassisR, pSM);
  AddEntity(pSM->createEntity(sGun,      "KingTiger/Gun.mesh"),     orGun,      pSM);
  AddEntity(pSM->createEntity(sTrackL,   "KingTiger/TrackL.mesh"),  orTrackL,   pSM);
  AddEntity(pSM->createEntity(sTrackR,   "KingTiger/TrackR.mesh"),  orTrackR,   pSM);
  //###
}
//---------------------------------------------------------------------------------------------
void TClientLogic::ShowTanks()
{
  Ogre::Vector3 pos;
  pos.y = 0;
  pos.z = 0;
  for( int i = 0 ; i < 1 ; i++ )
  {
    pos.x = i*6;
    ShowTank(i, pos);
  }
  Ogre::Camera* pCamera = mComp.pGraphicEngine->GetGE()->GetCamera();
  pCamera->setPosition(160,160,160);
  pCamera->lookAt(0,0,0);
}
//---------------------------------------------------------------------------------------------
void TClientLogic::PhysicEndWork()
{
  if(fallMotionState)
  {
    static float minY = 100000;
    btTransform trans;
    fallMotionState->getWorldTransform(trans);
    x = trans.getOrigin().getX();
    minY = std::min(minY, trans.getOrigin().getY());
    y = minY;
    z = trans.getOrigin().getZ();
  }
}
//---------------------------------------------------------------------------------------------
void TClientLogic::InitPhysic()
{
  int id_world = mComp.pPhysicEngine->GetPE()->AddWorld();
  btDiscreteDynamicsWorld* dynamicsWorld = mComp.pPhysicEngine->GetPE()->GetWorld(id_world);
  dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

  btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
  btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
  btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
  btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
  dynamicsWorld->addRigidBody(groundRigidBody);
  //-----------------------------------------
  btCollisionShape* fallShape = new btSphereShape(1);
  /*btDefaultMotionState* */
  fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 15000, 0)));
  btScalar mass = 1;
  btVector3 fallInertia(0, 0, 0);
  fallShape->calculateLocalInertia(mass, fallInertia);
  btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
  btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
  dynamicsWorld->addRigidBody(fallRigidBody);
  //-----------------------------------------
  btCollisionShape* fallShape2 = new btSphereShape(1);
  btDefaultMotionState* 
  fallMotionState2 = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.1f, 1, 0.1f)));
  btScalar mass2 = 1;
  btVector3 fallInertia2(0, 0, 0);
  fallShape2->calculateLocalInertia(mass2, fallInertia2);
  btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI2(mass2, fallMotionState2, fallShape2, fallInertia2);
  btRigidBody* fallRigidBody2 = new btRigidBody(fallRigidBodyCI2);
  //dynamicsWorld->addRigidBody(fallRigidBody2);
}
//---------------------------------------------------------------------------------------------
void TClientLogic::GraphicBeginWork()
{

}
//---------------------------------------------------------------------------------------------
void TClientLogic::PhysicBeginWork()
{

}
//---------------------------------------------------------------------------------------------
void TClientLogic::FreeGraphicResource()
{
  delete mClientMain;
  delete mDebugPanel;
}
//---------------------------------------------------------------------------------------------
