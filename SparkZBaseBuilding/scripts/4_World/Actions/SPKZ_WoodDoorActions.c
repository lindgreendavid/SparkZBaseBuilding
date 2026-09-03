class SPKZ_ActionOpenWoodDoor: ActionInteractBase
{
 void SPKZ_ActionOpenWoodDoor(){m_CommandUID=DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;m_StanceMask=DayZPlayerConstants.STANCEMASK_ALL;}
 override string GetText(){return "Open door";}
 override void CreateConditionComponents()
 {
  m_ConditionItem=new CCINone;
  m_ConditionTarget=new CCTCursor(3.0);
 }
 override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
 {
  if(!target || !m_ConditionTarget.Can(player,target))
  {
   return false;
  }
  SPKZ_WoodWallDoor wall=SPKZ_WoodWallDoor.Cast(target.GetObject());
  if(!wall || !wall.SPKZ_HasDoor())
  {
   return false;
  }
  return !wall.IsOpen();
 }
 override void OnStartServer(ActionData action_data)
 {
  if(!ActionCondition(action_data.m_Player,action_data.m_Target,action_data.m_MainItem)){return;}
  SPKZ_WoodWallDoor wall=SPKZ_WoodWallDoor.Cast(action_data.m_Target.GetObject());
  if(wall){wall.Open();}
 }
}
class SPKZ_ActionCloseWoodDoor: ActionInteractBase
{
 void SPKZ_ActionCloseWoodDoor(){m_CommandUID=DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;m_StanceMask=DayZPlayerConstants.STANCEMASK_ALL;}
 override string GetText(){return "Close door";}
 override void CreateConditionComponents()
 {
  m_ConditionItem=new CCINone;
  m_ConditionTarget=new CCTCursor(3.0);
 }
 override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
 {
  if(!target || !m_ConditionTarget.Can(player,target))
  {
   return false;
  }
  SPKZ_WoodWallDoor wall=SPKZ_WoodWallDoor.Cast(target.GetObject());
  if(!wall || !wall.SPKZ_HasDoor())
  {
   return false;
  }
  return wall.IsOpen();
 }
 override void OnStartServer(ActionData action_data)
 {
  if(!ActionCondition(action_data.m_Player,action_data.m_Target,action_data.m_MainItem)){return;}
  SPKZ_WoodWallDoor wall=SPKZ_WoodWallDoor.Cast(action_data.m_Target.GetObject());
  if(wall){wall.Close();}
 }
}
modded class ActionConstructor
{
 override void RegisterActions(TTypenameArray actions)
 {
  super.RegisterActions(actions);
  actions.Insert(SPKZ_ActionOpenWoodDoor);
  actions.Insert(SPKZ_ActionCloseWoodDoor);
  actions.Insert(SPKZ_ActionDismantleWoodWall);
  actions.Insert(SPKZ_ActionOpenWoodWindow);
  actions.Insert(SPKZ_ActionCloseWoodWindow);
  actions.Insert(SPKZ_ActionOpenWoodGarage);
  actions.Insert(SPKZ_ActionCloseWoodGarage);
  actions.Insert(SPKZ_ActionAccessWorkbench);
  actions.Insert(SPKZ_ActionDismantleWorkbench);
 }
}
