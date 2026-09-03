class SPKZ_ActionDismantleWoodWallCB extends ActionContinuousBaseCB
{
 override void CreateActionComponent()
 {
  m_ActionData.m_ActionComponent=new CAContinuousTime(10.0);
 }
}
class SPKZ_ActionDismantleWoodWall extends ActionContinuousBase
{
 void SPKZ_ActionDismantleWoodWall()
 {
  m_CallbackClass=SPKZ_ActionDismantleWoodWallCB;
  m_CommandUID=DayZPlayerConstants.CMD_ACTIONFB_DISASSEMBLE;
  m_FullBody=true;
  m_StanceMask=DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
  m_Text="Dismantle to kit";
 }
 override void CreateConditionComponents()
 {
  m_ConditionItem=new CCINonRuined;
  m_ConditionTarget=new CCTCursor(3.0);
 }
 override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
 {
  if(!target || !m_ConditionTarget.Can(player,target)){return false;}
  SPKZ_WoodWallDoor wall=SPKZ_WoodWallDoor.Cast(target.GetObject());
  if(!wall){return false;}
  return wall.SPKZ_CanDismantle(player,item);
 }
 override void OnFinishProgressServer(ActionData action_data)
 {
  if(!ActionCondition(action_data.m_Player,action_data.m_Target,action_data.m_MainItem)){return;}
  SPKZ_WoodWallDoor wall=SPKZ_WoodWallDoor.Cast(action_data.m_Target.GetObject());
  if(wall){wall.SPKZ_Dismantle(action_data.m_Player,action_data.m_MainItem);}
 }
}
modded class Screwdriver
{
 override void SetActions()
 {
  super.SetActions();
  AddAction(SPKZ_ActionDismantleWoodWall);
 }
}
