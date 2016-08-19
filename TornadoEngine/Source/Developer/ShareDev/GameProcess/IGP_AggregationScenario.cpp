/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "IGP_AggregationScenario.h"
#include "IGP_Scenario_General.h"
#include <stddef.h>

IGP_AggregationScenario::IGP_AggregationScenario()
{
  mPtrCurrentScenario = NULL;
}
//-----------------------------------------------------------------
IGP_AggregationScenario::~IGP_AggregationScenario()
{

}
//-----------------------------------------------------------------
void IGP_AggregationScenario::Begin(IGP_Scenario_General* pNewScenario)
{
  mPtrCurrentScenario = pNewScenario;
}
//-----------------------------------------------------------------
void IGP_AggregationScenario::End()
{
  mPtrCurrentScenario = NULL;
}
//-----------------------------------------------------------------
int IGP_AggregationScenario::GetCurrentScenarioType()
{
  //if(mPtrCurrentScenario==NULL)
    return eUndefType;
  //return mPtrCurrentScenario->GetType();
}
//-----------------------------------------------------------------
