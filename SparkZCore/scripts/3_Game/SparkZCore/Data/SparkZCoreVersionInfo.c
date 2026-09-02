class SparkZCoreVersionInfo
{
    string ModName;
    string Version;
    int ProtocolVersion;

    void SparkZCoreVersionInfo()
    {
        ModName = SparkZCoreConstants.MOD_NAME;
        Version = SparkZCoreConstants.MOD_VERSION;
        ProtocolVersion = 2;
    }

    void WriteToContext(ParamsWriteContext ctx)
    {
        ctx.Write(ModName);
        ctx.Write(Version);
        ctx.Write(ProtocolVersion);
    }

    bool ReadFromContext(ParamsReadContext ctx)
    {
        if (!ctx.Read(ModName))
            return false;

        if (!ctx.Read(Version))
            return false;

        if (!ctx.Read(ProtocolVersion))
            return false;

        return true;
    }
}
