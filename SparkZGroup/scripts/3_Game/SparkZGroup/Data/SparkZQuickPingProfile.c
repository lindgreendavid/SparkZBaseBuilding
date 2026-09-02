class SparkZQuickPingProfile
{
    static const string SETTINGS_DIR = "$profile:SparkZ/Group";
    static const string SETTINGS_FILE = "$profile:SparkZ/Group/QuickPingProfile.json";
    protected static ref SparkZQuickPingProfile s_Instance;
    protected ref array<string> m_IconPaths;
    protected ref array<string> m_IconNames;
    protected int m_ColorARGB;
    protected int m_IconIndex;

    void SparkZQuickPingProfile()
    {
        m_ColorARGB = ARGB(255, 255, 211, 64);
        m_IconIndex = 0;
        m_IconPaths = new array<string>();
        m_IconNames = new array<string>();

        AddIcon("Signal", "communications.paa");
        AddIcon("Ping", "ping.paa");
        AddIcon("Marker", "marker.paa");
        AddIcon("Home", "home.paa");
        AddIcon("Flag", "flag.paa");
        AddIcon("Star", "star.paa");
        AddIcon("Triangle", "triangle.paa");
        AddIcon("Circle", "circle.paa");
        AddIcon("Cross", "cross.paa");
        AddIcon("Camp", "camp.paa");
        AddIcon("Car", "car.paa");
        AddIcon("Heli", "heli.paa");
        AddIcon("Skull", "skull.paa");
        Load();
    }

    static SparkZQuickPingProfile Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZQuickPingProfile();

        return s_Instance;
    }

    static void Delete()
    {
        s_Instance = null;
    }

    int GetColor()
    {
        return m_ColorARGB;
    }

    void SetColor(int colorARGB)
    {
        m_ColorARGB = colorARGB;
        Save();
    }

    string GetIconPath()
    {
        if (m_IconIndex < 0 || m_IconIndex >= m_IconPaths.Count())
            return "SparkZGroup\\gui\\icons\\markers\\communications.paa";

        return m_IconPaths.Get(m_IconIndex);
    }

    string GetIconName()
    {
        if (m_IconIndex < 0 || m_IconIndex >= m_IconNames.Count())
            return "Signal";

        return m_IconNames.Get(m_IconIndex);
    }

    int GetIconIndex()
    {
        return m_IconIndex;
    }

    void StepIcon(int direction)
    {
        if (m_IconPaths.Count() == 0)
            return;

        m_IconIndex = m_IconIndex + direction;

        if (m_IconIndex < 0)
            m_IconIndex = m_IconPaths.Count() - 1;

        if (m_IconIndex >= m_IconPaths.Count())
            m_IconIndex = 0;

        Save();
    }

    protected void Load()
    {
        if (!FileExist("$profile:SparkZ"))
            MakeDirectory("$profile:SparkZ");

        if (!FileExist(SETTINGS_DIR))
            MakeDirectory(SETTINGS_DIR);

        if (!FileExist(SETTINGS_FILE))
        {
            Save();
            return;
        }

        SparkZQuickPingProfileSettings settings = new SparkZQuickPingProfileSettings();
        JsonFileLoader<SparkZQuickPingProfileSettings>.JsonLoadFile(SETTINGS_FILE, settings);
        m_ColorARGB = settings.ColorARGB;
        m_IconIndex = settings.IconIndex;

        if (m_IconIndex < 0 || m_IconIndex >= m_IconPaths.Count())
            m_IconIndex = 0;
    }

    protected void Save()
    {
        if (!FileExist("$profile:SparkZ"))
            MakeDirectory("$profile:SparkZ");

        if (!FileExist(SETTINGS_DIR))
            MakeDirectory(SETTINGS_DIR);

        SparkZQuickPingProfileSettings settings = new SparkZQuickPingProfileSettings();
        settings.ColorARGB = m_ColorARGB;
        settings.IconIndex = m_IconIndex;
        JsonFileLoader<SparkZQuickPingProfileSettings>.JsonSaveFile(SETTINGS_FILE, settings);
    }

    protected void AddIcon(string displayName, string fileName)
    {
        m_IconNames.Insert(displayName);
        m_IconPaths.Insert("SparkZGroup\\gui\\icons\\markers\\" + fileName);
    }
}
