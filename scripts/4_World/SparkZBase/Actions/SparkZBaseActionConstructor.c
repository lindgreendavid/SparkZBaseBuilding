modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
        actions.Insert(SparkZActionClaimBase);
        actions.Insert(SparkZActionPayBaseUpkeep);
        actions.Insert(SparkZActionCheckBaseStatus);
    }
}
