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
 // 5_Mission-only SPKZ_WorkbenchMenu class directly (nor can Mission
 // reference this World-tier SPKZ_Workbench type - see Pitfall #8 in
 // docs/CODING_STANDARDS.md), so it hands off through
 // SPKZ_WorkbenchClientBridge (3_Game) instead; Mission-side polling (see
 // the modded MissionGameplay.OnUpdate in scripts/5_Mission/SPKZ_PlacementLegend.c)
 // opens the menu when it sees the pending request.
 override void OnStartClient(ActionData action_data)
 {
  super.OnStartClient(action_data);
  SPKZ_Workbench workbench = SPKZ_Workbench.Cast(action_data.m_Target.GetObject());
  if (!workbench) return;
  SPKZ_WorkbenchClientBridge.RequestAccess(workbench);
 }
}

// No dedicated dismantle action here: SPKZ_Workbench now extends
// SPKZ_WoodWallDoor, so the generic SPKZ_ActionDismantleWoodWall (see
// SPKZ_DismantleWoodWall.c, which casts its target to SPKZ_WoodWallDoor)
// already matches a workbench polymorphically. A separate action casting to
// SPKZ_Workbench specifically would just double the screwdriver's dismantle
// entry for the same object.
