class SparkZBaseNetworkClient
{
    protected static ref SparkZBaseNetworkClient s_Instance;

    static SparkZBaseNetworkClient Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZBaseNetworkClient();

        return s_Instance;
    }

    static void Delete()
    {
        s_Instance = null;
    }

    void RequestStatus()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Send(player, SparkZCoreRPCId.SPARKZ_RPC_BASE_STATUS_REQUEST, true, null);
    }

    void RequestPayNow()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Send(player, SparkZCoreRPCId.SPARKZ_RPC_BASE_PAY_REQUEST, true, null);
    }

    void RequestGiveUp()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Send(player, SparkZCoreRPCId.SPARKZ_RPC_BASE_GIVEUP_REQUEST, true, null);
    }
}
