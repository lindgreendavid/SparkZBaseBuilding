class SPKZ_ActionOpenWoodWindow extends SPKZ_ActionOpenWoodDoor
{
 override string GetText(){return "Open window";}
 override bool ActionCondition(PlayerBase player,ActionTarget target,ItemBase item)
 {
  if(!target || !m_ConditionTarget.Can(player,target)){return false;}
  SPKZ_WoodWindow window=SPKZ_WoodWindow.Cast(target.GetObject());
  if(!window){return false;}
  return !window.IsOpen();
 }
}
class SPKZ_ActionCloseWoodWindow extends SPKZ_ActionCloseWoodDoor
{
 override string GetText(){return "Close window";}
 override bool ActionCondition(PlayerBase player,ActionTarget target,ItemBase item)
 {
  if(!target || !m_ConditionTarget.Can(player,target)){return false;}
  SPKZ_WoodWindow window=SPKZ_WoodWindow.Cast(target.GetObject());
  if(!window){return false;}
  return window.IsOpen();
 }
}
