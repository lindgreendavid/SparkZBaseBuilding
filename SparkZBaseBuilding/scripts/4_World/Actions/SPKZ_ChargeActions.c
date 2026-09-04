// Plants whichever SPKZ_PlacedCharge (see SPKZ_PlacedCharge.c) the player is
// holding against the wall/window they're looking at. Registered directly
// on SPKZ_WoodWallDoor (see its SetActions()), not on the charge item -
// same pattern this addon already uses for SPKZ_ActionDismantleWorkbench
// (target = the structure, item = whatever's in the player's hands, the
// specific-type check happens inside ActionCondition).
class SPKZ_ActionPlaceChargeCB extends ActionContinuousBaseCB
{
 override void CreateActionComponent()
 {
  m_ActionData.m_ActionComponent = new CAContinuousTime(5.0);
 }
}

class SPKZ_ActionPlaceCharge extends ActionContinuousBase
{
 void SPKZ_ActionPlaceCharge()
 {
  m_CallbackClass = SPKZ_ActionPlaceChargeCB;
  m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
  m_FullBody = true;
  m_Text = "Plant charge";
 }
 override void CreateConditionComponents()
 {
  m_ConditionItem = new CCINonRuined;
  m_ConditionTarget = new CCTCursor(3.0);
 }
 override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
 {
  if (!target || !m_ConditionTarget.Can(player, target)) { return false; }
  if (!item || !item.IsInherited(SPKZ_PlacedCharge)) { return false; }
  SPKZ_WoodWallDoor wall = SPKZ_WoodWallDoor.Cast(target.GetObject());
  if (!wall || !wall.SPKZ_CanAcceptCharge()) { return false; }
  return true;
 }
 override void OnFinishProgressServer(ActionData action_data)
 {
  if (!ActionCondition(action_data.m_Player, action_data.m_Target, action_data.m_MainItem)) { return; }

  SPKZ_WoodWallDoor wall = SPKZ_WoodWallDoor.Cast(action_data.m_Target.GetObject());
  SPKZ_PlacedCharge charge = SPKZ_PlacedCharge.Cast(action_data.m_MainItem);
  if (!wall || !charge) { return; }

  PlayerBase player = action_data.m_Player;
  // Move the charge out of the player's hands/inventory and into the world
  // at the wall's position - same DropEntity call the installed game's own
  // Plastic_Explosive uses to move an item to the ground.
  player.GetInventory().DropEntity(InventoryMode.SERVER, player, charge);
  charge.SetPosition(wall.GetPosition());
  charge.SetOrientation(wall.GetOrientation());
  charge.PlaceOnSurface();
  charge.SPKZ_BeginFuse(wall);
 }
}
