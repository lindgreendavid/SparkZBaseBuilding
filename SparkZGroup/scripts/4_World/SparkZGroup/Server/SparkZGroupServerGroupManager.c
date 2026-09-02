class SparkZGroupServerGroupManager
{
    static const int LIVE_SYNC_INTERVAL_MS = 200;
    protected static const string PERSISTENCE_DIR = "$profile:SparkZ\\Groups";
    protected static const string PERSISTENCE_FILE = "$profile:SparkZ\\Groups\\Squads.json";
    protected static ref SparkZGroupServerGroupManager s_Instance;
    protected ref map<string, ref SparkZCoreGroupPayload> m_GroupsById;
    protected ref map<string, string> m_GroupIdByMemberId;
    protected ref map<string, string> m_PendingInviteGroupIdByInviteeId;
    protected ref map<string, ref array<ref SparkZCoreMarkerPayload>> m_SquadMarkersByGroupId;
    protected int m_NextGroupId;
    protected int m_Revision;

    void SparkZGroupServerGroupManager()
    {
        m_GroupsById = new map<string, ref SparkZCoreGroupPayload>();
        m_GroupIdByMemberId = new map<string, string>();
        m_PendingInviteGroupIdByInviteeId = new map<string, string>();
        m_SquadMarkersByGroupId = new map<string, ref array<ref SparkZCoreMarkerPayload>>();
        m_NextGroupId = 1;
        m_Revision = 1;
        LoadPersistentGroups();
    }

    static SparkZGroupServerGroupManager Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZGroupServerGroupManager();

        return s_Instance;
    }

    static void Delete()
    {
        s_Instance = null;
    }

    SparkZCoreGroupPayload GetGroupForIdentity(PlayerIdentity identity)
    {
        if (!identity)
            return null;

        return FindGroupForMember(identity.GetPlainId());
    }

    string GetGroupTagForIdentity(PlayerIdentity identity)
    {
        SparkZCoreGroupPayload payload = GetGroupForIdentity(identity);
        if (!payload || payload.Tag == "")
            return "";

        return payload.Tag;
    }

    array<PlayerIdentity> GetOnlineGroupIdentities(PlayerIdentity identity)
    {
        array<PlayerIdentity> identities = new array<PlayerIdentity>();
        SparkZCoreGroupPayload payload = GetGroupForIdentity(identity);
        if (!payload)
            return identities;

        for (int index = 0; index < payload.Members.Count(); index++)
        {
            SparkZCoreGroupMemberPayload member = payload.Members.Get(index);
            if (!member)
                continue;

            PlayerIdentity memberIdentity = FindOnlineIdentityByPlainId(member.PlainId);
            if (memberIdentity)
                identities.Insert(memberIdentity);
        }

        return identities;
    }

    bool OnRPC(PlayerBase targetPlayer, PlayerIdentity sender, int rpcType, ParamsReadContext ctx)
    {
        if (!IsHandledRPC(rpcType))
            return false;

        if (!SparkZCoreRPCGuard.ValidateClientToServer(targetPlayer, sender, rpcType))
            return true;

        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_CREATE_REQUEST)
        {
            HandleCreateRequest(sender, ctx);
            return true;
        }

        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_LEAVE_REQUEST)
        {
            HandleLeaveRequest(sender);
            return true;
        }

        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_INVITE_REQUEST)
        {
            HandleInviteRequest(sender, ctx);
            return true;
        }

        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_INVITE_RESPONSE)
        {
            HandleInviteResponse(sender, ctx);
            return true;
        }

        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_PING_CREATE_REQUEST)
        {
            HandleQuickPingRequest(sender, ctx);
            return true;
        }

        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_KICK_REQUEST)
        {
            HandleKickRequest(sender, ctx);
            return true;
        }

        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_PROMOTE_REQUEST)
        {
            HandlePromoteRequest(sender, ctx);
            return true;
        }

        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_PERMISSION_REQUEST)
        {
            HandlePermissionRequest(sender, ctx);
            return true;
        }

        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_MARKER_CREATE_REQUEST)
        {
            HandleMarkerUpsertRequest(sender, ctx);
            return true;
        }

        if (rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_MARKER_DELETE_REQUEST)
        {
            HandleMarkerDeleteRequest(sender, ctx);
            return true;
        }

        return false;
    }

    static void TickLiveSync()
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        SparkZGroupServerGroupManager.Get().RefreshLiveGroupStates();
    }

    protected bool IsHandledRPC(int rpcType)
    {
        return rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_CREATE_REQUEST || rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_LEAVE_REQUEST || rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_INVITE_REQUEST || rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_INVITE_RESPONSE || rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_PING_CREATE_REQUEST || rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_KICK_REQUEST || rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_PROMOTE_REQUEST || rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_PERMISSION_REQUEST || rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_MARKER_CREATE_REQUEST || rpcType == SparkZCoreRPCId.SPARKZ_RPC_GROUP_MARKER_DELETE_REQUEST;
    }

    protected void HandleCreateRequest(PlayerIdentity sender, ParamsReadContext ctx)
    {
        if (!sender)
            return;

        string groupName;
        string groupTag;

        if (!ctx.Read(groupName))
            groupName = "";

        if (!ctx.Read(groupTag))
            groupTag = "";

        SparkZCoreGroupPayload existingPayload = FindGroupForMember(sender.GetPlainId());
        if (existingPayload)
        {
            BroadcastGroupSync(existingPayload);
            SparkZCoreLogger.Info("SparkZ server ignored squad rename request because the sender is already in a squad.");
            return;
        }

        SparkZCoreGroupPayload payload = GetOrCreateSenderGroup(sender, groupName, groupTag);
        if (!payload)
            return;

        SavePersistentGroups();
        BroadcastGroupSync(payload);
        SparkZCoreLogger.Info("SparkZ server created squad " + payload.Name + " for " + sender.GetName() + ".");
    }

    protected void HandleLeaveRequest(PlayerIdentity sender)
    {
        if (!sender)
            return;

        string memberId = sender.GetPlainId();
        if (memberId == "")
            return;

        SparkZCoreGroupPayload payload = FindGroupForMember(memberId);
        if (!payload)
        {
            SendGroupClear(sender);
            return;
        }

        bool wasLeader = payload.LeaderId == memberId;
        RemoveMember(payload, memberId);
        m_GroupIdByMemberId.Remove(memberId);
        RemoveSquadMarkersForMember(payload.GroupId, memberId);

        if (payload.Members.Count() == 0 || wasLeader)
        {
            ClearGroup(payload);
            SavePersistentGroups();
            SparkZCoreLogger.Info("SparkZ server dissolved group after leader leave.");
            return;
        }

        BroadcastSquadMarkerSync(payload);
        payload.Revision = NextRevision();
        SavePersistentGroups();
        BroadcastGroupSync(payload);
        SendGroupClear(sender);
        SparkZCoreLogger.Info("SparkZ server removed " + sender.GetName() + " from group.");
    }

    protected void HandleInviteRequest(PlayerIdentity sender, ParamsReadContext ctx)
    {
        if (!sender)
            return;

        string targetName;
        if (!ctx.Read(targetName))
        {
            SendInviteResult(sender, "INVITE FAILED: NO PLAYER NAME.");
            return;
        }

        PlayerIdentity targetIdentity = FindOnlineIdentityByName(targetName);
        if (!targetIdentity)
        {
            SparkZCoreLogger.Warn("SparkZ group invite target not found: " + targetName + ".");
            SendInviteResult(sender, "PLAYER NOT FOUND: " + targetName);
            return;
        }

        string senderId = sender.GetPlainId();
        string targetId = targetIdentity.GetPlainId();
        if (senderId == "" || targetId == "" || senderId == targetId)
        {
            SendInviteResult(sender, "INVITE FAILED: INVALID TARGET.");
            return;
        }

        SparkZCoreGroupPayload payload = FindGroupForMember(senderId);
        if (!payload)
        {
            SendInviteResult(sender, "CREATE SQUAD FIRST.");
            return;
        }

        if (payload.LeaderId != senderId)
        {
            SparkZCoreLogger.Warn("SparkZ group invite rejected because sender is not the group leader.");
            SendInviteResult(sender, "ONLY GROUP LEADER CAN INVITE.");
            return;
        }

        m_PendingInviteGroupIdByInviteeId.Set(targetId, payload.GroupId);
        SendInviteSync(targetIdentity, payload, sender.GetName());
        SendInviteResult(sender, "INVITE SENT TO: " + targetIdentity.GetName());
        BroadcastGroupSync(payload);
        SparkZCoreLogger.Info("SparkZ server sent group invite from " + sender.GetName() + " to " + targetIdentity.GetName() + ".");
    }

    protected void HandleInviteResponse(PlayerIdentity sender, ParamsReadContext ctx)
    {
        if (!sender)
            return;

        string groupId;
        bool accepted;

        if (!ctx.Read(groupId))
            return;

        if (!ctx.Read(accepted))
            accepted = false;

        string memberId = sender.GetPlainId();
        if (memberId == "" || groupId == "")
            return;

        if (!m_PendingInviteGroupIdByInviteeId.Contains(memberId))
            return;

        string pendingGroupId = m_PendingInviteGroupIdByInviteeId.Get(memberId);
        m_PendingInviteGroupIdByInviteeId.Remove(memberId);

        if (pendingGroupId != groupId || !accepted)
        {
            SendInviteClear(sender);
            return;
        }

        SparkZCoreGroupPayload payload = m_GroupsById.Get(groupId);
        if (!payload)
            return;

        SparkZCoreGroupPayload existingPayload = FindGroupForMember(memberId);
        if (existingPayload && existingPayload.GroupId != payload.GroupId)
        {
            RemoveMember(existingPayload, memberId);
            m_GroupIdByMemberId.Remove(memberId);

            if (existingPayload.Members.Count() == 0 || existingPayload.LeaderId == memberId)
                ClearGroup(existingPayload);
            else
            {
                existingPayload.Revision = NextRevision();
                BroadcastGroupSync(existingPayload);
            }
        }

        AddMember(payload, memberId, sender.GetName(), SparkZGroupConstants.SQUAD_PERMISSION_BASE_ALL, true);
        m_GroupIdByMemberId.Set(memberId, payload.GroupId);
        payload.Revision = NextRevision();

        SavePersistentGroups();
        BroadcastGroupSync(payload);
        BroadcastSquadMarkerSync(payload);
        SparkZCoreLogger.Info("SparkZ server accepted group invite for " + sender.GetName() + ".");
    }

    protected SparkZCoreGroupPayload GetOrCreateSenderGroup(PlayerIdentity sender, string groupName, string groupTag)
    {
        string senderId = sender.GetPlainId();
        if (senderId == "")
            return null;

        SparkZCoreGroupPayload existingPayload = FindGroupForMember(senderId);
        if (existingPayload)
            return existingPayload;

        string leaderName = sender.GetName();
        if (leaderName == "")
            leaderName = "Survivor";

        string groupId = CreateNextGroupId();

        SparkZCoreGroupPayload payload = new SparkZCoreGroupPayload();
        payload.Set(groupId, SanitizeGroupName(groupName), SanitizeGroupTag(groupTag), senderId, NextRevision());
        AddMember(payload, senderId, leaderName, SparkZGroupConstants.SQUAD_ROLE_LEADER | SparkZGroupConstants.SQUAD_PERMISSION_BASE_ALL, true);

        m_GroupsById.Set(groupId, payload);
        m_GroupIdByMemberId.Set(senderId, groupId);
        return payload;
    }

    protected void HandleQuickPingRequest(PlayerIdentity sender, ParamsReadContext ctx)
    {
        if (!sender)
            return;

        vector position;
        int colorARGB;
        string iconPath;

        if (!ctx.Read(position))
            return;

        if (!ctx.Read(colorARGB))
            colorARGB = ARGB(255, 255, 216, 64);

        if (!ctx.Read(iconPath))
            iconPath = "SparkZGroup\\gui\\icons\\markers\\ping.paa";

        string senderId = sender.GetPlainId();
        SparkZCoreGroupPayload payload = FindGroupForMember(senderId);
        if (!payload)
            return;

        string label = sender.GetName();
        if (label == "")
            label = "Squad Ping";

        SparkZCoreMarkerPayload markerPayload = new SparkZCoreMarkerPayload();
        markerPayload.Set("quick_ping_" + senderId, label, position, colorARGB, iconPath, 1, senderId, payload.GroupId, NextRevision(), SparkZGroupConstants.QUICK_PING_TTL_SECONDS, true);
        BroadcastQuickPing(payload, markerPayload);
    }

    protected void HandleKickRequest(PlayerIdentity sender, ParamsReadContext ctx)
    {
        if (!sender)
            return;

        string targetToken;
        if (!ctx.Read(targetToken))
            return;

        string senderId = sender.GetPlainId();
        SparkZCoreGroupPayload payload = FindGroupForMember(senderId);
        if (!CanLeaderManageMember(sender, payload, targetToken))
            return;

        SparkZCoreGroupMemberPayload targetMember = FindMemberByNameOrPlainId(payload, targetToken);
        if (!targetMember || targetMember.PlainId == senderId)
            return;

        PlayerIdentity targetIdentity = FindOnlineIdentityByPlainId(targetMember.PlainId);
        RemoveMember(payload, targetMember.PlainId);
        m_GroupIdByMemberId.Remove(targetMember.PlainId);
        RemoveSquadMarkersForMember(payload.GroupId, targetMember.PlainId);
        BroadcastSquadMarkerSync(payload);
        payload.Revision = NextRevision();
        SavePersistentGroups();

        if (targetIdentity)
            SendGroupClear(targetIdentity);

        BroadcastGroupSync(payload);
        SendInviteResult(sender, "KICKED: " + targetMember.Name);
        SparkZCoreLogger.Info("SparkZ squad member kicked: " + targetMember.Name + ".");
    }

    protected void HandlePromoteRequest(PlayerIdentity sender, ParamsReadContext ctx)
    {
        if (!sender)
            return;

        string targetToken;
        if (!ctx.Read(targetToken))
            return;

        string senderId = sender.GetPlainId();
        SparkZCoreGroupPayload payload = FindGroupForMember(senderId);
        if (!CanLeaderManageMember(sender, payload, targetToken))
            return;

        SparkZCoreGroupMemberPayload targetMember = FindMemberByNameOrPlainId(payload, targetToken);
        if (!targetMember)
            return;

        targetMember.RoleFlags = SetRoleFlag(targetMember.RoleFlags, SparkZGroupConstants.SQUAD_ROLE_LEADER, true);

        payload.LeaderId = targetMember.PlainId;
        payload.Revision = NextRevision();
        SavePersistentGroups();
        BroadcastGroupSync(payload);
        SendInviteResult(sender, "PROMOTED: " + targetMember.Name);
        SparkZCoreLogger.Info("SparkZ squad leader promoted: " + targetMember.Name + ".");
    }

    protected void HandlePermissionRequest(PlayerIdentity sender, ParamsReadContext ctx)
    {
        if (!sender)
            return;

        string targetToken;
        int requestedFlags;

        if (!ctx.Read(targetToken))
            return;

        if (!ctx.Read(requestedFlags))
            requestedFlags = 0;

        string senderId = sender.GetPlainId();
        SparkZCoreGroupPayload payload = FindGroupForMember(senderId);
        if (!CanLeaderManageMember(sender, payload, targetToken))
            return;

        SparkZCoreGroupMemberPayload targetMember = FindMemberByNameOrPlainId(payload, targetToken);
        if (!targetMember)
            return;

        targetMember.RoleFlags = NormalizeBaseRoleFlags(requestedFlags);

        payload.Revision = NextRevision();
        SavePersistentGroups();
        BroadcastGroupSync(payload);
        SendInviteResult(sender, "PERMISSIONS SAVED: " + targetMember.Name);
        SparkZCoreLogger.Info("SparkZ squad permissions saved for " + targetMember.Name + ".");
    }

    protected void HandleMarkerUpsertRequest(PlayerIdentity sender, ParamsReadContext ctx)
    {
        if (!sender)
            return;

        SparkZCoreMarkerPayload marker = new SparkZCoreMarkerPayload();
        if (!marker.ReadFromContext(ctx))
            return;

        string senderId = sender.GetPlainId();
        SparkZCoreGroupPayload payload = FindGroupForMember(senderId);
        if (!payload)
            return;

        marker.OwnerId = senderId;
        marker.GroupId = payload.GroupId;
        marker.Channel = SparkZGroupMarkerCache.MARKER_CHANNEL_GROUP;

        array<ref SparkZCoreMarkerPayload> markers = GetOrCreateSquadMarkerList(payload.GroupId);
        UpsertSquadMarker(markers, marker);
        BroadcastSquadMarkerSync(payload);
        SavePersistentGroups();
    }

    protected void HandleMarkerDeleteRequest(PlayerIdentity sender, ParamsReadContext ctx)
    {
        if (!sender)
            return;

        string markerId;
        if (!ctx.Read(markerId))
            return;

        string senderId = sender.GetPlainId();
        SparkZCoreGroupPayload payload = FindGroupForMember(senderId);
        if (!payload)
            return;

        array<ref SparkZCoreMarkerPayload> markers = GetOrCreateSquadMarkerList(payload.GroupId);
        for (int index = markers.Count() - 1; index >= 0; index--)
        {
            SparkZCoreMarkerPayload marker = markers.Get(index);
            if (marker && marker.Id == markerId && marker.OwnerId == senderId)
                markers.Remove(index);
        }

        BroadcastSquadMarkerSync(payload);
        SavePersistentGroups();
    }

    protected array<ref SparkZCoreMarkerPayload> GetOrCreateSquadMarkerList(string groupId)
    {
        if (!m_SquadMarkersByGroupId.Contains(groupId))
            m_SquadMarkersByGroupId.Set(groupId, new array<ref SparkZCoreMarkerPayload>());

        return m_SquadMarkersByGroupId.Get(groupId);
    }

    protected void UpsertSquadMarker(array<ref SparkZCoreMarkerPayload> markers, SparkZCoreMarkerPayload marker)
    {
        for (int index = 0; index < markers.Count(); index++)
        {
            SparkZCoreMarkerPayload existing = markers.Get(index);
            if (existing && existing.Id == marker.Id && existing.OwnerId == marker.OwnerId)
            {
                markers.Set(index, marker);
                return;
            }
        }

        markers.Insert(marker);
    }

    protected void RemoveSquadMarkersForMember(string groupId, string plainId)
    {
        if (!m_SquadMarkersByGroupId.Contains(groupId))
            return;

        array<ref SparkZCoreMarkerPayload> markers = m_SquadMarkersByGroupId.Get(groupId);
        for (int index = markers.Count() - 1; index >= 0; index--)
        {
            SparkZCoreMarkerPayload marker = markers.Get(index);
            if (marker && marker.OwnerId == plainId)
                markers.Remove(index);
        }
    }

    protected void BroadcastSquadMarkerSync(SparkZCoreGroupPayload payload)
    {
        if (!payload)
            return;

        array<ref SparkZCoreMarkerPayload> markers = GetOrCreateSquadMarkerList(payload.GroupId);

        for (int index = 0; index < payload.Members.Count(); index++)
        {
            SparkZCoreGroupMemberPayload member = payload.Members.Get(index);
            if (!member)
                continue;

            PlayerIdentity identity = FindOnlineIdentityByPlainId(member.PlainId);
            if (!identity)
                continue;

            PlayerBase targetPlayer = FindOnlinePlayerByPlainId(member.PlainId);
            if (!targetPlayer)
                continue;

            ScriptRPC rpc = new ScriptRPC();
            rpc.Write(markers.Count());
            for (int markerIndex = 0; markerIndex < markers.Count(); markerIndex++)
            {
                markers.Get(markerIndex).WriteToContext(rpc);
            }

            rpc.Send(targetPlayer, SparkZCoreRPCId.SPARKZ_RPC_GROUP_MARKER_SYNC, true, identity);
        }
    }

    protected SparkZCoreGroupPayload FindGroupForMember(string plainId)
    {
        if (!m_GroupIdByMemberId.Contains(plainId))
            return RecoverGroupForMember(plainId);

        string groupId = m_GroupIdByMemberId.Get(plainId);
        return m_GroupsById.Get(groupId);
    }

    string GetGroupIdForMember(string plainId)
    {
        SparkZCoreGroupPayload payload = FindGroupForMember(plainId);
        if (!payload)
            return "";

        return payload.GroupId;
    }

    int GetMemberRoleFlags(string plainId)
    {
        SparkZCoreGroupPayload payload = FindGroupForMember(plainId);
        SparkZCoreGroupMemberPayload member = FindMemberByNameOrPlainId(payload, plainId);
        if (!member)
            return 0;

        return NormalizeBaseRoleFlags(member.RoleFlags);
    }

    bool HasMemberRoleFlag(string plainId, int flag)
    {
        return (GetMemberRoleFlags(plainId) & flag) == flag;
    }

    bool HasBaseAccess(string plainId)
    {
        int flags = GetMemberRoleFlags(plainId);
        if ((flags & SparkZGroupConstants.SQUAD_PERMISSION_BASE_ACCESS) == SparkZGroupConstants.SQUAD_PERMISSION_BASE_ACCESS)
            return true;

        if ((flags & SparkZGroupConstants.SQUAD_PERMISSION_BASE_MANAGER) == SparkZGroupConstants.SQUAD_PERMISSION_BASE_MANAGER)
            return true;

        return (flags & SparkZGroupConstants.SQUAD_ROLE_LEADER) == SparkZGroupConstants.SQUAD_ROLE_LEADER;
    }

    bool HasBaseManager(string plainId)
    {
        int flags = GetMemberRoleFlags(plainId);
        if ((flags & SparkZGroupConstants.SQUAD_PERMISSION_BASE_MANAGER) == SparkZGroupConstants.SQUAD_PERMISSION_BASE_MANAGER)
            return true;

        return (flags & SparkZGroupConstants.SQUAD_ROLE_LEADER) == SparkZGroupConstants.SQUAD_ROLE_LEADER;
    }

    bool IsSameGroup(string plainIdA, string plainIdB)
    {
        string groupA = GetGroupIdForMember(plainIdA);
        if (groupA == "")
            return false;

        return groupA == GetGroupIdForMember(plainIdB);
    }

    int NormalizeBaseRoleFlags(int roleFlags)
    {
        int normalized = roleFlags & SparkZGroupConstants.SQUAD_PERMISSION_MANAGEABLE_ALL;
        if ((roleFlags & SparkZGroupConstants.SQUAD_PERMISSION_BASE_LEGACY_ALL) != 0)
            normalized = normalized | SparkZGroupConstants.SQUAD_PERMISSION_BASE_ACCESS;

        if ((normalized & SparkZGroupConstants.SQUAD_PERMISSION_BASE_MANAGER) == SparkZGroupConstants.SQUAD_PERMISSION_BASE_MANAGER)
            normalized = normalized | SparkZGroupConstants.SQUAD_PERMISSION_BASE_ACCESS;

        normalized = normalized & ~SparkZGroupConstants.SQUAD_PERMISSION_BASE_DOORS;
        normalized = normalized & ~SparkZGroupConstants.SQUAD_PERMISSION_BASE_STORAGE;
        return normalized;
    }

    protected SparkZCoreGroupPayload RecoverGroupForMember(string plainId)
    {
        if (plainId == "")
            return null;

        foreach (string groupId, SparkZCoreGroupPayload payload: m_GroupsById)
        {
            if (!payload || !payload.Members)
                continue;

            for (int index = 0; index < payload.Members.Count(); index++)
            {
                SparkZCoreGroupMemberPayload member = payload.Members.Get(index);
                if (member && member.PlainId == plainId)
                {
                    m_GroupIdByMemberId.Set(plainId, groupId);
                    return payload;
                }
            }
        }

        return null;
    }

    protected void AddMember(SparkZCoreGroupPayload payload, string plainId, string name, int roleFlags, bool online)
    {
        if (!payload || plainId == "")
            return;

        for (int index = 0; index < payload.Members.Count(); index++)
        {
            SparkZCoreGroupMemberPayload existingMember = payload.Members.Get(index);
            if (existingMember && existingMember.PlainId == plainId)
            {
                existingMember.Name = name;
                existingMember.RoleFlags = roleFlags;
                existingMember.Online = online;
                return;
            }
        }

        SparkZCoreGroupMemberPayload member = new SparkZCoreGroupMemberPayload();
        member.Set(plainId, name, roleFlags, online);
        payload.Members.Insert(member);
    }

    protected void RefreshLiveGroupStates()
    {
        foreach (string groupId, SparkZCoreGroupPayload payload: m_GroupsById)
        {
            if (!payload)
                continue;

            RefreshGroupLiveState(payload);
            payload.Revision = NextRevision();
            BroadcastGroupSync(payload);
        }
    }

    protected void RefreshGroupLiveState(SparkZCoreGroupPayload payload)
    {
        if (!payload)
            return;

        for (int index = 0; index < payload.Members.Count(); index++)
        {
            SparkZCoreGroupMemberPayload member = payload.Members.Get(index);
            if (!member)
                continue;

            PlayerBase player = FindOnlinePlayerByPlainId(member.PlainId);
            if (!player)
            {
                member.Online = false;
                member.SetLiveStateEx("0 0 0", 0.0, 1.95, false);
                continue;
            }

            member.Online = true;
            member.SetLiveStateEx(player.GetPosition(), GetPlayerHealth01(player), GetPlayerDisplayHeightOffset(player), true);

            PlayerIdentity identity = player.GetIdentity();
            if (identity && identity.GetName() != "")
                member.Name = identity.GetName();
        }
    }

    protected float GetPlayerHealth01(PlayerBase player)
    {
        if (!player)
            return 0.0;

        float health = player.GetHealth("", "Health");
        float health01 = health / 100.0;

        if (health01 < 0.0)
            return 0.0;

        if (health01 > 1.0)
            return 1.0;

        return health01;
    }

    protected float GetPlayerDisplayHeightOffset(PlayerBase player)
    {
        if (!player)
            return 1.95;

        if (player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE))
            return 0.62;

        if (player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_CROUCH))
            return 1.25;

        return 1.95;
    }

    protected void RemoveMember(SparkZCoreGroupPayload payload, string plainId)
    {
        if (!payload)
            return;

        for (int index = payload.Members.Count() - 1; index >= 0; index--)
        {
            SparkZCoreGroupMemberPayload member = payload.Members.Get(index);
            if (member && member.PlainId == plainId)
                payload.Members.Remove(index);
        }
    }

    protected void ClearGroup(SparkZCoreGroupPayload payload)
    {
        if (!payload)
            return;

        for (int index = 0; index < payload.Members.Count(); index++)
        {
            SparkZCoreGroupMemberPayload member = payload.Members.Get(index);
            if (!member)
                continue;

            m_GroupIdByMemberId.Remove(member.PlainId);

            PlayerIdentity identity = FindOnlineIdentityByPlainId(member.PlainId);
            if (identity)
                SendGroupClear(identity);
        }

        m_GroupsById.Remove(payload.GroupId);
        m_SquadMarkersByGroupId.Remove(payload.GroupId);
    }

    protected string CreateNextGroupId()
    {
        EnsureNextGroupIdFree();
        string groupId = "sparkz_group_" + m_NextGroupId;
        m_NextGroupId++;
        return groupId;
    }

    protected void EnsureNextGroupIdFree()
    {
        while (m_GroupsById.Contains("sparkz_group_" + m_NextGroupId))
        {
            m_NextGroupId++;
        }
    }

    protected void EnsurePersistenceDirectory()
    {
        if (!FileExist("$profile:SparkZ"))
            MakeDirectory("$profile:SparkZ");

        if (!FileExist(PERSISTENCE_DIR))
            MakeDirectory(PERSISTENCE_DIR);
    }

    protected void LoadPersistentGroups()
    {
        EnsurePersistenceDirectory();
        if (!FileExist(PERSISTENCE_FILE))
        {
            SparkZCoreLogger.Info("SparkZ squad persistence file not found. Starting with empty server squad store.");
            return;
        }

        SparkZGroupPersistentStore store = new SparkZGroupPersistentStore();
        JsonFileLoader<SparkZGroupPersistentStore>.JsonLoadFile(PERSISTENCE_FILE, store);
        if (!store || !store.Groups)
        {
            SparkZCoreLogger.Warn("SparkZ squad persistence file could not be loaded.");
            return;
        }

        m_GroupsById.Clear();
        m_GroupIdByMemberId.Clear();

        int loadedCount = 0;
        for (int groupIndex = 0; groupIndex < store.Groups.Count(); groupIndex++)
        {
            SparkZGroupPersistentRecord record = store.Groups.Get(groupIndex);
            SparkZCoreGroupPayload payload = BuildPayloadFromPersistentRecord(record);
            if (!payload)
                continue;

            m_GroupsById.Set(payload.GroupId, payload);
            RegisterGroupMembers(payload);
            loadedCount++;

            if (payload.Revision >= m_Revision)
                m_Revision = payload.Revision + 1;
        }

        if (m_NextGroupId <= loadedCount)
            m_NextGroupId = loadedCount + 1;

        EnsureNextGroupIdFree();
        SparkZCoreLogger.Info("SparkZ loaded " + loadedCount + " persisted squads.");
    }

    protected void SavePersistentGroups()
    {
        EnsurePersistenceDirectory();
        SparkZGroupPersistentStore store = new SparkZGroupPersistentStore();

        foreach (string groupId, SparkZCoreGroupPayload payload: m_GroupsById)
        {
            SparkZGroupPersistentRecord record = BuildPersistentRecord(payload);
            if (record && record.IsValid())
                store.Groups.Insert(record);
        }

        JsonFileLoader<SparkZGroupPersistentStore>.JsonSaveFile(PERSISTENCE_FILE, store);
        SparkZCoreLogger.Info("SparkZ saved " + store.Groups.Count() + " persisted squads.");
    }

    protected SparkZCoreGroupPayload BuildPayloadFromPersistentRecord(SparkZGroupPersistentRecord record)
    {
        if (!record || !record.IsValid())
            return null;

        SparkZCoreGroupPayload payload = new SparkZCoreGroupPayload();
        payload.Set(record.GroupId, SanitizeGroupName(record.Name), SanitizeGroupTag(record.Tag), record.LeaderId, record.Revision);

        for (int memberIndex = 0; memberIndex < record.Members.Count(); memberIndex++)
        {
            SparkZGroupPersistentMember persistentMember = record.Members.Get(memberIndex);
            if (!persistentMember || !persistentMember.IsValid())
                continue;

            SparkZCoreGroupMemberPayload member = new SparkZCoreGroupMemberPayload();
            member.Set(persistentMember.PlainId, persistentMember.Name, persistentMember.RoleFlags, false);
            member.SetLiveState("0 0 0", 0.0, false);
            payload.Members.Insert(member);
        }

        if (payload.Members.Count() == 0)
            return null;

        array<ref SparkZCoreMarkerPayload> restoredMarkers = new array<ref SparkZCoreMarkerPayload>();
        for (int markerIndex = 0; markerIndex < record.Markers.Count(); markerIndex++)
        {
            SparkZCoreMarkerPayload marker = record.Markers.Get(markerIndex);
            if (marker && marker.IsValid())
                restoredMarkers.Insert(marker);
        }
        m_SquadMarkersByGroupId.Set(payload.GroupId, restoredMarkers);

        return payload;
    }

    protected SparkZGroupPersistentRecord BuildPersistentRecord(SparkZCoreGroupPayload payload)
    {
        if (!payload || !payload.IsValid() || !payload.Members)
            return null;

        SparkZGroupPersistentRecord record = new SparkZGroupPersistentRecord();
        record.GroupId = payload.GroupId;
        record.Name = payload.Name;
        record.Tag = payload.Tag;
        record.LeaderId = payload.LeaderId;
        record.Revision = payload.Revision;

        for (int memberIndex = 0; memberIndex < payload.Members.Count(); memberIndex++)
        {
            SparkZCoreGroupMemberPayload member = payload.Members.Get(memberIndex);
            if (!member || !member.IsValid())
                continue;

            SparkZGroupPersistentMember persistentMember = new SparkZGroupPersistentMember();
            persistentMember.Set(member.PlainId, member.Name, member.RoleFlags);
            record.Members.Insert(persistentMember);
        }

        array<ref SparkZCoreMarkerPayload> markers = GetOrCreateSquadMarkerList(payload.GroupId);
        for (int markerIndex = 0; markerIndex < markers.Count(); markerIndex++)
        {
            SparkZCoreMarkerPayload marker = markers.Get(markerIndex);
            if (marker && marker.IsValid())
                record.Markers.Insert(marker);
        }

        return record;
    }

    protected void RegisterGroupMembers(SparkZCoreGroupPayload payload)
    {
        if (!payload || !payload.Members)
            return;

        for (int memberIndex = 0; memberIndex < payload.Members.Count(); memberIndex++)
        {
            SparkZCoreGroupMemberPayload member = payload.Members.Get(memberIndex);
            if (member && member.PlainId != "")
                m_GroupIdByMemberId.Set(member.PlainId, payload.GroupId);
        }
    }

    protected bool CanLeaderManageMember(PlayerIdentity sender, SparkZCoreGroupPayload payload, string targetToken)
    {
        if (!sender || !payload)
            return false;

        if (payload.LeaderId != sender.GetPlainId())
        {
            SendInviteResult(sender, "ONLY SQUAD LEADER CAN MANAGE.");
            return false;
        }

        if (targetToken == "")
        {
            SendInviteResult(sender, "SELECT A MEMBER FIRST.");
            return false;
        }

        if (!FindMemberByNameOrPlainId(payload, targetToken))
        {
            SendInviteResult(sender, "SQUAD MEMBER NOT FOUND: " + targetToken);
            return false;
        }

        return true;
    }

    protected SparkZCoreGroupMemberPayload FindMemberByNameOrPlainId(SparkZCoreGroupPayload payload, string playerToken)
    {
        if (!payload || !payload.Members || playerToken == "")
            return null;

        for (int index = 0; index < payload.Members.Count(); index++)
        {
            SparkZCoreGroupMemberPayload member = payload.Members.Get(index);
            if (member && (member.Name == playerToken || member.PlainId == playerToken))
                return member;
        }

        return null;
    }

    protected int SetRoleFlag(int roleFlags, int flag, bool enabled)
    {
        if (enabled)
            return roleFlags | flag;

        return roleFlags & ~flag;
    }

    protected int NextRevision()
    {
        int revision = m_Revision;
        m_Revision++;
        return revision;
    }

    protected string SanitizeGroupName(string groupName)
    {
        if (groupName == "")
            return "SparkZ Squad";

        return groupName;
    }

    protected string SanitizeGroupTag(string groupTag)
    {
        if (groupTag == "")
            return "SPZ";

        return groupTag;
    }

    protected void BroadcastGroupSync(SparkZCoreGroupPayload payload)
    {
        if (!payload)
            return;

        for (int index = 0; index < payload.Members.Count(); index++)
        {
            SparkZCoreGroupMemberPayload member = payload.Members.Get(index);
            if (!member)
                continue;

            PlayerIdentity identity = FindOnlineIdentityByPlainId(member.PlainId);
            if (identity)
                SendGroupSync(identity, payload);
        }
    }

    protected void BroadcastQuickPing(SparkZCoreGroupPayload payload, SparkZCoreMarkerPayload markerPayload)
    {
        if (!payload || !markerPayload)
            return;

        for (int index = 0; index < payload.Members.Count(); index++)
        {
            SparkZCoreGroupMemberPayload member = payload.Members.Get(index);
            if (!member)
                continue;

            if (member.PlainId == markerPayload.OwnerId)
                continue;

            PlayerIdentity identity = FindOnlineIdentityByPlainId(member.PlainId);
            if (!identity)
                continue;

            PlayerBase targetPlayer = FindOnlinePlayerByPlainId(identity.GetPlainId());
            if (!targetPlayer)
                continue;

            ScriptRPC rpc = new ScriptRPC();
            markerPayload.WriteToContext(rpc);
            rpc.Send(targetPlayer, SparkZCoreRPCId.SPARKZ_RPC_GROUP_PING_SYNC, true, identity);
        }
    }

    protected void SendGroupSync(PlayerIdentity identity, SparkZCoreGroupPayload payload)
    {
        if (!identity || !payload)
            return;

        PlayerBase targetPlayer = FindOnlinePlayerByPlainId(identity.GetPlainId());
        if (!targetPlayer)
            return;

        payload.CurrentMemberId = identity.GetPlainId();

        ScriptRPC rpc = new ScriptRPC();
        payload.WriteToContext(rpc);
        rpc.Send(targetPlayer, SparkZCoreRPCId.SPARKZ_RPC_GROUP_SYNC, true, identity);
    }

    protected void SendGroupClear(PlayerIdentity identity)
    {
        if (!identity)
            return;

        SparkZCoreGroupPayload emptyPayload = new SparkZCoreGroupPayload();
        emptyPayload.Revision = NextRevision();
        emptyPayload.CurrentMemberId = identity.GetPlainId();
        SendGroupSync(identity, emptyPayload);
    }

    protected void SendInviteSync(PlayerIdentity identity, SparkZCoreGroupPayload payload, string leaderName)
    {
        if (!identity || !payload)
            return;

        PlayerBase targetPlayer = FindOnlinePlayerByPlainId(identity.GetPlainId());
        if (!targetPlayer)
            return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Write(payload.GroupId);
        rpc.Write(payload.Name);
        rpc.Write(payload.Tag);
        rpc.Write(leaderName);
        rpc.Send(targetPlayer, SparkZCoreRPCId.SPARKZ_RPC_GROUP_INVITE_SYNC, true, identity);
    }

    protected void SendInviteClear(PlayerIdentity identity)
    {
        if (!identity)
            return;

        PlayerBase targetPlayer = FindOnlinePlayerByPlainId(identity.GetPlainId());
        if (!targetPlayer)
            return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Write("");
        rpc.Write("");
        rpc.Write("");
        rpc.Write("");
        rpc.Send(targetPlayer, SparkZCoreRPCId.SPARKZ_RPC_GROUP_INVITE_SYNC, true, identity);
    }

    protected void SendInviteResult(PlayerIdentity identity, string statusText)
    {
        if (!identity)
            return;

        PlayerBase targetPlayer = FindOnlinePlayerByPlainId(identity.GetPlainId());
        if (!targetPlayer)
            return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Write(statusText);
        rpc.Send(targetPlayer, SparkZCoreRPCId.SPARKZ_RPC_GROUP_INVITE_RESULT_SYNC, true, identity);
    }

    protected PlayerIdentity FindOnlineIdentityByName(string playerName)
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        for (int index = 0; index < players.Count(); index++)
        {
            PlayerBase player = PlayerBase.Cast(players.Get(index));
            if (!player)
                continue;

            PlayerIdentity identity = player.GetIdentity();
            if (identity && identity.GetName() == playerName)
                return identity;
        }

        return null;
    }

    protected PlayerIdentity FindOnlineIdentityByPlainId(string plainId)
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        for (int index = 0; index < players.Count(); index++)
        {
            PlayerBase player = PlayerBase.Cast(players.Get(index));
            if (!player)
                continue;

            PlayerIdentity identity = player.GetIdentity();
            if (identity && identity.GetPlainId() == plainId)
                return identity;
        }

        return null;
    }

    protected PlayerBase FindOnlinePlayerByPlainId(string plainId)
    {
        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);

        for (int index = 0; index < players.Count(); index++)
        {
            PlayerBase player = PlayerBase.Cast(players.Get(index));
            if (!player)
                continue;

            PlayerIdentity identity = player.GetIdentity();
            if (identity && identity.GetPlainId() == plainId)
                return player;
        }

        return null;
    }
}
