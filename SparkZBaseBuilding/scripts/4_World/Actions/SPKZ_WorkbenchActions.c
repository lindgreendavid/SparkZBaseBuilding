// Uses the vanilla look-and-interact action system (ActionInteractBase) so
// opening the build menu uses whatever key the player already has bound to
// interact - the same mechanism SPKZ_ActionOpenWoodDoor already uses, no new
// custom keybind needed.
class SPKZ_ActionAccessWorkbench: ActionInteractBase
{
 void SPKZ_ActionAccessWorkbench() { m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW; m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL; }
 override string GetText() { return "Access Workbench"; }
 override void CreateConditionComponents()
 {
  m_ConditionItem = new CCINone;
  m_ConditionTarget = new CCTCursor(3.0);
 }
 override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
 {
  if (!target || !m_ConditionTarget.Can(player, target))
  {
   return false;
  }
  return SPKZ_Workbench.Cast(target.GetObject()) != null;
 }
 // Opening a menu is a pure client-side view - the server never needs to be
 // told "the menu opened", only the OPEN_REQUEST RPC the menu itself sends
 // once it's constructed. This 4_World action cannot reference the
 // 5_Mission-only SPKZ_WorkbenchMenu class directly, so it calls a virtual
 // hook on the entity instead - SPKZ_Workbench's base (4_World) version of
 // that hook does nothing; scripts/5_Mission/UI/SPKZ_WorkbenchNetworking.c
 // overrides it to actually open the menu.
 override void OnStartClient(ActionData action_data)
 {
  super.OnStartClient(action_data);
  SPKZ_Workbench workbench = SPKZ_Workbench.Cast(action_data.m_Target.GetObject());
  if (!workbench) return;
  workbench.SPKZ_OnAccessRequested(action_data.m_Player);
 }
}

class SPKZ_ActionDismantleWorkbenchCB extends ActionContinuousBaseCB
{
 override void CreateActionComponent()
 {
  m_ActionData.m_ActionComponent = new CAContinuousTime(10.0);
 }
}

class SPKZ_ActionDismantleWorkbench extends ActionContinuousBase
{
 void SPKZ_ActionDismantleWorkbench()
 {
  m_CallbackClass = SPKZ_ActionDismantleWorkbenchCB;
  m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DISASSEMBLE;
  m_FullBody = true;
  m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
  m_Text = "Dismantle to kit";
 }
 override void CreateConditionComponents()
 {
  m_ConditionItem = new CCINonRuined;
  m_ConditionTarget = new CCTCursor(3.0);
 }
 override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
 {
  if (!target || !m_ConditionTarget.Can(player, target)) { return false; }
  SPKZ_Workbench workbench = SPKZ_Workbench.Cast(target.GetObject());
  if (!workbench) { return false; }
  return workbench.SPKZ_CanDismantle(player, item);
 }
 override void OnFinishProgressServer(ActionData action_data)
 {
  if (!ActionCondition(action_data.m_Player, action_data.m_Target, action_data.m_MainItem)) { return; }
  SPKZ_Workbench workbench = SPKZ_Workbench.Cast(action_data.m_Target.GetObject());
  if (workbench) { workbench.SPKZ_Dismantle(action_data.m_Player, action_data.m_MainItem); }
 }
}
