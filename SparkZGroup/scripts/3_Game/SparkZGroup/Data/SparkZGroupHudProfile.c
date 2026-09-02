class SparkZGroupHudProfile
{
    static const string SETTINGS_DIR = "$profile:SparkZ/Group";
    static const string SETTINGS_FILE = "$profile:SparkZ/Group/SquadHudProfile.json";
    protected static ref SparkZGroupHudProfile s_Instance;
    protected int m_NameTagColorARGB;

    void SparkZGroupHudProfile()
    {
        m_NameTagColorARGB = ARGB(255, 155, 225, 255);
        Load();
    }

    static SparkZGroupHudProfile Get()
    {
        if (!s_Instance)
            s_Instance = new SparkZGroupHudProfile();

        return s_Instance;
    }

    static void Delete()
    {
        s_Instance = null;
    }

    int GetNameTagColor()
    {
        return m_NameTagColorARGB;
    }

    void SetNameTagColor(int colorARGB)
    {
        m_NameTagColorARGB = colorARGB;
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

        SparkZGroupHudProfileSettings settings = new SparkZGroupHudProfileSettings();
        JsonFileLoader<SparkZGroupHudProfileSettings>.JsonLoadFile(SETTINGS_FILE, settings);
        m_NameTagColorARGB = settings.NameTagColorARGB;
    }

    protected void Save()
    {
        if (!FileExist("$profile:SparkZ"))
            MakeDirectory("$profile:SparkZ");

        if (!FileExist(SETTINGS_DIR))
            MakeDirectory(SETTINGS_DIR);

        SparkZGroupHudProfileSettings settings = new SparkZGroupHudProfileSettings();
        settings.NameTagColorARGB = m_NameTagColorARGB;
        JsonFileLoader<SparkZGroupHudProfileSettings>.JsonSaveFile(SETTINGS_FILE, settings);
    }
}
