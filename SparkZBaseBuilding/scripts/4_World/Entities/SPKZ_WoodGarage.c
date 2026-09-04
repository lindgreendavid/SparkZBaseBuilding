class SPKZ_WoodGarage extends SPKZ_WoodWallDoor
{
 override bool SPKZ_HasDoor(){return false;}
 override string SPKZ_ReturnKitType(){return "SPKZ_WoodGarageKit";}
 // Not in scope for charges yet (per direction: walls and windows only) -
 // SetActions() below doesn't call super so SPKZ_ActionPlaceCharge was never
 // registered on it anyway, but this makes the exclusion explicit rather
 // than incidental.
 override bool SPKZ_CanAcceptCharge(){return false;}
 void SPKZ_UpdateGarage()
 {
  float phase=0;
  if(IsOpen()){phase=1;}
  SetAnimationPhase("GarageTilt",phase);
  SetAnimationPhase("GarageLift1",phase);
  SetAnimationPhase("GarageLift2",phase);
  SetAnimationPhase("GarageLift3",phase);
  SetAnimationPhase("GarageLift4",phase);
 }
 override void Open(){m_Openable.Open();SPKZ_UpdateGarage();SetSynchDirty();}
 override void Close(){m_Openable.Close();SPKZ_UpdateGarage();SetSynchDirty();}
 override void OnVariablesSynchronized(){super.OnVariablesSynchronized();SPKZ_UpdateGarage();}
 override bool OnStoreLoad(ParamsReadContext ctx,int version)
 {
  if(!super.OnStoreLoad(ctx,version)){return false;}
  SPKZ_UpdateGarage();
  return true;
 }
 override void SetActions()
 {
  AddAction(SPKZ_ActionOpenWoodGarage);
  AddAction(SPKZ_ActionCloseWoodGarage);
 }
}
class SPKZ_WoodGarageKit extends SPKZ_WoodWallKit
{
 override string SPKZ_PlacedType(){return "SPKZ_WoodGarage";}
 override string SPKZ_ProjectionType(){return "SPKZ_WoodGarage_Hologram";}
}
