/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#pragma once

#include <string>
#include <map>
#include <set>
#include <vector>
#include <list>

#include "BreakPacket.h"
#include "TypeDef.h"
#include "ContainerRise.h"

class ISerializable;

class DllExport TPushMaster
{
  TBreakPacket mCollectorMember;
public:
  void CopyInBuffer( TContainerRise& receiveBuffer, int offset = 0 );
  void Clear();
public:
  //bool, unsigned char, char, unsigned short, short, unsigned int, int, float, double
  template<typename T>
  void Push(const T& t);
  //std::string
  void PushStr(const std::string& str);
  //ISerializable
  template<typename T>
  void PushSer(const T& serializable);
  //ISerializable*
  template<typename Type>
  void PushPtrSer(const Type* t);

  //std::vector/list
  // unsigned char, char, unsigned short, short, unsigned int, int, float, double
  template<typename T, typename Array>
  void PushArray(const Array& array);
  //std::vector/list/set<std::string>
  template<typename Array>
  void PushArrayStr(const Array& array);
  //std::vector/list/set<ISerializable>
  template<typename T, typename Array>
  void PushArraySer(const Array& array);
  //std::vector/list/set<ISerializable*>
  template<typename Array>
  void PushArrayPtrSer(const Array& array);

  //std::set
  // unsigned char, char, unsigned short, short, unsigned int, int, float, double
  template<typename T, typename Set>
  void PushSet(const Set& s);
  //std::set<std::string>
  template<typename Set>
  void PushSetStr(const Set& s);
  //std::set<ISerializable>
  template<typename T, typename Set>
  void PushSetSer(const Set& s);
  //std::set<ISerializable*>
  template<typename Set>
  void PushSetPtrSer(const Set& s);

  // std::map
  template<typename Key, typename Value, typename Map>
  void PushMap__(const Map& m);
  template<typename Key, typename Map>
  void PushMap_Str(const Map& m);
  template<typename Key, typename Value, typename Map>
  void PushMap_Ser(const Map& m);
  template<typename Key, typename Map>
  void PushMap_PtrSer(const Map& m);

  template<typename Value, typename Map>
  void PushMapStr_(const Map& m);
  template<typename Map>
  void PushMapStrStr(const Map& m);
  template<typename Value, typename Map>
  void PushMapStrSer(const Map& m);
  template<typename Map>
  void PushMapStrPtrSer(const Map& m);

