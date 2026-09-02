class SparkZActionPayBaseUpkeep: ActionInteractBase
{
    void SparkZActionPayBaseUpkeep()
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
        return "Pay SparkZ Base Upkeep";
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!player || !target || !target.GetObject())
            return false;

        if (!IsInReach(player, target, 3.0))
            return false;

        if (!target.GetObject().IsInherited(TerritoryFlag))
            return false;

        return item && item.IsKindOf("SparkZNote");
    }

    override void OnStartServer(ActionData action_data)
    {
        if (!action_data || !action_data.m_Player || !action_data.m_Target)
            return;

        SparkZBaseManagerServer.Get().PayUpkeep(action_data.m_Player, action_data.m_Target.GetObject(), action_data.m_MainItem);
    }
}
