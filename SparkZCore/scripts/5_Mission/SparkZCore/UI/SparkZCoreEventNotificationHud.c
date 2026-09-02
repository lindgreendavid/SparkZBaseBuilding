class SparkZCoreEventNotificationHud
{
    protected static ref SparkZCoreEventNotificationHud s_Instance;
    protected Widget m_Root;
    protected Widget m_Backdrop;
    protected ImageWidget m_Icon;
    protected TextWidget m_Title;
    protected TextWidget m_Message;
    protected int m_ExpiresAtMs;

    static SparkZCoreEventNotificationHud Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZCoreEventNotificationHud();

        return s_Instance;
    }

    static void Delete()
    {
        if (s_Instance)
            s_Instance.Destroy();

        s_Instance = null;
    }

    void Update(float timeslice)
    {
        EnsureLayout();
        DrainNotifications();

        if (m_Root && m_Root.IsVisible() && GetGame().GetTime() >= m_ExpiresAtMs)
            m_Root.Show(false);
    }

    protected void EnsureLayout()
    {
        if (m_Root)
            return;

        m_Root = GetGame().GetWorkspace().CreateWidgets("SparkZCore/gui/layouts/hud/sparkz_event_notification.layout");
        if (!m_Root)
            return;

        m_Backdrop = m_Root.FindAnyWidget("SparkZEventNotifyBack");
        Class.CastTo(m_Icon, m_Root.FindAnyWidget("SparkZEventNotifyIcon"));
        Class.CastTo(m_Title, m_Root.FindAnyWidget("SparkZEventNotifyTitle"));
        Class.CastTo(m_Message, m_Root.FindAnyWidget("SparkZEventNotifyMessage"));
        m_Root.Show(false);
    }

    protected void DrainNotifications()
    {
        SparkZCoreEventNotificationPayload payload = SparkZCoreEventNotificationState.Get().PopNext();
        while (payload)
        {
            Show(payload);
            payload = SparkZCoreEventNotificationState.Get().PopNext();
        }
    }

    protected void Show(SparkZCoreEventNotificationPayload payload)
    {
        if (!payload || !m_Root)
            return;

        if (m_Title)
        {
            m_Title.SetText(payload.Title);
            m_Title.SetColor(ARGB(255, 255, 255, 255));
        }

        if (m_Message)
        {
            m_Message.SetText(payload.Message);
            m_Message.SetColor(ARGB(255, 224, 232, 234));
        }

        if (m_Icon)
        {
            if (payload.IconPath != "")
                m_Icon.LoadImageFile(0, payload.IconPath);
            m_Icon.SetColor(payload.ColorARGB);
        }

        if (m_Backdrop)
            m_Backdrop.SetColor(ARGB(184, 12, 15, 18));

        m_ExpiresAtMs = GetGame().GetTime() + payload.DurationMs;
        m_Root.Show(true);
    }

    protected void Destroy()
    {
        if (m_Root)
            m_Root.Unlink();

        m_Root = null;
    }
}