  template<typename Key, typename Value, typename Map>
  void PushMapSer_(const Map& m);
  template<typename Key, typename Map>
  void PushMapSerStr(const Map& m);
  template<typename Key, typename Value, typename Map>
  void PushMapSerSer(const Map& m);
  template<typename Key, typename Map>
  void PushMapSerPtrSer(const Map& m);
private:
  void PushSize(const int& size);
};
//------------------------------------------------------------------------
template<typename T>
void TPushMaster::Push(const T& t)
{
  mCollectorMember.PushBack((char*)&t, sizeof(T));
}
//------------------------------------------------------------------------
template<typename T>
void TPushMaster::PushSer(const T& serializable)
{
  serializable.Serialize(this);
}
//------------------------------------------------------------------------
template<typename Type>
void TPushMaster::PushPtrSer(const Type* t)
{
  unsigned char isNotNULL = 1;
  if (t == NULL)
    isNotNULL = 0;
  Push(isNotNULL);
  if (t)
    t->Serialize(this);
}
//-----------------------------------------------------------------------
template<typename T, typename Array>
void TPushMaster::PushArray(const Array& array)
{
  int size = array.size();
  Push(size);
  for (auto& el : array)
    Push(el);
}
//------------------------------------------------------------------------
template<typename Array>
void TPushMaster::PushArrayStr(const Array& array)
{
  int size = array.size();
  Push(size);
  for (auto& el : array)
    PushStr(el);
}
//------------------------------------------------------------------------
template<typename T, typename Array>
void TPushMaster::PushArraySer(const Array& array)
{
  int size = array.size();
  Push(size);
  for (auto& el : array)
    PushSer(el);
}
//------------------------------------------------------------------------
template<typename Array>
void TPushMaster::PushArrayPtrSer(const Array& array)
{
  int size = array.size();
  Push(size);
  for (auto& el : array)
    PushPtrSer(el);
}
//------------------------------------------------------------------------
template<typename T, typename Set>
void TPushMaster::PushSet(const Set& s)
{
  int size = s.size();
  PushSize(size);
  typename Set::iterator bit = s.begin();
  typename Set::iterator eit = s.end();
  while (bit != eit)
  {
    T t = *bit;
    Push<T>(t);
    bit++;
  }
}
//------------------------------------------------------------------------
template<typename Set>
void TPushMaster::PushSetStr(const Set& s)
{
  int size = s.size();
  PushSize(size);
  typename Set::iterator bit = s.begin();
  typename Set::iterator eit = s.end();
  while (bit != eit)
  {
    std::string t = *bit;
    PushStr(t);
    bit++;
  }
}
//------------------------------------------------------------------------
template<typename T, typename Set>
void TPushMaster::PushSetSer(const Set& s)
{
  int size = s.size();
  PushSize(size);
  typename Set::iterator bit = s.begin();
  typename Set::iterator eit = s.end();
  while (bit != eit)
  {
    T t = *bit;
    PushSer<T>(t);
    bit++;
  }
}
//------------------------------------------------------------------------
template<typename Set>
void TPushMaster::PushSetPtrSer(const Set& s)
{
  int size = s.size();
  PushSize(size);
  for (auto& bit : s)
  {
    PushPtrSer(bit);
  }
}
//------------------------------------------------------------------------
template<typename Key, typename Value, typename Map>
void TPushMaster::PushMap__(const Map& m)
{
  int size = m.size();
  Push(size);
  for (auto& bit : m)
  {
    auto& key = *((Key*)&(bit.first));
    Push(key);
    auto& value = bit.second;
    Push(value);
  }
}
//------------------------------------------------------------------------
template<typename Key, typename Map>
void TPushMaster::PushMap_Str(const Map& m)
{
  int size = m.size();
  PushSize(size);
  for (auto& bit : m)
  {
    auto& key = *((Key*)&(bit.first));
    Push(key);
    const std::string& value = bit.second;
    PushStr(value);
  }
}
//------------------------------------------------------------------------
template<typename Key, typename Value, typename Map>
void TPushMaster::PushMap_Ser(const Map& m)
{
  int size = m.size();
  PushSize(size);
  for (auto& bit : m)
  {
    auto& key = *((Key*)&(bit.first));
    Push(key);
    auto& value = bit.second;
    PushSer(value);
  }
}
//------------------------------------------------------------------------
template<typename Key, typename Map>
void TPushMaster::PushMap_PtrSer(const Map& m)
{
  int size = m.size();
  PushSize(size);
  for (auto& bit : m)
  {
    auto& key = *((Key*)&(bit.first));
    Push(key);
    auto* value = bit.second;
    PushPtrSer(value);
  }
}
//------------------------------------------------------------------------
template<typename Value, typename Map>
void TPushMaster::PushMapStr_(const Map& m)
{
  int size = m.size();
  PushSize(size);
  for (auto& bit : m)
  {
    const std::string& key = *((std::string*)&(bit.first));
    PushStr(key);
    auto& value = bit.second;
    Push(value);
  }
}
//------------------------------------------------------------------------
template<typename Map>
void TPushMaster::PushMapStrStr(const Map& m)
{
  int size = m.size();
  PushSize(size);
  for (auto & bit : m)
  {
    const std::string& key = *((std::string*)&(bit.first));
    PushStr(key);
    const std::string& value = bit.second;
    PushStr(value);
  }
}
//------------------------------------------------------------------------
template<typename Value, typename Map>
void TPushMaster::PushMapStrSer(const Map& m)
{
  int size = m.size();
  PushSize(size);
  for (auto& bit : m)
  {
    const std::string& key = *((std::string*)&(bit.first));
    PushStr(key);
    auto& value = bit.second;
    PushSer(value);
  }
}
//------------------------------------------------------------------------
template<typename Map>
void TPushMaster::PushMapStrPtrSer(const Map& m)
{
  int size = m.size();
  PushSize(size);
  for (auto& bit : m)
  {
    const std::string& key = *((std::string*)&(bit.first));
    PushStr(key);
    auto* value = bit.second;
    PushPtrSer(value);
  }
}
//------------------------------------------------------------------------
template<typename Key, typename Value, typename Map>
void TPushMaster::PushMapSer_(const Map& m)
{
  int size = m.size();
  PushSize(size);
  for (auto& bit : m)
  {
    auto& key = *((Key*)&(bit.first));
    PushSer(key);
    auto& value = bit.second;
    Push(value);
  }
}
//------------------------------------------------------------------------
template<typename Key, typename Map>
void TPushMaster::PushMapSerStr(const Map& m)
{
  int size = m.size();
  PushSize(size);
  for (auto& bit : m)
  {
    auto& key = *((Key*)&(bit.first));
    PushSer(key);
    const std::string& value = bit.second;
    PushStr(value);
  }
}
//------------------------------------------------------------------------
template<typename Key, typename Value, typename Map>
void TPushMaster::PushMapSerSer(const Map& m)
{
  int size = m.size();
  PushSize(size);
  for (auto& bit : m)
  {
    auto& key = *((Key*)&(bit.first));
    PushSer(key);
    auto& value = bit.second;
    PushSer(value);
  }
}
//------------------------------------------------------------------------
template<typename Key, typename Map>
void TPushMaster::PushMapSerPtrSer(const Map& m)
{
  int size = m.size();
  PushSize(size);
  for (auto& bit : m)
  {
    auto& key = *((Key*)&(bit.first));
    PushSer(key);
    auto* value = bit.second;
    PushPtrSer(value);
  }
}
