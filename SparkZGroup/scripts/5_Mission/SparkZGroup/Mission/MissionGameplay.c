modded class MissionGameplay
{
    protected bool m_SparkZGroup_GuiUpdateRegistered;
    protected int m_SparkZGroup_LastUpdateFrameMs;
    protected int m_SparkZGroup_LastMapToggleMs;

    void MissionGameplay()
    {
        SparkZGroup_RegisterGuiUpdate();
    }

    void ~MissionGameplay()
    {
        SparkZGroup_UnregisterGuiUpdate();
        SparkZCoreEventNotificationHud.Delete();
        SparkZCoreEventNotificationState.Delete();
    }

    override void OnUpdate(float timeslice)
    {
        SparkZGroup_RegisterGuiUpdate();
        super.OnUpdate(timeslice);
        SparkZGroup_OnGuiUpdate(timeslice);
    }

    override void OnInit()
    {
        super.OnInit();
        SparkZGroup_RegisterGuiUpdate();
        SparkZCoreLogger.Info("SparkZGroup MissionGameplay OnInit hook active. Tactical map input bridge ready.");
    }

    override void OnMissionStart()
    {
        super.OnMissionStart();
        SparkZGroup_RegisterGuiUpdate();
        SparkZCoreLogger.Info("SparkZGroup MissionGameplay OnMissionStart hook active.");
    }

    override void OnKeyPress(int key)
    {
        if (key == KeyCode.KC_M)
        {
            SparkZGroup_HandleTacticalMenuKeyPress("OnKeyPress");
            return;
        }

        super.OnKeyPress(key);
    }

    protected void SparkZGroup_HandleTacticalMenuKeyPress(string source)
    {
        if (!GetGame())
            return;

        int currentMs = GetGame().GetTime();
        if (currentMs - m_SparkZGroup_LastMapToggleMs < 275)
            return;

        m_SparkZGroup_LastMapToggleMs = currentMs;
        SparkZCoreLogger.Debug("SparkZ tactical menu toggle requested from " + source + ".");
        SparkZTacticalMenuManager.Get().HandleMapToggleInput();
    }

    protected void SparkZGroup_RegisterGuiUpdate()
    {
        if (m_SparkZGroup_GuiUpdateRegistered || !GetGame())
            return;

        GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(this.SparkZGroup_OnGuiUpdate);
        m_SparkZGroup_GuiUpdateRegistered = true;
        SparkZCoreLogger.Info("SparkZGroup GUI update hook registered.");
    }

    protected void SparkZGroup_UnregisterGuiUpdate()
    {
        if (!m_SparkZGroup_GuiUpdateRegistered || !GetGame())
            return;

        GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(this.SparkZGroup_OnGuiUpdate);
        m_SparkZGroup_GuiUpdateRegistered = false;
    }

    protected void SparkZGroup_OnGuiUpdate(float timeslice)
    {
        if (!GetGame())
            return;

        int currentFrameMs = GetGame().GetTime();
        if (m_SparkZGroup_LastUpdateFrameMs == currentFrameMs)
            return;

        m_SparkZGroup_LastUpdateFrameMs = currentFrameMs;
        SparkZGroup_HandleMapToggleInputFallback();
        SparkZGroup_HandleQuickPingInput();
        SparkZQuickPing3D.Get().Update(timeslice);
        SparkZLocalMarker3D.Get().Update(timeslice);
        SparkZSquadHud.Get().Update(timeslice);
        SparkZTeammate3D.Get().Update(timeslice);
        SparkZCoreEventNotificationHud.Get().Update(timeslice);
    }

    protected bool SparkZGroup_HandleMapToggleInputFallback()
    {
        if (!GetGame() || !GetGame().GetInput())
            return false;

        SparkZTacticalMenuManager menuManager = SparkZTacticalMenuManager.Get();
        if (menuManager.ShouldSuppressMapToggleInput())
            return false;

        if (GetGame().GetUIManager() && GetGame().GetUIManager().GetMenu() && !menuManager.IsOpen())
            return false;

        if (!SparkZGroup_IsMapTogglePressed())
            return false;

        SparkZGroup_HandleTacticalMenuKeyPress("input fallback");
        return true;
    }

    protected bool SparkZGroup_IsMapTogglePressed()
    {
        if (GetUApi())
        {
            UAInput sparkZMapInput = GetUApi().GetInputByName(SparkZGroupConstants.INPUT_MAP_SPARKZ);
            if (sparkZMapInput && sparkZMapInput.LocalPress())
            {
                sparkZMapInput.Supress();
                SparkZCoreLogger.Debug("SparkZ tactical menu UApi input pressed: " + SparkZGroupConstants.INPUT_MAP_SPARKZ + ".");
                return true;
            }

            UAInput vanillaMapInput = GetUApi().GetInputByName(SparkZGroupConstants.INPUT_MAP_FALLBACK);
            if (vanillaMapInput && vanillaMapInput.LocalPress())
            {
                vanillaMapInput.Supress();
                SparkZCoreLogger.Debug("SparkZ tactical menu UApi input pressed: " + SparkZGroupConstants.INPUT_MAP_FALLBACK + ".");
                return true;
            }
        }

        if (GetGame().GetInput().LocalPress(SparkZGroupConstants.INPUT_MAP_TOGGLE, false))
            return true;

        if (GetGame().GetInput().LocalPress(SparkZGroupConstants.INPUT_MAP_FALLBACK, false))
            return true;

        if (GetGame().GetInput().LocalPress(SparkZGroupConstants.INPUT_MAP_SPARKZ, false))
            return true;

        return false;
    }

    protected bool SparkZGroup_HandleQuickPingInput()
    {
        if (!GetUApi())
            return false;

        if (SparkZTacticalMenuManager.Get().IsOpen())
            return false;

        UAInput quickPingInput = GetUApi().GetInputByName(SparkZGroupConstants.INPUT_QUICK_PING);
        if (!quickPingInput || !quickPingInput.LocalPress())
            return false;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player || !player.IsAlive() || player.IsUnconscious())
            return false;

        vector pingPosition = SparkZGroup_GetQuickPingPosition(player);
        SparkZCoreMapPoint quickPing = SparkZGroupMarkerCache.Get().AddOrMoveQuickPing(pingPosition, SparkZGroupConstants.QUICK_PING_TTL_SECONDS);
        SparkZQuickPing3D.Get().SetMarker(quickPing);
        SparkZGroup_SendQuickPingToServer(player, pingPosition);

        SparkZCoreLogger.Info("SparkZ quick group ping moved.");
        quickPingInput.Supress();
        return true;
    }

    protected void SparkZGroup_SendQuickPingToServer(PlayerBase player, vector pingPosition)
    {
        if (!player || !SparkZLocalGroupSession.Get().IsActive())
            return;

        ScriptRPC rpc = new ScriptRPC();
        rpc.Write(pingPosition);
        rpc.Write(SparkZQuickPingProfile.Get().GetColor());
        rpc.Write(SparkZQuickPingProfile.Get().GetIconPath());
        rpc.Send(player, SparkZCoreRPCId.SPARKZ_RPC_GROUP_PING_CREATE_REQUEST, true, null);
    }

    protected vector SparkZGroup_GetQuickPingPosition(PlayerBase player)
    {
        vector cameraPosition = GetGame().GetCurrentCameraPosition();
        vector cameraDirection = GetGame().GetCurrentCameraDirection().Normalized() * SparkZGroupConstants.QUICK_PING_DISTANCE_METERS;
        Object hitObject;
        vector hitPosition;
        vector hitNormal;
        float hitFraction;
        PhxInteractionLayers layers = PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.VEHICLE_NOTERRAIN | PhxInteractionLayers.BUILDING | PhxInteractionLayers.CHARACTER | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.FIREGEOM | PhxInteractionLayers.DOOR | PhxInteractionLayers.WATERLAYER | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.FENCE | PhxInteractionLayers.AI;

        if (DayZPhysics.RayCastBullet(cameraPosition, cameraPosition + cameraDirection, layers, player, hitObject, hitPosition, hitNormal, hitFraction))
            return hitPosition;

        vector fallbackPosition = player.GetPosition() + (player.GetDirection() * SparkZGroupConstants.QUICK_PING_FALLBACK_DISTANCE_METERS);
        fallbackPosition[1] = GetGame().SurfaceY(fallbackPosition[0], fallbackPosition[2]);
        return fallbackPosition;
    }
}
