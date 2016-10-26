/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef GameObjectH
#define GameObjectH

#include "TypeDef.h"
#include "MathTools.h"
#include <string>
#include <map>
#include <boost/smart_ptr/scoped_ptr.hpp>

class TGameObjectComponent_Graphic;
class TGameObjectComponent_Physic;
class TGameObjectComponent_Sound;

class TBehaviourPatternModel;
class TBehaviourPatternContext;

class TBuilderGameObjectFromGameItem;
class TDestructorGameObjectFromGameItem;
class TUpdaterGameObjectFromGameItem;

/*
  Содержит данные, которые не зависят от типа Паттерна.
  Зависит только от Ogre, Bullet и OpenAL.
*/

class DllExport TGameObject
{
  int mID;

  // данные
  boost::scoped_ptr<TGameObjectComponent_Graphic> mPtrGraphic;
  boost::scoped_ptr<TGameObjectComponent_Physic>  mPtrPhysic;
  boost::scoped_ptr<TGameObjectComponent_Sound>   mPtrSound;
  // поведение - зависит от типа (Model, Terrain, Zone, Light, Sound, Animated, Skybox)
  TBehaviourPatternModel*   mPtrModel;
  TBehaviourPatternContext* mPtrContext;

  // атомарные операции, характерные для каждого из движком
  // 6 штук: 3 Builder_XXX, 3 Destructor_XXX
  TBuilderGameObjectFromGameItem*    mPtrBuilder;
  TDestructorGameObjectFromGameItem* mPtrDestructor;
  TUpdaterGameObjectFromGameItem*    mPtrUpdater;
public:
  TGameObject();
  virtual ~TGameObject();

  int GetID() const;
  void SetID( int id );

  void SetModel(TBehaviourPatternModel* p);
  TBehaviourPatternModel* GetModel();

  void SetContext(TBehaviourPatternContext* p);
  TBehaviourPatternContext* GetContext();

  void SetBuilder(TBuilderGameObjectFromGameItem* p);
  TBuilderGameObjectFromGameItem* GetBuilder();
  void SetDestructor(TDestructorGameObjectFromGameItem* p);
  TDestructorGameObjectFromGameItem* GetDestructor();
  void SetUpdater(TUpdaterGameObjectFromGameItem* p);
  TUpdaterGameObjectFromGameItem* GetUpdater();

  TGameObjectComponent_Graphic* GetGraphic();
  TGameObjectComponent_Physic*  GetPhysic();
  TGameObjectComponent_Sound*   GetSound();
};

#endif