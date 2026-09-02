class SparkZTeammate3D
{
    protected static const float HEAD_CLEARANCE_PIXELS = 3;
    protected static ref SparkZTeammate3D s_Instance;
    protected ref array<Widget> m_Roots;
    protected ref array<ImageWidget> m_Icons;
    protected ref array<TextWidget> m_Names;
    protected ref array<TextWidget> m_Distances;
    protected ref array<vector> m_LastScreenPositions;

    void SparkZTeammate3D()
    {
        m_Roots = new array<Widget>();
        m_Icons = new array<ImageWidget>();
        m_Names = new array<TextWidget>();
        m_Distances = new array<TextWidget>();
        m_LastScreenPositions = new array<vector>();
    }

    static SparkZTeammate3D Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZTeammate3D();

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
        if (!ShouldShow())
        {
            HideAll();
            return;
        }

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        string localPlainId = GetLocalPlainId(player);
        array<ref SparkZCoreGroupMemberPayload> members = SparkZLocalGroupSession.Get().GetMembers();
        int visibleSlot = 0;

        for (int index = 0; index < members.Count(); index++)
        {
            SparkZCoreGroupMemberPayload member = members.Get(index);
            if (!ShouldShowMember(member, localPlainId, player))
                continue;

            EnsureWidget(visibleSlot);
            if (visibleSlot >= m_Roots.Count())
                continue;

            if (UpdateWidget(visibleSlot, member, player))
                visibleSlot++;
        }

        HideFromIndex(visibleSlot);
    }

    protected bool ShouldShow()
    {
        if (!GetGame() || !GetGame().GetWorkspace())
            return false;

        if (SparkZTacticalMenuManager.Get().IsOpen())
            return false;

        if (!SparkZLocalGroupSession.Get().IsActive())
            return false;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player || !player.IsAlive() || player.IsUnconscious())
            return false;

        return true;
    }

    protected bool ShouldShowMember(SparkZCoreGroupMemberPayload member, string localPlainId, PlayerBase player)
    {
        if (!member || !member.IsValid())
            return false;

        if (member.PlainId == localPlainId)
            return false;

        if (!member.Online || !member.HasLiveState)
            return false;

        if (vector.Distance(player.GetPosition(), member.Position) > SparkZGroupConstants.SQUAD_NAME_TAG_MAX_DISTANCE_METERS)
            return false;

        return true;
    }

    protected bool UpdateWidget(int slot, SparkZCoreGroupMemberPayload member, PlayerBase player)
    {
        Widget root = m_Roots.Get(slot);
        if (!root)
            return false;

        vector livePosition = SparkZLocalGroupSession.Get().GetExtrapolatedPosition(member.PlainId, member.Position);
        vector displayPosition = BuildDisplayPosition(livePosition, member.DisplayHeightOffset);
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

        float targetX = screenPosition[0] - (widgetWidth * 0.5);
        float targetY = screenPosition[1] - widgetHeight - HEAD_CLEARANCE_PIXELS;
        vector smoothedScreenPosition = SmoothScreenPosition(slot, targetX, targetY);
        root.SetPos(smoothedScreenPosition[0], smoothedScreenPosition[1]);

        int nameColor = SparkZGroupHudProfile.Get().GetNameTagColor();

        ImageWidget icon = m_Icons.Get(slot);
        if (icon)
        {
            icon.LoadImageFile(0, "SparkZGroup\\gui\\icons\\markers\\player.paa");
            icon.SetColor(nameColor);
        }

        TextWidget nameWidget = m_Names.Get(slot);
        if (nameWidget)
        {
            nameWidget.SetText(GetMemberName(member));
            nameWidget.SetColor(nameColor);
        }

        TextWidget distanceWidget = m_Distances.Get(slot);
        if (distanceWidget)
            distanceWidget.SetText(FormatDistance(vector.Distance(player.GetPosition(), livePosition)));

        root.Show(true);
        return true;
    }

    protected void EnsureWidget(int slot)
    {
        if (!GetGame() || !GetGame().GetWorkspace())
            return;

        while (m_Roots.Count() <= slot)
        {
            Widget root = GetGame().GetWorkspace().CreateWidgets(SparkZGroupConstants.LAYOUT_TEAMMATE_3D);
            ImageWidget icon;
            TextWidget nameWidget;
            TextWidget distanceWidget;

            if (root)
            {
                Class.CastTo(icon, root.FindAnyWidget("SparkZTeammate3DIcon"));
                Class.CastTo(nameWidget, root.FindAnyWidget("SparkZTeammate3DName"));
                Class.CastTo(distanceWidget, root.FindAnyWidget("SparkZTeammate3DDistance"));
            }

            m_Roots.Insert(root);
            m_Icons.Insert(icon);
            m_Names.Insert(nameWidget);
            m_Distances.Insert(distanceWidget);
            m_LastScreenPositions.Insert(Vector(-99999, -99999, 0));
        }
    }

    protected vector BuildDisplayPosition(vector memberPosition, float displayHeightOffset)
    {
        float displayY = memberPosition[1];
        if (GetGame())
        {
            float terrainY = GetGame().SurfaceY(memberPosition[0], memberPosition[2]);
            if (displayY < terrainY)
                displayY = terrainY;
        }

        if (displayHeightOffset <= 0.0)
            displayHeightOffset = 1.95;

        return Vector(memberPosition[0], displayY + displayHeightOffset, memberPosition[2]);
    }

    protected vector SmoothScreenPosition(int slot, float targetX, float targetY)
    {
        if (slot < 0 || slot >= m_LastScreenPositions.Count())
            return Vector(targetX, targetY, 0);

        vector previous = m_LastScreenPositions.Get(slot);
        if (previous[0] < -90000)
        {
            vector initial = Vector(targetX, targetY, 0);
            m_LastScreenPositions.Set(slot, initial);
            return initial;
        }

        float dx = targetX - previous[0];
        float dy = targetY - previous[1];
        float absDx = dx;
        if (absDx < 0.0)
            absDx = -absDx;

        float absDy = dy;
        if (absDy < 0.0)
            absDy = -absDy;

        if ((absDx + absDy) > 240.0)
        {
            vector snapped = Vector(targetX, targetY, 0);
            m_LastScreenPositions.Set(slot, snapped);
            return snapped;
        }

        vector smoothed = Vector(previous[0] + (dx * 0.45), previous[1] + (dy * 0.45), 0);
        m_LastScreenPositions.Set(slot, smoothed);
        return smoothed;
    }

    protected string GetMemberName(SparkZCoreGroupMemberPayload member)
    {
        if (!member || member.Name == "")
            return "Survivor";

        return member.Name;
    }

    protected string GetLocalPlainId(PlayerBase player)
    {
        if (!player)
            return "";

        PlayerIdentity identity = player.GetIdentity();
        if (!identity)
            return "";

        return identity.GetPlainId();
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
        m_LastScreenPositions.Clear();
    }
}
