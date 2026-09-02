modded class MissionGameplay
{
    protected bool m_SparkZCoreNotifyGuiUpdateRegistered;
    protected int m_SparkZCoreNotifyLastUpdateFrameMs;

    void MissionGameplay()
    {
        SparkZCoreNotify_RegisterGuiUpdate();
    }

    void ~MissionGameplay()
    {
        SparkZCoreNotify_UnregisterGuiUpdate();
        SparkZCoreEventNotificationHud.Delete();
        SparkZCoreEventNotificationState.Delete();
    }

    override void OnUpdate(float timeslice)
    {
        SparkZCoreNotify_RegisterGuiUpdate();
        super.OnUpdate(timeslice);
        SparkZCoreNotify_OnGuiUpdate(timeslice);
    }

    override void OnInit()
    {
        super.OnInit();
        SparkZCoreNotify_RegisterGuiUpdate();
    }

    protected void SparkZCoreNotify_RegisterGuiUpdate()
    {
        if (m_SparkZCoreNotifyGuiUpdateRegistered || !GetGame())
            return;

        GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(this.SparkZCoreNotify_OnGuiUpdate);
        m_SparkZCoreNotifyGuiUpdateRegistered = true;
    }

    protected void SparkZCoreNotify_UnregisterGuiUpdate()
    {
        if (!m_SparkZCoreNotifyGuiUpdateRegistered || !GetGame())
            return;

        GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(this.SparkZCoreNotify_OnGuiUpdate);
        m_SparkZCoreNotifyGuiUpdateRegistered = false;
    }

    protected void SparkZCoreNotify_OnGuiUpdate(float timeslice)
    {
        if (!GetGame())
            return;

        int currentFrameMs = GetGame().GetTime();
        if (m_SparkZCoreNotifyLastUpdateFrameMs == currentFrameMs)
            return;

        m_SparkZCoreNotifyLastUpdateFrameMs = currentFrameMs;
        SparkZCoreEventNotificationHud.Get().Update(timeslice);
    }
}
