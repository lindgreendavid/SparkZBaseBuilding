class SPKZ_WoodGlassWindow extends SPKZ_WoodWall
{
 override string SPKZ_ReturnKitType(){return "SPKZ_WoodGlassWindowKit";}
}
class SPKZ_WoodGlassWindowKit extends SPKZ_WoodWallKit
{
 override string SPKZ_PlacedType(){return "SPKZ_WoodGlassWindow";}
 override string SPKZ_ProjectionType(){return "SPKZ_WoodGlassWindow_Hologram";}
}
