class SparkZBaseRecord
{
    string BaseId;
    string OwnerGroupId;
    string OwnerSquadName;
    vector FlagPosition;
    int RadiusMeters;
    int CreatedAt;
    int LastPaidAt;
    int LastDailyCost;
    bool Abandoned;

    void SparkZBaseRecord()
    {
        BaseId = "";
        OwnerGroupId = "";
        OwnerSquadName = "";
        FlagPosition = "0 0 0";
        RadiusMeters = 60;
        CreatedAt = 0;
        LastPaidAt = 0;
        LastDailyCost = 0;
        Abandoned = false;
    }

    bool IsValid()
    {
        return BaseId != "" && OwnerGroupId != "";
    }
}

class SparkZBasePersistentStore
{
    int Version;
    ref array<ref SparkZBaseRecord> Bases;

    void SparkZBasePersistentStore()
    {
        Version = 1;
        Bases = new array<ref SparkZBaseRecord>();
    }
}
