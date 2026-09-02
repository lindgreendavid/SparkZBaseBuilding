class SparkZCoreRPCGuard
{
    static bool ShouldRoute(int rpcType)
    {
        return SparkZCoreRPC.IsSparkZRPC(rpcType);
    }

    static bool ValidateClientToServer(PlayerBase targetPlayer, PlayerIdentity sender, int rpcType)
    {
        if (!ShouldRoute(rpcType))
            return false;

        if (!SparkZCoreIdentityUtils.IsSenderForPlayer(targetPlayer, sender))
        {
            SparkZCoreLogger.Warn("Rejected SparkZ RPC because the sender identity did not match the target player.");
            return false;
        }

        return true;
    }
}
