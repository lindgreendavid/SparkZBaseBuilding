class SPKZ_WoodWall extends SPKZ_WoodWallDoor
{
 override bool SPKZ_HasDoor(){return false;}
 override string SPKZ_ReturnKitType(){return "SPKZ_WoodWallKit";}
 override void SetActions()
 {
  // Solid wall: no door actions. Screwdriver carries the dismantle action.
 }
}
class SPKZ_WoodWallKit extends SPKZ_WoodWallDoorKit
{
 override string SPKZ_PlacedType(){return "SPKZ_WoodWall";}
 override string SPKZ_ProjectionType(){return "SPKZ_WoodWall_Hologram";}
}
