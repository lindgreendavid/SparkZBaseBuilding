// Opening/closing a door is intentionally NOT gated by base permissions here. An unlocked
// door on a claimed base is free for anyone to use, same as vanilla - the actual access
// control comes from whether a CodeLock is attached and engaged (see
// SparkZBaseCodeLockHooks.c, which also lets Base Access holders bypass the passcode on
// their own base's locked doors). Gating plain open/close here would block everyone but
// squad members even from doors that were never locked in the first place.

modded class ActionBuildPart
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (target && target.GetObject() && target.GetObject().IsInherited(BBP_BASE))
        {
            if (!SparkZBaseManagerServer.Get().CanUseBaseAtPosition(player, target.GetObject().GetPosition()))
                return false;
        }

        return super.ActionCondition(player, target, item);
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        if (action_data && action_data.m_Target && action_data.m_Target.GetObject() && action_data.m_Target.GetObject().IsInherited(BBP_BASE))
        {
            if (!SparkZBaseManagerServer.Get().CanUseBaseAtPosition(action_data.m_Player, action_data.m_Target.GetObject().GetPosition()))
                return;
        }

        super.OnFinishProgressServer(action_data);
    }
}

modded class ActionDismantleBBP
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (target && target.GetObject() && target.GetObject().IsInherited(BBP_BASE))
        {
            if (!SparkZBaseManagerServer.Get().CanUseBaseAtPosition(player, target.GetObject().GetPosition()))
                return false;
        }

        return super.ActionCondition(player, target, item);
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        if (action_data && action_data.m_Target && action_data.m_Target.GetObject() && action_data.m_Target.GetObject().IsInherited(BBP_BASE))
        {
            if (!SparkZBaseManagerServer.Get().CanUseBaseAtPosition(action_data.m_Player, action_data.m_Target.GetObject().GetPosition()))
                return;
        }

        super.OnFinishProgressServer(action_data);
    }
}

modded class ActionDeployObject
{
    override bool ActionConditionContinue(ActionData action_data)
    {
        if (action_data && action_data.m_Player && action_data.m_MainItem && action_data.m_MainItem.IsInherited(BBP_KIT_BASE))
        {
            vector position = action_data.m_Player.GetLocalProjectionPosition();
            if (!SparkZBaseManagerServer.Get().CanBuildAtPosition(action_data.m_Player, position))
                return false;
        }

        return super.ActionConditionContinue(action_data);
    }

    override void OnFinishProgressServer(ActionData action_data)
    {
        if (action_data && action_data.m_Player && action_data.m_MainItem && action_data.m_MainItem.IsInherited(BBP_KIT_BASE))
        {
            vector position = action_data.m_Player.GetLocalProjectionPosition();
            if (!SparkZBaseManagerServer.Get().CanBuildAtPosition(action_data.m_Player, position))
            {
                action_data.m_Player.MessageStatus("[SparkZBase] You do not have base access here.");
                return;
            }
        }

        super.OnFinishProgressServer(action_data);
    }
}
