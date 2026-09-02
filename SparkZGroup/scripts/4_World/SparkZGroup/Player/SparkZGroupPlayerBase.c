modded class PlayerBase
{
    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        if (SparkZCoreRPC.IsGroupRPC(rpc_type))
        {
            if (GetGame().IsServer())
            {
                SparkZGroupServerGroupManager.Get().OnRPC(this, sender, rpc_type, ctx);
                return;
            }

            SparkZGroupNetworkState.Get().OnRPC(sender, this, rpc_type, ctx);
            return;
        }

        super.OnRPC(sender, rpc_type, ctx);
    }
}
