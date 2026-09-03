class SPKZ_WoodFloor extends SPKZ_WoodWall
{
 override string SPKZ_ReturnKitType(){return "SPKZ_WoodFloorKit";}
}
class SPKZ_WoodFloorKit extends SPKZ_WoodWallKit
{
 override string SPKZ_PlacedType(){return "SPKZ_WoodFloor";}
 override string SPKZ_ProjectionType(){return "SPKZ_WoodFloor_Hologram";}
}
