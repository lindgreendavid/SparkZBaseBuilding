 class Inventory_Base;
 class SPKZ_WorkbenchCollision: HouseNoDestruct
 {
  scope=1;
  model="SparkZBaseBuilding\Data\Workbench\SPKZ_WorkbenchCollision.p3d";
  carveNavmesh=1;
 };
 class Hacksaw: Inventory_Base {inventorySlot[] += {"SPKZ_WB_Hacksaw"};};
 class HandSaw: Inventory_Base {inventorySlot[] += {"SPKZ_WB_HandSaw"};};
 class Hammer: Inventory_Base {inventorySlot[] += {"SPKZ_WB_Hammer"};};
 class Shovel: Inventory_Base {inventorySlot[] += {"SPKZ_WB_Shovel"};};
 class Screwdriver: Inventory_Base {inventorySlot[] += {"SPKZ_WB_Screwdriver"};};
 class Pliers: Inventory_Base {inventorySlot[] += {"SPKZ_WB_Pliers"};};
 class SledgeHammer: Inventory_Base {inventorySlot[] += {"SPKZ_WB_SledgeHammer"};};
 class Whetstone: Inventory_Base {inventorySlot[] += {"SPKZ_WB_SharpeningStone"};};
 // SPKZ_Workbench deliberately does NOT extend SPKZ_WoodWallDoor here: that
 // family ultimately traces back to HouseNoDestruct (simulation="house",
 // confirmed via the installed game's own base config), which gives the
 // engine's proxy-only BuildingInventory - no real drag-and-drop cargo grid,
 // and it never shows up in the player's nearby-storage Tab screen. A
 // container that needs real 500-slot cargo needs Container_Base
 // (simulation="inventoryItem", the same ancestor tents/backpacks/crates
 // use). The SPKZ_Workbench SCRIPT class still extends SPKZ_WoodWallDoor
 // (see spkz_workbench.c) for its owner/dismantle/persistence logic - the
 // config ancestor and the script ancestor are independent, the engine only
 // matches script classes to CfgVehicles entries by name.
 class SPKZ_Workbench: Container_Base
 {
  scope=2;displayName="Building Workbench";
  descriptionShort="A workshop bench with 500 cargo slots and eight visible tool mounts. Crafting recipes are configured separately.";
  model="SparkZBaseBuilding\Data\Workbench\SPKZ_Workbench.p3d";
  weight=15000;heavyItem=1;physLayer="item_large";canBeDigged=0;carveNavmesh=1;
  class DamageSystem {class GlobalHealth {class Health {hitpoints=1000;};};};
  attachments[]={"SPKZ_WB_Hacksaw","SPKZ_WB_HandSaw","SPKZ_WB_Hammer","SPKZ_WB_Shovel","SPKZ_WB_Screwdriver","SPKZ_WB_Pliers","SPKZ_WB_SledgeHammer","SPKZ_WB_SharpeningStone"};
  itemsCargoSize[]={20,25};
  class Cargo {itemsCargoSize[]={20,25};openable=0;allowOwnedCargoManipulation=1;};
  class AnimationSources {};
  class GUIInventoryAttachmentsProps
  {
   class Tools {name="Workbench tools";description="One of each tool";attachmentSlots[]={"SPKZ_WB_Hacksaw","SPKZ_WB_HandSaw","SPKZ_WB_Hammer","SPKZ_WB_Shovel","SPKZ_WB_Screwdriver","SPKZ_WB_Pliers","SPKZ_WB_SledgeHammer","SPKZ_WB_SharpeningStone"};icon="set:dayz_inventory image:cat_common_cargo";};
  };
 };
 class SPKZ_WorkbenchKit: SPKZ_WoodWallDoorKit
 {
  alignHologramToTerain=1;
  scope=2;displayName="Workbench Kit";descriptionShort="A packaged building workbench with storage and tool mounts.";
 };
 class SPKZ_Workbench_Hologram: Container_Base
 {
  alignHologramToTerain=1;
  scope=1;model="SparkZBaseBuilding\Data\Workbench\SPKZ_Workbench.p3d";
 };
