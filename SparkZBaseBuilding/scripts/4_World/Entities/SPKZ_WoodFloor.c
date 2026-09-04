class SPKZ_WoodFloor extends SPKZ_WoodWall
{
 override string SPKZ_ReturnKitType(){return "SPKZ_WoodFloorKit";}
 // A floor/roof/ramp isn't a wall or window you'd breach through with a
 // charge - per direction, charges only target walls and windows.
 override bool SPKZ_CanAcceptCharge(){return false;}
}
class SPKZ_WoodFloorKit extends SPKZ_WoodWallKit
{
 override string SPKZ_PlacedType(){return "SPKZ_WoodFloor";}
 override string SPKZ_ProjectionType(){return "SPKZ_WoodFloor_Hologram";}
}
