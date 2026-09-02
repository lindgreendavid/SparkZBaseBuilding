class SparkZBaseConfig
{
    int Enabled;
    int RequireClaimForBBPBuild;
    int DefaultClaimRadiusMeters;
    int GraceDaysBeforeAbandoned;
    int BaseDailyCost;
    int BBPPartDailyCost;
    int DoorDailyCost;
    int StorageDailyCost;
    string CurrencyClassName;
    int MaxObjectsToScan;

    void SparkZBaseConfig()
    {
        Enabled = 1;
        RequireClaimForBBPBuild = 1;
        DefaultClaimRadiusMeters = 60;
        GraceDaysBeforeAbandoned = 7;
        BaseDailyCost = 500;
        BBPPartDailyCost = 25;
        DoorDailyCost = 75;
        StorageDailyCost = 50;
        CurrencyClassName = "SparkZNote";
        MaxObjectsToScan = 1200;
    }

    static SparkZBaseConfig Load()
    {
        if (!FileExist("$profile:SparkZ"))
            MakeDirectory("$profile:SparkZ");

        if (!FileExist(SparkZBaseConstants.PROFILE_DIR))
            MakeDirectory(SparkZBaseConstants.PROFILE_DIR);

        SparkZBaseConfig config = new SparkZBaseConfig();
        if (FileExist(SparkZBaseConstants.CONFIG_FILE))
            JsonFileLoader<SparkZBaseConfig>.JsonLoadFile(SparkZBaseConstants.CONFIG_FILE, config);

        JsonFileLoader<SparkZBaseConfig>.JsonSaveFile(SparkZBaseConstants.CONFIG_FILE, config);
        return config;
    }
}
