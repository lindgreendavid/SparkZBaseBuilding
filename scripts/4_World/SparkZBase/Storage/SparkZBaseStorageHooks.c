// SparkZStorage is a shared base class family: every current and future container
// (SPKZ_LargeCrate, SPKZ_SmallCrate, SPKZ_GunCase, SPKZ_LargeGunCase, SPKZ_GunLocker,
// SPKZ_TacticalWardrobe, SPKZ_MedicalCrate, ...) extends SPKZ_MilitaryCrate, and every
// deployable kit extends SPKZ_MilitaryCrateKit. Hooking just these two base classes
// covers the whole family without needing per-item updates as the mod grows.

// SPKZ_MilitaryCrateKit places itself via the vanilla ActionPlaceObject, which already
// calls PlaceEntity()/PlacingCompleteServer() - detaching the item from the player's hand -
// before it ever calls the item's own OnPlacementComplete. Gating inside OnPlacementComplete
// itself is too late: by then the kit has already left the player's hand, so rejecting there
// just orphans the item instead of returning it. Gating ActionPlaceObject.OnFinishProgressServer
// runs before any of that happens, so a rejection simply never lets the placement start and
// the kit stays in the player's hand - matching the pattern already used for BBP's
// ActionDeployObject in SparkZBaseBBPHooks.c.
modded class ActionPlaceObject
{
    override void OnFinishProgressServer(ActionData action_data)
    {
        if (action_data && action_data.m_Player && action_data.m_MainItem && action_data.m_MainItem.IsInherited(SPKZ_MilitaryCrateKit))
        {
            vector position = action_data.m_Player.GetLocalProjectionPosition();
            if (!SparkZBaseManagerServer.Get().CanPlaceRestrictedItemAtPosition(action_data.m_Player, position))
            {
                action_data.m_Player.MessageStatus("[SparkZBase] Storage can only be placed inside your claimed base, by a Base Manager.");
                return;
            }
        }

        super.OnFinishProgressServer(action_data);
    }
}

modded class ActionOpenSPKZMilitaryCrate
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        SPKZ_MilitaryCrate crate = SPKZ_MilitaryCrate.ResolveActionTarget(target);
        if (crate && !SparkZBaseManagerServer.Get().CanUseBaseAtPosition(player, crate.GetPosition()))
            return false;

        return super.ActionCondition(player, target, item);
    }
}

modded class ActionCloseSPKZMilitaryCrate
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        SPKZ_MilitaryCrate crate = SPKZ_MilitaryCrate.ResolveActionTarget(target);
        if (crate && !SparkZBaseManagerServer.Get().CanUseBaseAtPosition(player, crate.GetPosition()))
            return false;

        return super.ActionCondition(player, target, item);
    }
}

// Dismantle is different from Open/Close: the real SPKZ_MilitaryCrate.CanDismantle() (called by
// super.ActionCondition below) only ever allows the single player who originally placed the
// crate (a solo IsOwner() check baked into the mod itself) - which is why a Base Manager who
// didn't personally place a given crate couldn't dismantle it even with full base permissions.
// This bypasses that native check entirely instead of falling through to it: any squad member
// with Base Access can dismantle any of the squad's own storage, and non-squad players are
// blocked outright here - they should never be able to dismantle it by any means, only raid it
// with breaching charges.
modded class ActionDismantleSPKZMilitaryCrate
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        SPKZ_MilitaryCrate crate = SPKZ_MilitaryCrate.ResolveActionTarget(target);
        if (!crate)
            return super.ActionCondition(player, target, item);

        return SparkZBaseManagerServer.Get().CanUseBaseAtPosition(player, crate.GetPosition());
    }
}
