modded class TerritoryFlag
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(SparkZActionClaimBase);
        AddAction(SparkZActionPayBaseUpkeep);
        AddAction(SparkZActionCheckBaseStatus);
    }
}
