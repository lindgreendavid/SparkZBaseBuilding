modded class MissionServer
{
    void MissionServer()
    {
        SparkZBaseManagerServer.Get();

        if (GetGame() && GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM))
        {
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SparkZBaseManagerServer.TickAbandonCheck, SparkZBaseManagerServer.ABANDON_CHECK_INTERVAL_MS, true);
            // SparkZBaseBreachCompat.Apply() is disabled - see SparkZBaseBreachCompat.c. Its
            // door/window class-name strings were guessed from reading the breach mod's source
            // (never verified live) and turned out wrong, which made it clear and replace the
            // breach mod's entire target list with entries that matched nothing - breaking
            // charge placement on doors AND windows entirely, worse than doing nothing. BBP's
            // own BBP_DisableDestroy=1 setting already restricts BBP's native tool-based raiding
            // to doors/gates; the explosive-charge-specific restriction needs the server's real
            // breachingcharge.json content before it can be done correctly instead of guessed.
        }
    }

    void ~MissionServer()
    {
        if (GetGame() && GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM))
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(SparkZBaseManagerServer.TickAbandonCheck);

        SparkZBaseManagerServer.Delete();
    }

    override void OnInit()
    {
        super.OnInit();
        SparkZBaseManagerServer.Get();
    }
}
