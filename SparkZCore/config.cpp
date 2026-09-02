class CfgPatches
{
    class SparkZCore
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] =
        {
            "DZ_Data"
        };
    };
};

class CfgMods
{
    class SparkZCore
    {
        dir = "SparkZCore";
        name = "SparkZCore";
        picture = "SparkZCore\\gui\\branding\\loading\\SZLoading5.paa";
        logo = "SparkZCore\\gui\\branding\\loading\\SZLoading5.paa";
        logoSmall = "SparkZCore\\gui\\branding\\loading\\SZLoading5.paa";
        logoOver = "SparkZCore\\gui\\branding\\loading\\SZLoading5.paa";
        action = "";
        hideName = 0;
        hidePicture = 0;
        credits = "";
        author = "SparkZ";
        authorID = "0";
        version = "0.4.8";
        extra = 0;
        type = "mod";
        dependencies[] =
        {
            "Game",
            "World"
        };

        class defs
        {
            class gameScriptModule
            {
                value = "";
                files[] =
                {
                    "SparkZCore/scripts/3_Game"
                };
            };

            class worldScriptModule
            {
                value = "";
                files[] =
                {
                    "SparkZCore/scripts/4_World"
                };
            };

        };
    };
};
