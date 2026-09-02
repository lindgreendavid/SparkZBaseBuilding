class SparkZGroupPersistentMember
{
    string PlainId;
    string Name;
    int RoleFlags;

    void SparkZGroupPersistentMember()
    {
        PlainId = "";
        Name = "";
        RoleFlags = 0;
    }

    void Set(string plainId, string name, int roleFlags)
    {
        PlainId = plainId;
        Name = name;
        RoleFlags = roleFlags;
    }

    bool IsValid()
    {
        return PlainId != "";
    }
}

class SparkZGroupPersistentRecord
{
    string GroupId;
    string Name;
    string Tag;
    string LeaderId;
    int Revision;
    ref array<ref SparkZGroupPersistentMember> Members;
    ref array<ref SparkZCoreMarkerPayload> Markers;

    void SparkZGroupPersistentRecord()
    {
        GroupId = "";
        Name = "";
        Tag = "";
        LeaderId = "";
        Revision = 0;
        Members = new array<ref SparkZGroupPersistentMember>();
        Markers = new array<ref SparkZCoreMarkerPayload>();
    }

    bool IsValid()
    {
        return GroupId != "" && Name != "" && LeaderId != "" && Members && Members.Count() > 0;
    }
}

class SparkZGroupPersistentStore
{
    int Version;
    ref array<ref SparkZGroupPersistentRecord> Groups;

    void SparkZGroupPersistentStore()
    {
        Version = 1;
        Groups = new array<ref SparkZGroupPersistentRecord>();
    }
}
