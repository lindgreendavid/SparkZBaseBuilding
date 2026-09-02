class SparkZGroupNetworkClient
{
    protected static ref SparkZGroupNetworkClient s_Instance;

    static SparkZGroupNetworkClient Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZGroupNetworkClient();

        return s_Instance;
    }

    static void Delete()
    {
        s_Instance = null;
    }

    void RequestCreateGroup(string groupName, string groupTag)
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Write(groupName);
        rpc.Write(groupTag);
        rpc.Send(player, SparkZCoreRPCId.SPARKZ_RPC_GROUP_CREATE_REQUEST, true, null);
    }

    void RequestLeaveGroup()
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Send(player, SparkZCoreRPCId.SPARKZ_RPC_GROUP_LEAVE_REQUEST, true, null);
    }

    void RequestInvite(string targetName)
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Write(targetName);
        rpc.Send(player, SparkZCoreRPCId.SPARKZ_RPC_GROUP_INVITE_REQUEST, true, null);
    }

    void RequestKick(string targetName)
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Write(targetName);
        rpc.Send(player, SparkZCoreRPCId.SPARKZ_RPC_GROUP_KICK_REQUEST, true, null);
    }

    void RequestPromote(string targetName)
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Write(targetName);
        rpc.Send(player, SparkZCoreRPCId.SPARKZ_RPC_GROUP_PROMOTE_REQUEST, true, null);
    }

    void RequestPermissions(string targetToken, int roleFlags)
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Write(targetToken);
        rpc.Write(roleFlags);
        rpc.Send(player, SparkZCoreRPCId.SPARKZ_RPC_GROUP_PERMISSION_REQUEST, true, null);
    }

    void RequestMarkerUpsert(SparkZCoreMarkerPayload marker)
    {
        if (!marker)
            return;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        ScriptRPC rpc = new ScriptRPC();
        marker.WriteToContext(rpc);
        rpc.Send(player, SparkZCoreRPCId.SPARKZ_RPC_GROUP_MARKER_CREATE_REQUEST, true, null);
    }

    void RequestMarkerDelete(string markerId)
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Write(markerId);
        rpc.Send(player, SparkZCoreRPCId.SPARKZ_RPC_GROUP_MARKER_DELETE_REQUEST, true, null);
    }

    void RespondToInvite(bool accepted)
    {
        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        string groupId = SparkZGroupInviteState.Get().GetGroupId();
        if (groupId == "")
            return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Write(groupId);
        rpc.Write(accepted);
        rpc.Send(player, SparkZCoreRPCId.SPARKZ_RPC_GROUP_INVITE_RESPONSE, true, null);

        if (!accepted)
            SparkZGroupInviteState.Get().Clear();
    }

}
