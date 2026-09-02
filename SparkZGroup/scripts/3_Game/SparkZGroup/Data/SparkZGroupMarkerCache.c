// Client-side persistence for a player's own private map pins (channel PRIVATE only - quick
// pings and squad pings live on the server's own squad-marker persistence instead, since those
// are shared/synced state, not personal). Stored per-client under $profile: so pins survive a
// relog/reconnect on the same machine, matching how personal preferences are already kept here.
class SparkZGroupPrivateMarkerEntry
{
    string Id;
    string Label;
    vector Position;
    int ColorARGB;
    string IconPath;
    bool Show3D;

    void SparkZGroupPrivateMarkerEntry()
    {
        Id = "";
        Label = "";
        Position = "0 0 0";
        ColorARGB = ARGB(255, 255, 255, 255);
        IconPath = "";
        Show3D = false;
    }
}

class SparkZGroupPrivateMarkerStore
{
    ref array<ref SparkZGroupPrivateMarkerEntry> Markers;

    void SparkZGroupPrivateMarkerStore()
    {
        Markers = new array<ref SparkZGroupPrivateMarkerEntry>();
    }
}

class SparkZGroupMarkerCache
{
    static const int MARKER_CHANNEL_PRIVATE = 0;
    static const int MARKER_CHANNEL_GROUP = 1;
    static const int MARKER_CHANNEL_SERVER = 2;
    static const string QUICK_PING_ID = "quick_ping_local";
    static const string QUICK_PING_PREFIX = "quick_ping_";
    protected static const string PRIVATE_MARKERS_DIR = "$profile:SparkZGroup";
    protected static const string PRIVATE_MARKERS_FILE = "$profile:SparkZGroup/PrivateMarkers.json";

    protected static ref SparkZGroupMarkerCache s_Instance;
    protected ref array<ref SparkZCoreMapPoint> m_PrivateMarkers;
    protected ref array<string> m_PrivateMarkerIcons;
    protected ref array<int> m_PrivateMarkerChannels;
    protected ref array<float> m_PrivateMarkerExpiresAt;
    protected ref array<bool> m_PrivateMarker3DVisible;
    protected ref array<ref SparkZCoreMapPoint> m_ServerMarkers;
    protected ref array<string> m_ServerMarkerIcons;
    protected ref array<float> m_ServerMarkerExpiresAt;
    protected ref array<ref SparkZCoreMarkerPayload> m_RemoteSquadMarkers;
    protected int m_Revision;

    void SparkZGroupMarkerCache()
    {
        m_PrivateMarkers = new array<ref SparkZCoreMapPoint>();
        m_PrivateMarkerIcons = new array<string>();
        m_PrivateMarkerChannels = new array<int>();
        m_PrivateMarkerExpiresAt = new array<float>();
        m_PrivateMarker3DVisible = new array<bool>();
        m_ServerMarkers = new array<ref SparkZCoreMapPoint>();
        m_ServerMarkerIcons = new array<string>();
        m_ServerMarkerExpiresAt = new array<float>();
        m_RemoteSquadMarkers = new array<ref SparkZCoreMarkerPayload>();
        m_Revision = 0;

        BuildFixedServerMarkers();
        LoadPrivateMarkers();
    }

    protected void LoadPrivateMarkers()
    {
        if (!GetGame() || !GetGame().IsClient())
            return;

        if (!FileExist(PRIVATE_MARKERS_FILE))
            return;

        SparkZGroupPrivateMarkerStore store = new SparkZGroupPrivateMarkerStore();
        JsonFileLoader<SparkZGroupPrivateMarkerStore>.JsonLoadFile(PRIVATE_MARKERS_FILE, store);
        if (!store || !store.Markers)
            return;

        for (int index = 0; index < store.Markers.Count(); index++)
        {
            SparkZGroupPrivateMarkerEntry entry = store.Markers.Get(index);
            if (!entry || entry.Id == "" || entry.Label == "")
                continue;

            m_Revision++;
            SparkZCoreMapPoint marker = new SparkZCoreMapPoint();
            marker.Set(entry.Id, entry.Label, entry.Position, entry.ColorARGB, m_Revision);
            m_PrivateMarkers.Insert(marker);
            m_PrivateMarkerIcons.Insert(entry.IconPath);
            m_PrivateMarkerChannels.Insert(MARKER_CHANNEL_PRIVATE);
            m_PrivateMarkerExpiresAt.Insert(0.0);
            m_PrivateMarker3DVisible.Insert(entry.Show3D);
        }
    }

