class SPKZ_WoodWallDoor extends BuildingSuper
{
 protected ref OpenableBehaviour m_Openable;
 protected string m_SPKZOwnerId;
 protected int m_SPKZOwnerHash;
 // How many SPKZ_PlacedCharge detonations this specific wall instance has
 // taken so far - compared against SPKZ_RequiredChargeCount() each time one
 // lands (see SPKZ_ApplyChargeHit). Persisted so a raid can span a charge's
 // fuse time surviving a server restart mid-siege.
 protected int m_SPKZChargeHits;
 void SPKZ_WoodWallDoor()
 {
  m_Openable=new OpenableBehaviour(false);
  RegisterNetSyncVariableBool("m_Openable.m_IsOpened");
  RegisterNetSyncVariableInt("m_SPKZOwnerHash");
  SetAllowDamage(false); SetCanBeDestroyed(false);
 }
 void Open(){m_Openable.Open(); SetAnimationPhase("Door",1); SetSynchDirty();}
 void Close(){m_Openable.Close(); SetAnimationPhase("Door",0); SetSynchDirty();}
 bool IsOpen(){return m_Openable.IsOpened();}
 bool SPKZ_HasDoor(){return true;}
 string SPKZ_ReturnKitType(){return "SPKZ_WoodWallDoorKit";}
 void SPKZ_RefreshLifetime()
 {
  if(!GetGame().IsServer()){return;}
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
  if(SPKZ_HasDoor())
  {
   if(IsOpen()){SetAnimationPhase("Door",1);}
   else {SetAnimationPhase("Door",0);}
  }
 }
 void SPKZ_SetOwner(PlayerBase player)
 {
  if(!GetGame().IsServer() || !player || !player.GetIdentity()){return;}
  m_SPKZOwnerId=player.GetIdentity().GetId();
  m_SPKZOwnerHash=m_SPKZOwnerId.Hash();
  SPKZ_RefreshLifetime();
  SetSynchDirty();
 }
 bool SPKZ_CanDismantle(PlayerBase player, ItemBase tool)
 {
  if(!player || !player.GetIdentity() || !tool){return false;}
  if(!tool.IsKindOf("Screwdriver") || tool.IsRuined()){return false;}
  string id=player.GetIdentity().GetId();
  if(GetGame().IsServer())
  {
   return m_SPKZOwnerId!="" && m_SPKZOwnerId==id;
  }
  return m_SPKZOwnerHash!=0 && m_SPKZOwnerHash==id.Hash();
 }
 void SPKZ_Dismantle(PlayerBase player, ItemBase tool)
 {
  if(!GetGame().IsServer() || !SPKZ_CanDismantle(player,tool)){return;}
  EntityAI kit=EntityAI.Cast(GetGame().CreateObjectEx(SPKZ_ReturnKitType(),player.GetPosition(),ECE_PLACE_ON_SURFACE));
  if(!kit){return;}
  tool.AddHealth("","Health",-5);
  GetGame().ObjectDelete(this);
 }
 // Whether an SPKZ_PlacedCharge (see SPKZ_PlacedCharge.c) can be planted on
 // this piece at all - true for walls/doors/windows by default, false for
 // pieces that aren't a wall the player is trying to breach through (floors,
 // the garage door - see the overrides in SPKZ_WoodFloor.c/SPKZ_WoodGarage.c).
 bool SPKZ_CanAcceptCharge(){return true;}
 // How many charge detonations this piece needs before it breaches. Wood
 // tier is 1; a future metal-tier piece overrides this to 2 (per direction:
 // "2 homemade charges to get through metal"). SPKZ_FabricatedChargeBlock
 // (the "C4" tier) always counts as a full breach in a single hit regardless
 // of this value - see SPKZ_PlacedCharge.c's SPKZ_AlwaysBreaches().
 int SPKZ_RequiredChargeCount(){return 1;}
 // Called by SPKZ_PlacedCharge when its fuse ends. Server-only. Deletes the
 // wall outright - no kit is returned, unlike a screwdriver dismantle -
 // breaching is meant to be a one-way loss for whoever owns the base.
 void SPKZ_ApplyChargeHit(bool alwaysBreaches)
 {
  if(!GetGame().IsServer()){return;}
  m_SPKZChargeHits=m_SPKZChargeHits+1;
  if(alwaysBreaches || m_SPKZChargeHits>=SPKZ_RequiredChargeCount())
  {
   GetGame().ObjectDelete(this);
  }
  else
  {
   SetSynchDirty();
  }
 }
 override void OnStoreSave(ParamsWriteContext ctx)
 {
  super.OnStoreSave(ctx);
  ctx.Write(IsOpen());
  ctx.Write(m_SPKZOwnerId);
  ctx.Write(m_SPKZChargeHits);
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
  if(!ctx.Read(m_SPKZOwnerId)){m_SPKZOwnerId="";}
  if(m_SPKZOwnerId!=""){m_SPKZOwnerHash=m_SPKZOwnerId.Hash();}
  // Older walls saved before charge tracking existed have no stored hit
  // count - default to undamaged, not partially-breached.
  if(!ctx.Read(m_SPKZChargeHits)){m_SPKZChargeHits=0;}
  SPKZ_RefreshLifetime();
  if(SPKZ_HasDoor())
  {
   if(IsOpen()){SetAnimationPhase("Door",1);}
   else {SetAnimationPhase("Door",0);}
  }
  return true;
 }
 override void SetActions(){super.SetActions();AddAction(SPKZ_ActionOpenWoodDoor);AddAction(SPKZ_ActionCloseWoodDoor);AddAction(SPKZ_ActionPlaceCharge);}
}

class SPKZ_WoodWallDoorKit extends ItemBase
{
 string SPKZ_PlacedType(){return "SPKZ_WoodWallDoor";}
 string SPKZ_ProjectionType(){return "SPKZ_WoodWallDoor_Hologram";}
 override bool IsBasebuildingKit(){return true;}
 override bool IsDeployable(){return true;}
 override void OnPlacementComplete(Man player,vector position="0 0 0",vector orientation="0 0 0")
 {
  super.OnPlacementComplete(player,position,orientation);
  if(!GetGame().IsServer())return;
  Object wall=GetGame().CreateObjectEx(SPKZ_PlacedType(),position,ECE_OBJECT_SWAP);
  if(!wall)return;
  SPKZ_WoodWallDoor placed=SPKZ_WoodWallDoor.Cast(wall);
  if(placed){placed.SPKZ_SetOwner(PlayerBase.Cast(player));}
  wall.SetPosition(position);
  wall.SetOrientation(orientation);
  // ActionDeployObject deletes base-building kits at the end of the action.
  // Keep the kit alive until its remaining server callbacks have finished.
 }
 override void SetActions(){super.SetActions();AddAction(ActionTogglePlaceObject);AddAction(ActionPlaceObject);}
}
