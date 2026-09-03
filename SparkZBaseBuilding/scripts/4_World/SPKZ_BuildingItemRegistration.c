modded class ModItemRegisterCallbacks
{
 override void RegisterTwoHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
 {
  super.RegisterTwoHanded(pType, pBehavior);
  pType.AddItemInHandsProfileIK("SPKZ_WoodGarageKit", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/BatteryTruck.anm");
  pType.AddItemInHandsProfileIK("SPKZ_WoodGlassWindowKit", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/BatteryTruck.anm");
  pType.AddItemInHandsProfileIK("SPKZ_WoodWindowKit", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/BatteryTruck.anm");
  pType.AddItemInHandsProfileIK("SPKZ_WoodFloorKit", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/BatteryTruck.anm");
  pType.AddItemInHandsProfileIK("SPKZ_WoodWallKit", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/BatteryTruck.anm");
  pType.AddItemInHandsProfileIK("SPKZ_WoodWallDoorKit", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/BatteryTruck.anm");
 }
}
