modded class ActionAttachCodeLockToFence
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (target && target.GetObject())
        {
            if (!SparkZBaseManagerServer.Get().CanPlaceRestrictedItemAtPosition(player, target.GetObject().GetPosition()))
                return false;
        }

        return super.ActionCondition(player, target, item);
    }
}

modded class ActionAttachCodeLockToTent
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (target && target.GetParent())
        {
            if (!SparkZBaseManagerServer.Get().CanPlaceRestrictedItemAtPosition(player, target.GetParent().GetPosition()))
                return false;
        }

        return super.ActionCondition(player, target, item);
    }
}

// Squad members with Base Access can always open a locked door on their own claimed
// base, without needing the passcode - only non-squad raiders still need the real
// code. Setting a fresh passcode (lock not yet engaged) is untouched and still goes
// through the normal CodeLock flow.
modded class ActionInteractLockOnFence
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (target && target.GetObject())
        {
            Fence fence = Fence.Cast(target.GetObject());
            if (fence)
            {
                CodeLock codelock = CodeLock.Cast(fence.GetCodeLock());
                if (codelock && codelock.GetLockState() && SparkZBaseManagerServer.Get().HasActiveBaseAccessAtPosition(player, fence.GetPosition()))
                    return true;
            }
        }

        return super.ActionCondition(player, target, item);
    }

    override void OnStartServer(ActionData action_data)
    {
        if (action_data && action_data.m_Player && action_data.m_Target && action_data.m_Target.GetObject())
        {
            Fence fence = Fence.Cast(action_data.m_Target.GetObject());
            if (fence)
            {
                CodeLock codelock = CodeLock.Cast(fence.GetCodeLock());
                if (codelock && codelock.GetLockState() && SparkZBaseManagerServer.Get().HasActiveBaseAccessAtPosition(action_data.m_Player, fence.GetPosition()))
                {
                    fence.OpenFence();
                    return;
                }
            }
        }

        super.OnStartServer(action_data);
    }
}
