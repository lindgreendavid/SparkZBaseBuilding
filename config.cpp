class CfgPatches
{
    class SparkZBase
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "DZ_Data",
            "DZ_Gear_Camping",
            "DZ_Gear_Containers",
            "DZ_Gear_Tools",
            "SparkZCore",
            "SparkZGroup",
            "BaseBuildingPlus",
            "SparkZNote_Data",
            "CodeLock"
        };
    };
};

class CfgMods
{
    class SparkZBase
    {
        dir = "SparkZBase";
        name = "SparkZBase";
        picture = "";
        logo = "";
        logoSmall = "";
        logoOver = "";
        action = "";
        hideName = 0;
        hidePicture = 0;
        credits = "";
        author = "SparkZ";
        authorID = "0";
        version = "0.1.0";
        extra = 0;
        type = "mod";
        dependencies[] =
        {
            "Game",
            "World",
            "Mission"
        };

        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] =
                {
                    "SparkZBase/scripts/3_Game"
                };
            };

            class worldScriptModule
            {
                value = "";
                files[] =
                {
                    "SparkZBase/scripts/4_World"
                };
            };

            class missionScriptModule
            {
                value = "";
                files[] =
                {
                    "SparkZBase/scripts/5_Mission"
                };
            };
        };
    };
};
