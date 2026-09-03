class CfgPatches
{
 class SPKZ_BaseBuilding_Standalone
 {
  units[]={"SPKZ_WoodWallDoorKit","SPKZ_WoodWallDoor","SPKZ_WoodWallDoor_Hologram","SPKZ_WoodWallKit","SPKZ_WoodWall","SPKZ_WoodWall_Hologram","SPKZ_WoodFloorKit","SPKZ_WoodFloor","SPKZ_WoodFloor_Hologram","SPKZ_WoodWindowKit","SPKZ_WoodWindow","SPKZ_WoodWindow_Hologram","SPKZ_WoodGlassWindowKit","SPKZ_WoodGlassWindow","SPKZ_WoodGlassWindow_Hologram","SPKZ_WoodGarageKit","SPKZ_WoodGarage","SPKZ_WoodGarage_Hologram","SPKZ_WorkbenchKit","SPKZ_Workbench","SPKZ_Workbench_Hologram"};
  weapons[]={}; requiredVersion=0.1;
  requiredAddons[]={"DZ_Data","DZ_Gear_Containers","DZ_Gear_Tools"};
 };
};
class CfgMods
{
 class SPKZ_BaseBuilding_Standalone
 {
  dir="SparkZBaseBuilding"; name="SparkZ Base Building"; type="mod";
  dependencies[]={"World","Mission"};
  inputs="SparkZBaseBuilding/inputsSparkZBaseBuilding.xml";
  class defs
  {
   class imageSets {files[]={"SparkZBaseBuilding/gui/imagesets/spkz_workbench.imageset"};};
   class gameScriptModule {value=""; files[]={"SparkZBaseBuilding/scripts/3_Game"};};
   class worldScriptModule {value=""; files[]={"SparkZBaseBuilding/scripts/4_World"};};
   class missionScriptModule {value=""; files[]={"SparkZBaseBuilding/scripts/5_Mission"};};
  };
 };
};
#include "workbench_slots.hpp"
class CfgVehicles
{
 class Container_Base;
 class HouseNoDestruct;
 class SPKZ_WoodWallDoorKit: Container_Base
 {
  scope=2; displayName="Wood Wall Door Kit";
  descriptionShort="A packaged wooden wall with an opening door.";
  model="SparkZBaseBuilding\Data\SparkZKit\SPKZ_CardboardKit.p3d";
  itemSize[]={4,3}; weight=5000; rotationFlags=34; itemBehaviour=1; physLayer="item_small";
  itemsCargoSize[]={0,0}; heavyItem=1; canBeDigged=0;
  class DamageSystem {class GlobalHealth {class Health {hitpoints=100;};};};
 };
 class SPKZ_WoodWallDoor: HouseNoDestruct
 {
  scope=2; displayName="Wood Wall with Door";
  model="SparkZBaseBuilding\Data\WoodDoorWall\SPKZ_WoodDoorWall.p3d";
  carveNavmesh=1; alignHologramToTerain=0;
  class AnimationSources {class Door {source="user"; initPhase=0; animPeriod=1;};};
 };
 class SPKZ_WoodWallKit: SPKZ_WoodWallDoorKit
 {
  scope=2;
  displayName="Wood Wall Kit";
  descriptionShort="A packaged solid wooden wall.";
 };
 class SPKZ_WoodWall: SPKZ_WoodWallDoor
 {
  scope=2;
  displayName="Wood Wall";
  model="SparkZBaseBuilding\Data\WoodWall\SPKZ_WoodWall.p3d";
 };
 class SPKZ_WoodWall_Hologram: Container_Base
 {
  scope=1;
  model="SparkZBaseBuilding\Data\WoodWall\SPKZ_WoodWall.p3d";
  hiddenSelections[]={"wall"};
  alignHologramToTerain=0;
 };
 class SPKZ_WoodFloorKit: SPKZ_WoodWallKit
 {
  scope=2;
  displayName="Wood Floor Kit";
  descriptionShort="A packaged wooden floor. Can also be placed as a roof or ramp.";
 };
 class SPKZ_WoodFloor: SPKZ_WoodWall
 {
  scope=2;
  displayName="Wood Floor";
  model="SparkZBaseBuilding\Data\WoodFloor\SPKZ_WoodFloor.p3d";
 };
 class SPKZ_WoodFloor_Hologram: SPKZ_WoodWall_Hologram
 {
  scope=1;
  model="SparkZBaseBuilding\Data\WoodFloor\SPKZ_WoodFloor.p3d";
 };
 class SPKZ_WoodWindowKit: SPKZ_WoodWallKit
 {
  scope=2; displayName="Wood Window Kit";
  descriptionShort="A wooden wall with a square outward-opening split window.";
 };
 class SPKZ_WoodWindow: SPKZ_WoodWall
 {
  scope=2; displayName="Wood Window Wall";
  model="SparkZBaseBuilding\Data\WoodWindow\SPKZ_WoodWindow.p3d";
  class AnimationSources
  {
   class WindowLeft {source="user"; initPhase=0; animPeriod=1;};
   class WindowRight {source="user"; initPhase=0; animPeriod=1;};
  };
 };
 class SPKZ_WoodWindow_Hologram: SPKZ_WoodWall_Hologram
 {
  scope=1;
  model="SparkZBaseBuilding\Data\WoodWindow\SPKZ_WoodWindow.p3d";
  hiddenSelections[]={"wall","window_left","window_right"};
  class AnimationSources
  {
   class WindowLeft {source="user"; initPhase=0; animPeriod=1;};
   class WindowRight {source="user"; initPhase=0; animPeriod=1;};
  };
 };
 class SPKZ_WoodGlassWindowKit: SPKZ_WoodWallKit
 {
  scope=2; displayName="Wood Glass Window Kit";
  descriptionShort="A wooden wall with a wide, fixed bulletproof glass window.";
 };
 class SPKZ_WoodGlassWindow: SPKZ_WoodWall
 {
  scope=2; displayName="Wood Glass Window Wall";
  model="SparkZBaseBuilding\Data\WoodGlassWindow\SPKZ_WoodGlassWindow.p3d";
 };
 class SPKZ_WoodGlassWindow_Hologram: SPKZ_WoodWall_Hologram
 {
  scope=1;
  model="SparkZBaseBuilding\Data\WoodGlassWindow\SPKZ_WoodGlassWindow.p3d";
  hiddenSelections[]={"wall","glass"};
 };
 class SPKZ_WoodGarageKit: SPKZ_WoodWallKit
 {
  scope=2;displayName="Wood Garage Door Kit";
  descriptionShort="A wooden garage wall with a horizontal-plank up-and-over door.";
 };
 class SPKZ_WoodGarage: SPKZ_WoodWall
 {
  scope=2;displayName="Wood Garage Door";
  model="SparkZBaseBuilding\Data\WoodGarage\SPKZ_WoodGarage.p3d";
  class AnimationSources
  {
   class GarageTilt {source="user";initPhase=0;animPeriod=3;};
   class GarageLift1: GarageTilt {};
   class GarageLift2: GarageTilt {};
   class GarageLift3: GarageTilt {};
   class GarageLift4: GarageTilt {};
  };
 };
 class SPKZ_WoodGarage_Hologram: SPKZ_WoodWall_Hologram
 {
  scope=1;model="SparkZBaseBuilding\Data\WoodGarage\SPKZ_WoodGarage.p3d";
  hiddenSelections[]={"wall","garage_panel"};
  class AnimationSources
  {
   class GarageTilt {source="user";initPhase=0;animPeriod=3;};
   class GarageLift1: GarageTilt {};
   class GarageLift2: GarageTilt {};
   class GarageLift3: GarageTilt {};
   class GarageLift4: GarageTilt {};
  };
 };
 class SPKZ_WoodWallDoor_Hologram: Container_Base
 {
  scope=1;
  model="SparkZBaseBuilding\Data\WoodDoorWall\SPKZ_WoodDoorWall.p3d";
  hiddenSelections[]={"wall","door"};
  alignHologramToTerain=0;
  class AnimationSources {class Door {source="user"; initPhase=0; animPeriod=1;};};
 };
 #include "workbench_vehicles.hpp"
};