    protected void SavePrivateMarkers()
    {
        if (!GetGame() || !GetGame().IsClient())
            return;

        if (!FileExist(PRIVATE_MARKERS_DIR))
            MakeDirectory(PRIVATE_MARKERS_DIR);

        SparkZGroupPrivateMarkerStore store = new SparkZGroupPrivateMarkerStore();
        for (int index = 0; index < m_PrivateMarkers.Count(); index++)
        {
            if (m_PrivateMarkerChannels.Get(index) != MARKER_CHANNEL_PRIVATE)
                continue;

            SparkZCoreMapPoint marker = m_PrivateMarkers.Get(index);
            if (!marker)
                continue;

            SparkZGroupPrivateMarkerEntry entry = new SparkZGroupPrivateMarkerEntry();
            entry.Id = marker.Id;
            entry.Label = marker.Label;
            entry.Position = marker.Position;
            entry.ColorARGB = marker.ColorARGB;
            entry.IconPath = m_PrivateMarkerIcons.Get(index);
            entry.Show3D = m_PrivateMarker3DVisible.Get(index);
            store.Markers.Insert(entry);
        }

        JsonFileLoader<SparkZGroupPrivateMarkerStore>.JsonSaveFile(PRIVATE_MARKERS_FILE, store);
    }

    int GetRevision()
    {
        return m_Revision;
    }

    static SparkZGroupMarkerCache Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZGroupMarkerCache();

