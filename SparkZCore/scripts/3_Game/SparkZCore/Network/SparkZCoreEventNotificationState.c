class SparkZCoreEventNotificationState
{
    protected static ref SparkZCoreEventNotificationState s_Instance;
    protected ref array<ref SparkZCoreEventNotificationPayload> m_Pending;

    void SparkZCoreEventNotificationState()
    {
        m_Pending = new array<ref SparkZCoreEventNotificationPayload>();
    }

    static SparkZCoreEventNotificationState Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZCoreEventNotificationState();

        return s_Instance;
    }

    static void Delete()
    {
        s_Instance = null;
    }

    bool OnRPC(PlayerIdentity sender, Object target, int rpcType, ParamsReadContext ctx)
    {
        if (rpcType != SparkZCoreRPCId.SPARKZ_RPC_CORE_EVENT_NOTIFY_SYNC)
            return false;

        SparkZCoreEventNotificationPayload payload = new SparkZCoreEventNotificationPayload();
        if (payload.ReadFromContext(ctx))
            m_Pending.Insert(payload);

        return true;
    }

    SparkZCoreEventNotificationPayload PopNext()
    {
        if (!m_Pending || m_Pending.Count() == 0)
            return null;

        SparkZCoreEventNotificationPayload payload = m_Pending.Get(0);
        m_Pending.Remove(0);
        return payload;
    }
}
