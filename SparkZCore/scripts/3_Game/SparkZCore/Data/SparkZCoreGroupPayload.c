class SparkZCoreGroupPayload
{
    string GroupId;
    string Name;
    string Tag;
    string LeaderId;
    string CurrentMemberId;
    int Revision;
    ref array<ref SparkZCoreGroupMemberPayload> Members;

    void SparkZCoreGroupPayload()
    {
        GroupId = "";
        Name = "";
        Tag = "";
        LeaderId = "";
        CurrentMemberId = "";
        Revision = 0;
        Members = new array<ref SparkZCoreGroupMemberPayload>();
    }

    void Set(string groupId, string name, string tag, string leaderId, int revision)
    {
        GroupId = groupId;
        Name = name;
        Tag = tag;
        LeaderId = leaderId;
        Revision = revision;
    }

    bool IsValid()
    {
        return GroupId != "" && Name != "";
    }

    void WriteToContext(ParamsWriteContext ctx)
    {
        ctx.Write(GroupId);
        ctx.Write(Name);
        ctx.Write(Tag);
        ctx.Write(LeaderId);
        ctx.Write(CurrentMemberId);
        ctx.Write(Revision);
        ctx.Write(Members.Count());

        foreach (SparkZCoreGroupMemberPayload member: Members)
        {
            if (member)
            {
                member.WriteToContext(ctx);
            }
            else
            {
                SparkZCoreGroupMemberPayload emptyMember = new SparkZCoreGroupMemberPayload();
                emptyMember.WriteToContext(ctx);
            }
        }
    }

    bool ReadFromContext(ParamsReadContext ctx)
    {
        int count;

        if (!ctx.Read(GroupId))
            return false;

        if (!ctx.Read(Name))
            return false;

        if (!ctx.Read(Tag))
            return false;

        if (!ctx.Read(LeaderId))
            return false;

        if (!ctx.Read(CurrentMemberId))
            return false;

        if (!ctx.Read(Revision))
            return false;

        if (!ctx.Read(count))
            return false;

        if (count < 0 || count > 128)
            return false;

        Members.Clear();
        for (int index = 0; index < count; index++)
        {
            SparkZCoreGroupMemberPayload member = new SparkZCoreGroupMemberPayload();
            if (!member.ReadFromContext(ctx))
                return false;

            Members.Insert(member);
        }

        return true;
    }
}
