class SparkZBaseStatusPayload
{
    bool IsValid;
    string Message;
    string BaseId;
    string OwnerSquadName;
    int RadiusMeters;
    int GraceDays;
    int DaysOwed;
    int SecondsUntilNextCharge;
    int AmountOwed;
    int BaseCost;
    int BBPParts;
    int BBPPartsCost;
    int Doors;
    int DoorsCost;
    int Storage;
    int StorageCost;
    int DailyCost;
    int PlayerNotes;
    bool Abandoned;
    bool IsLeader;

    void SparkZBaseStatusPayload()
    {
        IsValid = false;
        Message = "";
        BaseId = "";
        OwnerSquadName = "";
        RadiusMeters = 0;
        GraceDays = 0;
        DaysOwed = 0;
        SecondsUntilNextCharge = 0;
        AmountOwed = 0;
        BaseCost = 0;
        BBPParts = 0;
        BBPPartsCost = 0;
        Doors = 0;
        DoorsCost = 0;
        Storage = 0;
        StorageCost = 0;
        DailyCost = 0;
        PlayerNotes = 0;
        Abandoned = false;
        IsLeader = false;
    }

    void WriteToContext(Serializer ctx)
    {
        ctx.Write(IsValid);
        ctx.Write(Message);
        ctx.Write(BaseId);
        ctx.Write(OwnerSquadName);
        ctx.Write(RadiusMeters);
        ctx.Write(GraceDays);
        ctx.Write(DaysOwed);
        ctx.Write(SecondsUntilNextCharge);
        ctx.Write(AmountOwed);
        ctx.Write(BaseCost);
        ctx.Write(BBPParts);
        ctx.Write(BBPPartsCost);
        ctx.Write(Doors);
        ctx.Write(DoorsCost);
        ctx.Write(Storage);
        ctx.Write(StorageCost);
        ctx.Write(DailyCost);
        ctx.Write(PlayerNotes);
        ctx.Write(Abandoned);
        ctx.Write(IsLeader);
    }

    bool ReadFromContext(ParamsReadContext ctx)
    {
        if (!ctx.Read(IsValid)) return false;
        if (!ctx.Read(Message)) return false;
        if (!ctx.Read(BaseId)) return false;
        if (!ctx.Read(OwnerSquadName)) return false;
        if (!ctx.Read(RadiusMeters)) return false;
        if (!ctx.Read(GraceDays)) return false;
        if (!ctx.Read(DaysOwed)) return false;
        if (!ctx.Read(SecondsUntilNextCharge)) return false;
        if (!ctx.Read(AmountOwed)) return false;
        if (!ctx.Read(BaseCost)) return false;
        if (!ctx.Read(BBPParts)) return false;
        if (!ctx.Read(BBPPartsCost)) return false;
        if (!ctx.Read(Doors)) return false;
        if (!ctx.Read(DoorsCost)) return false;
        if (!ctx.Read(Storage)) return false;
        if (!ctx.Read(StorageCost)) return false;
        if (!ctx.Read(DailyCost)) return false;
        if (!ctx.Read(PlayerNotes)) return false;
        if (!ctx.Read(Abandoned)) return false;
        if (!ctx.Read(IsLeader)) return false;
        return true;
    }
}
