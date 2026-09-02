class SparkZQuickPing3D
{
    protected static ref SparkZQuickPing3D s_Instance;
    protected ref array<Widget> m_Roots;
    protected ref array<ImageWidget> m_Icons;
    protected ref array<TextWidget> m_Distances;
    protected ref array<string> m_LastIconPaths;
    protected ref array<int> m_LastColorARGBs;

    void SparkZQuickPing3D()
    {
        m_Roots = new array<Widget>();
        m_Icons = new array<ImageWidget>();
        m_Distances = new array<TextWidget>();
        m_LastIconPaths = new array<string>();
        m_LastColorARGBs = new array<int>();
    }

    static SparkZQuickPing3D Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZQuickPing3D();

        return s_Instance;
    }

    static void Delete()
    {
        if (s_Instance)
            s_Instance.Destroy();

        s_Instance = null;
    }

    void SetMarker(SparkZCoreMapPoint marker)
    {
        EnsureWidget(0);
    }

    void Update(float timeslice)
    {
        if (!GetGame())
            return;

        SparkZGroupMarkerCache.Get().ExpirePrivateMarkers();

        if (!ShouldShow())
        {
            HideFromIndex(0);
            return;
        }

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        int visibleSlot = 0;
        int quickPingCount = SparkZGroupMarkerCache.Get().GetQuickPingCount();

        for (int index = 0; index < quickPingCount; index++)
        {
            SparkZCoreMapPoint marker = SparkZGroupMarkerCache.Get().GetQuickPingBySlot(index);
            if (!marker || !marker.IsValid())
                continue;

            EnsureWidget(visibleSlot);
            if (UpdateWidget(visibleSlot, marker, player))
                visibleSlot++;
        }

        HideFromIndex(visibleSlot);
    }

    protected bool ShouldShow()
    {
        if (SparkZTacticalMenuManager.Get().IsOpen())
            return false;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player || !player.IsAlive() || player.IsUnconscious())
            return false;

        return SparkZGroupMarkerCache.Get().GetQuickPingCount() > 0;
    }

    protected void EnsureWidget(int slot)
    {
        if (!GetGame() || !GetGame().GetWorkspace())
            return;

        while (m_Roots.Count() <= slot)
        {
            Widget root = GetGame().GetWorkspace().CreateWidgets(SparkZGroupConstants.LAYOUT_QUICK_PING_3D);
            ImageWidget icon;
            TextWidget distance;

            if (root)
            {
                Class.CastTo(icon, root.FindAnyWidget("SparkZQuickPingIcon"));
                Class.CastTo(distance, root.FindAnyWidget("SparkZQuickPingDistance"));
            }

            m_Roots.Insert(root);
            m_Icons.Insert(icon);
            m_Distances.Insert(distance);
            m_LastIconPaths.Insert("");
            m_LastColorARGBs.Insert(0);
        }
    }

    protected bool UpdateWidget(int slot, SparkZCoreMapPoint marker, PlayerBase player)
    {
        Widget root = m_Roots.Get(slot);
        if (!root || !marker || !player)
            return false;

        vector markerPosition = marker.Position;
        vector displayPosition = Vector(markerPosition[0], markerPosition[1] + 1.4, markerPosition[2]);
        vector screenPosition = GetGame().GetScreenPos(displayPosition);
        if (screenPosition[2] <= 0)
        {
            root.Show(false);
            return false;
        }

        int screenWidth;
        int screenHeight;
        GetScreenSize(screenWidth, screenHeight);
        if (screenPosition[0] <= 0 || screenPosition[0] >= screenWidth || screenPosition[1] <= 0 || screenPosition[1] >= screenHeight)
        {
            root.Show(false);
            return false;
        }

        float widgetWidth;
        float widgetHeight;
        root.GetScreenSize(widgetWidth, widgetHeight);
        root.SetPos(screenPosition[0] - (widgetWidth * 0.5), screenPosition[1] - (widgetHeight * 0.5));

        string iconPath = SparkZGroupMarkerCache.Get().GetQuickPingIconById(marker.Id);
        int colorARGB = marker.ColorARGB;

        ImageWidget icon = m_Icons.Get(slot);
        if (icon)
        {
            if (iconPath != m_LastIconPaths.Get(slot))
            {
                icon.LoadImageFile(0, iconPath);
                m_LastIconPaths.Set(slot, iconPath);
            }

            if (colorARGB != m_LastColorARGBs.Get(slot))
                icon.SetColor(colorARGB);
        }

        TextWidget distance = m_Distances.Get(slot);
        if (distance)
        {
            distance.SetText(FormatDistance(vector.Distance(player.GetPosition(), markerPosition)));
            if (colorARGB != m_LastColorARGBs.Get(slot))
                distance.SetColor(colorARGB);
        }

        m_LastColorARGBs.Set(slot, colorARGB);
        root.Show(true);
        return true;
    }

    protected string FormatDistance(float distance)
    {
        if (distance < 1000.0)
        {
            int meters = (int)distance;
            return meters.ToString() + "m";
        }

        int roundedHundreds = (int)(distance / 100.0);
        float kilometers = ((float)roundedHundreds) / 10.0;
        return kilometers.ToString() + "km";
    }

    protected void HideFromIndex(int startIndex)
    {
        for (int slot = startIndex; slot < m_Roots.Count(); slot++)
        {
            Widget root = m_Roots.Get(slot);
            if (root)
                root.Show(false);
        }
    }

    protected void Destroy()
    {
        for (int slot = 0; slot < m_Roots.Count(); slot++)
        {
            Widget root = m_Roots.Get(slot);
            if (root)
                root.Unlink();
        }

        m_Roots.Clear();
        m_Icons.Clear();
        m_Distances.Clear();
        m_LastIconPaths.Clear();
        m_LastColorARGBs.Clear();
    }
}
