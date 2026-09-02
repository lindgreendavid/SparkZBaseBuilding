class SparkZLocalGroupSession
{
    protected static ref SparkZLocalGroupSession s_Instance;
    protected bool m_Active;
    protected string m_GroupId;
    protected string m_GroupName;
    protected string m_GroupTag;
    protected string m_LocalMemberId;
    protected string m_LocalMemberName;
    protected bool m_ServerBacked;
    protected ref array<ref SparkZCoreGroupMemberPayload> m_Members;
    protected ref map<string, vector> m_LastSyncPosition;
    protected ref map<string, vector> m_VelocityPerMs;
    protected ref map<string, float> m_LastSyncTimeMs;

    void SparkZLocalGroupSession()
    {
        m_Members = new array<ref SparkZCoreGroupMemberPayload>();
        m_LastSyncPosition = new map<string, vector>();
        m_VelocityPerMs = new map<string, vector>();
        m_LastSyncTimeMs = new map<string, float>();
        Reset();
    }

    static SparkZLocalGroupSession Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZLocalGroupSession();

        return s_Instance;
    }

    static void Delete()
    {
        s_Instance = null;
    }

    void CreateLocalPreview(string memberId, string memberName, string groupName, string groupTag)
    {
        if (memberId == "")
            memberId = "local";

        if (memberName == "")
            memberName = "You";

        if (groupName == "")
            groupName = "SparkZ Squad";

        if (groupTag == "")
            groupTag = "SPZ";

        m_Active = true;
        m_GroupId = "local_preview_group";
        m_GroupName = groupName;
        m_GroupTag = groupTag;
        m_LocalMemberId = memberId;
        m_LocalMemberName = memberName;
        m_ServerBacked = false;
        m_Members.Clear();
        AddMember(memberId, memberName, 1, true);
    }

    void SetIdentity(string groupName, string groupTag)
    {
        if (groupName == "")
            groupName = "SparkZ Squad";

        if (groupTag == "")
            groupTag = "SPZ";

        m_GroupName = groupName;
        m_GroupTag = groupTag;
    }

    void Leave()
    {
        Reset();
    }

    void ApplyServerSync(SparkZCoreGroupPayload payload)
    {
        if (!payload || !payload.IsValid())
        {
            Reset();
            return;
        }

        m_Active = true;
        m_GroupId = payload.GroupId;
        m_GroupName = payload.Name;
        m_GroupTag = payload.Tag;
        m_LocalMemberId = payload.CurrentMemberId;
        if (m_LocalMemberId == "")
            m_LocalMemberId = payload.LeaderId;

        m_LocalMemberName = FindMemberName(payload, m_LocalMemberId);
        m_ServerBacked = true;
        m_Members.Clear();

        if (payload.Members)
        {
            foreach (SparkZCoreGroupMemberPayload payloadMember: payload.Members)
            {
                if (payloadMember && payloadMember.IsValid())
                    AddMemberFromPayload(payloadMember);
            }
        }

        if (m_LocalMemberName == "")
            m_LocalMemberName = "You";
    }

    void ApplyServerLeave()
    {
        Reset();
    }

    bool IsActive()
    {
        return m_Active;
    }

    bool IsServerBacked()
    {
        return m_ServerBacked;
    }

    string GetGroupName()
    {
        return m_GroupName;
    }

    string GetGroupTag()
    {
        return m_GroupTag;
    }

    string GetChatPrefix()
    {
        if (m_GroupTag == "")
            return "";

        return "[" + m_GroupTag + "]";
    }

    string GetTaggedLocalMemberName()
    {
        string prefix = GetChatPrefix();
        if (prefix == "")
            return m_LocalMemberName;

        return prefix + " " + m_LocalMemberName;
    }

    string GetLocalMemberName()
    {
        return m_LocalMemberName;
    }

    int GetMemberCount()
    {
        if (!m_Active)
            return 0;

        return m_Members.Count();
    }

    array<ref SparkZCoreGroupMemberPayload> GetMembers()
    {
        return m_Members;
    }

    string GetLocalMemberId()
    {
        return m_LocalMemberId;
    }

    SparkZCoreGroupMemberPayload FindMemberByPlainId(string plainId)
    {
        if (plainId == "")
            return null;

        for (int index = 0; index < m_Members.Count(); index++)
        {
            SparkZCoreGroupMemberPayload member = m_Members.Get(index);
            if (member && member.PlainId == plainId)
                return member;
        }

        return null;
    }

    string GetLeaderName()
    {
        string leaderName = FindLeaderName();
        if (leaderName != "")
            return leaderName;

        return m_LocalMemberName;
    }

    string BuildRosterText()
    {
        if (!m_Active)
            return "";

        string rosterText = "";
        for (int index = 0; index < m_Members.Count(); index++)
        {
            SparkZCoreGroupMemberPayload member = m_Members.Get(index);
            if (!member)
                continue;

            if (rosterText != "")
                rosterText = rosterText + "\n";

            string line = member.Name;
            if (line == "")
                line = "Survivor";

            if (HasRoleFlag(member.RoleFlags, SparkZGroupConstants.SQUAD_ROLE_LEADER))
                line = line + " / LEADER";

            string permissions = BuildPermissionText(member.RoleFlags);
            if (permissions != "")
                line = line + " / " + permissions;

            if (!member.Online)
                line = line + " / OFFLINE";

            rosterText = rosterText + line;
        }

        return rosterText;
    }

    static bool HasRoleFlag(int roleFlags, int flag)
    {
        return (roleFlags & flag) == flag;
    }

    static string BuildPermissionText(int roleFlags)
    {
        string text = "";

        if (HasBaseAccess(roleFlags))
            text = AppendPermission(text, "BASE ACCESS");

        if (HasRoleFlag(roleFlags, SparkZGroupConstants.SQUAD_PERMISSION_BASE_MANAGER))
            text = AppendPermission(text, "BASE MANAGER");

        if (HasRoleFlag(roleFlags, SparkZGroupConstants.SQUAD_ROLE_LEADER))
            text = AppendPermission(text, "LEADER");

        return text;
    }

    static bool HasBaseAccess(int roleFlags)
    {
        if (HasRoleFlag(roleFlags, SparkZGroupConstants.SQUAD_PERMISSION_BASE_ACCESS))
            return true;

        if (HasRoleFlag(roleFlags, SparkZGroupConstants.SQUAD_PERMISSION_BASE_MANAGER))
            return true;

        return (roleFlags & SparkZGroupConstants.SQUAD_PERMISSION_BASE_LEGACY_ALL) != 0;
    }

    protected static string AppendPermission(string existing, string label)
    {
        if (existing == "")
            return label;

        return existing + ", " + label;
    }

    protected string FindMemberName(SparkZCoreGroupPayload payload, string plainId)
    {
        if (payload && payload.Members)
        {
            for (int index = 0; index < payload.Members.Count(); index++)
            {
                SparkZCoreGroupMemberPayload member = payload.Members.Get(index);
                if (member && member.PlainId == plainId)
                    return member.Name;
            }
        }

        return m_LocalMemberName;
    }

    protected string FindLeaderName()
    {
        for (int index = 0; index < m_Members.Count(); index++)
        {
            SparkZCoreGroupMemberPayload member = m_Members.Get(index);
            if (member && HasRoleFlag(member.RoleFlags, SparkZGroupConstants.SQUAD_ROLE_LEADER))
                return member.Name;
        }

        return "";
    }

    protected void AddMember(string plainId, string name, int roleFlags, bool online)
    {
        SparkZCoreGroupMemberPayload member = new SparkZCoreGroupMemberPayload();
        member.Set(plainId, name, roleFlags, online);
        m_Members.Insert(member);
    }

    protected void AddMemberFromPayload(SparkZCoreGroupMemberPayload payloadMember)
    {
        if (!payloadMember)
            return;

        SparkZCoreGroupMemberPayload member = new SparkZCoreGroupMemberPayload();
        member.Set(payloadMember.PlainId, payloadMember.Name, payloadMember.RoleFlags, payloadMember.Online);
        member.SetLiveStateEx(payloadMember.Position, payloadMember.Health01, payloadMember.DisplayHeightOffset, payloadMember.HasLiveState);
        m_Members.Insert(member);

        if (payloadMember.HasLiveState)
            UpdateMovementTracking(payloadMember.PlainId, payloadMember.Position);
    }

    protected void UpdateMovementTracking(string plainId, vector newPosition)
    {
        if (!GetGame() || plainId == "")
            return;

        float now = GetGame().GetTime();
        vector velocity = Vector(0, 0, 0);

        if (m_LastSyncTimeMs.Contains(plainId))
        {
            float lastTime = m_LastSyncTimeMs.Get(plainId);
            vector lastPosition = m_LastSyncPosition.Get(plainId);
            float dt = now - lastTime;

            if (dt > 30.0 && dt < 3000.0)
            {
                float distanceMoved = vector.Distance(newPosition, lastPosition);
                if (distanceMoved < 30.0)
                    velocity = (newPosition - lastPosition) * (1.0 / dt);
            }
        }

        m_VelocityPerMs.Set(plainId, velocity);
        m_LastSyncPosition.Set(plainId, newPosition);
        m_LastSyncTimeMs.Set(plainId, now);
    }

    vector GetExtrapolatedPosition(string plainId, vector fallbackPosition)
    {
        if (!GetGame() || plainId == "" || !m_LastSyncTimeMs.Contains(plainId))
            return fallbackPosition;

        float elapsed = GetGame().GetTime() - m_LastSyncTimeMs.Get(plainId);
        if (elapsed < 0.0)
            elapsed = 0.0;

        if (elapsed > 800.0)
            elapsed = 800.0;

        vector velocity = m_VelocityPerMs.Get(plainId);
        return m_LastSyncPosition.Get(plainId) + (velocity * elapsed);
    }

    protected void Reset()
    {
        m_Active = false;
        m_GroupId = "";
        m_GroupName = "SparkZ Squad";
        m_GroupTag = "SPZ";
        m_LocalMemberId = "";
        m_LocalMemberName = "";
        m_ServerBacked = false;
        if (m_Members)
            m_Members.Clear();

        if (m_LastSyncPosition)
            m_LastSyncPosition.Clear();

        if (m_VelocityPerMs)
            m_VelocityPerMs.Clear();

        if (m_LastSyncTimeMs)
            m_LastSyncTimeMs.Clear();
    }
}
