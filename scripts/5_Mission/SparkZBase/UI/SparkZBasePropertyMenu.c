class SparkZBasePropertyMenu: UIScriptedMenu
{
    protected static const string BRAND_LOGO = "SparkZBase\\gui\\branding\\sparkz_base_icon.paa";

    protected ImageWidget m_BrandLogo;
    protected TextWidget m_Title;
    protected TextWidget m_Status;
    protected TextWidget m_TimeText;
    protected TextWidget m_TimeDetailText;
    protected TextWidget m_TotalText;
    protected TextWidget m_NotesText;
    protected Widget m_CloseButton;
    protected Widget m_RefreshButton;
    protected Widget m_PayNowButton;
    protected Widget m_GiveUpButton;
    protected bool m_TextInputActive;

    protected TextWidget m_RowRadiusValue;
    protected TextWidget m_RowBaseFeeValue;
    protected TextWidget m_RowBBPValue;
    protected TextWidget m_RowDoorsValue;
    protected TextWidget m_RowStorageValue;
    protected TextWidget m_RowDaysOwedValue;
    protected TextWidget m_RowGraceValue;
    protected TextWidget m_RowTotalValue;
    protected TextWidget m_HintText;

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets(SparkZBaseConstants.LAYOUT_PROPERTY_MENU);
        if (!layoutRoot)
            return null;

        Class.CastTo(m_BrandLogo, layoutRoot.FindAnyWidget("SparkZBaseBrandLogo"));
        Class.CastTo(m_Title, layoutRoot.FindAnyWidget("SparkZBaseMenuTitle"));
        Class.CastTo(m_Status, layoutRoot.FindAnyWidget("SparkZBaseMenuStatus"));
        Class.CastTo(m_TimeText, layoutRoot.FindAnyWidget("SparkZBaseMenuTime"));
        Class.CastTo(m_TimeDetailText, layoutRoot.FindAnyWidget("SparkZBaseMenuTimeDetail"));
        Class.CastTo(m_TotalText, layoutRoot.FindAnyWidget("SparkZBaseMenuTotal"));
        Class.CastTo(m_NotesText, layoutRoot.FindAnyWidget("SparkZBaseMenuNotes"));
        m_CloseButton = layoutRoot.FindAnyWidget("SparkZBaseMenuClose");
        m_RefreshButton = layoutRoot.FindAnyWidget("SparkZBaseMenuRefresh");
        m_PayNowButton = layoutRoot.FindAnyWidget("SparkZBaseMenuPayNow");
        m_GiveUpButton = layoutRoot.FindAnyWidget("SparkZBaseMenuGiveUp");

        Class.CastTo(m_RowRadiusValue, layoutRoot.FindAnyWidget("SparkZBaseRowRadiusValue"));
        Class.CastTo(m_RowBaseFeeValue, layoutRoot.FindAnyWidget("SparkZBaseRowBaseFeeValue"));
        Class.CastTo(m_RowBBPValue, layoutRoot.FindAnyWidget("SparkZBaseRowBBPValue"));
        Class.CastTo(m_RowDoorsValue, layoutRoot.FindAnyWidget("SparkZBaseRowDoorsValue"));
        Class.CastTo(m_RowStorageValue, layoutRoot.FindAnyWidget("SparkZBaseRowStorageValue"));
        Class.CastTo(m_RowDaysOwedValue, layoutRoot.FindAnyWidget("SparkZBaseRowDaysOwedValue"));
        Class.CastTo(m_RowGraceValue, layoutRoot.FindAnyWidget("SparkZBaseRowGraceValue"));
        Class.CastTo(m_RowTotalValue, layoutRoot.FindAnyWidget("SparkZBaseRowTotalValue"));

        Class.CastTo(m_HintText, layoutRoot.FindAnyWidget("SparkZBaseMenuHint"));

        if (m_BrandLogo)
            m_BrandLogo.LoadImageFile(0, BRAND_LOGO);

        // Layout .layout files do not interpret "\n" as a real line break (it renders
        // literally), so multi-line static text has to be set from script instead, where
        // "\n" is an actual newline character.
        if (m_HintText)
            m_HintText.SetText("OPEN THIS MENU WITH J\nWHILE STANDING INSIDE\nYOUR CLAIMED BASE.");

        SetLoading();
        return layoutRoot;
    }

    override void OnShow()
    {
        super.OnShow();
        if (GetGame() && GetGame().GetUIManager())
            GetGame().GetUIManager().ShowUICursor(true);

        Mission mission = GetGame().GetMission();
        if (mission)
            mission.AddActiveInputExcludes({"movement", "aiming", "menu"});

        SetIngameHudVisible(false);
        PPEffects.SetBlurMenu(0.35);
    }

    override void OnHide()
    {
        super.OnHide();

        Mission mission = GetGame().GetMission();
        if (mission)
        {
            mission.PlayerControlEnable(true);
            mission.RemoveActiveInputExcludes({"menu", "movement", "aiming"}, true);
            mission.RefreshExcludes();
        }

        SetIngameHudVisible(true);
        PPEffects.SetBlurMenu(0);

        if (GetGame() && GetGame().GetUIManager())
            GetGame().GetUIManager().ShowUICursor(false);

        SparkZBasePropertyMenuManager.Get().OnMenuHidden(this);
    }

    protected void SetIngameHudVisible(bool visible)
    {
        if (!GetGame())
            return;

        Mission mission = GetGame().GetMission();
        if (!mission)
            return;

        IngameHud hud = IngameHud.Cast(mission.GetHud());
        if (!hud)
            return;

        hud.ShowQuickbarUI(GetGame().GetProfileOption(EDayZProfilesOptions.QUICKBAR) && visible);
        hud.ShowHudUI(GetGame().GetProfileOption(EDayZProfilesOptions.HUD) && visible);
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (w == m_CloseButton)
        {
            CloseMenu();
            return true;
        }

        if (w == m_RefreshButton)
        {
            SetLoading();
            SparkZBaseNetworkClient.Get().RequestStatus();
            return true;
        }

        if (w == m_PayNowButton)
        {
            SparkZBaseNetworkClient.Get().RequestPayNow();
            return true;
        }

        if (w == m_GiveUpButton)
        {
            SetLoading();
            SparkZBaseNetworkClient.Get().RequestGiveUp();
            return true;
        }

        return super.OnClick(w, x, y, button);
    }

    override bool OnKeyDown(Widget w, int x, int y, int key)
    {
        if (key == KeyCode.KC_ESCAPE || key == KeyCode.KC_J)
        {
            CloseMenu();
            return true;
        }

        return super.OnKeyDown(w, x, y, key);
    }

    void CloseMenu()
    {
        Close();
    }

    void ApplyStatus(SparkZBaseStatusPayload payload)
    {
        if (!payload)
            return;

        if (m_Status)
            m_Status.SetText(payload.Message);

        if (!payload.IsValid)
        {
            if (m_Title) m_Title.SetText("SPARKZ BASE");
            if (m_TimeText) m_TimeText.SetText("NO PROPERTY FOUND");
            if (m_TimeDetailText) m_TimeDetailText.SetText("");
            if (m_TotalText) m_TotalText.SetText("DAILY PAYMENT: -");
            if (m_NotesText) m_NotesText.SetText("SPARKZ NOTES: " + payload.PlayerNotes.ToString());
            SetRowText(m_RowRadiusValue, "-");
            SetRowText(m_RowBaseFeeValue, "-");
            SetRowText(m_RowBBPValue, "-");
            SetRowText(m_RowDoorsValue, "-");
            SetRowText(m_RowStorageValue, "-");
            SetRowText(m_RowDaysOwedValue, "-");
            SetRowText(m_RowGraceValue, "-");
            SetRowText(m_RowTotalValue, "-");
            SetPaymentButtons(false);
            SetGiveUpButton(false);
            return;
        }

        if (m_Title)
            m_Title.SetText(payload.OwnerSquadName + " PROPERTY");

        if (m_TimeText)
            m_TimeText.SetText(BuildTimeHeadline(payload));

        if (m_TimeDetailText)
            m_TimeDetailText.SetText(BuildTimeDetail(payload));

        if (m_TotalText)
            m_TotalText.SetText("DAILY PAYMENT: " + payload.DailyCost.ToString() + " SPARKZ NOTES");

        if (m_NotesText)
            m_NotesText.SetText("SPARKZ NOTES: " + payload.PlayerNotes.ToString());

        SetRowText(m_RowRadiusValue, payload.RadiusMeters.ToString() + "M");
        SetRowText(m_RowBaseFeeValue, payload.BaseCost.ToString());
        SetRowText(m_RowBBPValue, payload.BBPParts.ToString() + " x " + (payload.BBPPartsCost / Math.Max(payload.BBPParts, 1)).ToString() + " = " + payload.BBPPartsCost.ToString());
        SetRowText(m_RowDoorsValue, payload.Doors.ToString() + " = " + payload.DoorsCost.ToString());
        SetRowText(m_RowStorageValue, payload.Storage.ToString() + " = " + payload.StorageCost.ToString());
        SetRowText(m_RowDaysOwedValue, payload.DaysOwed.ToString());
        SetRowText(m_RowGraceValue, payload.GraceDays.ToString() + " DAY(S)");
        SetRowText(m_RowTotalValue, payload.AmountOwed.ToString());

        SetPaymentButtons(!payload.Abandoned && payload.DaysOwed > 0);
        SetGiveUpButton(payload.IsLeader && !payload.Abandoned);
    }

    protected void SetRowText(TextWidget widget, string text)
    {
        if (widget)
            widget.SetText(text);
    }

    protected void SetLoading()
    {
        if (m_Title) m_Title.SetText("SPARKZ BASE");
        if (m_Status) m_Status.SetText("Checking property...");
        if (m_TimeText) m_TimeText.SetText("LOADING");
        if (m_TimeDetailText) m_TimeDetailText.SetText("");
        if (m_TotalText) m_TotalText.SetText("DAILY PAYMENT: -");
        if (m_NotesText) m_NotesText.SetText("SPARKZ NOTES: -");
        SetRowText(m_RowRadiusValue, "-");
        SetRowText(m_RowBaseFeeValue, "-");
        SetRowText(m_RowBBPValue, "-");
        SetRowText(m_RowDoorsValue, "-");
        SetRowText(m_RowStorageValue, "-");
        SetRowText(m_RowDaysOwedValue, "-");
        SetRowText(m_RowGraceValue, "-");
        SetRowText(m_RowTotalValue, "-");
        SetPaymentButtons(false);
        SetGiveUpButton(false);
    }

    protected void SetPaymentButtons(bool enabled)
    {
        if (m_PayNowButton)
            m_PayNowButton.Enable(enabled);
    }

    protected void SetGiveUpButton(bool enabled)
    {
        if (m_GiveUpButton)
            m_GiveUpButton.Enable(enabled);
    }

    protected string BuildTimeHeadline(SparkZBaseStatusPayload payload)
    {
        if (payload.Abandoned)
            return "BASE ABANDONED";

        if (payload.DaysOwed <= 0)
            return "PAID UP";

        return "DAY " + payload.DaysOwed.ToString() + " - OWES " + payload.AmountOwed.ToString();
    }

    protected string BuildTimeDetail(SparkZBaseStatusPayload payload)
    {
        if (payload.Abandoned)
            return "Doors and storage are open for raiding.";

        if (payload.DaysOwed <= 0)
            return "Billed daily - next charge in " + FormatDuration(payload.SecondsUntilNextCharge);

        return "You owe " + payload.AmountOwed.ToString() + " SparkZ notes.";
    }

    protected string FormatDuration(int seconds)
    {
        if (seconds < 0)
            seconds = 0;

        int days = seconds / 86400;
        int hours = (seconds % 86400) / 3600;

        if (days > 0)
            return days.ToString() + " DAY(S) " + hours.ToString() + " HOUR(S)";

        return hours.ToString() + " HOUR(S)";
    }

}
