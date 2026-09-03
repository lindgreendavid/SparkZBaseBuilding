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
 class SPKZ_Workbench: SPKZ_WoodWallDoor
 {
  scope=2;displayName="Building Workbench";
  descriptionShort="A workshop bench with 500 cargo slots and eight visible tool mounts. Crafting recipes are configured separately.";
  model="SparkZBaseBuilding\Data\Workbench\SPKZ_Workbench.p3d";
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
 class SPKZ_Workbench_Hologram: SPKZ_WoodWall_Hologram
 {
  alignHologramToTerain=1;
  scope=1;model="SparkZBaseBuilding\Data\Workbench\SPKZ_Workbench.p3d";
 };
