class SparkZSquadHud
{
    protected static ref SparkZSquadHud s_Instance;
    protected Widget m_Root;
    protected ref array<Widget> m_Rows;
    protected ref array<TextWidget> m_Names;
    protected ref array<TextWidget> m_Distances;
    protected ref array<ImageWidget> m_HealthFills;

    void SparkZSquadHud()
    {
        m_Rows = new array<Widget>();
        m_Names = new array<TextWidget>();
        m_Distances = new array<TextWidget>();
        m_HealthFills = new array<ImageWidget>();
    }

    static SparkZSquadHud Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZSquadHud();

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
            Hide();
            return;
        }

        EnsureWidgets();
        if (!m_Root)
            return;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        string localPlainId = GetLocalPlainId(player);
        array<ref SparkZCoreGroupMemberPayload> members = SparkZLocalGroupSession.Get().GetMembers();

        int visibleSlot = 0;
        UpdateSelfRow(visibleSlot, player);
        visibleSlot++;

        for (int index = 0; index < members.Count(); index++)
        {
            SparkZCoreGroupMemberPayload member = members.Get(index);
            if (!ShouldShowMember(member, localPlainId))
                continue;

            if (visibleSlot >= m_Rows.Count())
                break;

            UpdateRow(visibleSlot, member, player);
            visibleSlot++;
        }

        HideRowsFrom(visibleSlot);
        m_Root.Show(true);
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

    protected bool ShouldShowMember(SparkZCoreGroupMemberPayload member, string localPlainId)
    {
        if (!member || !member.IsValid())
            return false;

        if (member.PlainId == localPlainId)
            return false;

        if (!member.Online || !member.HasLiveState)
            return false;

        return true;
    }

    protected void UpdateSelfRow(int slot, PlayerBase player)
    {
        Widget row = m_Rows.Get(slot);
        if (row)
            row.Show(true);

        float health01 = GetSelfHealth01(player);
        int nameColor = SparkZGroupHudProfile.Get().GetNameTagColor();

        TextWidget nameWidget = m_Names.Get(slot);
        if (nameWidget)
        {
            nameWidget.SetText(GetSelfName(player));
            nameWidget.SetColor(nameColor);
        }

        TextWidget distanceWidget = m_Distances.Get(slot);
        if (distanceWidget)
        {
            distanceWidget.SetText("YOU");
            distanceWidget.SetColor(ARGB(255, 245, 245, 240));
        }

        ImageWidget healthFill = m_HealthFills.Get(slot);
        if (healthFill)
        {
            float width = 210.0 * health01;
            if (width < 2.0)
                width = 2.0;

            healthFill.SetSize(width, 5);
            healthFill.SetColor(GetHealthColor(health01));
        }
    }

    protected string GetSelfName(PlayerBase player)
    {
        if (!player)
            return "You";

        PlayerIdentity identity = player.GetIdentity();
        if (identity && identity.GetName() != "")
            return identity.GetName();

        return "You";
    }

    protected float GetSelfHealth01(PlayerBase player)
    {
        if (!player)
            return 1.0;

        string localPlainId = GetLocalPlainId(player);
        SparkZCoreGroupMemberPayload localMember = SparkZLocalGroupSession.Get().FindMemberByPlainId(localPlainId);
        if (localMember && localMember.HasLiveState)
            return ClampHealth01(localMember.Health01);

        float health01 = player.GetHealth("", "Health") / 100.0;
        if (health01 <= 0.0 && player.IsAlive())
            return 1.0;

        return ClampHealth01(health01);
    }

    protected void UpdateRow(int slot, SparkZCoreGroupMemberPayload member, PlayerBase player)
    {
        Widget row = m_Rows.Get(slot);
        if (row)
            row.Show(true);

        int nameColor = SparkZGroupHudProfile.Get().GetNameTagColor();

        TextWidget nameWidget = m_Names.Get(slot);
        if (nameWidget)
        {
            nameWidget.SetText(GetMemberName(member));
            nameWidget.SetColor(nameColor);
        }

        TextWidget distanceWidget = m_Distances.Get(slot);
        if (distanceWidget)
        {
            distanceWidget.SetText(FormatDistance(vector.Distance(player.GetPosition(), member.Position)));
            distanceWidget.SetColor(ARGB(255, 245, 245, 240));
        }

        ImageWidget healthFill = m_HealthFills.Get(slot);
        if (healthFill)
        {
            float health01 = ClampHealth01(member.Health01);
            float width = 210.0 * health01;
            if (width < 2.0)
                width = 2.0;

            healthFill.SetSize(width, 5);
            healthFill.SetColor(GetHealthColor(health01));
        }
    }

    protected void EnsureWidgets()
    {
        if (m_Root)
            return;

        m_Root = GetGame().GetWorkspace().CreateWidgets(SparkZGroupConstants.LAYOUT_SQUAD_HUD);
        if (!m_Root)
            return;

        for (int slot = 0; slot < 5; slot++)
        {
            string suffix = slot.ToString();
            m_Rows.Insert(m_Root.FindAnyWidget("SparkZSquadHudRow" + suffix));

            TextWidget nameWidget;
            Class.CastTo(nameWidget, m_Root.FindAnyWidget("SparkZSquadHudName" + suffix));
            m_Names.Insert(nameWidget);

            TextWidget distanceWidget;
            Class.CastTo(distanceWidget, m_Root.FindAnyWidget("SparkZSquadHudDistance" + suffix));
            m_Distances.Insert(distanceWidget);

            ImageWidget healthFill;
            Class.CastTo(healthFill, m_Root.FindAnyWidget("SparkZSquadHudHealth" + suffix));
            if (healthFill)
                healthFill.LoadImageFile(0, "SparkZGroup\\gui\\textures\\ui\\sparkz_health_fill.paa");
            m_HealthFills.Insert(healthFill);
        }
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

    protected int GetHealthColor(float health01)
    {
        health01 = ClampHealth01(health01);

        if (health01 >= 0.6)
            return ARGB(255, 245, 245, 240);

        if (health01 >= 0.4)
            return ARGB(255, 235, 205, 60);

        float dangerT = 1.0 - (health01 / 0.4);
        int green = 90 - (int)(70.0 * dangerT);
        if (green < 20)
            green = 20;

        int blue = 40 - (int)(30.0 * dangerT);
        if (blue < 10)
            blue = 10;

        return ARGB(255, 220, green, blue);
    }

    protected float ClampHealth01(float health01)
    {
        if (health01 < 0.0)
            return 0.0;

        if (health01 > 1.0)
            return 1.0;

        return health01;
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

    protected void HideRowsFrom(int startIndex)
    {
        for (int slot = startIndex; slot < m_Rows.Count(); slot++)
        {
            Widget row = m_Rows.Get(slot);
            if (row)
                row.Show(false);
        }
    }

    protected void Hide()
    {
        if (m_Root)
            m_Root.Show(false);
    }

    protected void Destroy()
    {
        if (m_Root)
            m_Root.Unlink();

        m_Root = null;
        m_Rows.Clear();
        m_Names.Clear();
        m_Distances.Clear();
        m_HealthFills.Clear();
    }
}
