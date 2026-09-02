class SparkZCoreMarkerPayload
{
    string Id;
    string Label;
    vector Position;
    int ColorARGB;
    string IconPath;
    int Channel;
    string OwnerId;
    string GroupId;
    int Revision;
    float ExpiresAtMs;
    bool Show3D;

    void SparkZCoreMarkerPayload()
    {
        Id = "";
        Label = "";
        Position = "0 0 0";
        ColorARGB = ARGB(255, 255, 255, 255);
        IconPath = "";
        Channel = 0;
        OwnerId = "";
        GroupId = "";
        Revision = 0;
        ExpiresAtMs = 0.0;
        Show3D = false;
    }

    void Set(string id, string label, vector position, int colorARGB, string iconPath, int channel, string ownerId, string groupId, int revision, float expiresAtMs, bool show3D)
    {
        Id = id;
        Label = label;
        Position = position;
        ColorARGB = colorARGB;
        IconPath = iconPath;
        Channel = channel;
        OwnerId = ownerId;
        GroupId = groupId;
        Revision = revision;
        ExpiresAtMs = expiresAtMs;
        Show3D = show3D;
    }

    bool IsValid()
    {
        return Id != "" && Label != "";
    }

    void WriteToContext(ParamsWriteContext ctx)
    {
        ctx.Write(Id);
        ctx.Write(Label);
        ctx.Write(Position);
        ctx.Write(ColorARGB);
        ctx.Write(IconPath);
        ctx.Write(Channel);
        ctx.Write(OwnerId);
        ctx.Write(GroupId);
        ctx.Write(Revision);
        ctx.Write(ExpiresAtMs);
        ctx.Write(Show3D);
    }

    bool ReadFromContext(ParamsReadContext ctx)
    {
        if (!ctx.Read(Id))
            return false;

        if (!ctx.Read(Label))
            return false;

        if (!ctx.Read(Position))
            return false;

        if (!ctx.Read(ColorARGB))
            return false;

        if (!ctx.Read(IconPath))
            return false;

        if (!ctx.Read(Channel))
            return false;

        if (!ctx.Read(OwnerId))
            return false;

        if (!ctx.Read(GroupId))
            return false;

        if (!ctx.Read(Revision))
            return false;

        if (!ctx.Read(ExpiresAtMs))
            return false;

        if (!ctx.Read(Show3D))
            return false;

        return IsValid();
    }
}
