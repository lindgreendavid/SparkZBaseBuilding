 // Both charge tiers extend the installed game's own ExplosivesBase config
 // ancestor - the exact same one vanilla's Plastic_Explosive uses - keeping
 // config and script ancestors matched (see the top of
 // scripts/4_World/Entities/SPKZ_PlacedCharge.c for why that matters).
 //
 // model= is a placeholder (the SparkZKit cardboard box, already used for
 // every other kit in this addon before its real model arrived) - the
 // friend's dayz_custom_items_v4.zip package covers the real textures/
 // geometry for both of these (Homemade Breaching Charge -> the "Improvised
 // Breaching Charge" item in that package; C4 -> "Fabricated Charge Block"),
 // pending their Blender pass and a real P3D export. Swap model= once that
 // lands - nothing else in this file or the scripts needs to change.
 class SPKZ_HomemadeBreachingCharge: ExplosivesBase
 {
  scope=2; displayName="Improvised Breaching Charge";
  descriptionShort="A homemade demolition charge. Works on wood on its own; needs two to get through anything tougher.";
  model="SparkZBaseBuilding\Data\SparkZKit\SPKZ_CardboardKit.p3d";
  weight=3000; itemSize[]={3,2}; rotationFlags=34; physLayer="item_medium";
  class DamageSystem {class GlobalHealth {class Health {hitpoints=100;};};};
 };
 class SPKZ_FabricatedChargeBlock: ExplosivesBase
 {
  scope=2; displayName="Fabricated Charge Block";
  descriptionShort="A factory-sealed demolition charge. Breaches wood or anything tougher in a single plant - purely fictional, not a real munition.";
  model="SparkZBaseBuilding\Data\SparkZKit\SPKZ_CardboardKit.p3d";
  weight=5000; itemSize[]={4,2}; rotationFlags=34; physLayer="item_medium";
  class DamageSystem {class GlobalHealth {class Health {hitpoints=100;};};};
 };
