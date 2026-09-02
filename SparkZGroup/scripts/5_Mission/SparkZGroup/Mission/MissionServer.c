modded class MissionServer
{
    void MissionServer()
    {
        if (GetGame() && GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM))
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SparkZGroupServerGroupManager.TickLiveSync, SparkZGroupServerGroupManager.LIVE_SYNC_INTERVAL_MS, true);
    }

    void ~MissionServer()
    {
        if (GetGame() && GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM))
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(SparkZGroupServerGroupManager.TickLiveSync);
    }
}
