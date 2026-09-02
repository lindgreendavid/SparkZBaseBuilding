// Restricts HDSN_BreachingCharge (the "@Breachingcharge" / "@BreachingCharge_Remastered" mods)
// so charges can only be planted on BBP doors and windows, never on walls/gates/pillars/roofs/
// floors. This is done by rewriting its own DestroyableObjects targeting map at boot, using the
// mod's real public API (HDSN_BreachingChargeConfigManager/HDSN_BreachingChargeConfigData) -
// no separate JSON file to hand-edit, no risk of a serialization-format mismatch.
//
// The mod's own default Tiers already give exactly the "wood=1 charge, metal=2 light charges
// (or 1 heavy/homemade charge, since HDSN_BreachingChargeHeavy deals 2 damage per hit vs the
// light charge's 1)" behavior out of the box:
//   Tier1 Health=1, accepts HDSN_BreachingCharge + HDSN_BreachingChargeHeavy
//   Tier2 Health=2, accepts HDSN_BreachingCharge + HDSN_BreachingChargeHeavy
// so this hook only needs to point the right BBP door/window class names at those tiers and
// leave every non-door/window class name out entirely (IsChargeAllowed denies anything not
// present in DestroyableObjects).
//
// Deliberately NOT integrated with SparkZBase claims: raiders are supposed to be able to plant
// charges on a base they don't own, so this must never call CanUseBaseAtPosition or similar.
//
// Requires @Breachingcharge (HDSN_BreachingCharge) to be loaded before @SparkZBase - it already
// is in this server's mod order, and that load-order alone is what makes these class references
// compile. Deliberately NOT added to SparkZBase's own requiredAddons[] in config.cpp: we found
// the hard way this session that any entry there corrupts the server browser's Steam query
// response (the SPKZ_Storage requiredAddons entry did the exact same thing even though that
// addon resolves correctly), so cross-mod dependencies here are load-order-only on purpose.
class SparkZBaseBreachCompat
{
    static void Apply()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        HDSN_BreachingChargeConfigData config = HDSN_BreachingChargeConfigManager.GetInstance().GetConfigData();
        if (!config || !config.DestroyableObjects)
            return;

        config.DestroyableObjects.Clear();

        array<string> woodTier = new array<string>();
        woodTier.Insert("_Frame");
        woodTier.Insert("_T1");

        array<string> metalTier = new array<string>();
        metalTier.Insert("_T2");
        metalTier.Insert("_T3");

        // Doors: base type + wall tier suffix + matching door-part tier suffix (frame doors
        // carry no door-part suffix at all, since no door tier has been installed yet).
        RegisterDoor(config, "BBP_BDoor", "_Frame", "", "Tier1");
        RegisterDoor(config, "BBP_BDoor", "_T1", "_Door_T1", "Tier1");
        RegisterDoor(config, "BBP_BDoor", "_T2", "_Door_T2", "Tier2");
        RegisterDoor(config, "BBP_BDoor", "_T3", "_Door_T3", "Tier2");

        RegisterDoor(config, "BBP_SDoor", "_Frame", "", "Tier1");
        RegisterDoor(config, "BBP_SDoor", "_T1", "_Door_T1", "Tier1");
        RegisterDoor(config, "BBP_SDoor", "_T2", "_Door_T2", "Tier2");
        RegisterDoor(config, "BBP_SDoor", "_T3", "_Door_T3", "Tier2");

        // Windows: base type + wall tier suffix only (no door-part concept for windows).
        RegisterWindow(config, "BBP_BWindow", woodTier, "Tier1");
        RegisterWindow(config, "BBP_BWindow", metalTier, "Tier2");
        RegisterWindow(config, "BBP_SWindow", woodTier, "Tier1");
        RegisterWindow(config, "BBP_SWindow", metalTier, "Tier2");

        SparkZCoreLogger.Info("[SparkZBase] Breach compat applied: charges restricted to BBP doors/windows only, " + config.DestroyableObjects.Count() + " target entries registered.");
    }

    protected static void RegisterDoor(HDSN_BreachingChargeConfigData config, string baseType, string wallSuffix, string doorSuffix, string tier)
    {
        config.DestroyableObjects.Insert(baseType + wallSuffix + doorSuffix, tier);
    }

    protected static void RegisterWindow(HDSN_BreachingChargeConfigData config, string baseType, array<string> suffixes, string tier)
    {
        for (int i = 0; i < suffixes.Count(); i++)
        {
            config.DestroyableObjects.Insert(baseType + suffixes.Get(i), tier);
        }
    }
}
