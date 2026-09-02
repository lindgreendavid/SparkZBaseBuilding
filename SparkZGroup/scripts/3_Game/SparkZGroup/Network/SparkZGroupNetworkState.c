class SparkZGroupNetworkState
{
    protected static ref SparkZGroupNetworkState s_Instance;

    static SparkZGroupNetworkState Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZGroupNetworkState();

        return s_Instance;
    }

    static void Delete()
    {
        s_Instance = null;
    }

    bool OnRPC(PlayerIdentity sender, Object target, int rpcType, ParamsReadContext ctx)
    {
        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_MARKER_SYNC)
        {
            HandleSquadMarkerSync(ctx);
            return true;
        }

        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_INVITE_SYNC)
        {
            HandleInviteSync(ctx);
            return true;
        }

        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_INVITE_RESULT_SYNC)
        {
            HandleInviteResultSync(ctx);
            return true;
        }

        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_EVENT_MARKER_SYNC)
        {
            HandleEventMarkerSync(ctx);
            return true;
        }

        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_EVENT_MARKER_CLEAR_SYNC)
        {
            HandleEventMarkerClearSync(ctx);
            return true;
        }

        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_PING_SYNC)
        {
            HandleQuickPingSync(ctx);
            return true;
        }

        if (rpcType != SparkZCoreRPCId.SPARKZ_RPC_GROUP_SYNC)
            return false;

        SparkZCoreGroupPayload payload = new SparkZCoreGroupPayload();
        if (!payload.ReadFromContext(ctx))
        {
            SparkZCoreLogger.Warn("SparkZ group sync payload could not be read.");
            return true;
        }

        if (!payload.IsValid())
        {
            SparkZLocalGroupSession.Get().ApplyServerLeave();
            SparkZCoreLogger.Info("SparkZ server group sync cleared local group.");
            return true;
        }

        SparkZLocalGroupSession.Get().ApplyServerSync(payload);
        SparkZGroupInviteState.Get().Clear();
        SparkZCoreLogger.Info("SparkZ server group sync applied.");
        return true;
    }

    protected void HandleSquadMarkerSync(ParamsReadContext ctx)
    {
        int count;
        if (!ctx.Read(count))
            return;

        array<ref SparkZCoreMarkerPayload> markers = new array<ref SparkZCoreMarkerPayload>();
        for (int index = 0; index < count; index++)
        {
            SparkZCoreMarkerPayload marker = new SparkZCoreMarkerPayload();
            if (marker.ReadFromContext(ctx))
                markers.Insert(marker);
        }

        SparkZGroupMarkerCache.Get().ApplyRemoteSquadMarkers(markers);
        SparkZCoreLogger.Info("SparkZ squad marker sync applied: " + markers.Count() + " markers.");
    }

    protected void HandleInviteSync(ParamsReadContext ctx)
    {
        string groupId;
        string groupName;
        string groupTag;
        string leaderName;

        if (!ctx.Read(groupId))
            return;

        if (!ctx.Read(groupName))
            return;

        if (!ctx.Read(groupTag))
            groupTag = "";

        if (!ctx.Read(leaderName))
            leaderName = "Survivor";

        if (groupId == "")
        {
            SparkZGroupInviteState.Get().Clear();
            SparkZCoreLogger.Info("SparkZ group invite cleared.");
            return;
        }

        SparkZGroupInviteState.Get().ApplyInvite(groupId, groupName, groupTag, leaderName);
        SparkZCoreLogger.Info("SparkZ group invite received.");
    }

    protected void HandleInviteResultSync(ParamsReadContext ctx)
    {
        string statusText;
        if (!ctx.Read(statusText))
            return;

        SparkZGroupInviteState.Get().SetStatusText(statusText);
        SparkZCoreLogger.Info("SparkZ group invite result received: " + statusText);
    }

    protected void HandleQuickPingSync(ParamsReadContext ctx)
    {
        SparkZCoreMarkerPayload payload = new SparkZCoreMarkerPayload();
        if (!payload.ReadFromContext(ctx))
        {
            SparkZCoreLogger.Warn("SparkZ squad quick ping payload could not be read.");
            return;
        }

        SparkZGroupMarkerCache.Get().UpsertSquadQuickPing(payload);
        SparkZCoreLogger.Info("SparkZ squad quick ping received: " + payload.Label);
    }

    protected void HandleEventMarkerSync(ParamsReadContext ctx)
    {
        SparkZCoreMarkerPayload payload = new SparkZCoreMarkerPayload();
        if (!payload.ReadFromContext(ctx))
        {
            SparkZCoreLogger.Warn("SparkZ event marker payload could not be read.");
            return;
        }

        if (!SparkZGroupMarkerCache.Get().UpsertServerEventMarker(payload))
        {
            SparkZCoreLogger.Warn("SparkZ event marker could not be applied: " + payload.Id);
            return;
        }

        SparkZCoreLogger.Info("SparkZ event marker applied: " + payload.Label);
    }

    protected void HandleEventMarkerClearSync(ParamsReadContext ctx)
    {
        string markerId;
        if (!ctx.Read(markerId))
            return;

        SparkZGroupMarkerCache.Get().RemoveServerEventMarker(markerId);
        SparkZCoreLogger.Info("SparkZ event marker cleared: " + markerId);
    }
}