        return s_Instance;
    }

    static void Delete()
    {
        s_Instance = null;
    }

    SparkZCoreMapPoint AddPrivateMarker(vector position)
    {
        m_Revision++;
        position = NormalizeTerrainPosition(position);

        SparkZCoreMapPoint marker = new SparkZCoreMapPoint();
        marker.Set("private_" + m_Revision, "My Pin " + m_Revision, position, ARGB(255, 255, 216, 64), m_Revision);
        m_PrivateMarkers.Insert(marker);
        m_PrivateMarkerIcons.Insert("SparkZGroup\\gui\\icons\\markers\\marker.paa");
        m_PrivateMarkerChannels.Insert(MARKER_CHANNEL_PRIVATE);
        m_PrivateMarkerExpiresAt.Insert(0.0);
        m_PrivateMarker3DVisible.Insert(true);
        SavePrivateMarkers();
        return marker;
    }

    SparkZCoreMapPoint AddOrMoveQuickPing(vector position, float ttlSeconds)
    {
        SparkZQuickPingProfile profile = SparkZQuickPingProfile.Get();
        return UpsertQuickPing(QUICK_PING_ID, "Quick Ping", position, profile.GetColor(), profile.GetIconPath(), ttlSeconds);
    }

    SparkZCoreMapPoint UpsertSquadQuickPing(SparkZCoreMarkerPayload payload)
    {
        if (!payload || !payload.IsValid())
            return null;

        return UpsertQuickPing(payload.Id, payload.Label, payload.Position, payload.ColorARGB, payload.IconPath, SparkZGroupConstants.QUICK_PING_TTL_SECONDS);
    }

    protected SparkZCoreMapPoint UpsertQuickPing(string id, string label, vector position, int colorARGB, string iconPath, float ttlSeconds)
    {
        if (id == "")
            id = QUICK_PING_ID;

        if (label == "")
            label = "Quick Ping";

        if (iconPath == "")
            iconPath = "SparkZGroup\\gui\\icons\\markers\\ping.paa";

        int quickPingIndex = FindPrivateMarkerIndexById(id);
        if (quickPingIndex >= 0)
        {
            m_Revision++;
            SparkZCoreMapPoint existingMarker = m_PrivateMarkers.Get(quickPingIndex);
            existingMarker.Set(id, label, position, colorARGB, m_Revision);
            m_PrivateMarkerIcons.Set(quickPingIndex, iconPath);
            m_PrivateMarkerChannels.Set(quickPingIndex, MARKER_CHANNEL_GROUP);
            m_PrivateMarkerExpiresAt.Set(quickPingIndex, GetExpiryTime(ttlSeconds));
            m_PrivateMarker3DVisible.Set(quickPingIndex, false);
            return existingMarker;
        }

        m_Revision++;

        SparkZCoreMapPoint marker = new SparkZCoreMapPoint();
        marker.Set(id, label, position, colorARGB, m_Revision);
        m_PrivateMarkers.Insert(marker);
        m_PrivateMarkerIcons.Insert(iconPath);
        m_PrivateMarkerChannels.Insert(MARKER_CHANNEL_GROUP);
        m_PrivateMarkerExpiresAt.Insert(GetExpiryTime(ttlSeconds));
        m_PrivateMarker3DVisible.Insert(false);
        return marker;
    }

    bool RemovePrivateMarker(int index)
    {
        if (index < 0 || index >= m_PrivateMarkers.Count())
            return false;

        m_PrivateMarkers.Remove(index);
        m_PrivateMarkerIcons.Remove(index);
        m_PrivateMarkerChannels.Remove(index);
        m_PrivateMarkerExpiresAt.Remove(index);
        m_PrivateMarker3DVisible.Remove(index);
        m_Revision++;
        SavePrivateMarkers();
        return true;
    }

    void ClearPrivateMarkers()
    {
        m_PrivateMarkers.Clear();
        m_PrivateMarkerIcons.Clear();
        m_PrivateMarkerChannels.Clear();
        m_PrivateMarkerExpiresAt.Clear();
        m_PrivateMarker3DVisible.Clear();
        m_Revision++;
        SavePrivateMarkers();
    }

    bool ExpirePrivateMarkers()
    {
        if (!GetGame())
            return false;

        float now = GetGame().GetTime();
        bool expiredAny = false;

        for (int index = m_PrivateMarkers.Count() - 1; index >= 0; index--)
        {
            if (index >= m_PrivateMarkerExpiresAt.Count())
                continue;

            float expiresAt = m_PrivateMarkerExpiresAt.Get(index);
            if (expiresAt <= 0.0 || expiresAt > now)
                continue;

            RemovePrivateMarker(index);
            expiredAny = true;
        }

        return expiredAny;
    }

    bool RenamePrivateMarker(int index, string label)
    {
        SparkZCoreMapPoint marker = GetPrivateMarker(index);
        if (!marker)
            return false;

        if (label == "")
            return false;

        m_Revision++;
        marker.Label = label;
        marker.Revision = m_Revision;
        SavePrivateMarkers();
        return true;
    }

    bool SetPrivateMarkerColor(int index, int colorARGB)
    {
        SparkZCoreMapPoint marker = GetPrivateMarker(index);
        if (!marker)
            return false;

        m_Revision++;
        marker.ColorARGB = colorARGB;
        marker.Revision = m_Revision;
        SavePrivateMarkers();
        return true;
    }

    bool SetPrivateMarkerIcon(int index, string iconPath)
    {
        if (index < 0 || index >= m_PrivateMarkerIcons.Count())
            return false;

        m_Revision++;
        m_PrivateMarkerIcons.Set(index, iconPath);
        SavePrivateMarkers();
        return true;
    }

    bool SetPrivateMarkerChannel(int index, int channel)
    {
        if (index < 0 || index >= m_PrivateMarkerChannels.Count())
            return false;

        if (channel < MARKER_CHANNEL_PRIVATE || channel >= MARKER_CHANNEL_SERVER)
            return false;

        m_Revision++;
        m_PrivateMarkerChannels.Set(index, channel);
        return true;
    }

    bool EnsurePrivateMarkerSquadId(int index, string ownerPlainId)
    {
        if (index < 0 || index >= m_PrivateMarkers.Count())
            return false;

        SparkZCoreMapPoint marker = m_PrivateMarkers.Get(index);
        if (!marker)
            return false;

        if (marker.Id.IndexOf("squad_") == 0)
            return true;

        if (ownerPlainId == "")
            ownerPlainId = "local";

        m_Revision++;
        marker.Id = "squad_" + ownerPlainId + "_" + m_Revision.ToString();
        marker.Revision = m_Revision;
        return true;
    }

    int GetPrivateMarkerCount()
    {
        return m_PrivateMarkers.Count();
    }

    array<ref SparkZCoreMapPoint> GetPrivateMarkers()
    {
        return m_PrivateMarkers;
    }

    SparkZCoreMapPoint GetPrivateMarker(int index)
    {
        if (index < 0 || index >= m_PrivateMarkers.Count())
            return null;

        return m_PrivateMarkers.Get(index);
    }

    string GetPrivateMarkerIcon(int index)
    {
        if (index < 0 || index >= m_PrivateMarkerIcons.Count())
            return "";

        return m_PrivateMarkerIcons.Get(index);
    }

    int GetPrivateMarkerChannel(int index)
    {
        if (index < 0 || index >= m_PrivateMarkerChannels.Count())
            return MARKER_CHANNEL_PRIVATE;

        return m_PrivateMarkerChannels.Get(index);
    }

    int GetPrivateMarkerCountByChannel(int channel)
    {
        int count = 0;
        for (int index = 0; index < m_PrivateMarkerChannels.Count(); index++)
        {
            if (m_PrivateMarkerChannels.Get(index) == channel)
                count++;
        }

        return count;
    }

    bool IsPrivateMarker3DVisible(int index)
    {
        if (index < 0 || index >= m_PrivateMarker3DVisible.Count())
            return false;

        return m_PrivateMarker3DVisible.Get(index);
    }

    bool TogglePrivateMarker3DVisible(int index)
    {
        if (index < 0 || index >= m_PrivateMarkers.Count())
            return false;

        if (index >= m_PrivateMarker3DVisible.Count())
            return false;

        SparkZCoreMapPoint marker = m_PrivateMarkers.Get(index);
        if (!marker || marker.Id == QUICK_PING_ID)
            return false;

        bool visible = !m_PrivateMarker3DVisible.Get(index);
        m_PrivateMarker3DVisible.Set(index, visible);
        m_Revision++;
        marker.Revision = m_Revision;
        return true;
    }

    SparkZCoreMapPoint GetQuickPingMarker()
    {
        int quickPingIndex = FindPrivateMarkerIndexById(QUICK_PING_ID);
        if (quickPingIndex < 0)
            return null;

        return m_PrivateMarkers.Get(quickPingIndex);
    }

    int GetQuickPingCount()
    {
        int count = 0;
        for (int index = 0; index < m_PrivateMarkers.Count(); index++)
        {
            SparkZCoreMapPoint marker = m_PrivateMarkers.Get(index);
            if (marker && IsQuickPingId(marker.Id))
                count++;
        }

        return count;
    }

    SparkZCoreMapPoint GetQuickPingBySlot(int slot)
    {
        int quickPingSlot = 0;
        for (int index = 0; index < m_PrivateMarkers.Count(); index++)
        {
            SparkZCoreMapPoint marker = m_PrivateMarkers.Get(index);
            if (!marker || !IsQuickPingId(marker.Id))
                continue;

            if (quickPingSlot == slot)
                return marker;

            quickPingSlot++;
        }

        return null;
    }

    string GetQuickPingIconById(string id)
    {
        int quickPingIndex = FindPrivateMarkerIndexById(id);
        if (quickPingIndex < 0 || quickPingIndex >= m_PrivateMarkerIcons.Count())
            return "SparkZGroup\\gui\\icons\\markers\\ping.paa";

        return m_PrivateMarkerIcons.Get(quickPingIndex);
    }

    void ApplyQuickPingProfileToActivePing()
    {
        int quickPingIndex = FindPrivateMarkerIndexById(QUICK_PING_ID);
        if (quickPingIndex < 0)
            return;

        SparkZCoreMapPoint marker = m_PrivateMarkers.Get(quickPingIndex);
        if (!marker)
            return;

        SparkZQuickPingProfile profile = SparkZQuickPingProfile.Get();
        m_Revision++;
        marker.ColorARGB = profile.GetColor();
        marker.Revision = m_Revision;
        m_PrivateMarkerIcons.Set(quickPingIndex, profile.GetIconPath());
        m_PrivateMarkerChannels.Set(quickPingIndex, MARKER_CHANNEL_GROUP);
        m_PrivateMarker3DVisible.Set(quickPingIndex, false);
    }

    int GetServerMarkerCount()
    {
        return m_ServerMarkers.Count();
    }

    array<ref SparkZCoreMapPoint> GetServerMarkers()
    {
        return m_ServerMarkers;
    }

    SparkZCoreMapPoint GetServerMarker(int index)
    {
        if (index < 0 || index >= m_ServerMarkers.Count())
            return null;

        return m_ServerMarkers.Get(index);
    }

    string GetServerMarkerIcon(int index)
    {
        if (index < 0 || index >= m_ServerMarkerIcons.Count())
            return "";

        return m_ServerMarkerIcons.Get(index);
    }

    bool UpsertServerEventMarker(SparkZCoreMarkerPayload payload)
    {
        if (!payload || !payload.IsValid())
            return false;

        if (payload.IconPath == "")
            payload.IconPath = "SparkZGroup\\gui\\icons\\markers\\heli.paa";

        m_Revision++;
        vector position = NormalizeTerrainPosition(payload.Position);
        int markerIndex = FindServerMarkerIndexById(payload.Id);

        if (markerIndex >= 0)
        {
            SparkZCoreMapPoint existingMarker = m_ServerMarkers.Get(markerIndex);
            if (!existingMarker)
                return false;

            existingMarker.Set(payload.Id, payload.Label, position, payload.ColorARGB, m_Revision);
            m_ServerMarkerIcons.Set(markerIndex, payload.IconPath);
            m_ServerMarkerExpiresAt.Set(markerIndex, GetExpiryTime(payload.ExpiresAtMs));
            return true;
        }

        SparkZCoreMapPoint marker = new SparkZCoreMapPoint();
        marker.Set(payload.Id, payload.Label, position, payload.ColorARGB, m_Revision);
        m_ServerMarkers.Insert(marker);
        m_ServerMarkerIcons.Insert(payload.IconPath);
        m_ServerMarkerExpiresAt.Insert(GetExpiryTime(payload.ExpiresAtMs));
        return true;
    }

    bool RemoveServerEventMarker(string id)
    {
        int markerIndex = FindServerMarkerIndexById(id);
        if (markerIndex < 0)
            return false;

        if (markerIndex >= m_ServerMarkerExpiresAt.Count())
            return false;

        if (m_ServerMarkerExpiresAt.Get(markerIndex) <= 0.0)
            return false;

        m_ServerMarkers.Remove(markerIndex);
        m_ServerMarkerIcons.Remove(markerIndex);
        m_ServerMarkerExpiresAt.Remove(markerIndex);
        m_Revision++;
        return true;
    }

    bool ExpireServerEventMarkers()
    {
        if (!GetGame())
            return false;

        float now = GetGame().GetTime();
        bool expiredAny = false;

        for (int index = m_ServerMarkers.Count() - 1; index >= 0; index--)
        {
            if (index >= m_ServerMarkerExpiresAt.Count())
                continue;

            float expiresAt = m_ServerMarkerExpiresAt.Get(index);
            if (expiresAt <= 0.0 || expiresAt > now)
                continue;

            m_ServerMarkers.Remove(index);
            m_ServerMarkerIcons.Remove(index);
            m_ServerMarkerExpiresAt.Remove(index);
            m_Revision++;
            expiredAny = true;
        }

        return expiredAny;
    }

    void ApplyRemoteSquadMarkers(array<ref SparkZCoreMarkerPayload> markers)
    {
        m_RemoteSquadMarkers.Clear();
        if (markers)
        {
            foreach (SparkZCoreMarkerPayload marker: markers)
            {
                if (marker && marker.IsValid())
                    m_RemoteSquadMarkers.Insert(marker);
            }
        }

        m_Revision++;
    }

    void ClearRemoteSquadMarkers()
    {
        m_RemoteSquadMarkers.Clear();
        m_Revision++;
    }

    array<ref SparkZCoreMarkerPayload> GetRemoteSquadMarkers()
    {
        array<ref SparkZCoreMarkerPayload> result = new array<ref SparkZCoreMarkerPayload>();
        string localMemberId = SparkZLocalGroupSession.Get().GetLocalMemberId();

        foreach (SparkZCoreMarkerPayload marker: m_RemoteSquadMarkers)
        {
            if (marker && marker.OwnerId != localMemberId)
                result.Insert(marker);
        }

        return result;
    }

    protected void BuildFixedServerMarkers()
    {
        AddFixedServerMarker("server_altar_trader", "Altar Trader", Vector(8188.0, 480.0, 9062.0), ARGB(255, 70, 230, 140), "SparkZGroup\\gui\\icons\\markers\\safezone.paa");
        AddFixedServerMarker("server_black_market_north", "North Black Market", Vector(8520.326172, 208.337631, 13995.438477), ARGB(255, 235, 70, 70), "SparkZGroup\\gui\\icons\\markers\\blackmarket.paa");
        AddFixedServerMarker("server_black_market_west", "West Black Market", Vector(1651.059692, 322.697815, 8503.717773), ARGB(255, 235, 70, 70), "SparkZGroup\\gui\\icons\\markers\\blackmarket.paa");
        AddFixedServerMarker("server_weed_dealer_green_mountain", "Weed Dealer", Vector(3767.17, 370.0, 6010.54), ARGB(255, 80, 230, 80), "SparkZGroup\\gui\\icons\\markers\\ranger-station.paa");
    }

    protected void AddFixedServerMarker(string id, string label, vector position, int colorARGB, string iconPath)
    {
        SparkZCoreMapPoint marker = new SparkZCoreMapPoint();
        marker.Set(id, label, position, colorARGB, m_Revision);
        m_ServerMarkers.Insert(marker);
        m_ServerMarkerIcons.Insert(iconPath);
        m_ServerMarkerExpiresAt.Insert(0.0);
    }

    protected vector NormalizeTerrainPosition(vector position)
    {
        if (!GetGame())
            return position;

        float terrainY = GetGame().SurfaceY(position[0], position[2]);
        if (position[1] < terrainY)
            position[1] = terrainY;

        return position;
    }

    protected int FindPrivateMarkerIndexById(string id)
    {
        for (int index = 0; index < m_PrivateMarkers.Count(); index++)
        {
            SparkZCoreMapPoint marker = m_PrivateMarkers.Get(index);
            if (marker && marker.Id == id)
                return index;
        }

        return -1;
    }

    protected bool IsQuickPingId(string id)
    {
        if (id == QUICK_PING_ID)
            return true;

        return id.IndexOf(QUICK_PING_PREFIX) == 0;
    }

    protected int FindServerMarkerIndexById(string id)
    {
        for (int index = 0; index < m_ServerMarkers.Count(); index++)
        {
            SparkZCoreMapPoint marker = m_ServerMarkers.Get(index);
            if (marker && marker.Id == id)
                return index;
        }

        return -1;
    }

    protected float GetExpiryTime(float ttlSeconds)
    {
        if (!GetGame() || ttlSeconds <= 0.0)
            return 0.0;

        return GetGame().GetTime() + (ttlSeconds * 1000.0);
    }
}
