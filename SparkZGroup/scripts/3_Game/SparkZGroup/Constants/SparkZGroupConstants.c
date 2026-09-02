class SparkZGroupConstants
{
    static const string MOD_NAME = "SparkZGroup";
    static const string MOD_VERSION = "1.1.0-PERM-REVOKE-FONT-FIX";
    static const string LOG_PREFIX = "[SparkZGroup]";
    static const string LAYOUT_TACTICAL_MENU = "SparkZGroup/gui/layouts/menus/sparkz_tactical_menu.layout";
    static const string LAYOUT_QUICK_PING_3D = "SparkZGroup/gui/layouts/hud/sparkz_quick_ping_3d.layout";
    static const string LAYOUT_MARKER_3D = "SparkZGroup/gui/layouts/hud/sparkz_marker_3d.layout";
    static const string LAYOUT_SQUAD_HUD = "SparkZGroup/gui/layouts/hud/sparkz_squad_hud.layout";
    static const string LAYOUT_TEAMMATE_3D = "SparkZGroup/gui/layouts/hud/sparkz_teammate_3d.layout";
    static const string INPUT_MAP_TOGGLE = "UAMapToggle";
    static const string INPUT_MAP_FALLBACK = "UAMap";
    static const string INPUT_MAP_SPARKZ = "UASparkZMap";
    static const string INPUT_UI_BACK = "UAUIBack";
    static const string INPUT_QUICK_PING = "UASparkZQuickPing";
    static const float QUICK_PING_DISTANCE_METERS = 2000.0;
    static const float QUICK_PING_FALLBACK_DISTANCE_METERS = 60.0;
    static const float QUICK_PING_TTL_SECONDS = 10.0;
    static const float SQUAD_NAME_TAG_MAX_DISTANCE_METERS = 1000.0;
    static const int SQUAD_ROLE_LEADER = 1;
    static const int SQUAD_PERMISSION_BASE_ACCESS = 2;
    static const int SQUAD_PERMISSION_BASE_MANAGER = 16;
    static const int SQUAD_PERMISSION_BASE_BUILD = 2;
    static const int SQUAD_PERMISSION_BASE_DOORS = 4;
    static const int SQUAD_PERMISSION_BASE_STORAGE = 8;
    static const int SQUAD_PERMISSION_BASE_LEGACY_ALL = 14;
    static const int SQUAD_PERMISSION_BASE_ALL = 18;
    static const int SQUAD_PERMISSION_MANAGEABLE_ALL = 19;

    static string GetCoreDependencyName()
    {
        return SparkZCoreConstants.MOD_NAME;
    }
}
