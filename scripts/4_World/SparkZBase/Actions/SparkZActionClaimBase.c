class SparkZActionClaimBase: ActionInteractBase
{
    void SparkZActionClaimBase()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_HUDCursorIcon = CursorIcons.OpenDoors;
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINone;
        m_ConditionTarget = new CCTNone;
    }

    override string GetText()
    {
        return "Claim SparkZ Base";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !target || !target.GetObject())
            return false;

        if (!IsInReach(player, target, 3.0))
            return false;

        return target.GetObject().IsInherited(TerritoryFlag);
    }

    override void OnStartServer(ActionData action_data)
    {
        if (!action_data || !action_data.m_Player || !action_data.m_Target)
            return;

        SparkZBaseManagerServer.Get().ClaimFlag(action_data.m_Player, action_data.m_Target.GetObject());
    }
}
