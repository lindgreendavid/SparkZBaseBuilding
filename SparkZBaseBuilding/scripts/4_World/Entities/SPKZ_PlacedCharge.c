// Raiding charges for SPKZBaseBuilding's own wood walls/windows (per
// direction: charges only target this addon's wall/window pieces for now,
// not BaseBuildingPlus or any other mod's structures - a future "metal"
// SPKZ piece would just override SPKZ_RequiredChargeCount() to 2, no
// changes needed here).
//
// Extends the installed game's own ExplosivesBase (see Plastic_Explosive/
// ImprovisedExplosive for the vanilla precedent) instead of reinventing a
// timer/explosion system - that gives us real, tested explosion damage,
// particle/light effects, and sound handling for free. What we add on top
// is much simpler than vanilla's own multi-trigger (clock/remote/kitchen
// timer) arming system: a single fixed fuse per charge type that starts the
// moment it's planted, no separate arm step, matching "after placing it
// takes N minutes to explode" exactly as specced.
//
// Placement itself is NOT vanilla's ground-deploy ghost (ActionDeployObject)
// - that has no concept of "must be next to a wall". Instead
// SPKZ_ActionPlaceCharge (see SPKZ_ChargeActions.c) is registered directly
// on SPKZ_WoodWallDoor (and everything that extends it) and only allows
// planting while the player is holding a charge and looking at a piece
// where SPKZ_CanAcceptCharge() is true.
class SPKZ_PlacedCharge extends ExplosivesBase
{
 // The specific wall/window instance this charge is planted against - set
 // by SPKZ_ActionPlaceCharge.OnFinishProgressServer, read back when the
 // fuse ends. 4_World-tier on both sides, so direct typing is fine (see
 // Pitfall #8 in docs/CODING_STANDARDS.md - that only applies across the
 // World/Mission boundary, not within World).
 protected SPKZ_WoodWallDoor m_SPKZTargetWall;
 protected bool m_SPKZFuseStarted;

 void SPKZ_PlacedCharge()
 {
  SetAmmoType("Plastic_Explosive_Ammo");
  SetParticleExplosion(ParticleList.PLASTIC_EXPLOSION);
 }

 // ExplosivesBase.InitItemSounds() already wires GetArmSoundset() up to the
 // named ITEM_EXPLOSIVE_ARM sound slot (see StartItemSoundServer below) -
 // reusing that same real, MP-replicated, server-callable mechanism vanilla
 // uses for its own arm-click sound, just fired repeatedly as a countdown
 // beep instead of once. RemoteDetonator_Trigger_SoundSet is a real vanilla
 // SoundSet (an electronic detonator click) - closer to a "beeping bomb"
 // than the mechanical KitchenTimer_Ticking_Loop alternative.
 override string GetArmSoundset(){return "RemoteDetonator_Trigger_SoundSet";}

 // Seconds from planting to detonation. Overridden per charge tier.
 float SPKZ_FuseSeconds(){return 210;} // 3:30 - Homemade Breaching Charge default

 // Whether this charge tier breaches in one hit regardless of
 // SPKZ_RequiredChargeCount() - true for the Fabricated Charge Block ("C4"),
 // which per direction "immediately goes through metal and reinforced
 // things" rather than needing to stack like the homemade charge does.
 bool SPKZ_AlwaysBreaches(){return false;}

 // Called by SPKZ_ActionPlaceCharge once planting finishes. Server-only -
 // the position/orientation and target are already set by the caller before
 // this runs.
 void SPKZ_BeginFuse(SPKZ_WoodWallDoor targetWall)
 {
  if (!GetGame().IsServer() || m_SPKZFuseStarted) return;
  m_SPKZTargetWall = targetWall;
  m_SPKZFuseStarted = true;
  Arm();
  SPKZ_PlayBeep(); // immediate first beep, then every 4s for the rest of the fuse
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SPKZ_PlayBeep, 4000, true);
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SPKZ_Detonate, SPKZ_FuseSeconds() * 1000, false);
 }

 void SPKZ_PlayBeep()
 {
  if (!GetGame() || !GetGame().IsServer()) return;
  if (GetHealthLevel("") == GameConstants.STATE_RUINED) return;
  StartItemSoundServer(SoundConstants.ITEM_EXPLOSIVE_ARM);
 }

 void SPKZ_Detonate()
 {
  if (!GetGame().IsServer()) return;
  GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(SPKZ_PlayBeep);
  if (GetHealthLevel("") == GameConstants.STATE_RUINED) return; // already gone

  // The wall may have been dismantled or already breached by an earlier
  // charge while this one's fuse was still running - only apply a hit if
  // it's still there.
  if (m_SPKZTargetWall) { m_SPKZTargetWall.SPKZ_ApplyChargeHit(SPKZ_AlwaysBreaches()); }

  SetHealth("", "", 0.0);
  InitiateExplosion(); // real vanilla explosion: damage/particle/light/sound (Plastic_Explosive_Ammo's soundSetExplosion), then deletes itself
 }

 override void EEDelete(EntityAI parent)
 {
  if (GetGame()) { GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(SPKZ_PlayBeep); }
  super.EEDelete(parent);
 }

 // Explosion damage from this charge should never re-trigger another
 // planted charge nearby (matches Plastic_Explosive's own
 // EEOnDamageCalculated guard against charge-on-charge chain reactions).
 override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
 {
  if (damageType == DamageType.EXPLOSION) return false;
  return super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
 }
}

class SPKZ_HomemadeBreachingCharge extends SPKZ_PlacedCharge
{
 override float SPKZ_FuseSeconds(){return 210;} // 3:30
}

class SPKZ_FabricatedChargeBlock extends SPKZ_PlacedCharge
{
 override float SPKZ_FuseSeconds(){return 300;} // 5:00
 override bool SPKZ_AlwaysBreaches(){return true;}
}
