class SparkZLocalMarker3D
{
    protected static ref SparkZLocalMarker3D s_Instance;
    protected ref array<Widget> m_Roots;
    protected ref array<ImageWidget> m_Icons;
    protected ref array<TextWidget> m_Names;
    protected ref array<TextWidget> m_Distances;
    protected ref array<string> m_LastIcons;
    protected ref array<int> m_LastColors;

    void SparkZLocalMarker3D()
    {
        m_Roots = new array<Widget>();
        m_Icons = new array<ImageWidget>();
        m_Names = new array<TextWidget>();
        m_Distances = new array<TextWidget>();
        m_LastIcons = new array<string>();
        m_LastColors = new array<int>();
    }

    static SparkZLocalMarker3D Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZLocalMarker3D();

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
        if (!GetGame())
            return;

        SparkZGroupMarkerCache.Get().ExpirePrivateMarkers();

        if (!ShouldShow())
        {
            HideAll();
            return;
        }

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        array<ref SparkZCoreMapPoint> markers = SparkZGroupMarkerCache.Get().GetPrivateMarkers();
        int visibleSlot = 0;

        for (int markerIndex = 0; markerIndex < markers.Count(); markerIndex++)
        {
            SparkZCoreMapPoint marker = markers.Get(markerIndex);
            if (!ShouldShowMarker(markerIndex, marker))
                continue;

            EnsureWidget(visibleSlot);
            if (visibleSlot >= m_Roots.Count())
                continue;

            if (UpdateWidget(visibleSlot, markerIndex, marker, player))
                visibleSlot++;
        }

        array<ref SparkZCoreMarkerPayload> remoteMarkers = SparkZGroupMarkerCache.Get().GetRemoteSquadMarkers();
        foreach (SparkZCoreMarkerPayload remoteMarker: remoteMarkers)
        {
            if (!ShouldShowRemoteMarker(remoteMarker))
                continue;

            EnsureWidget(visibleSlot);
            if (visibleSlot >= m_Roots.Count())
                continue;

            if (UpdateRemoteWidget(visibleSlot, remoteMarker, player))
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

        return true;
    }

    protected bool ShouldShowMarker(int markerIndex, SparkZCoreMapPoint marker)
    {
        if (!marker || !marker.IsValid())
            return false;

        if (marker.Id == SparkZGroupMarkerCache.QUICK_PING_ID)
            return false;

        return SparkZGroupMarkerCache.Get().IsPrivateMarker3DVisible(markerIndex);
    }

    protected bool ShouldShowRemoteMarker(SparkZCoreMarkerPayload marker)
    {
        if (!marker || !marker.IsValid())
            return false;

        return marker.Show3D;
    }

    protected bool UpdateWidget(int slot, int markerIndex, SparkZCoreMapPoint marker, PlayerBase player)
    {
        Widget root = m_Roots.Get(slot);
        if (!root)
            return false;

        vector markerPosition = marker.Position;
        vector displayPosition = BuildDisplayPosition(markerPosition);
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

        int colorARGB = marker.ColorARGB;
        string iconPath = SparkZGroupMarkerCache.Get().GetPrivateMarkerIcon(markerIndex);
        if (iconPath == "")
            iconPath = "SparkZGroup\\gui\\icons\\markers\\marker.paa";

        ImageWidget icon = m_Icons.Get(slot);
        if (icon)
        {
            if (m_LastIcons.Get(slot) != iconPath)
                icon.LoadImageFile(0, iconPath);

            if (m_LastColors.Get(slot) != colorARGB)
                icon.SetColor(colorARGB);
        }

        TextWidget nameWidget = m_Names.Get(slot);
        if (nameWidget)
        {
            nameWidget.SetText(marker.Label);
            nameWidget.SetColor(colorARGB);
        }

        string distanceText = FormatDistance(vector.Distance(player.GetPosition(), markerPosition));
        TextWidget distanceWidget = m_Distances.Get(slot);
        if (distanceWidget)
        {
            distanceWidget.SetText(distanceText);
            distanceWidget.SetColor(colorARGB);
        }

        m_LastIcons.Set(slot, iconPath);
        m_LastColors.Set(slot, colorARGB);
        root.Show(true);
        return true;
    }

    protected bool UpdateRemoteWidget(int slot, SparkZCoreMarkerPayload marker, PlayerBase player)
    {
        Widget root = m_Roots.Get(slot);
        if (!root)
            return false;

        vector markerPosition = marker.Position;
        vector displayPosition = BuildDisplayPosition(markerPosition);
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

        int colorARGB = marker.ColorARGB;
        string iconPath = marker.IconPath;
        if (iconPath == "")
            iconPath = "SparkZGroup\\gui\\icons\\markers\\marker.paa";

        ImageWidget icon = m_Icons.Get(slot);
        if (icon)
        {
            if (m_LastIcons.Get(slot) != iconPath)
                icon.LoadImageFile(0, iconPath);

            if (m_LastColors.Get(slot) != colorARGB)
                icon.SetColor(colorARGB);
        }

        TextWidget nameWidget = m_Names.Get(slot);
        if (nameWidget)
        {
            nameWidget.SetText(marker.Label);
            nameWidget.SetColor(colorARGB);
        }

        string distanceText = FormatDistance(vector.Distance(player.GetPosition(), markerPosition));
        TextWidget distanceWidget = m_Distances.Get(slot);
        if (distanceWidget)
        {
            distanceWidget.SetText(distanceText);
            distanceWidget.SetColor(colorARGB);
        }

        m_LastIcons.Set(slot, iconPath);
        m_LastColors.Set(slot, colorARGB);
        root.Show(true);
        return true;
    }

    protected vector BuildDisplayPosition(vector markerPosition)
    {
        float displayY = markerPosition[1];
        if (GetGame())
        {
            float terrainY = GetGame().SurfaceY(markerPosition[0], markerPosition[2]);
            if (displayY < terrainY)
                displayY = terrainY;
        }

        return Vector(markerPosition[0], displayY + 2.2, markerPosition[2]);
    }

    protected void EnsureWidget(int slot)
    {
        if (!GetGame() || !GetGame().GetWorkspace())
            return;

        while (m_Roots.Count() <= slot)
        {
            Widget root = GetGame().GetWorkspace().CreateWidgets(SparkZGroupConstants.LAYOUT_MARKER_3D);
            ImageWidget icon;
            TextWidget nameWidget;
            TextWidget distanceWidget;

            if (root)
            {
                Class.CastTo(icon, root.FindAnyWidget("SparkZMarker3DIcon"));
                Class.CastTo(nameWidget, root.FindAnyWidget("SparkZMarker3DName"));
                Class.CastTo(distanceWidget, root.FindAnyWidget("SparkZMarker3DDistance"));
            }

            m_Roots.Insert(root);
            m_Icons.Insert(icon);
            m_Names.Insert(nameWidget);
            m_Distances.Insert(distanceWidget);
            m_LastIcons.Insert("");
            m_LastColors.Insert(0);
        }
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

    protected void HideAll()
    {
        HideFromIndex(0);
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
        m_Names.Clear();
        m_Distances.Clear();
        m_LastIcons.Clear();
        m_LastColors.Clear();
    }
}
