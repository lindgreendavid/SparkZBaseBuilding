class SPKZ_WoodWindow extends SPKZ_WoodWallDoor
{
 override bool SPKZ_HasDoor(){return false;}
 override string SPKZ_ReturnKitType(){return "SPKZ_WoodWindowKit";}
 void SPKZ_UpdateWindow()
 {
  float phase=0;
  if(IsOpen()){phase=1;}
  SetAnimationPhase("WindowLeft",phase);
  SetAnimationPhase("WindowRight",phase);
 }
 override void Open(){m_Openable.Open();SPKZ_UpdateWindow();SetSynchDirty();}
 override void Close(){m_Openable.Close();SPKZ_UpdateWindow();SetSynchDirty();}
 override void OnVariablesSynchronized()
 {
  super.OnVariablesSynchronized();
  SPKZ_UpdateWindow();
 }
 override bool OnStoreLoad(ParamsReadContext ctx,int version)
 {
  if(!super.OnStoreLoad(ctx,version)){return false;}
  SPKZ_UpdateWindow();
  return true;
 }
 override void SetActions()
 {
  AddAction(SPKZ_ActionOpenWoodWindow);
  AddAction(SPKZ_ActionCloseWoodWindow);
 }
}
class SPKZ_WoodWindowKit extends SPKZ_WoodWallKit
{
 override string SPKZ_PlacedType(){return "SPKZ_WoodWindow";}
 override string SPKZ_ProjectionType(){return "SPKZ_WoodWindow_Hologram";}
}
