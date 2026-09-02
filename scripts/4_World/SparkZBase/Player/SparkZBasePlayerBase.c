modded class PlayerBase
{
    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        if (SparkZCoreRPC.IsBaseRPC(rpc_type))
        {
            if (GetGame().IsServer())
            {
                SparkZBaseManagerServer.Get().OnRPC(this, sender, rpc_type, ctx);
                return;
            }

            SparkZBaseNetworkState.Get().OnRPC(sender, this, rpc_type, ctx);
            return;
        }

        super.OnRPC(sender, rpc_type, ctx);
    }
}
