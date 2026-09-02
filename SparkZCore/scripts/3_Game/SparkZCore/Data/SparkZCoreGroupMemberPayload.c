class SparkZCoreGroupMemberPayload
{
    string PlainId;
    string Name;
    int RoleFlags;
    bool Online;
    bool HasLiveState;
    vector Position;
    float Health01;
    float DisplayHeightOffset;

    void SparkZCoreGroupMemberPayload()
    {
        PlainId = "";
        Name = "";
        RoleFlags = 0;
        Online = false;
        HasLiveState = false;
        Position = "0 0 0";
        Health01 = 0.0;
        DisplayHeightOffset = 1.95;
    }

    void Set(string plainId, string name, int roleFlags, bool online)
    {
        PlainId = plainId;
        Name = name;
        RoleFlags = roleFlags;
        Online = online;
    }

    void SetLiveState(vector position, float health01, bool hasLiveState)
    {
        Position = position;
        Health01 = health01;
        HasLiveState = hasLiveState;
    }

    void SetLiveStateEx(vector position, float health01, float displayHeightOffset, bool hasLiveState)
    {
        Position = position;
        Health01 = health01;
        DisplayHeightOffset = displayHeightOffset;
        HasLiveState = hasLiveState;
    }

    bool IsValid()
    {
        return PlainId != "";
    }

    void WriteToContext(ParamsWriteContext ctx)
    {
        ctx.Write(PlainId);
        ctx.Write(Name);
        ctx.Write(RoleFlags);
        ctx.Write(Online);
        ctx.Write(HasLiveState);
        ctx.Write(Position);
        ctx.Write(Health01);
        ctx.Write(DisplayHeightOffset);
    }

    bool ReadFromContext(ParamsReadContext ctx)
    {
        if (!ctx.Read(PlainId))
            return false;

        if (!ctx.Read(Name))
            return false;

        if (!ctx.Read(RoleFlags))
            return false;

        if (!ctx.Read(Online))
            return false;

        if (!ctx.Read(HasLiveState))
            return false;

        if (!ctx.Read(Position))
            return false;

        if (!ctx.Read(Health01))
            return false;

        if (!ctx.Read(DisplayHeightOffset))
            DisplayHeightOffset = 1.95;

        return IsValid();
    }
}
