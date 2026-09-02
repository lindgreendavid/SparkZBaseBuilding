class SparkZGroupLoadInfo
{
    string ModName;
    string Version;
    string RequiredCore;

    void SparkZGroupLoadInfo()
    {
        ModName = SparkZGroupConstants.MOD_NAME;
        Version = SparkZGroupConstants.MOD_VERSION;
        RequiredCore = SparkZGroupConstants.GetCoreDependencyName();
    }
}
