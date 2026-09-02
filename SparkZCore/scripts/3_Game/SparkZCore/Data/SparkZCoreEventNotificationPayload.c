class SparkZCoreEventNotificationPayload
{
    string Title;
    string Message;
    string IconPath;
    int ColorARGB;
    int DurationMs;

    void SparkZCoreEventNotificationPayload()
    {
        Title = "";
        Message = "";
        IconPath = "";
        ColorARGB = ARGB(255, 255, 220, 80);
        DurationMs = 9000;
    }

    void Set(string title, string message, string iconPath, int colorARGB, int durationMs)
    {
        Title = title;
        Message = message;
        IconPath = iconPath;
        ColorARGB = colorARGB;
        DurationMs = durationMs;
    }

    void WriteToContext(ParamsWriteContext ctx)
    {
        ctx.Write(Title);
        ctx.Write(Message);
        ctx.Write(IconPath);
        ctx.Write(ColorARGB);
        ctx.Write(DurationMs);
    }

    bool ReadFromContext(ParamsReadContext ctx)
    {
        if (!ctx.Read(Title))
            return false;

        if (!ctx.Read(Message))
            return false;

        if (!ctx.Read(IconPath))
            IconPath = "";

        if (!ctx.Read(ColorARGB))
            ColorARGB = ARGB(255, 255, 220, 80);

        if (!ctx.Read(DurationMs))
            DurationMs = 9000;

        if (DurationMs < 2500)
            DurationMs = 2500;

        if (DurationMs > 20000)
            DurationMs = 20000;

        return Title != "" || Message != "";
    }
}
