class SparkZCoreMapPoint
{
    string Id;
    string Label;
    vector Position;
    int ColorARGB;
    int Revision;

    void SparkZCoreMapPoint()
    {
        Id = "";
        Label = "";
        Position = "0 0 0";
        ColorARGB = ARGB(255, 255, 255, 255);
        Revision = 0;
    }

    void Set(string id, string label, vector position, int colorARGB, int revision)
    {
        Id = id;
        Label = label;
        Position = position;
        ColorARGB = colorARGB;
        Revision = revision;
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
        ctx.Write(Revision);
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

        if (!ctx.Read(Revision))
            return false;

        return IsValid();
    }
}
