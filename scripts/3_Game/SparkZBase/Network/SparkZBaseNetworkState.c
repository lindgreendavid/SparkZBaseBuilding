class SparkZBaseNetworkState
{
    protected static ref SparkZBaseNetworkState s_Instance;
    protected ref SparkZBaseStatusPayload m_LatestStatus;
    protected int m_Revision;

    static SparkZBaseNetworkState Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZBaseNetworkState();

        return s_Instance;
    }

    static void Delete()
    {
        s_Instance = null;
    }

    bool OnRPC(PlayerIdentity sender, Object target, int rpcType, ParamsReadContext ctx)
    {
        if (rpcType != SparkZCoreRPCId.SPARKZ_RPC_BASE_STATUS_RESPONSE)
            return false;

        SparkZBaseStatusPayload payload = new SparkZBaseStatusPayload();
        if (!payload.ReadFromContext(ctx))
            return true;

        m_LatestStatus = payload;
        m_Revision++;
        return true;
    }

    SparkZBaseStatusPayload GetLatestStatus()
    {
        return m_LatestStatus;
    }

    int GetRevision()
    {
        return m_Revision;
    }
}
