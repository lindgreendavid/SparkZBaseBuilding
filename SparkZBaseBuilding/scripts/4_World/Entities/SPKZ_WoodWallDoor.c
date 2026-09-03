class SPKZ_WoodWallDoor extends BuildingSuper
{
 protected ref OpenableBehaviour m_Openable;
 protected string m_SPKZOwnerId;
 protected int m_SPKZOwnerHash;
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
  if(!ctx.Read(m_SPKZOwnerId)){m_SPKZOwnerId="";}
  if(m_SPKZOwnerId!=""){m_SPKZOwnerHash=m_SPKZOwnerId.Hash();}
  SPKZ_RefreshLifetime();
  if(SPKZ_HasDoor())
  {
   if(IsOpen()){SetAnimationPhase("Door",1);}
   else {SetAnimationPhase("Door",0);}
  }
  return true;
 }
 override void SetActions(){super.SetActions();AddAction(SPKZ_ActionOpenWoodDoor);AddAction(SPKZ_ActionCloseWoodDoor);}
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
