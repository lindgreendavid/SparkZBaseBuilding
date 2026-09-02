class SparkZBasePropertyMenuManager
{
    protected static ref SparkZBasePropertyMenuManager s_Instance;
    protected ref SparkZBasePropertyMenu m_Menu;
    protected int m_LastAppliedRevision;

    static SparkZBasePropertyMenuManager Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZBasePropertyMenuManager();

        return s_Instance;
    }

    static void Delete()
    {
        s_Instance = null;
    }

    void Toggle()
    {
        if (IsOpen())
        {
            Close();
            return;
        }

        Open();
    }

    void Open()
    {
        if (!CanOpen())
            return;

        m_Menu = new SparkZBasePropertyMenu();
        GetGame().GetUIManager().ShowScriptedMenu(m_Menu, null);
        m_LastAppliedRevision = SparkZBaseNetworkState.Get().GetRevision();
        SparkZBaseNetworkClient.Get().RequestStatus();
    }

    void Poll()
    {
        if (!IsOpen() || !m_Menu)
            return;

        int revision = SparkZBaseNetworkState.Get().GetRevision();
        if (revision == m_LastAppliedRevision)
            return;

        m_LastAppliedRevision = revision;
        SparkZBaseStatusPayload payload = SparkZBaseNetworkState.Get().GetLatestStatus();
        if (payload)
            m_Menu.ApplyStatus(payload);
    }

    void Close()
    {
        if (m_Menu)
            m_Menu.CloseMenu();
    }

    bool IsOpen()
    {
        if (!m_Menu || !GetGame() || !GetGame().GetUIManager())
            return false;

        return GetGame().GetUIManager().GetMenu() == m_Menu;
    }

    void OnMenuHidden(SparkZBasePropertyMenu menu)
    {
        if (m_Menu == menu)
            m_Menu = null;
    }

    protected bool CanOpen()
    {
        if (!GetGame() || !GetGame().GetUIManager())
            return false;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player || !player.IsAlive() || player.IsUnconscious())
            return false;

        if (GetGame().GetUIManager().GetMenu())
            return false;

        return true;
    }
}
