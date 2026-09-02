modded class PlayerBase
{
    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        if (rpc_type == SparkZCoreRPCId.SPARKZ_RPC_CORE_EVENT_NOTIFY_SYNC)
        {
            if (!GetGame().IsServer())
                SparkZCoreEventNotificationState.Get().OnRPC(sender, this, rpc_type, ctx);

            return;
        }

        super.OnRPC(sender, rpc_type, ctx);
    }
}
