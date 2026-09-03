modded class Hologram
{
 protected static bool s_SPKZActive;
 protected static bool s_SPKZFrozen;
 protected static int s_SPKZAxis;
 protected static bool s_SPKZRotate;
 protected static bool s_SPKZFreeLook;
 protected static bool s_SPKZSnap;
 protected SPKZ_WoodWallDoor m_SPKZSnapTarget;
 protected int m_SPKZSnapSide=-1;
 protected vector m_SPKZSnapLocalAnchor;
 protected vector m_SPKZLockedPosition;
 protected vector m_SPKZManualOffset;
 protected vector m_SPKZOrientation;
 protected bool m_SPKZInitialized;
 protected int m_SPKZControl;
 protected vector m_SPKZForward;
 protected vector m_SPKZRight;
 protected vector m_SPKZAdjustment;
 protected static string s_SPKZRotationReadout;
 protected static string s_SPKZPositionReadout;
 protected static string s_SPKZAdjustmentReadout;
 protected static string s_SPKZKitTitle;
 static string SPKZ_KitTitle(){return s_SPKZKitTitle;}

 void ~Hologram()
 {
  if(GetGame() && !GetGame().IsDedicatedServer())
  {
   s_SPKZActive=false;
   s_SPKZFrozen=false;
  }
 }
 static bool SPKZ_IsActive(){return s_SPKZActive;}
 static bool SPKZ_IsFrozen(){return s_SPKZFrozen;}
 static bool SPKZ_FreeLook(){return s_SPKZFreeLook;}
 static bool SPKZ_SnapEnabled(){return s_SPKZSnap;}
 static string SPKZ_RotationReadout(){return s_SPKZRotationReadout;}
 static string SPKZ_PositionReadout(){return s_SPKZPositionReadout;}
 static string SPKZ_AdjustmentReadout(){return s_SPKZAdjustmentReadout;}
 protected string SPKZ_Number(float value)
 {
  float rounded=Math.Round(value*100.0)/100.0;
  return rounded.ToString();
 }
 protected string SPKZ_Degrees(float value)
 {
  value=value-Math.Floor(value/360.0)*360.0;
  return SPKZ_Number(value);
 }
 protected void SPKZ_UpdateReadout()
 {
  if(!m_Projection || !m_Player){return;}
  vector angles=m_Projection.GetOrientation();
  vector position=m_Projection.GetPosition();
  s_SPKZRotationReadout="Yaw "+SPKZ_Degrees(angles[0])+"°  |  Tilt "+SPKZ_Number(angles[1])+"°  |  Roll "+SPKZ_Number(angles[2])+"°";
  float height=position[1]-GetGame().SurfaceY(position[0],position[2]);
  float reach=vector.Distance(m_Player.GetPosition(),position);
  s_SPKZPositionReadout="Height "+SPKZ_Number(height)+" m  |  Reach "+SPKZ_Number(reach)+" m";
  s_SPKZAdjustmentReadout="";
  if(s_SPKZRotate)
  {
   if(s_SPKZSnap && m_SPKZSnapTarget)
   {
    vector baseAngles=m_SPKZSnapTarget.GetOrientation();
    s_SPKZAdjustmentReadout="  |  Joint "+SPKZ_Degrees(angles[0]-baseAngles[0])+"° yaw";
   }
  }
  else
  {
   float amount=m_SPKZAdjustment[1];
   if(s_SPKZAxis==0){amount=vector.Dot(m_SPKZAdjustment,m_SPKZRight);}
   if(s_SPKZAxis==2){amount=vector.Dot(m_SPKZAdjustment,m_SPKZForward);}
   s_SPKZAdjustmentReadout="  |  Moved "+SPKZ_Number(amount)+" m";
  }
 }
 static string SPKZ_AxisName()
 {
  if(!s_SPKZRotate)
  {
   if(s_SPKZAxis==0){return "LEFT / RIGHT";}
   if(s_SPKZAxis==1){return "UP / DOWN";}
   return "CLOSER / FURTHER";
  }
  if(s_SPKZAxis==0){return "TILT";}
  if(s_SPKZAxis==1){return "SPIN";}
  return "ROLL";
 }
 static string SPKZ_ModeName()
 {
  if(s_SPKZRotate){return "ROTATE";}
  return "MOVE";
 }
 // The workbench's kit extends SPKZ_WoodWallDoorKit (for its shared kit
 // logic - IsBasebuildingKit/deploy actions/OnPlacementComplete), but it's
 // a standalone piece of furniture, not a wall-snapping/joining part - it
 // should place like a plain vanilla deployable (a storage crate, a tent),
 // not with the axis-rotate/freeze/snap control scheme built for stitching
 // wall/door/window/floor/garage pieces together. Every method below falls
 // through to super.X() (real vanilla Hologram behaviour) whenever this
 // returns false, so excluding the workbench here is the single point that
 // gives it a vanilla placement animation.
 protected bool SPKZ_IsWallKit(){return SPKZ_WoodWallDoorKit.Cast(m_Parent)!=null && SPKZ_WorkbenchKit.Cast(m_Parent)==null;}
 protected bool SPKZ_Pressed(string name)
 {
  UAInput key=GetUApi().GetInputByName(name);
  if(!key){return false;}
  return key.LocalPress();
 }
 protected float SPKZ_Wheel(string name)
 {
  UAInput key=GetUApi().GetInputByName(name);
  if(!key){return 0;}
  return key.LocalValue();
 }
 override string ProjectionBasedOnParent()
 {
  SPKZ_WoodWallDoorKit kit=SPKZ_WoodWallDoorKit.Cast(m_Parent);
  if(kit){return kit.SPKZ_ProjectionType();}
  return super.ProjectionBasedOnParent();
 }
 override string GetProjectionName(ItemBase item)
 {
  SPKZ_WoodWallDoorKit kit=SPKZ_WoodWallDoorKit.Cast(m_Parent);
  if(kit){return kit.SPKZ_ProjectionType();}
  return super.GetProjectionName(item);
 }
 override void SetAnimations()
 {
  if(SPKZ_IsWallKit())
  {
   SetSelectionToRefresh("wall");
   if(SPKZ_WoodGarageKit.Cast(m_Parent)){SetSelectionToRefresh("garage_panel");}
   if(SPKZ_WoodGlassWindowKit.Cast(m_Parent)){SetSelectionToRefresh("glass");}
   if(SPKZ_WoodWindowKit.Cast(m_Parent))
   {
    SetSelectionToRefresh("window_left");
    SetSelectionToRefresh("window_right");
    if(m_Projection)
    {
     m_Projection.SetAnimationPhase("WindowLeft",0);
     m_Projection.SetAnimationPhase("WindowRight",0);
    }
   }
   if(!SPKZ_WoodWallKit.Cast(m_Parent))
   {
    SetSelectionToRefresh("door");
    if(m_Projection){m_Projection.SetAnimationPhase("Door",0);}
   }
   return;
  }
  super.SetAnimations();
 }
 override vector AlignProjectionOnTerrain(float timeslice)
 {
  if(SPKZ_IsWallKit() && m_SPKZInitialized){return m_SPKZOrientation;}
  return super.AlignProjectionOnTerrain(timeslice);
 }
 override void SetProjectionPosition(vector position)
 {
  if(SPKZ_IsWallKit() && m_Projection)
  {
   m_Projection.SetPosition(position);
   return;
  }
  super.SetProjectionPosition(position);
 }
 override protected vector GetProjectionEntityPosition(PlayerBase player)
 {
  if(!SPKZ_IsWallKit()){return super.GetProjectionEntityPosition(player);}
  if(s_SPKZFrozen){return m_SPKZLockedPosition;}
  vector position=SPKZ_BasePosition(player);
  if(s_SPKZSnap){position=SPKZ_SnapPosition(player,position);}
  else {m_SPKZSnapTarget=null;}
  return position+m_SPKZManualOffset;
 }
 protected vector SPKZ_BasePosition(PlayerBase player)
 {
  if(s_SPKZFreeLook){return super.GetProjectionEntityPosition(player);}
  vector direction=GetGame().GetCurrentCameraDirection();
  direction[1]=0;
  if(direction.Length()<0.01){direction=m_SPKZForward;}
  direction.Normalize();
  vector position=player.GetPosition()+direction*4.0;
  position[1]=GetGame().SurfaceY(position[0],position[2]);
  // Floor origin is its top face: clear its 20 cm thickness plus 2 cm.
  // Applied only to the ground-following fallback, never to snap anchors.
  if(SPKZ_WoodFloorKit.Cast(m_Parent)){position[1]=position[1]+0.22;}
  return position;
 }
 protected vector SPKZ_SnapPosition(PlayerBase player, vector fallback)
 {
  array<Object> objects=new array<Object>;
  array<CargoBase> cargos=new array<CargoBase>;
  GetGame().GetObjectsAtPosition3D(player.GetPosition(),15.0,objects,cargos);
  vector camera=GetGame().GetCurrentCameraPosition();
  vector direction=GetGame().GetCurrentCameraDirection();
  float best=1.25;
  SPKZ_WoodWallDoor chosen;
  vector result=fallback;
  vector chosenAnchor;
  vector chosenWorldAnchor;
  vector chosenOrientation;
  int chosenSide=-1;
  bool placingFloor=SPKZ_WoodFloorKit.Cast(m_Parent)!=null;
  foreach(Object object: objects)
  {
   SPKZ_WoodWallDoor wall=SPKZ_WoodWallDoor.Cast(object);
   if(!wall){continue;}
   bool targetFloor=SPKZ_WoodFloor.Cast(wall)!=null;
   vector localPlayer=wall.WorldToModel(player.GetPosition());
   // All anchors use the target's coordinate frame. Floor origins are at
   // their walking surface, wall origins are at the bottom of the wall.
   for(int side=0;side<4;side++)
   {
    vector edge;
    vector offset;
    vector anchor;
    if(side==0){edge=Vector(-2.25,1.6,0);offset=Vector(-4.5,0,0);}
    if(side==1){edge=Vector(2.25,1.6,0);offset=Vector(4.5,0,0);}
    if(side==2){edge=Vector(0,3.2,0);offset=Vector(0,3.2,0);}
    if(side==3){edge=Vector(0,0,0);offset=Vector(0,-3.2,0);}
    anchor=edge-offset;
    if(targetFloor)
    {
     if(side==0){edge=Vector(-2.25,0,0);anchor=Vector(2.25,0,0);}
     if(side==1){edge=Vector(2.25,0,0);anchor=Vector(-2.25,0,0);}
     if(side==2){edge=Vector(0,0,-2.25);anchor=Vector(0,0,2.25);}
     if(side==3){edge=Vector(0,0,2.25);anchor=Vector(0,0,-2.25);}
     if(!placingFloor){anchor=vector.Zero;}
    }
    else if(placingFloor)
    {
     // Four choices: floor or roof, on either face of the wall. Looking
     // from a face chooses that side, avoiding two equal-distance snaps.
     if(side<2){continue;}
     float face=1.0;
     if(localPlayer[2]<0){face=-1.0;}
     if(side==2){edge=Vector(0,3.2,0);}
     if(side==3){edge=Vector(0,0,0);}
     anchor=Vector(0,0,-face*2.25);
    }
    offset=edge-anchor;
    vector candidateOrientation=wall.GetOrientation();
    if(targetFloor && !placingFloor && side<2)
    {
     // Wall width must run along the chosen floor edge, not across it.
     // Compose locally so this also works on tilted floors/ramps.
     vector targetBasis[3];
     vector quarterTurn[3];
     vector joinedBasis[3];
     Math3D.YawPitchRollMatrix(candidateOrientation,targetBasis);
     Math3D.YawPitchRollMatrix(Vector(90,0,0),quarterTurn);
     Math3D.MatrixMultiply3(targetBasis,quarterTurn,joinedBasis);
     candidateOrientation=Math3D.MatrixToAngles(joinedBasis);
    }
    vector worldEdge=wall.ModelToWorld(edge);
    vector toEdge=worldEdge-camera;
    float along=vector.Dot(toEdge,direction);
    if(along<0 || along>15.0){continue;}
    float score=vector.Distance(worldEdge,camera+direction*along);
    vector candidate=wall.ModelToWorld(offset);
    if(score<best && vector.Distance(player.GetPosition(),candidate)<=15.0)
    {
     best=score;
     result=candidate;
     chosen=wall;
     chosenAnchor=anchor;
     chosenWorldAnchor=worldEdge;
     chosenOrientation=candidateOrientation;
     chosenSide=side;
    }
   }
  }
  if(chosen && (chosen!=m_SPKZSnapTarget || (SPKZ_WoodFloor.Cast(chosen) && !placingFloor && chosenSide!=m_SPKZSnapSide)))
  {
   m_SPKZOrientation=chosenOrientation;
  }
  m_SPKZSnapTarget=chosen;
  m_SPKZSnapSide=chosenSide;
  if(chosen)
  {
   m_SPKZSnapLocalAnchor=chosenAnchor;
   // Keep the joining edge fixed while the new wall turns around it.
   result=chosenWorldAnchor-SPKZ_RotateAnchor(m_SPKZOrientation);
  }
  return result;
 }
 protected vector SPKZ_RotateAnchor(vector orientation)
 {
  vector rotation[3];
  Math3D.YawPitchRollMatrix(orientation,rotation);
  return m_SPKZSnapLocalAnchor.Multiply3(rotation);
 }
 override void UpdateHologram(float timeslice)
 {
  if(!SPKZ_IsWallKit() || GetGame().IsDedicatedServer())
  {
   super.UpdateHologram(timeslice);
   return;
  }
  if(!m_Projection){s_SPKZActive=false;return;}
  s_SPKZActive=true;
  s_SPKZKitTitle=m_Parent.GetDisplayName();
  if(!m_SPKZInitialized)
  {
   m_SPKZOrientation=GetDefaultOrientation();
   s_SPKZFrozen=false;
   s_SPKZAxis=1;
   s_SPKZRotate=true;
   s_SPKZFreeLook=false;
   s_SPKZSnap=true;
   m_SPKZControl=0;
   m_SPKZForward=GetGame().GetCurrentCameraDirection();
   m_SPKZForward[1]=0;
   m_SPKZForward.Normalize();
   m_SPKZRight=Vector(m_SPKZForward[2],0,-m_SPKZForward[0]);
   m_SPKZInitialized=true;
  }
  if(GetUpdatePosition() && !GetGame().GetUIManager().GetMenu())
  {
   if(SPKZ_Pressed("UASPKZBuildFreeLook")){s_SPKZFreeLook=!s_SPKZFreeLook;}
   if(SPKZ_Pressed("UASPKZBuildSnap")){s_SPKZSnap=!s_SPKZSnap;}
   if(SPKZ_Pressed("UASPKZBuildAxis"))
   {
    m_SPKZControl=(m_SPKZControl+1)%6;
    s_SPKZRotate=false;
    if(m_SPKZControl==0){s_SPKZAxis=1;s_SPKZRotate=true;}
    if(m_SPKZControl==1){s_SPKZAxis=2;}
    if(m_SPKZControl==2){s_SPKZAxis=1;}
    if(m_SPKZControl==3){s_SPKZAxis=0;}
    if(m_SPKZControl==4){s_SPKZAxis=0;s_SPKZRotate=true;}
    if(m_SPKZControl==5){s_SPKZAxis=2;s_SPKZRotate=true;}
   }
   if(SPKZ_Pressed("UASPKZBuildFreeze"))
   {
    if(!s_SPKZFrozen)
    {
     m_SPKZLockedPosition=m_Projection.GetPosition();
     m_SPKZOrientation=m_Projection.GetOrientation();
     s_SPKZFrozen=true;
    }
    else
    {
     s_SPKZFrozen=false;
     vector unlockedBase=SPKZ_BasePosition(m_Player);
     if(s_SPKZSnap){unlockedBase=SPKZ_SnapPosition(m_Player,unlockedBase);}
     m_SPKZManualOffset=m_SPKZLockedPosition-unlockedBase;
    }
   }
   float step=0;
   if(SPKZ_Wheel("UASPKZBuildWheelUpV2")>0){step=0.10;}
   if(SPKZ_Wheel("UASPKZBuildWheelDownV2")>0){step=-0.10;}
   if(SPKZ_Pressed("UASPKZBuildPositiveV2")){step=0.01;}
   if(SPKZ_Pressed("UASPKZBuildNegativeV2")){step=-0.01;}
   if(step!=0)
   {
    if(s_SPKZRotate)
    {
     // DayZ orientation order is yaw(Y), pitch(X), roll(Z).
     int component=s_SPKZAxis;
     if(s_SPKZAxis==0){component=1;}
     if(s_SPKZAxis==1){component=0;}
     vector previousAnchor=SPKZ_RotateAnchor(m_SPKZOrientation);
     m_SPKZOrientation[component]=m_SPKZOrientation[component]+step*50;
     if(s_SPKZFrozen && s_SPKZSnap && m_SPKZSnapTarget)
     {
      // Compensate the origin so the world-space attachment point does
      // not move. This also retains deliberate frozen translation offsets.
      m_SPKZLockedPosition=m_SPKZLockedPosition+previousAnchor-SPKZ_RotateAnchor(m_SPKZOrientation);
     }
    }
    else
    {
     vector movement=Vector(0,step,0);
     if(s_SPKZAxis==0){movement=m_SPKZRight*step;}
     if(s_SPKZAxis==2){movement=m_SPKZForward*step;}
     m_SPKZAdjustment=m_SPKZAdjustment+movement;
     if(s_SPKZFrozen){m_SPKZLockedPosition=m_SPKZLockedPosition+movement;}
     else {m_SPKZManualOffset=m_SPKZManualOffset+movement;}
    }
   }
  }
  super.UpdateHologram(timeslice);
  SPKZ_UpdateReadout();
 }
 override void EvaluateCollision(ItemBase action_item=null)
 {
  if(!SPKZ_IsWallKit()){super.EvaluateCollision(action_item);return;}
  SetIsCollidingPlayer(false);
  SetIsFloating(false);
  if(!m_Projection || !m_Player){SetIsColliding(true);return;}
  SetIsColliding(vector.Distance(m_Player.GetPosition(),m_Projection.GetPosition())>15.0);
 }
 override EntityAI PlaceEntity(EntityAI entity_for_placing)
 {
  if(SPKZ_IsWallKit()){return entity_for_placing;}
  return super.PlaceEntity(entity_for_placing);
 }
}
