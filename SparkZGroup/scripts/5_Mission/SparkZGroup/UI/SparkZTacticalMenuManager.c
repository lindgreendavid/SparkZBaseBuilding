class SparkZTacticalMenuManager
{
    protected static ref SparkZTacticalMenuManager s_Instance;
    protected ref SparkZTacticalMenu m_Menu;

    static SparkZTacticalMenuManager Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZTacticalMenuManager();

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

    void HandleMapToggleInput()
    {
        if (IsOpen())
        {
            if (m_Menu && m_Menu.ShouldSuppressMapToggleInput())
                return;

            Close();
            return;
        }

        Open();
    }

    void Open()
    {
        if (!CanOpen())
            return;

        m_Menu = new SparkZTacticalMenu();
        GetGame().GetUIManager().ShowScriptedMenu(m_Menu, null);
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

    bool ShouldSuppressMapToggleInput()
    {
        if (!IsOpen() || !m_Menu)
            return false;

        return m_Menu.ShouldSuppressMapToggleInput();
    }

    void OnMenuHidden(SparkZTacticalMenu menu)
    {
        if (m_Menu == menu)
            m_Menu = null;
    }

    void RefreshGroupPageFromNetwork()
    {
        if (!IsOpen() || !m_Menu)
            return;

        m_Menu.RefreshGroupPageFromNetwork();
    }

    void RefreshMarkersPageFromNetwork()
    {
        if (!IsOpen() || !m_Menu)
            return;

        m_Menu.RefreshMarkersPageFromNetwork();
    }

    protected bool CanOpen()
    {
        if (!GetGame() || !GetGame().GetUIManager())
            return false;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
        {
            SparkZCoreLogger.Debug("Tactical menu open blocked because no local player is available.");
            return false;
        }

        if (!player.IsAlive() || player.IsUnconscious())
        {
            SparkZCoreLogger.Debug("Tactical menu open blocked because the local player cannot use it.");
            return false;
        }

        return true;
    }
}
