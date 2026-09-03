// Shared base for built pieces that need real Inventory/Cargo persistence
// (Container_Base config ancestor, simulation="inventoryItem") while staying
// non-pickable, matching the exact combination the installed game's own
// TentBase/WorldContainer_Base-family objects use - never a House/
// HouseNoDestruct ancestor, since that pairs with a completely different
// script hierarchy (Building -> EntityAI, bypassing ItemBase/its GetActions
// override entirely - see SPKZ_Workbench.c for the full story of why that
// mismatch broke everything silently). CanReceiveItemIntoCargo/
// CanDisplayCargo default to false here since most built pieces (walls,
// doors, windows) have no cargo of their own; SPKZ_Workbench overrides them
// back to true.
class SPKZ_WorkbenchStorageBase extends Container_Base
{
 protected ref OpenableBehaviour m_Openable;
 protected string m_SPKZOwnerId;
 protected int m_SPKZOwnerHash;
 void SPKZ_WorkbenchStorageBase()
 {
  m_Openable = new OpenableBehaviour(false);
  RegisterNetSyncVariableBool("m_Openable.m_IsOpened");
  RegisterNetSyncVariableInt("m_SPKZOwnerHash");
  SetAllowDamage(false); SetCanBeDestroyed(false);
 }
 override void Open() { m_Openable.Open(); SetAnimationPhase("Door", 1); SetSynchDirty(); }
 override void Close() { m_Openable.Close(); SetAnimationPhase("Door", 0); SetSynchDirty(); }
 override bool IsOpen() { return m_Openable.IsOpened(); }
 // Inventory inheritance is for persistence only; built pieces are not loot.
 override bool IsTakeable() { return false; }
 override bool CanPutInCargo(EntityAI parent) { return false; }
 override bool CanPutIntoHands(EntityAI parent) { return false; }
 override bool CanReceiveItemIntoCargo(EntityAI item) { return false; }
 override bool CanDisplayCargo() { return false; }
 void SPKZ_AnchorPhysics()
 {
  // Keep elevated/rotated pieces fixed without disabling their animation.
  if (dBodyIsSet(this)) { dBodyDynamic(this, false); }
 }
 override void EEInit()
 {
  super.EEInit();
  SPKZ_AnchorPhysics();
  SPKZ_RefreshLifetime();
 }
 override void OnCreatePhysics()
 {
  super.OnCreatePhysics();
  SPKZ_AnchorPhysics();
 }
 override void AfterStoreLoad()
 {
  super.AfterStoreLoad();
  SPKZ_AnchorPhysics();
  SetSynchDirty();
 }
 bool SPKZ_HasDoor() { return true; }
 string SPKZ_ReturnKitType() { return "SPKZ_WorkbenchStorageBaseKit"; }
 void SPKZ_RefreshLifetime()
 {
  if (!GetGame().IsServer()) { return; }
  // Native CE owns persistence. Refresh cleanup lifetime, never recreate
  // deleted entities: dismantling and external administrative deletion win.
  SetLifetimeMax(315360000);
  SetLifetime(315360000);
 }
 override void OnCEUpdate()
 {
  super.OnCEUpdate();
  SPKZ_RefreshLifetime();
 }
 override void OnVariablesSynchronized()
 {
  super.OnVariablesSynchronized();
  if (SPKZ_HasDoor())
  {
   if (IsOpen()) { SetAnimationPhase("Door", 1); }
   else { SetAnimationPhase("Door", 0); }
  }
 }
 void SPKZ_SetOwner(PlayerBase player)
 {
  if (!GetGame().IsServer() || !player || !player.GetIdentity()) { return; }
  m_SPKZOwnerId = player.GetIdentity().GetId();
  m_SPKZOwnerHash = m_SPKZOwnerId.Hash();
  SPKZ_RefreshLifetime();
  SetSynchDirty();
 }
 bool SPKZ_CanDismantle(PlayerBase player, ItemBase tool)
 {
  if (!player || !player.GetIdentity() || !tool) { return false; }
  if (!tool.IsKindOf("Screwdriver") || tool.IsRuined()) { return false; }
  string id = player.GetIdentity().GetId();
  if (GetGame().IsServer())
  {
   return m_SPKZOwnerId != "" && m_SPKZOwnerId == id;
  }
  return m_SPKZOwnerHash != 0 && m_SPKZOwnerHash == id.Hash();
 }
 void SPKZ_Dismantle(PlayerBase player, ItemBase tool)
 {
  if (!GetGame().IsServer() || !SPKZ_CanDismantle(player, tool)) { return; }
  EntityAI kit = EntityAI.Cast(GetGame().CreateObjectEx(SPKZ_ReturnKitType(), player.GetPosition(), ECE_PLACE_ON_SURFACE));
  if (!kit) { return; }
  tool.AddHealth("", "Health", -5);
  GetGame().ObjectDelete(this);
 }
 override void OnStoreSave(ParamsWriteContext ctx)
 {
  super.OnStoreSave(ctx);
  ctx.Write(IsOpen());
  ctx.Write(m_SPKZOwnerId);
 }
 override bool OnStoreLoad(ParamsReadContext ctx, int version)
 {
  if (!super.OnStoreLoad(ctx, version))
  {
   return false;
  }

  bool opened;
  if (!ctx.Read(opened))
  {
   return false;
  }

  if (opened)
  {
   m_Openable.Open();
  }
  else
  {
   m_Openable.Close();
  }

  // Older test walls had no stored owner. Do not assign them to a stranger.
  if (!ctx.Read(m_SPKZOwnerId)) { m_SPKZOwnerId = ""; }
  if (m_SPKZOwnerId != "") { m_SPKZOwnerHash = m_SPKZOwnerId.Hash(); }
  SPKZ_RefreshLifetime();
  if (SPKZ_HasDoor())
  {
   if (IsOpen()) { SetAnimationPhase("Door", 1); }
   else { SetAnimationPhase("Door", 0); }
  }
  return true;
 }
 override void SetActions() { super.SetActions(); AddAction(SPKZ_ActionOpenWoodDoor); AddAction(SPKZ_ActionCloseWoodDoor); }
}
