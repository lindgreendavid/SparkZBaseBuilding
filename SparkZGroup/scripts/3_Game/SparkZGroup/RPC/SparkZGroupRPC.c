enum SparkZGroupRPCId
{
    SPARKZ_RPC_GROUP_HELLO = 781001,
    SPARKZ_RPC_GROUP_RESERVED_MARKER_START = 781100,
    SPARKZ_RPC_GROUP_RESERVED_GROUP_START = 781300,
    SPARKZ_RPC_GROUP_RESERVED_PING_START = 781600
}

class SparkZGroupRPC
{
    static bool IsGroupRPC(int rpcId)
    {
        return SparkZCoreRPC.IsGroupRPC(rpcId);
    }
}
