class SparkZBaseManagerServer
{
    static const int ABANDON_CHECK_INTERVAL_MS = 300000;

    protected static ref SparkZBaseManagerServer s_Instance;
    protected ref SparkZBaseConfig m_Config;
    protected ref array<ref SparkZBaseRecord> m_Bases;

    void SparkZBaseManagerServer()
    {
        m_Config = SparkZBaseConfig.Load();
        m_Bases = new array<ref SparkZBaseRecord>();
        LoadBases();
    }

    static SparkZBaseManagerServer Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZBaseManagerServer();

        return s_Instance;
    }

    static void Delete()
    {
        s_Instance = null;
    }

    static void TickAbandonCheck()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        SparkZBaseManagerServer.Get().CheckAbandonedBases();
    }

    bool IsEnabled()
    {
        return m_Config && m_Config.Enabled == 1;
    }

    bool CanUseBaseAtPosition(PlayerBase player, vector position, bool managerRequired = false)
    {
        if (!GetGame() || !GetGame().IsServer())
            return true;

        if (!IsEnabled() || !player || !player.GetIdentity())
            return true;

        SparkZBaseRecord record = FindBaseAtPosition(position);
        if (!record)
            return !managerRequired;

        if (record.Abandoned)
            return true;

        string playerId = player.GetIdentity().GetPlainId();
        if (SparkZGroupServerGroupManager.Get().GetGroupIdForMember(playerId) != record.OwnerGroupId)
            return false;

        if (managerRequired)
            return SparkZGroupServerGroupManager.Get().HasBaseManager(playerId);

        return SparkZGroupServerGroupManager.Get().HasBaseAccess(playerId);
    }

    bool CanBuildAtPosition(PlayerBase player, vector position)
    {
        if (!GetGame() || !GetGame().IsServer())
            return true;

        if (!IsEnabled() || !player || !player.GetIdentity())
            return true;

        SparkZBaseRecord record = FindBaseAtPosition(position);
        if (!record)
            return m_Config.RequireClaimForBBPBuild == 0;

        if (record.Abandoned)
            return true;

        string playerId = player.GetIdentity().GetPlainId();
        if (SparkZGroupServerGroupManager.Get().GetGroupIdForMember(playerId) != record.OwnerGroupId)
            return false;

        return SparkZGroupServerGroupManager.Get().HasBaseAccess(playerId);
    }

    // Strict placement check shared by CodeLock and SparkZStorage integrations: unlike
    // CanUseBaseAtPosition/CanBuildAtPosition, this denies by default (no active claimed
    // base here = no) instead of allowing free-for-all placement outside SparkZ territory.
    bool CanPlaceRestrictedItemAtPosition(PlayerBase player, vector position)
    {
        if (!GetGame() || !GetGame().IsServer())
            return true;

        if (!IsEnabled())
            return true;

        if (!player || !player.GetIdentity())
            return false;

        SparkZBaseRecord record = FindBaseAtPosition(position);
        if (!record || record.Abandoned)
            return false;

        string playerId = player.GetIdentity().GetPlainId();
        if (SparkZGroupServerGroupManager.Get().GetGroupIdForMember(playerId) != record.OwnerGroupId)
            return false;

        return SparkZGroupServerGroupManager.Get().HasBaseManager(playerId);
    }

    bool HasActiveBaseAccessAtPosition(PlayerBase player, vector position)
    {
        if (!GetGame() || !GetGame().IsServer())
            return false;

        if (!IsEnabled() || !player || !player.GetIdentity())
            return false;

        SparkZBaseRecord record = FindBaseAtPosition(position);
        if (!record || record.Abandoned)
            return false;

        string playerId = player.GetIdentity().GetPlainId();
        if (SparkZGroupServerGroupManager.Get().GetGroupIdForMember(playerId) != record.OwnerGroupId)
            return false;

        return SparkZGroupServerGroupManager.Get().HasBaseAccess(playerId);
    }

    bool ClaimFlag(PlayerBase player, Object flag)
    {
        if (!IsEnabled() || !player || !player.GetIdentity() || !flag)
            return false;

        string playerId = player.GetIdentity().GetPlainId();
        string groupId = SparkZGroupServerGroupManager.Get().GetGroupIdForMember(playerId);
        if (groupId == "")
        {
            Notify(player, "Create or join a SparkZ squad first.");
            return false;
        }

        if (!SparkZGroupServerGroupManager.Get().HasBaseManager(playerId))
        {
            Notify(player, "Only Leaders and Base Managers can claim bases.");
            return false;
        }

        vector flagPosition = flag.GetPosition();
        SparkZBaseRecord existing = FindBaseAtPosition(flagPosition);
        if (existing && !existing.Abandoned)
        {
            if (existing.OwnerGroupId == groupId)
                Notify(player, "This flagpole area is already claimed by your squad.");
            else
                Notify(player, "This flagpole area is already claimed by another squad.");

            return false;
        }

        SparkZBaseRecord otherActiveBase = FindActiveBaseForGroup(groupId);
        if (otherActiveBase && otherActiveBase != existing)
        {
            Notify(player, "Your squad already owns a base. Give it up first before claiming another.");
            return false;
        }

        int now = SparkZBaseTime.Now();
        SparkZBaseRecord record = existing;
        if (!record)
        {
            record = new SparkZBaseRecord();
            record.BaseId = "base_" + groupId + "_" + now.ToString();
            record.FlagPosition = flagPosition;
            record.RadiusMeters = m_Config.DefaultClaimRadiusMeters;
            record.CreatedAt = now;
            m_Bases.Insert(record);
        }

        record.OwnerGroupId = groupId;
        record.OwnerSquadName = "SparkZ Squad";
        record.LastPaidAt = now;
        SparkZBaseStatusPayload claimQuote = BuildQuoteForRecord(record, player);
        record.LastDailyCost = claimQuote.DailyCost;
        record.Abandoned = false;

        SaveBases();
        Notify(player, "Base claimed. The clock starts now.");
        return true;
    }

    bool PayUpkeep(PlayerBase player, Object flag, ItemBase paymentItem)
    {
        if (!IsEnabled() || !player || !player.GetIdentity() || !flag)
            return false;

        SparkZBaseRecord record = FindBaseAtPosition(flag.GetPosition());
        if (!record)
        {
            Notify(player, "Claim this flagpole area before paying upkeep.");
            return false;
        }

        if (!CanUseBaseAtPosition(player, flag.GetPosition(), true))
        {
            Notify(player, "Only Leaders and Base Managers can pay base upkeep.");
            return false;
        }

        SparkZBaseStatusPayload quote = BuildQuoteForRecord(record, player);
        if (quote.DaysOwed <= 0)
        {
            Notify(player, "Nothing owed yet. Next payment in " + FormatDuration(quote.SecondsUntilNextCharge) + ".");
            return false;
        }

        int cost = quote.AmountOwed;
        if (!paymentItem || !paymentItem.IsKindOf(m_Config.CurrencyClassName) || paymentItem.GetQuantity() < cost)
        {
            Notify(player, "Hold enough SparkZ notes in your hands. You owe " + cost.ToString() + " for " + quote.DaysOwed.ToString() + " day(s).");
            return false;
        }

        paymentItem.AddQuantity(-cost);

        record.LastPaidAt = SparkZBaseTime.Now();
        record.LastDailyCost = quote.DailyCost;
        record.Abandoned = false;
        SaveBases();
        Notify(player, "Paid off base upkeep: " + cost.ToString() + " SparkZ notes for " + quote.DaysOwed.ToString() + " day(s).");
        return true;
    }

    string BuildStatusText(Object flag)
    {
        if (!flag)
            return "No base selected.";

        SparkZBaseRecord record = FindBaseAtPosition(flag.GetPosition());
        if (!record)
            return "Base not claimed.";

        if (record.Abandoned)
            return "Base abandoned. Doors and storage are open for raiding.";

        SparkZBaseStatusPayload quote = BuildQuoteForRecord(record, null);

        if (quote.DaysOwed <= 0)
            return "Base paid up. Billed daily - next charge in " + FormatDuration(quote.SecondsUntilNextCharge) + ". Daily upkeep: " + quote.DailyCost.ToString() + " SparkZ notes.";

        return "Now it's Day " + quote.DaysOwed.ToString() + ". You owe " + quote.AmountOwed.ToString() + " SparkZ notes. Daily upkeep: " + quote.DailyCost.ToString() + " SparkZ notes.";
    }

    bool RequestBaseStatus(PlayerBase player)
    {
        SendBaseStatus(player, "");
        return true;
    }

    bool PayBaseFromMenu(PlayerBase player)
    {
        SparkZBaseRecord record = FindManagedBaseForPlayer(player);
        if (!record)
        {
            SendBaseStatus(player, "Stand inside your claimed property. Only Leaders and Base Managers can pay upkeep.");
            return false;
        }

        SparkZBaseStatusPayload quote = BuildQuoteForRecord(record, player);
        if (quote.DaysOwed <= 0)
        {
            SendBaseStatus(player, "Nothing owed yet. Next payment in " + FormatDuration(quote.SecondsUntilNextCharge) + ".");
            return false;
        }

        int cost = quote.AmountOwed;
        int notes = CountCurrency(player);
        if (notes < cost)
        {
            SendBaseStatus(player, "Not enough SparkZ notes. You owe " + cost.ToString() + " for " + quote.DaysOwed.ToString() + " day(s).");
            return false;
        }

        RemoveCurrency(player, cost);

        record.LastPaidAt = SparkZBaseTime.Now();
        record.LastDailyCost = quote.DailyCost;
        record.Abandoned = false;
        SaveBases();
        SendBaseStatus(player, "Paid off " + quote.DaysOwed.ToString() + " day(s): " + cost.ToString() + " SparkZ notes.");
        return true;
    }

    bool GiveUpBase(PlayerBase player)
    {
        if (!IsEnabled() || !player || !player.GetIdentity())
            return false;

        PlayerIdentity identity = player.GetIdentity();
        string playerId = identity.GetPlainId();
        SparkZCoreGroupPayload groupPayload = SparkZGroupServerGroupManager.Get().GetGroupForIdentity(identity);
        if (!groupPayload || groupPayload.LeaderId != playerId)
        {
            SendBaseStatus(player, "Only the squad Leader can give up the base.");
            return false;
        }

        SparkZBaseRecord record = FindActiveBaseForGroup(groupPayload.GroupId);
        if (!record)
        {
            SendBaseStatus(player, "Your squad does not have an active base to give up.");
            return false;
        }

        record.Abandoned = true;
        OpenAbandonedBase(record);
        SaveBases();
        SendBaseStatus(player, "Base given up. Doors and windows are gone and the flagpole is free to claim.");
        return true;
    }

    protected string FormatDuration(int seconds)
    {
        if (seconds < 0)
            seconds = 0;

        int days = seconds / 86400;
        int hours = (seconds % 86400) / 3600;

        if (days > 0)
            return days.ToString() + " day(s) " + hours.ToString() + " hour(s)";

        return hours.ToString() + " hour(s)";
    }

    bool OnRPC(PlayerBase player, PlayerIdentity sender, int rpcType, ParamsReadContext ctx)
    {
        if (!SparkZCoreRPCGuard.ValidateClientToServer(player, sender, rpcType))
            return true;

        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_BASE_STATUS_REQUEST)
        {
            RequestBaseStatus(player);
            return true;
        }

        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_BASE_PAY_REQUEST)
        {
            PayBaseFromMenu(player);
            return true;
        }

        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_BASE_GIVEUP_REQUEST)
        {
            GiveUpBase(player);
            return true;
        }

        return false;
    }

    void CheckAbandonedBases()
    {
        if (!IsEnabled())
            return;

        int now = SparkZBaseTime.Now();
        bool changed = false;
        for (int index = 0; index < m_Bases.Count(); index++)
        {
            SparkZBaseRecord record = m_Bases.Get(index);
            if (!record || record.Abandoned)
                continue;

            if (!FlagStillExists(record))
            {
                record.Abandoned = true;
                changed = true;
                continue;
            }

            SparkZBaseStatusPayload quote = BuildQuoteForRecord(record, null);
            if (quote.DaysOwed >= m_Config.GraceDaysBeforeAbandoned)
            {
                record.Abandoned = true;
                OpenAbandonedBase(record);
                changed = true;
            }
        }

        if (changed)
            SaveBases();
    }

    protected SparkZBaseStatusPayload BuildQuoteForRecord(SparkZBaseRecord record, PlayerBase player)
    {
        SparkZBaseStatusPayload quote = new SparkZBaseStatusPayload();
        quote.GraceDays = m_Config.GraceDaysBeforeAbandoned;
        quote.BaseCost = m_Config.BaseDailyCost;
        quote.Message = "";

        if (!record)
        {
            quote.Message = "No claimed base found here.";
            quote.DailyCost = m_Config.BaseDailyCost;
            if (player)
                quote.PlayerNotes = CountCurrency(player);

            return quote;
        }

        quote.IsValid = true;
        quote.BaseId = record.BaseId;
        quote.OwnerSquadName = record.OwnerSquadName;
        quote.RadiusMeters = record.RadiusMeters;
        quote.Abandoned = record.Abandoned;

        if (player && player.GetIdentity())
        {
            string statusPlayerId = player.GetIdentity().GetPlainId();
            SparkZCoreGroupPayload groupPayload = SparkZGroupServerGroupManager.Get().GetGroupForIdentity(player.GetIdentity());
            // Checked two ways on purpose: groupPayload.LeaderId is the group's own record of
            // who leads it, while HasMemberRoleFlag reads the member's own SQUAD_ROLE_LEADER bit.
            // These should always agree, but if they ever desync (e.g. a promotion that updated
            // one but not the other) this treats either signal as sufficient instead of silently
            // locking the real leader out of Give Up Base.
            bool isRecordedLeader = groupPayload && groupPayload.LeaderId == statusPlayerId;
            bool hasLeaderRoleFlag = SparkZGroupServerGroupManager.Get().HasMemberRoleFlag(statusPlayerId, SparkZGroupConstants.SQUAD_ROLE_LEADER);
            quote.IsLeader = isRecordedLeader || hasLeaderRoleFlag;
        }

        int now = SparkZBaseTime.Now();
        int elapsed = now - record.LastPaidAt;
        if (elapsed < 0)
            elapsed = 0;

        quote.DaysOwed = elapsed / 86400;
        quote.SecondsUntilNextCharge = 86400 - (elapsed % 86400);

        ref array<Object> objects = new array<Object>();
        GetGame().GetObjectsAtPosition(record.FlagPosition, record.RadiusMeters, objects, null);

        int bbpParts = 0;
        int doors = 0;
        int storage = 0;
        int scanned = 0;

        for (int index = 0; index < objects.Count(); index++)
        {
            if (scanned >= m_Config.MaxObjectsToScan)
                break;

            Object obj = objects.Get(index);
            if (!obj)
                continue;

            scanned++;
            BBP_BASE bbp = BBP_BASE.Cast(obj);
            if (bbp)
            {
                bbpParts++;
                if (bbp.BBP_HasDoor() || bbp.IsBBPGate() || bbp.isBBPDoor() || bbp.isWindow())
                    doors++;

                continue;
            }

            // SPKZ_MilitaryCrate is the shared base class for every SparkZStorage container
            // (crates, gun lockers, wardrobes, and anything added to the mod later), so this
            // one check covers the whole family without needing per-item updates.
            if (SPKZ_MilitaryCrate.Cast(obj))
                storage++;
        }

        quote.BBPParts = bbpParts;
        quote.BBPPartsCost = bbpParts * m_Config.BBPPartDailyCost;
        quote.Doors = doors;
        quote.DoorsCost = doors * m_Config.DoorDailyCost;
        quote.Storage = storage;
        quote.StorageCost = storage * m_Config.StorageDailyCost;
        quote.DailyCost = quote.BaseCost + quote.BBPPartsCost + quote.DoorsCost + quote.StorageCost;
        quote.AmountOwed = quote.DaysOwed * quote.DailyCost;

        if (player)
            quote.PlayerNotes = CountCurrency(player);

        return quote;
    }

    protected int CalculateDailyCost(SparkZBaseRecord record)
    {
        SparkZBaseStatusPayload quote = BuildQuoteForRecord(record, null);
        return quote.DailyCost;
    }

    protected SparkZBaseRecord FindBaseAtPosition(vector position)
    {
        for (int index = 0; index < m_Bases.Count(); index++)
        {
            SparkZBaseRecord record = m_Bases.Get(index);
            if (!record || !record.IsValid())
                continue;

            if (Distance2D(record.FlagPosition, position) <= record.RadiusMeters)
                return record;
        }

        return null;
    }

    protected bool FlagStillExists(SparkZBaseRecord record)
    {
        ref array<Object> objects = new array<Object>();
        GetGame().GetObjectsAtPosition(record.FlagPosition, 3.0, objects, null);

        for (int index = 0; index < objects.Count(); index++)
        {
            if (objects.Get(index) && objects.Get(index).IsInherited(TerritoryFlag))
                return true;
        }

        return false;
    }

    protected SparkZBaseRecord FindActiveBaseForGroup(string groupId)
    {
        if (groupId == "")
            return null;

        for (int index = 0; index < m_Bases.Count(); index++)
        {
            SparkZBaseRecord record = m_Bases.Get(index);
            if (!record || !record.IsValid() || record.Abandoned)
                continue;

            if (record.OwnerGroupId == groupId)
                return record;
        }

        return null;
    }

    protected SparkZBaseRecord FindManagedBaseForPlayer(PlayerBase player)
    {
        if (!IsEnabled() || !player || !player.GetIdentity())
            return null;

        SparkZBaseRecord record = FindBaseAtPosition(player.GetPosition());
        if (!record || record.Abandoned)
            return null;

        string playerId = player.GetIdentity().GetPlainId();
        if (SparkZGroupServerGroupManager.Get().GetGroupIdForMember(playerId) != record.OwnerGroupId)
            return null;

        if (!SparkZGroupServerGroupManager.Get().HasBaseManager(playerId))
            return null;

        return record;
    }

    protected float Distance2D(vector a, vector b)
    {
        float dx = a[0] - b[0];
        float dz = a[2] - b[2];
        return Math.Sqrt((dx * dx) + (dz * dz));
    }

    protected void SendBaseStatus(PlayerBase player, string overrideMessage)
    {
        if (!player || !player.GetIdentity())
            return;

        SparkZBaseRecord record = FindManagedBaseForPlayer(player);
        SparkZBaseStatusPayload quote = BuildQuoteForRecord(record, player);
        if (overrideMessage != "")
            quote.Message = overrideMessage;
        else if (!record)
            quote.Message = "Stand inside your claimed property. Only Leaders and Base Managers can open this menu.";

        ScriptRPC rpc = new ScriptRPC();
        quote.WriteToContext(rpc);
        rpc.Send(player, SparkZCoreRPCId.SPARKZ_RPC_BASE_STATUS_RESPONSE, true, player.GetIdentity());
    }

    protected int CountCurrency(PlayerBase player)
    {
        if (!player)
            return 0;

        int total = 0;
        array<EntityAI> items = new array<EntityAI>();
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
        for (int index = 0; index < items.Count(); index++)
        {
            ItemBase item = ItemBase.Cast(items.Get(index));
            if (item && item.IsKindOf(m_Config.CurrencyClassName))
                total += item.GetQuantity();
        }

        return total;
    }

    protected void RemoveCurrency(PlayerBase player, int amount)
    {
        if (!player || amount <= 0)
            return;

        array<EntityAI> items = new array<EntityAI>();
        player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
        for (int index = 0; index < items.Count(); index++)
        {
            if (amount <= 0)
                return;

            ItemBase item = ItemBase.Cast(items.Get(index));
            if (!item || !item.IsKindOf(m_Config.CurrencyClassName))
                continue;

            int quantity = item.GetQuantity();
            int take = Math.Min(quantity, amount);
            item.AddQuantity(-take);
            amount -= take;
        }
    }

    protected void OpenAbandonedBase(SparkZBaseRecord record)
    {
        ref array<Object> objects = new array<Object>();
        GetGame().GetObjectsAtPosition(record.FlagPosition, record.RadiusMeters, objects, null);

        for (int index = 0; index < objects.Count(); index++)
        {
            BBP_BASE bbp = BBP_BASE.Cast(objects.Get(index));
            if (!bbp)
                continue;

            if (bbp.BBP_HasDoor() || bbp.IsBBPGate() || bbp.isBBPDoor() || bbp.isWindow())
            {
                GetGame().ObjectDelete(bbp);
                continue;
            }

            if (bbp.CanOpenFence())
                bbp.OpenFence();
        }
    }

    protected void Notify(PlayerBase player, string message)
    {
        if (player)
            player.MessageStatus("[SparkZBase] " + message);
    }

    protected void LoadBases()
    {
        if (!FileExist("$profile:SparkZ"))
            MakeDirectory("$profile:SparkZ");

        if (!FileExist(SparkZBaseConstants.PROFILE_DIR))
            MakeDirectory(SparkZBaseConstants.PROFILE_DIR);

        if (!FileExist(SparkZBaseConstants.PERSISTENCE_FILE))
        {
            SaveBases();
            return;
        }

        SparkZBasePersistentStore store = new SparkZBasePersistentStore();
        JsonFileLoader<SparkZBasePersistentStore>.JsonLoadFile(SparkZBaseConstants.PERSISTENCE_FILE, store);
        if (!store || !store.Bases)
            return;

        m_Bases.Clear();
        foreach (SparkZBaseRecord record: store.Bases)
        {
            if (record && record.IsValid())
                m_Bases.Insert(record);
        }
    }

    protected void SaveBases()
    {
        SparkZBasePersistentStore store = new SparkZBasePersistentStore();
        for (int index = 0; index < m_Bases.Count(); index++)
        {
            SparkZBaseRecord record = m_Bases.Get(index);
            if (record && record.IsValid())
                store.Bases.Insert(record);
        }

        JsonFileLoader<SparkZBasePersistentStore>.JsonSaveFile(SparkZBaseConstants.PERSISTENCE_FILE, store);
    }
}
