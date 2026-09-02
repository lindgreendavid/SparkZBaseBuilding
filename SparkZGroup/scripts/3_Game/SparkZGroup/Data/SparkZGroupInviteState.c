class SparkZGroupInviteState
{
    protected static ref SparkZGroupInviteState s_Instance;
    protected bool m_HasInvite;
    protected string m_GroupId;
    protected string m_GroupName;
    protected string m_GroupTag;
    protected string m_LeaderName;
    protected string m_StatusText;

    static SparkZGroupInviteState Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZGroupInviteState();

        return s_Instance;
    }

    static void Delete()
    {
        s_Instance = null;
    }

    void ApplyInvite(string groupId, string groupName, string groupTag, string leaderName)
    {
        m_HasInvite = true;
        m_GroupId = groupId;
        m_GroupName = groupName;
        m_GroupTag = groupTag;
        m_LeaderName = leaderName;
        m_StatusText = "";
    }

    void Clear()
    {
        m_HasInvite = false;
        m_GroupId = "";
        m_GroupName = "";
        m_GroupTag = "";
        m_LeaderName = "";
    }

    void SetStatusText(string statusText)
    {
        m_StatusText = statusText;
    }

    void ClearStatusText()
    {
        m_StatusText = "";
    }

    string GetStatusText()
    {
        return m_StatusText;
    }

    bool HasInvite()
    {
        return m_HasInvite;
    }

    string GetGroupId()
    {
        return m_GroupId;
    }

    string BuildInviteText()
    {
        if (!m_HasInvite)
            return "NO PENDING INVITE";

        string tagText = "";
        if (m_GroupTag != "")
            tagText = " [" + m_GroupTag + "]";

        return "INVITE: " + m_GroupName + tagText + "\nFROM: " + m_LeaderName;
    }
}
