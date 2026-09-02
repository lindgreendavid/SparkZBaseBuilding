modded class MissionGameplay
{
    protected int m_SparkZBase_LastMenuToggleMs;
    protected bool m_SparkZBase_GuiUpdateRegistered;

    override void OnKeyPress(int key)
    {
        if (key == KeyCode.KC_J)
        {
            SparkZBase_HandlePropertyMenuKeyPress();
            return;
        }

        if (key == KeyCode.KC_ESCAPE && SparkZBasePropertyMenuManager.Get().IsOpen())
        {
            SparkZBasePropertyMenuManager.Get().Close();
            return;
        }

        super.OnKeyPress(key);
    }

    override void OnInit()
    {
        super.OnInit();
        SparkZBase_RegisterGuiUpdate();
    }

    override void OnMissionStart()
    {
        super.OnMissionStart();
        SparkZBase_RegisterGuiUpdate();
    }

    protected void SparkZBase_HandlePropertyMenuKeyPress()
    {
        if (!GetGame())
            return;

        int currentMs = GetGame().GetTime();
        if (currentMs - m_SparkZBase_LastMenuToggleMs < 275)
            return;

        m_SparkZBase_LastMenuToggleMs = currentMs;
        SparkZBasePropertyMenuManager.Get().Toggle();
    }

    protected void SparkZBase_RegisterGuiUpdate()
    {
        if (m_SparkZBase_GuiUpdateRegistered || !GetGame())
            return;

        GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(this.SparkZBase_OnGuiUpdate);
        m_SparkZBase_GuiUpdateRegistered = true;
    }

    protected void SparkZBase_OnGuiUpdate(float timeslice)
    {
        SparkZBasePropertyMenuManager.Get().Poll();
    }
}
