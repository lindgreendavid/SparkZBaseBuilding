class SPKZ_ActionOpenWoodGarage extends SPKZ_ActionOpenWoodDoor
{
 override string GetText(){return "Open garage door";}
 override bool ActionCondition(PlayerBase player,ActionTarget target,ItemBase item)
 {
  if(!target || !m_ConditionTarget.Can(player,target)){return false;}
  SPKZ_WoodGarage garage=SPKZ_WoodGarage.Cast(target.GetObject());
  if(!garage){return false;}
  return !garage.IsOpen();
 }
}
class SPKZ_ActionCloseWoodGarage extends SPKZ_ActionCloseWoodDoor
{
 override string GetText(){return "Close garage door";}
 override bool ActionCondition(PlayerBase player,ActionTarget target,ItemBase item)
 {
  if(!target || !m_ConditionTarget.Can(player,target)){return false;}
  SPKZ_WoodGarage garage=SPKZ_WoodGarage.Cast(target.GetObject());
  if(!garage){return false;}
  return garage.IsOpen();
 }
}
