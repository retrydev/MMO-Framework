/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ManagerSerializerItem.h"

#include "FactoryGameItem.h"

#include <boost/foreach.hpp>

TManagerSerializerItem::TManagerSerializerItem()
{
  mSerializerShape.reset(new TSerializerShapeItem_XML);
  mSerializerModel.reset(new TSerializerModelItem_XML);
  mSerializerMaterial.reset(new TSerializerMaterialItem_XML);
  mSerializerTerrain.reset(new TSerializerTerrainItem_XML);
  mSerializerMap.reset(new TSerializerMapItem_XML);

  mXML = NULL;

  mMapTypeSerializer.insert(TMapIntPtrSerilizerVT(TFactoryGameItem::Material, mSerializerMaterial.get()));
  mMapTypeSerializer.insert(TMapIntPtrSerilizerVT(TFactoryGameItem::Shape,    mSerializerShape.get()));
  mMapTypeSerializer.insert(TMapIntPtrSerilizerVT(TFactoryGameItem::Model,    mSerializerModel.get()));
  mMapTypeSerializer.insert(TMapIntPtrSerilizerVT(TFactoryGameItem::Terrain,  mSerializerTerrain.get()));
  mMapTypeSerializer.insert(TMapIntPtrSerilizerVT(TFactoryGameItem::Map,      mSerializerMap.get()));
}
//--------------------------------------------------------------------------------------
void TManagerSerializerItem::Init(IXML* xml)
{
  mXML = xml;

  BOOST_FOREACH(TMapIntPtrSerilizerVT& vt, mMapTypeSerializer)
    vt.second->Init(mXML);
}
//--------------------------------------------------------------------------------------
bool TManagerSerializerItem::Load(int type, TBaseItem* pItem)
{
  TBaseSerializerItem_XML* p = Find(type);
  if(p==NULL)
    return false;
  return p->Load(pItem);
}
//--------------------------------------------------------------------------------------
bool TManagerSerializerItem::Save(int type, TBaseItem* pItem)
{
  TBaseSerializerItem_XML* p = Find(type);
  if(p==NULL)
    return false;
  return p->Save(pItem);
}
//--------------------------------------------------------------------------------------
std::string TManagerSerializerItem::StrByType(int type)
{
  TBaseSerializerItem_XML* p = Find(type);
  if(p)
    return p->Type();
  return "";
}
//--------------------------------------------------------------------------------------
TBaseSerializerItem_XML* TManagerSerializerItem::Find(int type)
{
  TMapIntPtrSerilizerIt fit = mMapTypeSerializer.find(type);
  if(fit==mMapTypeSerializer.end())
    return NULL;
  return fit->second;
}
//--------------------------------------------------------------------------------------
