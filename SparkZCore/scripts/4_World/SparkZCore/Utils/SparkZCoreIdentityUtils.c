class SparkZCoreIdentityUtils
{
    static bool HasUsableIdentity(PlayerIdentity identity)
    {
        if (!identity)
            return false;

        if (identity.GetPlainId() == "")
            return false;

        return true;
    }

    static bool IsSenderForPlayer(PlayerBase player, PlayerIdentity sender)
    {
        if (!player)
            return false;

        if (!HasUsableIdentity(sender))
            return false;

        PlayerIdentity playerIdentity = player.GetIdentity();
        if (!HasUsableIdentity(playerIdentity))
            return false;

        return playerIdentity.GetPlainId() == sender.GetPlainId();
    }
}
