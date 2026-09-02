class SparkZTacticalMenu: UIScriptedMenu
{
    protected static const int PAGE_MAP = 0;
    protected static const int PAGE_MARKERS = 1;
    protected static const int PAGE_GROUP = 2;
    protected static const int PAGE_INFO = 3;
    protected static const int MAP_SIDEBAR_OVERFLOW_ROW = -999999;
    protected static const string BRAND_LOGO = "SparkZGroup\\gui\\branding\\SZLogoo.paa";
    protected static const string DEFAULT_MARKER_ICON = "SparkZGroup\\gui\\icons\\markers\\marker.paa";
    protected static const string PLAYER_MARKER_ICON = "SparkZGroup\\gui\\icons\\markers\\player.paa";
    protected static const string UI_TEX_LINE_BORDER = "SparkZGroup\\gui\\textures\\ui\\sparkz_line_border.paa";
    protected static const float MARKER_REFRESH_INTERVAL = 0.5;
    protected static const float MARKER_EDIT_SELECT_RADIUS_METERS = 180.0;

    protected ref array<string> m_AvailableMarkerIcons;
    protected ref array<string> m_AvailableMarkerIconNames;

    protected Widget m_CloseButton;
    protected Widget m_TabMap;
    protected Widget m_TabMarkers;
    protected Widget m_TabGroup;
    protected Widget m_TabInfo;
    protected Widget m_TabMapUnderline;
    protected Widget m_TabMarkersUnderline;
    protected Widget m_TabGroupUnderline;
    protected Widget m_TabInfoUnderline;
    protected Widget m_MapLayer;
    protected Widget m_MarkersLayer;
    protected Widget m_GroupLayer;
    protected Widget m_InfoLayer;
    protected ImageWidget m_MapVignette;
    protected ImageWidget m_MapBorderTop;
    protected ImageWidget m_MapBorderLeft;
    protected ImageWidget m_MapBorderRight;
    protected ImageWidget m_MapBorderBottom;
    protected ImageWidget m_BrandLogo;
    protected MapWidget m_MapWidget;
    protected TextWidget m_Title;
    protected TextWidget m_Status;
    protected TextWidget m_PageTitle;
    protected TextWidget m_PageSubtitle;
    protected TextWidget m_PageBody;
    protected TextWidget m_MainTitle;
    protected TextWidget m_GroupSessionText;
    protected TextWidget m_CoordinateReadout;
    protected Widget m_DividerA;
    protected Widget m_DividerB;
    protected Widget m_DividerBottom;
    protected TextWidget m_SidebarStatus;
    protected TextWidget m_Footer;
    protected TextWidget m_StatusBarLeft;
    protected TextWidget m_StatusBarRight;
    protected TextWidget m_MapStatus;
    protected TextWidget m_MarkersEmpty;
    protected TextListboxWidget m_MarkerList;
    protected TextListboxWidget m_GroupMarkerList;
    protected TextListboxWidget m_ServerMarkerList;
    protected Widget m_MapPrivateHeader;
    protected Widget m_MapGroupHeader;
    protected Widget m_MapServerHeader;
    protected ref array<Widget> m_MapPrivateRows;
    protected ref array<TextWidget> m_MapPrivateRowTexts;
    protected ref array<ImageWidget> m_MapPrivateRowIcons;
    protected ref array<Widget> m_MapGroupRows;
    protected ref array<TextWidget> m_MapGroupRowTexts;
    protected ref array<ImageWidget> m_MapGroupRowIcons;
    protected ref array<Widget> m_MapServerRows;
    protected ref array<TextWidget> m_MapServerRowTexts;
    protected ref array<ImageWidget> m_MapServerRowIcons;
    protected Widget m_MapMarkerEditButton;
    protected Widget m_MapMarkerDeleteButton;
    protected TextWidget m_MarkerDetail;
    protected Widget m_MarkerCenterButton;
    protected Widget m_MarkerDeleteButton;
    protected Widget m_Marker3DButton;
    protected Widget m_MarkerClearButton;
    protected Widget m_MarkerChannelPrivateButton;
    protected Widget m_MarkerChannelGroupButton;
    protected Widget m_MarkerChannelServerButton;
    protected EditBoxWidget m_MarkerNameEdit;
    protected Widget m_MarkerNameApplyButton;
    protected SliderWidget m_MarkerRedSlider;
    protected SliderWidget m_MarkerGreenSlider;
    protected SliderWidget m_MarkerBlueSlider;
    protected TextWidget m_MarkerRedValue;
    protected TextWidget m_MarkerGreenValue;
    protected TextWidget m_MarkerBlueValue;
    protected Widget m_MarkerColorPreview;
    protected Widget m_MarkerIconPrevButton;
    protected ImageWidget m_MarkerIconPreview;
    protected TextWidget m_MarkerIconName;
    protected Widget m_MarkerIconNextButton;
    protected TextWidget m_QuickPingProfileText;
    protected SliderWidget m_QuickPingRedSlider;
    protected SliderWidget m_QuickPingGreenSlider;
    protected SliderWidget m_QuickPingBlueSlider;
    protected TextWidget m_QuickPingRedValue;
    protected TextWidget m_QuickPingGreenValue;
    protected TextWidget m_QuickPingBlueValue;
    protected Widget m_QuickPingColorPreview;
    protected Widget m_QuickPingIconPrevButton;
    protected ImageWidget m_QuickPingIconPreview;
    protected TextWidget m_QuickPingIconName;
    protected Widget m_QuickPingIconNextButton;
    protected Widget m_GroupCreateButton;
    protected Widget m_GroupLeaveButton;
    protected Widget m_GroupNameLabel;
    protected Widget m_GroupTagLabel;
    protected EditBoxWidget m_GroupNameEdit;
    protected EditBoxWidget m_GroupTagEdit;
    protected Widget m_GroupIdentityApplyButton;
    protected TextWidget m_GroupInviteLabel;
    protected EditBoxWidget m_GroupInviteNameEdit;
    protected Widget m_GroupInviteButton;
    protected Widget m_GroupKickButton;
    protected Widget m_GroupPromoteButton;
    protected TextListboxWidget m_GroupRosterList;
    protected TextWidget m_GroupSelectedMemberText;
    protected Widget m_GroupPermBuildButton;
    protected Widget m_GroupPermDoorButton;
    protected Widget m_GroupPermStorageButton;
    protected Widget m_GroupPermApplyButton;
    protected TextWidget m_GroupPermStatusText;
    protected TextWidget m_GroupInvitePendingText;
    protected Widget m_GroupInviteAcceptButton;
    protected Widget m_GroupInviteDeclineButton;
    protected TextWidget m_GroupHudProfileText;
    protected SliderWidget m_GroupHudRedSlider;
    protected SliderWidget m_GroupHudGreenSlider;
    protected SliderWidget m_GroupHudBlueSlider;
    protected TextWidget m_GroupHudRedValue;
    protected TextWidget m_GroupHudGreenValue;
    protected TextWidget m_GroupHudBlueValue;
    protected Widget m_GroupHudColorPreview;
    protected TextWidget m_GroupHudPreviewName0;
    protected TextWidget m_GroupHudPreviewName1;
    protected TextWidget m_GroupHudPreviewName2;
    protected Widget m_PrivateMarkerHeader;
    protected Widget m_GroupMarkerHeader;
    protected Widget m_ServerMarkerHeader;
    protected ref array<int> m_PrivateListIndexes;
    protected ref array<int> m_GroupListIndexes;
    protected ref array<ref SparkZCoreMarkerPayload> m_DisplayedRemoteSquadMarkers;
    protected ref array<int> m_ServerListIndexes;
    protected ref array<int> m_MapPrivateListIndexes;
    protected ref array<int> m_MapGroupListIndexes;
    protected ref array<int> m_MapServerListIndexes;
    protected int m_SelectedMarkerIndex = -1;
    protected int m_SelectedServerMarkerIndex = -1;
    protected int m_CurrentPage = PAGE_MAP;
    protected float m_MarkerRefreshTimer;
    protected int m_LastAppliedMarkerCacheRevision = -1;
    protected bool m_SuppressMarkerEditorEvents;
    protected bool m_SuppressQuickPingProfileEvents;
    protected bool m_SuppressGroupHudProfileEvents;
    protected bool m_TextInputActive;
    protected bool m_GroupIdentityDraftDirty;
    protected bool m_GroupIdentityFieldsInitialized;
    protected string m_GroupInviteStatusOverride;
    protected ref array<string> m_GroupRosterIds;
    protected string m_SelectedGroupMemberId;
    protected string m_SelectedGroupMemberName;
    protected int m_SelectedGroupMemberRoleFlags;

    override Widget Init()
    {
        BuildMarkerIconList();
        m_GroupRosterIds = new array<string>();

        layoutRoot = GetGame().GetWorkspace().CreateWidgets(SparkZGroupConstants.LAYOUT_TACTICAL_MENU);
        if (!layoutRoot)
        {
            SparkZCoreLogger.Error("Could not create SparkZ tactical menu layout.");
            return null;
        }

        m_CloseButton = layoutRoot.FindAnyWidget("SparkZCloseButton");
        m_TabMap = layoutRoot.FindAnyWidget("SparkZTabMap");
        m_TabMarkers = layoutRoot.FindAnyWidget("SparkZTabMarkers");
        m_TabGroup = layoutRoot.FindAnyWidget("SparkZTabGroup");
        m_TabInfo = layoutRoot.FindAnyWidget("SparkZTabInfo");
        m_TabMapUnderline = layoutRoot.FindAnyWidget("SparkZTabMapUnderline");
        m_TabMarkersUnderline = layoutRoot.FindAnyWidget("SparkZTabMarkersUnderline");
        m_TabGroupUnderline = layoutRoot.FindAnyWidget("SparkZTabGroupUnderline");
        m_TabInfoUnderline = layoutRoot.FindAnyWidget("SparkZTabInfoUnderline");
        Class.CastTo(m_MapVignette, layoutRoot.FindAnyWidget("SparkZMapVignette"));
        Class.CastTo(m_MapBorderTop, layoutRoot.FindAnyWidget("SparkZMapBorderTop"));
        Class.CastTo(m_MapBorderLeft, layoutRoot.FindAnyWidget("SparkZMapBorderLeft"));
        Class.CastTo(m_MapBorderRight, layoutRoot.FindAnyWidget("SparkZMapBorderRight"));
        Class.CastTo(m_MapBorderBottom, layoutRoot.FindAnyWidget("SparkZMapBorderBottom"));
        m_MapLayer = layoutRoot.FindAnyWidget("SparkZMapLayer");
        m_MarkersLayer = layoutRoot.FindAnyWidget("SparkZMarkersLayer");
        m_GroupLayer = layoutRoot.FindAnyWidget("SparkZGroupLayer");
        m_InfoLayer = layoutRoot.FindAnyWidget("SparkZInfoLayer");
        m_PrivateMarkerHeader = layoutRoot.FindAnyWidget("SparkZPrivateMarkerHeader");
        m_GroupMarkerHeader = layoutRoot.FindAnyWidget("SparkZGroupMarkerHeader");
        m_ServerMarkerHeader = layoutRoot.FindAnyWidget("SparkZServerMarkerHeader");
        Class.CastTo(m_BrandLogo, layoutRoot.FindAnyWidget("SparkZBrandLogo"));
        Class.CastTo(m_MapWidget, layoutRoot.FindAnyWidget("SparkZMapWidget"));
        Class.CastTo(m_Title, layoutRoot.FindAnyWidget("SparkZTitle"));
        Class.CastTo(m_Status, layoutRoot.FindAnyWidget("SparkZStatus"));
        Class.CastTo(m_PageTitle, layoutRoot.FindAnyWidget("SparkZPageTitle"));
        Class.CastTo(m_PageSubtitle, layoutRoot.FindAnyWidget("SparkZPageSubtitle"));
        Class.CastTo(m_PageBody, layoutRoot.FindAnyWidget("SparkZPageBody"));
        Class.CastTo(m_MainTitle, layoutRoot.FindAnyWidget("SparkZMainTitle"));
        Class.CastTo(m_GroupSessionText, layoutRoot.FindAnyWidget("SparkZGroupEmpty"));
        Class.CastTo(m_CoordinateReadout, layoutRoot.FindAnyWidget("SparkZCoordinateReadout"));
        Class.CastTo(m_DividerA, layoutRoot.FindAnyWidget("SparkZDividerA"));
        Class.CastTo(m_DividerB, layoutRoot.FindAnyWidget("SparkZDividerB"));
        Class.CastTo(m_DividerBottom, layoutRoot.FindAnyWidget("SparkZDividerBottom"));
        Class.CastTo(m_SidebarStatus, layoutRoot.FindAnyWidget("SparkZSidebarStatus"));
        Class.CastTo(m_Footer, layoutRoot.FindAnyWidget("SparkZFooter"));
        Class.CastTo(m_StatusBarLeft, layoutRoot.FindAnyWidget("SparkZStatusBarLeft"));
        Class.CastTo(m_StatusBarRight, layoutRoot.FindAnyWidget("SparkZStatusBarRight"));
        Class.CastTo(m_MapStatus, layoutRoot.FindAnyWidget("SparkZMapStatus"));
        Class.CastTo(m_MarkersEmpty, layoutRoot.FindAnyWidget("SparkZMarkersEmpty"));
        Class.CastTo(m_MarkerList, layoutRoot.FindAnyWidget("SparkZMarkerList"));
        Class.CastTo(m_GroupMarkerList, layoutRoot.FindAnyWidget("SparkZGroupMarkerList"));
        Class.CastTo(m_ServerMarkerList, layoutRoot.FindAnyWidget("SparkZServerMarkerList"));
        m_MapPrivateHeader = layoutRoot.FindAnyWidget("SparkZMapPrivateHeader");
        m_MapGroupHeader = layoutRoot.FindAnyWidget("SparkZMapGroupHeader");
        m_MapServerHeader = layoutRoot.FindAnyWidget("SparkZMapServerHeader");
        m_MapMarkerEditButton = layoutRoot.FindAnyWidget("SparkZMapMarkerEditButton");
        m_MapMarkerDeleteButton = layoutRoot.FindAnyWidget("SparkZMapMarkerDeleteButton");
        BuildMapSidebarRows();
        Class.CastTo(m_MarkerDetail, layoutRoot.FindAnyWidget("SparkZMarkerDetail"));
        m_MarkerCenterButton = layoutRoot.FindAnyWidget("SparkZMarkerCenterButton");
        m_MarkerDeleteButton = layoutRoot.FindAnyWidget("SparkZMarkerDeleteButton");
        m_Marker3DButton = layoutRoot.FindAnyWidget("SparkZMarker3DButton");
        m_MarkerClearButton = layoutRoot.FindAnyWidget("SparkZMarkerClearButton");
        m_MarkerChannelPrivateButton = layoutRoot.FindAnyWidget("SparkZMarkerChannelPrivateButton");
        m_MarkerChannelGroupButton = layoutRoot.FindAnyWidget("SparkZMarkerChannelGroupButton");
        m_MarkerChannelServerButton = layoutRoot.FindAnyWidget("SparkZMarkerChannelServerButton");
        Class.CastTo(m_MarkerNameEdit, layoutRoot.FindAnyWidget("SparkZMarkerNameEdit"));
        m_MarkerNameApplyButton = layoutRoot.FindAnyWidget("SparkZMarkerNameApplyButton");
        Class.CastTo(m_MarkerRedSlider, layoutRoot.FindAnyWidget("SparkZMarkerRedSlider"));
        Class.CastTo(m_MarkerGreenSlider, layoutRoot.FindAnyWidget("SparkZMarkerGreenSlider"));
        Class.CastTo(m_MarkerBlueSlider, layoutRoot.FindAnyWidget("SparkZMarkerBlueSlider"));
        Class.CastTo(m_MarkerRedValue, layoutRoot.FindAnyWidget("SparkZMarkerRedValue"));
        Class.CastTo(m_MarkerGreenValue, layoutRoot.FindAnyWidget("SparkZMarkerGreenValue"));
        Class.CastTo(m_MarkerBlueValue, layoutRoot.FindAnyWidget("SparkZMarkerBlueValue"));
        m_MarkerColorPreview = layoutRoot.FindAnyWidget("SparkZMarkerColorPreview");
        m_MarkerIconPrevButton = layoutRoot.FindAnyWidget("SparkZMarkerIconPrevButton");
        Class.CastTo(m_MarkerIconPreview, layoutRoot.FindAnyWidget("SparkZMarkerIconPreview"));
        Class.CastTo(m_MarkerIconName, layoutRoot.FindAnyWidget("SparkZMarkerIconName"));
        m_MarkerIconNextButton = layoutRoot.FindAnyWidget("SparkZMarkerIconNextButton");
        Class.CastTo(m_QuickPingProfileText, layoutRoot.FindAnyWidget("SparkZQuickPingProfileText"));
        Class.CastTo(m_QuickPingRedSlider, layoutRoot.FindAnyWidget("SparkZQuickPingRedSlider"));
        Class.CastTo(m_QuickPingGreenSlider, layoutRoot.FindAnyWidget("SparkZQuickPingGreenSlider"));
        Class.CastTo(m_QuickPingBlueSlider, layoutRoot.FindAnyWidget("SparkZQuickPingBlueSlider"));
        Class.CastTo(m_QuickPingRedValue, layoutRoot.FindAnyWidget("SparkZQuickPingRedValue"));
        Class.CastTo(m_QuickPingGreenValue, layoutRoot.FindAnyWidget("SparkZQuickPingGreenValue"));
        Class.CastTo(m_QuickPingBlueValue, layoutRoot.FindAnyWidget("SparkZQuickPingBlueValue"));
        m_QuickPingColorPreview = layoutRoot.FindAnyWidget("SparkZQuickPingColorPreview");
        m_QuickPingIconPrevButton = layoutRoot.FindAnyWidget("SparkZQuickPingIconPrevButton");
        Class.CastTo(m_QuickPingIconPreview, layoutRoot.FindAnyWidget("SparkZQuickPingIconPreview"));
        Class.CastTo(m_QuickPingIconName, layoutRoot.FindAnyWidget("SparkZQuickPingIconName"));
        m_QuickPingIconNextButton = layoutRoot.FindAnyWidget("SparkZQuickPingIconNextButton");
        m_GroupCreateButton = layoutRoot.FindAnyWidget("SparkZGroupCreateButton");
        m_GroupLeaveButton = layoutRoot.FindAnyWidget("SparkZGroupLeaveButton");
        m_GroupNameLabel = layoutRoot.FindAnyWidget("SparkZGroupNameLabel");
        m_GroupTagLabel = layoutRoot.FindAnyWidget("SparkZGroupTagLabel");
        Class.CastTo(m_GroupNameEdit, layoutRoot.FindAnyWidget("SparkZGroupNameEdit"));
        Class.CastTo(m_GroupTagEdit, layoutRoot.FindAnyWidget("SparkZGroupTagEdit"));
        m_GroupIdentityApplyButton = layoutRoot.FindAnyWidget("SparkZGroupIdentityApplyButton");
        Class.CastTo(m_GroupInviteLabel, layoutRoot.FindAnyWidget("SparkZGroupInviteLabel"));
        Class.CastTo(m_GroupInviteNameEdit, layoutRoot.FindAnyWidget("SparkZGroupInviteNameEdit"));
        m_GroupInviteButton = layoutRoot.FindAnyWidget("SparkZGroupInviteButton");
        m_GroupKickButton = layoutRoot.FindAnyWidget("SparkZGroupKickButton");
        m_GroupPromoteButton = layoutRoot.FindAnyWidget("SparkZGroupPromoteButton");
        Class.CastTo(m_GroupRosterList, layoutRoot.FindAnyWidget("SparkZGroupRosterList"));
        Class.CastTo(m_GroupSelectedMemberText, layoutRoot.FindAnyWidget("SparkZGroupSelectedMemberText"));
        m_GroupPermBuildButton = layoutRoot.FindAnyWidget("SparkZGroupPermBuildButton");
        m_GroupPermDoorButton = layoutRoot.FindAnyWidget("SparkZGroupPermDoorButton");
        m_GroupPermStorageButton = layoutRoot.FindAnyWidget("SparkZGroupPermStorageButton");
        m_GroupPermApplyButton = layoutRoot.FindAnyWidget("SparkZGroupPermApplyButton");
        Class.CastTo(m_GroupPermStatusText, layoutRoot.FindAnyWidget("SparkZGroupPermStatusText"));
        Class.CastTo(m_GroupInvitePendingText, layoutRoot.FindAnyWidget("SparkZGroupInvitePendingText"));
        m_GroupInviteAcceptButton = layoutRoot.FindAnyWidget("SparkZGroupInviteAcceptButton");
        m_GroupInviteDeclineButton = layoutRoot.FindAnyWidget("SparkZGroupInviteDeclineButton");
        Class.CastTo(m_GroupHudProfileText, layoutRoot.FindAnyWidget("SparkZGroupHudProfileText"));
        Class.CastTo(m_GroupHudRedSlider, layoutRoot.FindAnyWidget("SparkZGroupHudRedSlider"));
        Class.CastTo(m_GroupHudGreenSlider, layoutRoot.FindAnyWidget("SparkZGroupHudGreenSlider"));
        Class.CastTo(m_GroupHudBlueSlider, layoutRoot.FindAnyWidget("SparkZGroupHudBlueSlider"));
        Class.CastTo(m_GroupHudRedValue, layoutRoot.FindAnyWidget("SparkZGroupHudRedValue"));
        Class.CastTo(m_GroupHudGreenValue, layoutRoot.FindAnyWidget("SparkZGroupHudGreenValue"));
        Class.CastTo(m_GroupHudBlueValue, layoutRoot.FindAnyWidget("SparkZGroupHudBlueValue"));
        m_GroupHudColorPreview = layoutRoot.FindAnyWidget("SparkZGroupHudColorPreview");
        Class.CastTo(m_GroupHudPreviewName0, layoutRoot.FindAnyWidget("SparkZGroupHudPreviewName0"));
        Class.CastTo(m_GroupHudPreviewName1, layoutRoot.FindAnyWidget("SparkZGroupHudPreviewName1"));
        Class.CastTo(m_GroupHudPreviewName2, layoutRoot.FindAnyWidget("SparkZGroupHudPreviewName2"));

        if (m_BrandLogo)
            m_BrandLogo.LoadImageFile(0, BRAND_LOGO);

        if (m_Title)
            m_Title.SetText("SPARKZ");

        if (m_Status)
            m_Status.SetText("/ TACTICAL MAP");

        if (m_StatusBarRight)
            m_StatusBarRight.SetText("SPARKZ / TACTICAL MAP v" + SparkZGroupConstants.MOD_VERSION);

        InitializeStaticSurfaces();
        SetPage(PAGE_MAP);

        return layoutRoot;
    }

    override void OnShow()
    {
        super.OnShow();
        SparkZCoreLogger.Info("SparkZ tactical menu opened.");

        if (GetGame() && GetGame().GetUIManager())
            GetGame().GetUIManager().ShowUICursor(true);

        Mission mission = GetGame().GetMission();
        if (mission)
            mission.AddActiveInputExcludes({"movement", "aiming", "menu"});

        SetIngameHudVisible(false);
        PPEffects.SetBlurMenu(0.48);
        CenterMapOnPlayer();
        RefreshMapMarkers();
    }

    override void OnHide()
    {
        super.OnHide();
        SparkZCoreLogger.Info("SparkZ tactical menu closed.");
        m_TextInputActive = false;

        Mission mission = GetGame().GetMission();
        if (mission)
        {
            mission.PlayerControlEnable(true);
            mission.RemoveActiveInputExcludes({"menu", "movement", "aiming"}, true);
            mission.RefreshExcludes();
        }

        PPEffects.SetBlurMenu(0);
        SetIngameHudVisible(true);

        if (GetGame() && GetGame().GetUIManager())
            GetGame().GetUIManager().ShowUICursor(false);

        SparkZTacticalMenuManager.Get().OnMenuHidden(this);
    }

    override void Update(float timeslice)
    {
        super.Update(timeslice);

        if (ShouldCloseFromInput())
        {
            CloseMenu();
            return;
        }

        UpdatePositionReadout();
        UpdateMarkerRefresh(timeslice);

        if (m_CurrentPage == PAGE_GROUP && !m_TextInputActive)
            RefreshLocalGroupSessionControls();
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (IsTextInputWidget(w))
        {
            m_TextInputActive = true;
            if (IsGroupIdentityTextInputWidget(w))
                m_GroupIdentityDraftDirty = true;

            return super.OnClick(w, x, y, button);
        }

        m_TextInputActive = false;

        if (w == m_CloseButton)
        {
            CloseMenu();
            return true;
        }

        if (w == m_TabMap)
        {
            SetPage(PAGE_MAP);
            return true;
        }

        if (w == m_TabMarkers)
        {
            SetPage(PAGE_MARKERS);
            return true;
        }

        if (w == m_TabGroup)
        {
            SetPage(PAGE_GROUP);
            return true;
        }

        if (w == m_TabInfo)
        {
            SetPage(PAGE_INFO);
            return true;
        }

        if (w == m_MarkerCenterButton)
        {
            CenterSelectedPrivateMarker();
            return true;
        }

        if (w == m_MarkerDeleteButton)
        {
            DeleteSelectedPrivateMarker();
            return true;
        }

        if (w == m_Marker3DButton)
        {
            ToggleSelectedMarker3D();
            return true;
        }

        if (w == m_MarkerClearButton)
        {
            ClearPrivateMarkers();
            return true;
        }

        if (w == m_MapMarkerEditButton)
        {
            OpenSelectedMarkerEditor();
            return true;
        }

        if (w == m_MapMarkerDeleteButton)
        {
            DeleteSelectedMarkerFromMapSidebar();
            return true;
        }

        if (HandleMapSidebarRowClick(w))
            return true;

        if (w == m_MarkerNameApplyButton)
        {
            ApplySelectedMarkerName();
            return true;
        }

        if (w == m_MarkerChannelPrivateButton)
        {
            SetSelectedMarkerChannel(SparkZGroupMarkerCache.MARKER_CHANNEL_PRIVATE);
            return true;
        }

        if (w == m_MarkerChannelGroupButton)
        {
            SetSelectedMarkerChannel(SparkZGroupMarkerCache.MARKER_CHANNEL_GROUP);
            return true;
        }

        if (w == m_MarkerChannelServerButton)
        {
            ShowServerChannelLocked();
            return true;
        }

        if (w == m_MarkerIconPrevButton)
        {
            StepSelectedMarkerIcon(-1);
            return true;
        }

        if (w == m_MarkerIconNextButton)
        {
            StepSelectedMarkerIcon(1);
            return true;
        }

        if (w == m_QuickPingIconPrevButton)
        {
            StepQuickPingProfileIcon(-1);
            return true;
        }

        if (w == m_QuickPingIconNextButton)
        {
            StepQuickPingProfileIcon(1);
            return true;
        }

        if (w == m_GroupCreateButton)
        {
            CreateLocalGroupPreview();
            return true;
        }

        if (w == m_GroupLeaveButton)
        {
            LeaveLocalGroupPreview();
            return true;
        }

        if (w == m_GroupIdentityApplyButton)
        {
            ApplyLocalGroupIdentity();
            return true;
        }

        if (w == m_GroupInviteButton)
        {
            SendGroupInvite();
            return true;
        }

        if (w == m_GroupKickButton)
        {
            KickGroupMember();
            return true;
        }

        if (w == m_GroupPermBuildButton)
        {
            ToggleSelectedGroupPermission(SparkZGroupConstants.SQUAD_PERMISSION_BASE_ACCESS);
            return true;
        }

        if (w == m_GroupPermDoorButton)
        {
            ToggleSelectedGroupPermission(SparkZGroupConstants.SQUAD_PERMISSION_BASE_MANAGER);
            return true;
        }

        if (w == m_GroupPermStorageButton)
        {
            ToggleSelectedGroupPermission(SparkZGroupConstants.SQUAD_ROLE_LEADER);
            return true;
        }

        if (w == m_GroupPermApplyButton)
        {
            ApplySelectedGroupPermissions();
            return true;
        }

        if (w == m_GroupInviteAcceptButton)
        {
            RespondToGroupInvite(true);
            return true;
        }

        if (w == m_GroupInviteDeclineButton)
        {
            RespondToGroupInvite(false);
            return true;
        }

        return super.OnClick(w, x, y, button);
    }

    override bool OnChange(Widget w, int x, int y, bool finished)
    {
        if (IsTextInputWidget(w))
        {
            m_TextInputActive = true;
            if (IsGroupIdentityTextInputWidget(w))
                m_GroupIdentityDraftDirty = true;

            return super.OnChange(w, x, y, finished);
        }

        if (m_SuppressMarkerEditorEvents)
            return super.OnChange(w, x, y, finished);

        if (w == m_MarkerRedSlider || w == m_MarkerGreenSlider || w == m_MarkerBlueSlider)
        {
            ApplySelectedMarkerColorFromSliders();
            return true;
        }

        if (m_SuppressQuickPingProfileEvents)
            return super.OnChange(w, x, y, finished);

        if (w == m_QuickPingRedSlider || w == m_QuickPingGreenSlider || w == m_QuickPingBlueSlider)
        {
            ApplyQuickPingProfileColorFromSliders();
            return true;
        }

        if (m_SuppressGroupHudProfileEvents)
            return super.OnChange(w, x, y, finished);

        if (w == m_GroupHudRedSlider || w == m_GroupHudGreenSlider || w == m_GroupHudBlueSlider)
        {
            ApplyGroupHudProfileColorFromSliders();
            return true;
        }

        return super.OnChange(w, x, y, finished);
    }

    override bool OnDoubleClick(Widget w, int x, int y, int button)
    {
        if (w == m_MapWidget && m_CurrentPage == PAGE_MAP)
        {
            if (button == 0)
            {
                AddPrivateMarkerAtScreenPosition(x, y);
                return true;
            }

            if (button == 1)
            {
                OpenPrivateMarkerEditorAtScreenPosition(x, y);
                return true;
            }

            return true;
        }

        if (w == m_MarkerList || w == m_GroupMarkerList || w == m_ServerMarkerList)
        {
            CenterSelectedPrivateMarker();
            return true;
        }

        if (IsMapSidebarRow(w))
        {
            OpenSelectedMarkerEditor();
            return true;
        }

        return super.OnDoubleClick(w, x, y, button);
    }

    override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn)
    {
        if (w == m_MarkerList)
        {
            SelectMarkerFromChannelList(m_PrivateListIndexes, row);
            return true;
        }

        if (w == m_GroupMarkerList)
        {
            if (m_GroupListIndexes && row >= m_GroupListIndexes.Count())
                SelectRemoteSquadMarker(row - m_GroupListIndexes.Count());
            else
                SelectMarkerFromChannelList(m_GroupListIndexes, row);
            return true;
        }

        if (w == m_ServerMarkerList)
        {
            SelectServerMarkerFromList(row);
            return true;
        }

        if (w == m_GroupRosterList)
        {
            SelectGroupRosterMember(row);
            return true;
        }

        return super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn);
    }

    override bool OnKeyDown(Widget w, int x, int y, int key)
    {
        if (m_TextInputActive && key != KeyCode.KC_ESCAPE)
        {
            if (m_CurrentPage == PAGE_GROUP)
                m_GroupIdentityDraftDirty = true;

            return super.OnKeyDown(w, x, y, key);
        }

        if (EditBoxWidget.Cast(w) && key != KeyCode.KC_ESCAPE)
        {
            m_TextInputActive = true;
            if (IsGroupIdentityTextInputWidget(w))
                m_GroupIdentityDraftDirty = true;

            return super.OnKeyDown(w, x, y, key);
        }

        if (ShouldReserveKeyForTextInput(key))
        {
            m_TextInputActive = true;
            return super.OnKeyDown(w, x, y, key);
        }

        if (key == KeyCode.KC_ESCAPE || key == KeyCode.KC_M)
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

    void RefreshGroupPageFromNetwork()
    {
        if (m_CurrentPage != PAGE_GROUP || m_TextInputActive)
            return;

        RefreshLocalGroupSessionControls();
    }

    void RefreshMarkersPageFromNetwork()
    {
        if (m_CurrentPage != PAGE_MARKERS)
            return;

        PopulateMarkerList();
        UpdateMarkerText();
    }

    bool ShouldSuppressMapToggleInput()
    {
        return m_TextInputActive;
    }

    protected bool IsTextInputWidget(Widget w)
    {
        if (!w)
            return false;

        if (w == m_MarkerNameEdit)
            return true;

        if (w == m_GroupNameEdit)
            return true;

        if (w == m_GroupTagEdit)
            return true;

        if (w == m_GroupInviteNameEdit)
            return true;

        return false;
    }

    protected bool IsGroupIdentityTextInputWidget(Widget w)
    {
        return w == m_GroupNameEdit || w == m_GroupTagEdit;
    }

    protected bool ShouldReserveKeyForTextInput(int key)
    {
        if (key != KeyCode.KC_M)
            return false;

        if (m_CurrentPage == PAGE_GROUP)
            return true;

        if (m_CurrentPage == PAGE_MARKERS && m_SelectedMarkerIndex >= 0)
            return true;

        return false;
    }

    protected bool ShouldCloseFromInput()
    {
        if (m_TextInputActive)
            return false;

        if (!GetGame() || !GetGame().GetInput())
            return false;

        if (GetGame().GetInput().LocalPress(SparkZGroupConstants.INPUT_UI_BACK, false))
            return true;

        return false;
    }

    protected void SetPage(int page)
    {
        m_CurrentPage = page;
        UpdateTabVisuals(page);
        ShowMapSidebarControls(page == PAGE_MAP);

        if (m_MapLayer)
            m_MapLayer.Show(page == PAGE_MAP);

        if (m_MarkersLayer)
            m_MarkersLayer.Show(page == PAGE_MARKERS);

        if (m_GroupLayer)
            m_GroupLayer.Show(page == PAGE_GROUP);

        if (m_InfoLayer)
            m_InfoLayer.Show(page == PAGE_INFO);

        if (page == PAGE_MAP)
        {
            SetTextSafe(m_PageTitle, "LOCATIONS");
            SetTextSafe(m_PageSubtitle, "FIXED SERVER POIS");
            SetTextSafe(m_StatusBarLeft, "CHERNARUS");
            SetTextSafe(m_MainTitle, "TACTICAL MAP");
            SetTextSafe(m_MapStatus, "CHERNARUS+ LIVE / EVENTS / SQUAD / POIS");
            CenterMapOnPlayer();
            RefreshMapMarkers();
            PopulateMapSidebarMarkerLists();
            return;
        }

        if (page == PAGE_MARKERS)
        {
            SetTextSafe(m_PageTitle, "PINGS");
            SetTextSafe(m_PageSubtitle, "");
            SetTextSafe(m_StatusBarLeft, "");
            SetTextSafe(m_PageBody, "SELECT A PIN OR DOUBLE-CLICK\nTHE MAP TO ADD ONE.");
            SetTextSafe(m_SidebarStatus, "PRIVATE: PERSONAL\nSQUAD: TEAM PINGS\nSERVER: EVENTS");
            SetTextSafe(m_Footer, "");
            SetTextSafe(m_MainTitle, "PING CONTROL");
            PopulateMarkerList();
            UpdateMarkerText();
            return;
        }

        if (page == PAGE_GROUP)
        {
            SetTextSafe(m_PageTitle, "SQUAD");
            SetTextSafe(m_PageSubtitle, "");
            SetTextSafe(m_StatusBarLeft, "");
            SetTextSafe(m_SidebarStatus, "CREATE SQUAD\nTHEN INVITE PLAYERS");
            SetTextSafe(m_Footer, "NAME AND TAG LOCK\nWHILE SQUAD IS ACTIVE");
            SetTextSafe(m_MainTitle, "SQUAD CONTROL");
            RefreshLocalGroupSessionControls();
            return;
        }

        SetTextSafe(m_PageTitle, "SETTINGS");
        SetTextSafe(m_PageSubtitle, "");
        SetTextSafe(m_StatusBarLeft, "");
        SetTextSafe(m_PageBody, "PING STYLE\nSQUAD HUD\nLOCAL PROFILE");
        SetTextSafe(m_SidebarStatus, "LOCAL PREFERENCES\nNO SERVER RESTART");
        SetTextSafe(m_Footer, "COLORS UPDATE LIVE\nFOR ACTIVE UI");
        SetTextSafe(m_MainTitle, "PLAYER SETTINGS");
        RefreshQuickPingProfileControls();
        RefreshGroupHudProfileControls();
    }

    protected void UpdateTabVisuals(int page)
    {
        SetTabState(m_TabMap, m_TabMapUnderline, page == PAGE_MAP);
        SetTabState(m_TabMarkers, m_TabMarkersUnderline, page == PAGE_MARKERS);
        SetTabState(m_TabGroup, m_TabGroupUnderline, page == PAGE_GROUP);
        SetTabState(m_TabInfo, m_TabInfoUnderline, page == PAGE_INFO);
    }

    protected void SetTabState(Widget tab, Widget underline, bool active)
    {
        if (tab)
        {
            if (active)
                tab.SetColor(ARGB(255, 229, 229, 224));
            else
                tab.SetColor(ARGB(255, 161, 165, 165));
        }

        if (underline)
            underline.Show(active);
    }

    protected void InitializeStaticSurfaces()
    {
        LoadPlateImage(m_MapVignette, UI_TEX_LINE_BORDER, ARGB(60, 0, 0, 0));
        LoadPlateImage(m_MapBorderTop, UI_TEX_LINE_BORDER, ARGB(170, 52, 56, 59));
        LoadPlateImage(m_MapBorderLeft, UI_TEX_LINE_BORDER, ARGB(170, 52, 56, 59));
        LoadPlateImage(m_MapBorderRight, UI_TEX_LINE_BORDER, ARGB(150, 52, 56, 59));
        LoadPlateImage(m_MapBorderBottom, UI_TEX_LINE_BORDER, ARGB(150, 52, 56, 59));
    }

    protected void LoadPlateImage(ImageWidget widget, string texturePath, int tintARGB)
    {
        if (!widget)
            return;

        widget.LoadImageFile(0, texturePath);
        widget.SetColor(tintARGB);
    }

    protected void CenterMapOnPlayer()
    {
        if (!m_MapWidget)
            return;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
            return;

        m_MapWidget.ClearUserMarks();
        m_MapWidget.SetScale(0.2);
        m_MapWidget.SetMapPos(player.GetPosition());
        UpdatePositionReadout();
    }

    protected void AddPrivateMarkerAtScreenPosition(int x, int y)
    {
        if (!m_MapWidget)
            return;

        vector markerPosition = m_MapWidget.ScreenToMap(Vector(x, y, 0));
        SparkZGroupMarkerCache.Get().AddPrivateMarker(markerPosition);
        m_SelectedMarkerIndex = SparkZGroupMarkerCache.Get().GetPrivateMarkerCount() - 1;
        m_SelectedServerMarkerIndex = -1;

        SparkZCoreLogger.Info("SparkZ private marker added.");
        RefreshMapMarkers();
        PopulateMarkerList();
        UpdateMarkerText();

        if (m_CurrentPage == PAGE_MAP)
            PopulateMapSidebarMarkerLists();
    }

    protected void OpenPrivateMarkerEditorAtScreenPosition(int x, int y)
    {
        if (!m_MapWidget)
            return;

        int markerIndex = FindPrivateMarkerIndexNearScreenPosition(x, y);
        if (markerIndex < 0)
            return;

        SetPage(PAGE_MARKERS);

        SelectMarkerByIndex(markerIndex);
    }

    protected int FindPrivateMarkerIndexNearScreenPosition(int x, int y)
    {
        vector mapPosition = m_MapWidget.ScreenToMap(Vector(x + 10, y + 10, 0));
        array<ref SparkZCoreMapPoint> privateMarkers = SparkZGroupMarkerCache.Get().GetPrivateMarkers();
        float bestDistanceSq = MARKER_EDIT_SELECT_RADIUS_METERS * MARKER_EDIT_SELECT_RADIUS_METERS;
        int bestIndex = -1;
        int markerIndex = 0;

        foreach (SparkZCoreMapPoint marker: privateMarkers)
        {
            if (!marker || !marker.IsValid())
            {
                markerIndex++;
                continue;
            }

            float dx = marker.Position[0] - mapPosition[0];
            float dz = marker.Position[2] - mapPosition[2];
            float distanceSq = (dx * dx) + (dz * dz);

            if (distanceSq <= bestDistanceSq)
            {
                bestDistanceSq = distanceSq;
                bestIndex = markerIndex;
            }

            markerIndex++;
        }

        return bestIndex;
    }

    protected void UpdateMarkerRefresh(float timeslice)
    {
        m_MarkerRefreshTimer += timeslice;
        if (m_MarkerRefreshTimer < MARKER_REFRESH_INTERVAL)
            return;

        m_MarkerRefreshTimer = 0;

        // A delete/create only updates the local cache instantly for markers this client owns.
        // Anything that has to round-trip the server first (e.g. a squad marker another member
        // deleted, or the server's own confirmation of our delete) only shows up once the cache's
        // revision counter bumps from an incoming sync - previously nothing polled that while the
        // menu was open, so the Markers list only ever caught up on next reopen. Poll it here
        // instead, on whichever page is actually open.
        int currentRevision = SparkZGroupMarkerCache.Get().GetRevision();
        bool revisionChanged = currentRevision != m_LastAppliedMarkerCacheRevision;
        m_LastAppliedMarkerCacheRevision = currentRevision;

        if (m_CurrentPage == PAGE_MAP)
        {
            RefreshMapMarkers();
        }
        else if (m_CurrentPage == PAGE_MARKERS && revisionChanged)
        {
            PopulateMarkerList();
        }
    }

    protected void RefreshMapMarkers()
    {
        if (!m_MapWidget)
            return;

        bool expiredMarkers = SparkZGroupMarkerCache.Get().ExpirePrivateMarkers();
        bool expiredServerMarkers = SparkZGroupMarkerCache.Get().ExpireServerEventMarkers();
        NormalizeMarkerSelections();

        m_MapWidget.ClearUserMarks();

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player)
            m_MapWidget.AddUserMark(player.GetPosition(), "You", ARGB(255, 40, 220, 255), PLAYER_MARKER_ICON);

        array<ref SparkZCoreMapPoint> serverMarkers = SparkZGroupMarkerCache.Get().GetServerMarkers();
        int serverIndex = 0;
        foreach (SparkZCoreMapPoint serverMarker: serverMarkers)
        {
            if (!serverMarker || !serverMarker.IsValid())
            {
                serverIndex++;
                continue;
            }

            m_MapWidget.AddUserMark(serverMarker.Position, serverMarker.Label, serverMarker.ColorARGB, GetServerMarkerIconSafe(serverIndex));
            serverIndex++;
        }

        array<ref SparkZCoreMapPoint> privateMarkers = SparkZGroupMarkerCache.Get().GetPrivateMarkers();
        int markerIndex = 0;
        foreach (SparkZCoreMapPoint marker: privateMarkers)
        {
            if (!marker || !marker.IsValid())
            {
                markerIndex++;
                continue;
            }

            m_MapWidget.AddUserMark(marker.Position, marker.Label, marker.ColorARGB, GetPrivateMarkerIconSafe(markerIndex));
            markerIndex++;
        }

        array<ref SparkZCoreMarkerPayload> remoteSquadMarkers = SparkZGroupMarkerCache.Get().GetRemoteSquadMarkers();
        foreach (SparkZCoreMarkerPayload remoteMarker: remoteSquadMarkers)
        {
            if (!remoteMarker || !remoteMarker.IsValid())
                continue;

            m_MapWidget.AddUserMark(remoteMarker.Position, remoteMarker.Label, remoteMarker.ColorARGB, remoteMarker.IconPath);
        }

        UpdateMarkerText();

        if ((expiredMarkers || expiredServerMarkers) && m_CurrentPage == PAGE_MAP)
            PopulateMapSidebarMarkerLists();
    }

    protected void UpdateMarkerText()
    {
        int totalCount = SparkZGroupMarkerCache.Get().GetPrivateMarkerCount();
        int privateCount = SparkZGroupMarkerCache.Get().GetPrivateMarkerCountByChannel(SparkZGroupMarkerCache.MARKER_CHANNEL_PRIVATE);
        int groupCount = SparkZGroupMarkerCache.Get().GetPrivateMarkerCountByChannel(SparkZGroupMarkerCache.MARKER_CHANNEL_GROUP);
        int serverCount = SparkZGroupMarkerCache.Get().GetServerMarkerCount();

        if (m_CurrentPage == PAGE_MAP)
            SetTextSafe(m_PageBody, BuildMapOverviewText());

        if (m_MarkersEmpty)
            SetTextSafe(m_MarkersEmpty, "PRIVATE: " + privateCount + " / SQUAD: " + groupCount + " / SERVER: " + serverCount);
    }

    protected void BuildMapSidebarRows()
    {
        m_MapPrivateRows = new array<Widget>();
        m_MapPrivateRowTexts = new array<TextWidget>();
        m_MapPrivateRowIcons = new array<ImageWidget>();
        m_MapGroupRows = new array<Widget>();
        m_MapGroupRowTexts = new array<TextWidget>();
        m_MapGroupRowIcons = new array<ImageWidget>();
        m_MapServerRows = new array<Widget>();
        m_MapServerRowTexts = new array<TextWidget>();
        m_MapServerRowIcons = new array<ImageWidget>();

        AddMapSidebarRow(m_MapPrivateRows, m_MapPrivateRowTexts, m_MapPrivateRowIcons, "SparkZMapPrivateRow0", "SparkZMapPrivateRow0Text", "SparkZMapPrivateRow0Icon");
        AddMapSidebarRow(m_MapPrivateRows, m_MapPrivateRowTexts, m_MapPrivateRowIcons, "SparkZMapPrivateRow1", "SparkZMapPrivateRow1Text", "SparkZMapPrivateRow1Icon");
        AddMapSidebarRow(m_MapPrivateRows, m_MapPrivateRowTexts, m_MapPrivateRowIcons, "SparkZMapPrivateRow2", "SparkZMapPrivateRow2Text", "SparkZMapPrivateRow2Icon");
        AddMapSidebarRow(m_MapPrivateRows, m_MapPrivateRowTexts, m_MapPrivateRowIcons, "SparkZMapPrivateRow3", "SparkZMapPrivateRow3Text", "SparkZMapPrivateRow3Icon");
        AddMapSidebarRow(m_MapPrivateRows, m_MapPrivateRowTexts, m_MapPrivateRowIcons, "SparkZMapPrivateRow4", "SparkZMapPrivateRow4Text", "SparkZMapPrivateRow4Icon");
        AddMapSidebarRow(m_MapGroupRows, m_MapGroupRowTexts, m_MapGroupRowIcons, "SparkZMapGroupRow0", "SparkZMapGroupRow0Text", "SparkZMapGroupRow0Icon");
        AddMapSidebarRow(m_MapGroupRows, m_MapGroupRowTexts, m_MapGroupRowIcons, "SparkZMapGroupRow1", "SparkZMapGroupRow1Text", "SparkZMapGroupRow1Icon");
        AddMapSidebarRow(m_MapGroupRows, m_MapGroupRowTexts, m_MapGroupRowIcons, "SparkZMapGroupRow2", "SparkZMapGroupRow2Text", "SparkZMapGroupRow2Icon");
        AddMapSidebarRow(m_MapGroupRows, m_MapGroupRowTexts, m_MapGroupRowIcons, "SparkZMapGroupRow3", "SparkZMapGroupRow3Text", "SparkZMapGroupRow3Icon");
        AddMapSidebarRow(m_MapGroupRows, m_MapGroupRowTexts, m_MapGroupRowIcons, "SparkZMapGroupRow4", "SparkZMapGroupRow4Text", "SparkZMapGroupRow4Icon");
        AddMapSidebarRow(m_MapServerRows, m_MapServerRowTexts, m_MapServerRowIcons, "SparkZMapServerRow0", "SparkZMapServerRow0Text", "SparkZMapServerRow0Icon");
        AddMapSidebarRow(m_MapServerRows, m_MapServerRowTexts, m_MapServerRowIcons, "SparkZMapServerRow1", "SparkZMapServerRow1Text", "SparkZMapServerRow1Icon");
        AddMapSidebarRow(m_MapServerRows, m_MapServerRowTexts, m_MapServerRowIcons, "SparkZMapServerRow2", "SparkZMapServerRow2Text", "SparkZMapServerRow2Icon");
        AddMapSidebarRow(m_MapServerRows, m_MapServerRowTexts, m_MapServerRowIcons, "SparkZMapServerRow3", "SparkZMapServerRow3Text", "SparkZMapServerRow3Icon");
        AddMapSidebarRow(m_MapServerRows, m_MapServerRowTexts, m_MapServerRowIcons, "SparkZMapServerRow4", "SparkZMapServerRow4Text", "SparkZMapServerRow4Icon");
        AddMapSidebarRow(m_MapServerRows, m_MapServerRowTexts, m_MapServerRowIcons, "SparkZMapServerRow5", "SparkZMapServerRow5Text", "SparkZMapServerRow5Icon");
    }

    protected void AddMapSidebarRow(array<Widget> rows, array<TextWidget> rowTexts, array<ImageWidget> rowIcons, string rowName, string textName, string iconName)
    {
        rows.Insert(layoutRoot.FindAnyWidget(rowName));

        TextWidget rowText;
        Class.CastTo(rowText, layoutRoot.FindAnyWidget(textName));
        rowTexts.Insert(rowText);

        ImageWidget rowIcon;
        Class.CastTo(rowIcon, layoutRoot.FindAnyWidget(iconName));
        rowIcons.Insert(rowIcon);
    }

    protected void ShowMapSidebarRows(array<Widget> rows, array<TextWidget> rowTexts, array<ImageWidget> rowIcons, bool visible)
    {
        if (!rows || !rowTexts || !rowIcons || visible)
            return;

        for (int rowIndex = 0; rowIndex < rows.Count(); rowIndex++)
        {
            Widget row = rows.Get(rowIndex);
            if (row)
                row.Show(false);

            TextWidget rowText = rowTexts.Get(rowIndex);
            if (rowText)
                rowText.Show(false);

            ImageWidget rowIcon = rowIcons.Get(rowIndex);
            if (rowIcon)
                rowIcon.Show(false);
        }
    }

    protected void ClearMapSidebarRows(array<Widget> rows, array<TextWidget> rowTexts, array<ImageWidget> rowIcons)
    {
        if (!rows || !rowTexts || !rowIcons)
            return;

        for (int rowIndex = 0; rowIndex < rows.Count(); rowIndex++)
        {
            Widget rowWidget = rows.Get(rowIndex);
            if (rowWidget)
            {
                rowWidget.Show(false);
            }

            TextWidget rowText = rowTexts.Get(rowIndex);
            if (rowText)
            {
                rowText.SetText("");
                rowText.SetColor(ARGB(255, 240, 245, 240));
                rowText.Show(false);
            }

            ImageWidget rowIcon = rowIcons.Get(rowIndex);
            if (rowIcon)
            {
                rowIcon.LoadImageFile(0, DEFAULT_MARKER_ICON);
                rowIcon.SetColor(ARGB(255, 240, 245, 240));
                rowIcon.Show(false);
            }
        }
    }

    protected void SetMapSidebarRow(array<Widget> rows, array<TextWidget> rowTexts, array<ImageWidget> rowIcons, int rowIndex, string label, int colorARGB, string iconPath)
    {
        if (!rows || !rowTexts || !rowIcons || rowIndex < 0 || rowIndex >= rows.Count())
            return;

        Widget rowWidget = rows.Get(rowIndex);
        if (rowWidget)
        {
            rowWidget.Show(m_CurrentPage == PAGE_MAP);
        }

        TextWidget rowText = rowTexts.Get(rowIndex);
        if (rowText)
        {
            rowText.SetText(label);
            rowText.SetColor(colorARGB);
            rowText.Show(m_CurrentPage == PAGE_MAP);
        }

        ImageWidget rowIcon = rowIcons.Get(rowIndex);
        if (rowIcon)
        {
            rowIcon.LoadImageFile(0, iconPath);
            rowIcon.SetColor(colorARGB);
            rowIcon.Show(m_CurrentPage == PAGE_MAP);
        }
    }

    protected void ShowMapSidebarControls(bool visible)
    {
        ShowWidgetSafe(m_PageBody, !visible);
        ShowWidgetSafe(m_DividerA, !visible);
        ShowWidgetSafe(m_DividerB, !visible);
        ShowWidgetSafe(m_SidebarStatus, !visible);
        ShowWidgetSafe(m_Footer, !visible);
        ShowWidgetSafe(m_MapPrivateHeader, visible);
        ShowWidgetSafe(m_MapGroupHeader, visible);
        ShowWidgetSafe(m_MapServerHeader, visible);
        ShowMapSidebarRows(m_MapPrivateRows, m_MapPrivateRowTexts, m_MapPrivateRowIcons, visible);
        ShowMapSidebarRows(m_MapGroupRows, m_MapGroupRowTexts, m_MapGroupRowIcons, visible);
        ShowMapSidebarRows(m_MapServerRows, m_MapServerRowTexts, m_MapServerRowIcons, visible);
        ShowWidgetSafe(m_MapMarkerEditButton, visible);
        ShowWidgetSafe(m_MapMarkerDeleteButton, visible);
    }

    protected void PopulateMapSidebarMarkerLists()
    {
        if (!m_MapPrivateRows || !m_MapGroupRows || !m_MapServerRows)
            return;

        SparkZGroupMarkerCache.Get().ExpirePrivateMarkers();
        SparkZGroupMarkerCache.Get().ExpireServerEventMarkers();
        NormalizeMarkerSelections();

        m_MapPrivateListIndexes = new array<int>();
        m_MapGroupListIndexes = new array<int>();
        m_MapServerListIndexes = new array<int>();
        ClearMapSidebarRows(m_MapPrivateRows, m_MapPrivateRowTexts, m_MapPrivateRowIcons);
        ClearMapSidebarRows(m_MapGroupRows, m_MapGroupRowTexts, m_MapGroupRowIcons);
        ClearMapSidebarRows(m_MapServerRows, m_MapServerRowTexts, m_MapServerRowIcons);

        int privateTotal = SparkZGroupMarkerCache.Get().GetPrivateMarkerCountByChannel(SparkZGroupMarkerCache.MARKER_CHANNEL_PRIVATE);
        int groupTotal = SparkZGroupMarkerCache.Get().GetPrivateMarkerCountByChannel(SparkZGroupMarkerCache.MARKER_CHANNEL_GROUP);
        int serverTotal = SparkZGroupMarkerCache.Get().GetServerMarkerCount();
        int privateLimit = GetMapSidebarRealRowLimit(m_MapPrivateRows, privateTotal);
        int groupLimit = GetMapSidebarRealRowLimit(m_MapGroupRows, groupTotal);
        int serverLimit = GetMapSidebarRealRowLimit(m_MapServerRows, serverTotal);

        array<ref SparkZCoreMapPoint> privateMarkers = SparkZGroupMarkerCache.Get().GetPrivateMarkers();
        for (int markerIndex = 0; markerIndex < privateMarkers.Count(); markerIndex++)
        {
            SparkZCoreMapPoint marker = privateMarkers.Get(markerIndex);
            if (!marker || !marker.IsValid())
                continue;

            int channel = SparkZGroupMarkerCache.Get().GetPrivateMarkerChannel(markerIndex);
            if (channel == SparkZGroupMarkerCache.MARKER_CHANNEL_GROUP)
            {
                if (m_MapGroupListIndexes.Count() >= groupLimit)
                    continue;

                m_MapGroupListIndexes.Insert(markerIndex);
                SetMapSidebarRow(m_MapGroupRows, m_MapGroupRowTexts, m_MapGroupRowIcons, m_MapGroupListIndexes.Count() - 1, BuildCompactMarkerListLabel(marker), marker.ColorARGB, GetPrivateMarkerIconSafe(markerIndex));
            }
            else
            {
                if (m_MapPrivateListIndexes.Count() >= privateLimit)
                    continue;

                m_MapPrivateListIndexes.Insert(markerIndex);
                SetMapSidebarRow(m_MapPrivateRows, m_MapPrivateRowTexts, m_MapPrivateRowIcons, m_MapPrivateListIndexes.Count() - 1, BuildCompactMarkerListLabel(marker), marker.ColorARGB, GetPrivateMarkerIconSafe(markerIndex));
            }
        }

        array<ref SparkZCoreMapPoint> serverMarkers = SparkZGroupMarkerCache.Get().GetServerMarkers();
        for (int serverIndex = 0; serverIndex < serverMarkers.Count(); serverIndex++)
        {
            SparkZCoreMapPoint serverMarker = serverMarkers.Get(serverIndex);
            if (!serverMarker || !serverMarker.IsValid())
                continue;

            if (m_MapServerListIndexes.Count() >= serverLimit)
                continue;

            m_MapServerListIndexes.Insert(serverIndex);
            SetMapSidebarRow(m_MapServerRows, m_MapServerRowTexts, m_MapServerRowIcons, m_MapServerListIndexes.Count() - 1, BuildCompactMarkerListLabel(serverMarker), serverMarker.ColorARGB, GetServerMarkerIconSafe(serverIndex));
        }

        AddMapSidebarOverflowRow(m_MapServerRows, m_MapServerRowTexts, m_MapServerRowIcons, m_MapServerListIndexes, serverTotal, serverLimit, "MORE POIS");
        AddMapSidebarOverflowRow(m_MapGroupRows, m_MapGroupRowTexts, m_MapGroupRowIcons, m_MapGroupListIndexes, groupTotal, groupLimit, "MORE SQUAD");
        AddMapSidebarOverflowRow(m_MapPrivateRows, m_MapPrivateRowTexts, m_MapPrivateRowIcons, m_MapPrivateListIndexes, privateTotal, privateLimit, "MORE PRIVATE");

        ReflowMapSidebar();
    }

    protected void ReflowMapSidebar()
    {
        int y = 92;
        y = ReflowMapSidebarSection(m_MapServerHeader, m_MapServerRows, m_MapServerRowTexts, m_MapServerRowIcons, m_MapServerListIndexes, y);
        y = ReflowMapSidebarSection(m_MapGroupHeader, m_MapGroupRows, m_MapGroupRowTexts, m_MapGroupRowIcons, m_MapGroupListIndexes, y);
        y = ReflowMapSidebarSection(m_MapPrivateHeader, m_MapPrivateRows, m_MapPrivateRowTexts, m_MapPrivateRowIcons, m_MapPrivateListIndexes, y);

        if (m_MapMarkerEditButton)
            m_MapMarkerEditButton.SetPos(20, y);

        if (m_MapMarkerDeleteButton)
            m_MapMarkerDeleteButton.SetPos(176, y);
    }

    protected int ReflowMapSidebarSection(Widget header, array<Widget> rows, array<TextWidget> rowTexts, array<ImageWidget> rowIcons, array<int> indexes, int y)
    {
        int visibleCount = 0;
        if (indexes)
            visibleCount = indexes.Count();

        bool onMapPage = (m_CurrentPage == PAGE_MAP);

        if (!header)
            return y;

        if (visibleCount <= 0)
        {
            header.Show(false);
            return y;
        }

        header.Show(onMapPage);
        header.SetPos(20, y);
        y = y + 26;

        for (int rowIndex = 0; rowIndex < visibleCount; rowIndex++)
        {
            Widget row = rows.Get(rowIndex);
            if (row)
                row.SetPos(20, y);

            ImageWidget icon = rowIcons.Get(rowIndex);
            if (icon)
                icon.SetPos(24, y + 3);

            TextWidget text = rowTexts.Get(rowIndex);
            if (text)
                text.SetPos(54, y + 4);

            y = y + 30;
        }

        return y + 14;
    }

    protected int GetMapSidebarRealRowLimit(array<Widget> rows, int totalCount)
    {
        if (!rows)
            return 0;

        int rowCount = rows.Count();
        if (rowCount <= 0)
            return 0;

        if (totalCount > rowCount && rowCount > 1)
            return rowCount - 1;

        return rowCount;
    }

    protected void AddMapSidebarOverflowRow(array<Widget> rows, array<TextWidget> rowTexts, array<ImageWidget> rowIcons, array<int> indexes, int totalCount, int realLimit, string label)
    {
        if (!rows || !indexes)
            return;

        if (totalCount <= rows.Count())
            return;

        int overflowCount = totalCount - realLimit;
        if (overflowCount <= 0 || indexes.Count() >= rows.Count())
            return;

        indexes.Insert(MAP_SIDEBAR_OVERFLOW_ROW);
        SetMapSidebarRow(rows, rowTexts, rowIcons, indexes.Count() - 1, "+ " + overflowCount + " " + label, ARGB(255, 180, 205, 210), DEFAULT_MARKER_ICON);
    }

    protected void PopulateMarkerList()
    {
        if (!m_MarkerList || !m_GroupMarkerList || !m_ServerMarkerList)
            return;

        SparkZGroupMarkerCache.Get().ExpirePrivateMarkers();
        NormalizeMarkerSelections();

        int previousSelection = m_SelectedMarkerIndex;
        int previousServerSelection = m_SelectedServerMarkerIndex;
        if (previousSelection < 0 && previousServerSelection < 0)
            previousSelection = GetFirstExistingMarkerIndex();

        m_MarkerList.ClearItems();
        m_GroupMarkerList.ClearItems();
        m_ServerMarkerList.ClearItems();
        m_PrivateListIndexes = new array<int>();
        m_GroupListIndexes = new array<int>();
        m_ServerListIndexes = new array<int>();

        array<ref SparkZCoreMapPoint> privateMarkers = SparkZGroupMarkerCache.Get().GetPrivateMarkers();
        for (int markerIndex = 0; markerIndex < privateMarkers.Count(); markerIndex++)
        {
            SparkZCoreMapPoint marker = privateMarkers.Get(markerIndex);
            if (!marker || !marker.IsValid())
                continue;

            AddMarkerToChannelList(markerIndex, marker);
        }

        array<ref SparkZCoreMapPoint> serverMarkers = SparkZGroupMarkerCache.Get().GetServerMarkers();
        for (int serverIndex = 0; serverIndex < serverMarkers.Count(); serverIndex++)
        {
            SparkZCoreMapPoint serverMarker = serverMarkers.Get(serverIndex);
            if (!serverMarker || !serverMarker.IsValid())
                continue;

            m_ServerListIndexes.Insert(serverIndex);
            m_ServerMarkerList.AddItem(BuildMarkerListLabel(serverMarker), null, 0);
        }

        m_DisplayedRemoteSquadMarkers = SparkZGroupMarkerCache.Get().GetRemoteSquadMarkers();
        for (int remoteIndex = 0; remoteIndex < m_DisplayedRemoteSquadMarkers.Count(); remoteIndex++)
        {
            SparkZCoreMarkerPayload remoteMarker = m_DisplayedRemoteSquadMarkers.Get(remoteIndex);
            m_GroupMarkerList.AddItem(BuildRemoteMarkerListLabel(remoteMarker), null, 0);
        }

        if (previousSelection >= 0)
            SelectMarkerByIndex(previousSelection);
        else if (previousServerSelection >= 0)
            SelectServerMarkerByIndex(previousServerSelection);
        else
            UpdateMarkerSelectionDetail(-1);

        m_MarkerList.Update();
        m_GroupMarkerList.Update();
        m_ServerMarkerList.Update();

        ReflowPingsLists();
    }

    protected void ReflowPingsLists()
    {
        int y = 132;
        y = ReflowPingsSection(m_PrivateMarkerHeader, m_MarkerList, m_PrivateListIndexes.Count(), 1, 5, y);
        int groupDisplayCount = m_GroupListIndexes.Count() + m_DisplayedRemoteSquadMarkers.Count();
        y = ReflowPingsSection(m_GroupMarkerHeader, m_GroupMarkerList, groupDisplayCount, 1, 6, y);
        ReflowPingsSection(m_ServerMarkerHeader, m_ServerMarkerList, m_ServerListIndexes.Count(), 1, 6, y);
    }

    protected int ReflowPingsSection(Widget header, TextListboxWidget listbox, int itemCount, int minRows, int maxRows, int y)
    {
        if (header)
            header.SetPos(40, y);

        y = y + 26;

        int rows = itemCount;
        if (rows < 0)
            rows = 0;
        if (rows > maxRows)
            rows = maxRows;

        int height = rows * 29;
        int gap = 18;

        if (rows <= 0)
        {
            height = 6;
            gap = 8;
        }

        if (listbox)
        {
            listbox.SetPos(40, y);
            listbox.SetSize(520, height);
        }

        return y + height + gap;
    }

    protected void AddMarkerToChannelList(int markerIndex, SparkZCoreMapPoint marker)
    {
        int channel = SparkZGroupMarkerCache.Get().GetPrivateMarkerChannel(markerIndex);
        if (channel == SparkZGroupMarkerCache.MARKER_CHANNEL_GROUP)
        {
            m_GroupListIndexes.Insert(markerIndex);
            m_GroupMarkerList.AddItem(BuildMarkerListLabel(marker), null, 0);
            return;
        }

        m_PrivateListIndexes.Insert(markerIndex);
        m_MarkerList.AddItem(BuildMarkerListLabel(marker), null, 0);
    }

    protected void SelectMarkerFromChannelList(array<int> indexList, int row)
    {
        if (!indexList || row < 0 || row >= indexList.Count())
            return;

        SelectMarkerByIndex(indexList.Get(row));
    }

    protected void SelectServerMarkerFromList(int row)
    {
        if (!m_ServerListIndexes || row < 0 || row >= m_ServerListIndexes.Count())
            return;

        SelectServerMarkerByIndex(m_ServerListIndexes.Get(row));
    }

    protected bool HandleMapSidebarRowClick(Widget w)
    {
        int row = FindWidgetRow(m_MapPrivateRows, w);
        if (row >= 0)
        {
            SelectMapMarkerFromChannelList(m_MapPrivateListIndexes, row);
            return true;
        }

        row = FindWidgetRow(m_MapGroupRows, w);
        if (row >= 0)
        {
            SelectMapMarkerFromChannelList(m_MapGroupListIndexes, row);
            return true;
        }

        row = FindWidgetRow(m_MapServerRows, w);
        if (row >= 0)
        {
            SelectMapServerMarkerFromList(row);
            return true;
        }

        return false;
    }

    protected bool IsMapSidebarRow(Widget w)
    {
        return FindWidgetRow(m_MapPrivateRows, w) >= 0 || FindWidgetRow(m_MapGroupRows, w) >= 0 || FindWidgetRow(m_MapServerRows, w) >= 0;
    }

    protected int FindWidgetRow(array<Widget> rows, Widget w)
    {
        if (!rows || !w)
            return -1;

        for (int row = 0; row < rows.Count(); row++)
        {
            if (rows.Get(row) == w)
                return row;
        }

        return -1;
    }

    protected void SelectMapMarkerFromChannelList(array<int> indexList, int row)
    {
        if (!indexList || row < 0 || row >= indexList.Count())
            return;

        int markerIndex = indexList.Get(row);
        if (markerIndex == MAP_SIDEBAR_OVERFLOW_ROW)
        {
            SetPage(PAGE_MARKERS);
            return;
        }

        SelectMarkerByIndex(markerIndex);
        SparkZCoreMapPoint marker = GetSelectedPrivateMarker();
        if (marker && m_MapWidget)
            m_MapWidget.SetMapPos(marker.Position);
    }

    protected void SelectMapServerMarkerFromList(int row)
    {
        if (!m_MapServerListIndexes || row < 0 || row >= m_MapServerListIndexes.Count())
            return;

        int markerIndex = m_MapServerListIndexes.Get(row);
        if (markerIndex == MAP_SIDEBAR_OVERFLOW_ROW)
        {
            SetPage(PAGE_MARKERS);
            return;
        }

        SelectServerMarkerByIndex(markerIndex);
        SparkZCoreMapPoint marker = GetSelectedServerMarker();
        if (marker && m_MapWidget)
            m_MapWidget.SetMapPos(marker.Position);
    }

    protected void SelectMarkerByIndex(int markerIndex)
    {
        SparkZCoreMapPoint marker = SparkZGroupMarkerCache.Get().GetPrivateMarker(markerIndex);
        if (!marker)
        {
            m_SelectedMarkerIndex = -1;
            UpdateMarkerSelectionDetail(-1);
            return;
        }

        m_SelectedMarkerIndex = markerIndex;
        m_SelectedServerMarkerIndex = -1;
        SelectVisibleChannelRow(markerIndex);
        UpdateMarkerSelectionDetail(markerIndex);
    }

    protected void SelectServerMarkerByIndex(int markerIndex)
    {
        SparkZCoreMapPoint marker = SparkZGroupMarkerCache.Get().GetServerMarker(markerIndex);
        if (!marker)
        {
            m_SelectedServerMarkerIndex = -1;
            UpdateMarkerSelectionDetail(-1);
            return;
        }

        m_SelectedMarkerIndex = -1;
        m_SelectedServerMarkerIndex = markerIndex;
        SelectVisibleServerRow(markerIndex);
        UpdateServerMarkerSelectionDetail(markerIndex);
    }

    protected void SelectVisibleServerRow(int markerIndex)
    {
        int row = FindMarkerIndexInList(m_ServerListIndexes, markerIndex);
        if (m_ServerMarkerList && row >= 0)
            m_ServerMarkerList.SelectRow(row);
    }

    protected void SelectVisibleChannelRow(int markerIndex)
    {
        int channel = SparkZGroupMarkerCache.Get().GetPrivateMarkerChannel(markerIndex);
        int row = GetChannelListRow(markerIndex, channel);

        if (channel == SparkZGroupMarkerCache.MARKER_CHANNEL_GROUP && m_GroupMarkerList && row >= 0)
        {
            m_GroupMarkerList.SelectRow(row);
            return;
        }

        if (m_MarkerList && row >= 0)
            m_MarkerList.SelectRow(row);
    }

    protected int GetChannelListRow(int markerIndex, int channel)
    {
        if (channel == SparkZGroupMarkerCache.MARKER_CHANNEL_GROUP)
            return FindMarkerIndexInList(m_GroupListIndexes, markerIndex);

        return FindMarkerIndexInList(m_PrivateListIndexes, markerIndex);
    }

    protected int FindMarkerIndexInList(array<int> indexList, int markerIndex)
    {
        if (!indexList)
            return -1;

        for (int row = 0; row < indexList.Count(); row++)
        {
            if (indexList.Get(row) == markerIndex)
                return row;
        }

        return -1;
    }

    protected int GetFirstExistingMarkerIndex()
    {
        if (SparkZGroupMarkerCache.Get().GetPrivateMarkerCount() <= 0)
            return -1;

        return 0;
    }

    protected void NormalizeMarkerSelections()
    {
        if (m_SelectedMarkerIndex >= SparkZGroupMarkerCache.Get().GetPrivateMarkerCount())
            m_SelectedMarkerIndex = -1;

        if (m_SelectedServerMarkerIndex >= SparkZGroupMarkerCache.Get().GetServerMarkerCount())
            m_SelectedServerMarkerIndex = -1;
    }

    protected string BuildRemoteMarkerListLabel(SparkZCoreMarkerPayload marker)
    {
        if (!marker)
            return "";

        string ownerName = GetSquadMemberDisplayName(marker.OwnerId);
        return marker.Label + "  " + FormatCoordinate(marker.Position[0]) + " / " + FormatCoordinate(marker.Position[2]) + "  (" + ownerName + ")";
    }

    protected string GetSquadMemberDisplayName(string plainId)
    {
        SparkZCoreGroupMemberPayload member = SparkZLocalGroupSession.Get().FindMemberByPlainId(plainId);
        if (member && member.Name != "")
            return member.Name;

        return "Squadmate";
    }

    protected void SelectRemoteSquadMarker(int remoteIndex)
    {
        if (!m_DisplayedRemoteSquadMarkers || remoteIndex < 0 || remoteIndex >= m_DisplayedRemoteSquadMarkers.Count())
            return;

        SparkZCoreMarkerPayload marker = m_DisplayedRemoteSquadMarkers.Get(remoteIndex);
        if (!marker)
            return;

        m_SelectedMarkerIndex = -1;
        m_SelectedServerMarkerIndex = -1;
        SetTextSafe(m_MarkerDetail, marker.Label + "\nPOS " + FormatCoordinate(marker.Position[0]) + " / " + FormatCoordinate(marker.Position[2]) + "\nSQUAD PIN / " + GetSquadMemberDisplayName(marker.OwnerId) + " / READ-ONLY");
    }

    protected string BuildMarkerListLabel(SparkZCoreMapPoint marker)
    {
        return marker.Label + "  " + FormatCoordinate(marker.Position[0]) + " / " + FormatCoordinate(marker.Position[2]);
    }

    protected string BuildCompactMarkerListLabel(SparkZCoreMapPoint marker)
    {
        return marker.Label + "  " + FormatCoordinate(marker.Position[0]) + " / " + FormatCoordinate(marker.Position[2]);
    }

    protected string BuildMapOverviewText()
    {
        int privateCount = SparkZGroupMarkerCache.Get().GetPrivateMarkerCountByChannel(SparkZGroupMarkerCache.MARKER_CHANNEL_PRIVATE);
        int groupCount = SparkZGroupMarkerCache.Get().GetPrivateMarkerCountByChannel(SparkZGroupMarkerCache.MARKER_CHANNEL_GROUP);
        int serverCount = SparkZGroupMarkerCache.Get().GetServerMarkerCount();
        return "EVENT NET: LIVE\nPRIVATE: " + privateCount + "\nSQUAD: " + groupCount + "\nSERVER POIS: " + serverCount;
    }

    protected void CenterSelectedPrivateMarker()
    {
        if (m_SelectedServerMarkerIndex >= 0)
        {
            CenterSelectedServerMarker();
            return;
        }

        SparkZCoreMapPoint marker = GetSelectedPrivateMarker();
        if (!marker || !m_MapWidget)
            return;

        SetPage(PAGE_MAP);
        m_MapWidget.SetMapPos(marker.Position);
        RefreshMapMarkers();
    }

    protected void CenterSelectedServerMarker()
    {
        SparkZCoreMapPoint marker = GetSelectedServerMarker();
        if (!marker || !m_MapWidget)
            return;

        SetPage(PAGE_MAP);
        m_MapWidget.SetMapPos(marker.Position);
        RefreshMapMarkers();
    }

    protected void DeleteSelectedPrivateMarker()
    {
        if (m_SelectedMarkerIndex < 0)
            return;

        SparkZCoreMapPoint markerBeingDeleted = SparkZGroupMarkerCache.Get().GetPrivateMarker(m_SelectedMarkerIndex);
        int channelBeingDeleted = SparkZGroupMarkerCache.Get().GetPrivateMarkerChannel(m_SelectedMarkerIndex);

        if (!SparkZGroupMarkerCache.Get().RemovePrivateMarker(m_SelectedMarkerIndex))
            return;

        if (markerBeingDeleted && channelBeingDeleted == SparkZGroupMarkerCache.MARKER_CHANNEL_GROUP)
            SparkZGroupNetworkClient.Get().RequestMarkerDelete(markerBeingDeleted.Id);

        m_SelectedMarkerIndex = -1;
        RefreshMapMarkers();
        PopulateMarkerList();
        UpdateMarkerText();
    }

    protected void SyncMarkerNetworkStateIfNeeded(int markerIndex)
    {
        SparkZCoreMapPoint marker = SparkZGroupMarkerCache.Get().GetPrivateMarker(markerIndex);
        if (!marker)
            return;

        int channel = SparkZGroupMarkerCache.Get().GetPrivateMarkerChannel(markerIndex);
        if (channel != SparkZGroupMarkerCache.MARKER_CHANNEL_GROUP)
        {
            SparkZGroupNetworkClient.Get().RequestMarkerDelete(marker.Id);
            return;
        }

        SparkZCoreMarkerPayload payload = new SparkZCoreMarkerPayload();
        payload.Set(marker.Id, marker.Label, marker.Position, marker.ColorARGB, GetPrivateMarkerIconSafe(markerIndex), SparkZGroupMarkerCache.MARKER_CHANNEL_GROUP, "", "", marker.Revision, 0.0, SparkZGroupMarkerCache.Get().IsPrivateMarker3DVisible(markerIndex));
        SparkZGroupNetworkClient.Get().RequestMarkerUpsert(payload);
    }

    protected void DeleteSelectedMarkerFromMapSidebar()
    {
        if (m_SelectedMarkerIndex == MAP_SIDEBAR_OVERFLOW_ROW || m_SelectedServerMarkerIndex == MAP_SIDEBAR_OVERFLOW_ROW)
            return;

        if (m_SelectedServerMarkerIndex >= 0)
        {
            ShowServerChannelLocked();
            return;
        }

        DeleteSelectedPrivateMarker();
    }

    protected void OpenSelectedMarkerEditor()
    {
        if (m_SelectedMarkerIndex == MAP_SIDEBAR_OVERFLOW_ROW || m_SelectedServerMarkerIndex == MAP_SIDEBAR_OVERFLOW_ROW)
        {
            SetPage(PAGE_MARKERS);
            return;
        }

        if (m_SelectedMarkerIndex < 0 && m_SelectedServerMarkerIndex < 0)
            return;

        int markerIndex = m_SelectedMarkerIndex;
        int serverMarkerIndex = m_SelectedServerMarkerIndex;
        SetPage(PAGE_MARKERS);

        if (markerIndex >= 0)
            SelectMarkerByIndex(markerIndex);
        else if (serverMarkerIndex >= 0)
            SelectServerMarkerByIndex(serverMarkerIndex);
    }

    protected void ClearPrivateMarkers()
    {
        SparkZGroupMarkerCache.Get().ClearPrivateMarkers();
        m_SelectedMarkerIndex = -1;
        RefreshMapMarkers();
        PopulateMarkerList();
        UpdateMarkerText();
    }

    protected void ApplySelectedMarkerName()
    {
        if (!ApplyPendingMarkerNameIfNeeded())
            return;

        int markerIndex = m_SelectedMarkerIndex;
        RefreshMapMarkers();
        PopulateMarkerList();
        SelectMarkerByIndex(markerIndex);
        UpdateMarkerText();
        SyncMarkerNetworkStateIfNeeded(markerIndex);
    }

    protected bool ApplyPendingMarkerNameIfNeeded()
    {
        if (!m_MarkerNameEdit || m_SelectedMarkerIndex < 0)
            return false;

        SparkZCoreMapPoint marker = SparkZGroupMarkerCache.Get().GetPrivateMarker(m_SelectedMarkerIndex);
        if (!marker)
            return false;

        string pendingName = m_MarkerNameEdit.GetText();
        if (pendingName == "")
            return false;

        if (pendingName == marker.Label)
            return true;

        return SparkZGroupMarkerCache.Get().RenamePrivateMarker(m_SelectedMarkerIndex, pendingName);
    }

    protected void SetSelectedMarkerColor(int colorARGB)
    {
        if (m_SelectedMarkerIndex < 0)
            return;

        string pendingName = GetPendingMarkerNameText();
        if (!SparkZGroupMarkerCache.Get().SetPrivateMarkerColor(m_SelectedMarkerIndex, colorARGB))
            return;

        RefreshMapMarkers();
        UpdateMarkerSelectionDetail(m_SelectedMarkerIndex);
        RestorePendingMarkerNameText(pendingName);
        UpdateMarkerText();
        SyncMarkerNetworkStateIfNeeded(m_SelectedMarkerIndex);
    }

    protected void SetSelectedMarkerIcon(string iconPath)
    {
        if (m_SelectedMarkerIndex < 0)
            return;

        string pendingName = GetPendingMarkerNameText();
        if (!SparkZGroupMarkerCache.Get().SetPrivateMarkerIcon(m_SelectedMarkerIndex, iconPath))
            return;

        RefreshMapMarkers();
        UpdateMarkerSelectionDetail(m_SelectedMarkerIndex);
        RestorePendingMarkerNameText(pendingName);
        UpdateMarkerText();
        SyncMarkerNetworkStateIfNeeded(m_SelectedMarkerIndex);
    }

    protected void SetSelectedMarkerChannel(int channel)
    {
        int markerIndex = m_SelectedMarkerIndex;
        ApplyPendingMarkerNameIfNeeded();
        if (!SparkZGroupMarkerCache.Get().SetPrivateMarkerChannel(markerIndex, channel))
            return;

        if (channel == SparkZGroupMarkerCache.MARKER_CHANNEL_GROUP)
            SparkZGroupMarkerCache.Get().EnsurePrivateMarkerSquadId(markerIndex, SparkZLocalGroupSession.Get().GetLocalMemberId());

        PopulateMarkerList();
        SelectMarkerByIndex(markerIndex);
        UpdateMarkerText();
        SyncMarkerNetworkStateIfNeeded(markerIndex);
    }

    protected void ToggleSelectedMarker3D()
    {
        int markerIndex = m_SelectedMarkerIndex;
        ApplyPendingMarkerNameIfNeeded();
        if (!SparkZGroupMarkerCache.Get().TogglePrivateMarker3DVisible(markerIndex))
        {
            if (m_SelectedServerMarkerIndex >= 0)
                ShowServerChannelLocked();

            return;
        }

        UpdateMarkerSelectionDetail(markerIndex);
        UpdateMarkerText();
        SyncMarkerNetworkStateIfNeeded(markerIndex);
    }

    protected string GetPendingMarkerNameText()
    {
        if (!m_MarkerNameEdit)
            return "";

        return m_MarkerNameEdit.GetText();
    }

    protected void RestorePendingMarkerNameText(string pendingName)
    {
        if (m_MarkerNameEdit)
            m_MarkerNameEdit.SetText(pendingName);
    }

    protected SparkZCoreMapPoint GetSelectedPrivateMarker()
    {
        return SparkZGroupMarkerCache.Get().GetPrivateMarker(m_SelectedMarkerIndex);
    }

    protected SparkZCoreMapPoint GetSelectedServerMarker()
    {
        return SparkZGroupMarkerCache.Get().GetServerMarker(m_SelectedServerMarkerIndex);
    }

    protected void UpdateMarkerSelectionDetail(int row)
    {
        if (!m_MarkerDetail)
            return;

        SparkZCoreMapPoint marker = SparkZGroupMarkerCache.Get().GetPrivateMarker(row);
        if (!marker)
        {
            SetTextSafe(m_MarkerDetail, "NO PRIVATE MARKER SELECTED\nDOUBLE-CLICK MAP TO ADD ONE");
            if (m_MarkerNameEdit)
                m_MarkerNameEdit.SetText("");
            SetColorSlidersFromColor(ARGB(255, 255, 216, 64));
            UpdateMarkerIconPreview(DEFAULT_MARKER_ICON);
            return;
        }

        if (m_MarkerNameEdit)
            m_MarkerNameEdit.SetText(marker.Label);

        SetColorSlidersFromColor(marker.ColorARGB);
        UpdateMarkerIconPreview(GetPrivateMarkerIconSafe(row));
        SetTextSafe(m_MarkerDetail, marker.Label + "\nPOS " + FormatCoordinate(marker.Position[0]) + " / " + FormatCoordinate(marker.Position[2]) + "\n" + GetMarkerChannelLabel(row) + "\n" + GetMarker3DLabel(row));
    }

    protected void UpdateServerMarkerSelectionDetail(int row)
    {
        if (!m_MarkerDetail)
            return;

        SparkZCoreMapPoint marker = SparkZGroupMarkerCache.Get().GetServerMarker(row);
        if (!marker)
        {
            UpdateMarkerSelectionDetail(-1);
            return;
        }

        if (m_MarkerNameEdit)
            m_MarkerNameEdit.SetText(marker.Label);

        SetColorSlidersFromColor(marker.ColorARGB);
        UpdateMarkerIconPreview(GetServerMarkerIconSafe(row));
        SetTextSafe(m_MarkerDetail, marker.Label + "\nPOS " + FormatCoordinate(marker.Position[0]) + " / " + FormatCoordinate(marker.Position[2]) + "\nSERVER FIXED POI / LOCKED");
    }

    protected void ShowServerChannelLocked()
    {
        SetTextSafe(m_MarkerDetail, "SERVER EVENTS ARE LOCKED\nFIXED POIS ARE OWNED BY THE SERVER");

        if (m_CurrentPage == PAGE_MAP)
            SetTextSafe(m_PageBody, "LOCAL SIGNAL: STABLE\nMAP SURFACE: STABLE\nFIXED POIS ARE SERVER LOCKED");
    }

    protected string GetMarkerChannelLabel(int markerIndex)
    {
        int channel = SparkZGroupMarkerCache.Get().GetPrivateMarkerChannel(markerIndex);
        if (channel == SparkZGroupMarkerCache.MARKER_CHANNEL_GROUP)
            return "SQUAD CHANNEL / LOCAL PREVIEW";

        if (channel == SparkZGroupMarkerCache.MARKER_CHANNEL_SERVER)
            return "SERVER CHANNEL / LOCAL PREVIEW";

        return "PRIVATE CHANNEL / LOCAL SESSION";
    }

    protected string GetMarker3DLabel(int markerIndex)
    {
        if (SparkZGroupMarkerCache.Get().IsPrivateMarker3DVisible(markerIndex))
            return "3D MARKER: ON";

        return "3D MARKER: OFF";
    }

    protected void BuildMarkerIconList()
    {
        m_AvailableMarkerIcons = new array<string>();
        m_AvailableMarkerIconNames = new array<string>();

        AddMarkerIcon("Marker", "marker.paa");
        AddMarkerIcon("Ping", "ping.paa");
        AddMarkerIcon("Flag", "flag.paa");
        AddMarkerIcon("Star", "star.paa");
        AddMarkerIcon("Triangle", "triangle.paa");
        AddMarkerIcon("Circle", "circle.paa");
        AddMarkerIcon("Cross", "cross.paa");
        AddMarkerIcon("Skull", "skull.paa");
        AddMarkerIcon("Camp", "camp.paa");
        AddMarkerIcon("Home", "home.paa");
        AddMarkerIcon("Car", "car.paa");
        AddMarkerIcon("Repair", "car-repair.paa");
        AddMarkerIcon("Heli", "heli.paa");
        AddMarkerIcon("Hospital", "hospital.paa");
        AddMarkerIcon("Safezone", "safezone.paa");
        AddMarkerIcon("Black Market", "blackmarket.paa");
        AddMarkerIcon("Dealer", "ranger-station.paa");
        AddMarkerIcon("Sniper", "sniper.paa");
        AddMarkerIcon("Water", "water.paa");
        AddMarkerIcon("Radio", "communications.paa");
        AddMarkerIcon("Post", "post.paa");
        AddMarkerIcon("Roadblock", "roadblock.paa");
        AddMarkerIcon("Ship", "ship.paa");
        AddMarkerIcon("Castle", "castle.paa");
    }

    protected void AddMarkerIcon(string displayName, string fileName)
    {
        m_AvailableMarkerIconNames.Insert(displayName);
        m_AvailableMarkerIcons.Insert("SparkZGroup\\gui\\icons\\markers\\" + fileName);
    }

    protected string GetPrivateMarkerIconSafe(int row)
    {
        string iconPath = SparkZGroupMarkerCache.Get().GetPrivateMarkerIcon(row);
        if (iconPath == "")
            return DEFAULT_MARKER_ICON;

        return iconPath;
    }

    protected string GetServerMarkerIconSafe(int row)
    {
        string iconPath = SparkZGroupMarkerCache.Get().GetServerMarkerIcon(row);
        if (iconPath == "")
            return DEFAULT_MARKER_ICON;

        return iconPath;
    }

    protected void StepSelectedMarkerIcon(int direction)
    {
        if (!m_MarkerList || !m_AvailableMarkerIcons || m_AvailableMarkerIcons.Count() == 0)
            return;

        if (!SparkZGroupMarkerCache.Get().GetPrivateMarker(m_SelectedMarkerIndex))
            return;

        int iconIndex = GetMarkerIconIndex(GetPrivateMarkerIconSafe(m_SelectedMarkerIndex));
        iconIndex = iconIndex + direction;

        if (iconIndex < 0)
            iconIndex = m_AvailableMarkerIcons.Count() - 1;

        if (iconIndex >= m_AvailableMarkerIcons.Count())
            iconIndex = 0;

        SetSelectedMarkerIcon(m_AvailableMarkerIcons.Get(iconIndex));
    }

    protected int GetMarkerIconIndex(string iconPath)
    {
        if (!m_AvailableMarkerIcons)
            return 0;

        for (int index = 0; index < m_AvailableMarkerIcons.Count(); index++)
        {
            if (m_AvailableMarkerIcons.Get(index) == iconPath)
                return index;
        }

        return 0;
    }

    protected void UpdateMarkerIconPreview(string iconPath)
    {
        int iconIndex = GetMarkerIconIndex(iconPath);
        if (m_MarkerIconPreview)
        {
            m_MarkerIconPreview.LoadImageFile(0, m_AvailableMarkerIcons.Get(iconIndex));
            m_MarkerIconPreview.SetColor(GetCurrentEditorColor());
        }

        if (m_MarkerIconName)
            m_MarkerIconName.SetText("");
    }

    protected void CreateLocalGroupPreview()
    {
        string memberId = "local";
        string memberName = "You";
        string groupName = GetLocalGroupNameInput();
        string groupTag = GetLocalGroupTagInput();

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player)
        {
            PlayerIdentity identity = player.GetIdentity();
            if (identity)
            {
                if (identity.GetPlainId() != "")
                    memberId = identity.GetPlainId();

                if (identity.GetName() != "")
                    memberName = identity.GetName();
            }
        }

        SparkZLocalGroupSession.Get().CreateLocalPreview(memberId, memberName, groupName, groupTag);
        SparkZGroupNetworkClient.Get().RequestCreateGroup(groupName, groupTag);
        m_GroupIdentityDraftDirty = false;
        m_GroupIdentityFieldsInitialized = false;
        RefreshLocalGroupSessionControls();
    }

    protected void LeaveLocalGroupPreview()
    {
        SparkZGroupNetworkClient.Get().RequestLeaveGroup();
        SparkZLocalGroupSession.Get().Leave();
        m_GroupIdentityDraftDirty = false;
        m_GroupIdentityFieldsInitialized = false;
        m_GroupInviteStatusOverride = "";
        RefreshLocalGroupSessionControls();
    }

    protected void RefreshLocalGroupSessionControls()
    {
        SparkZLocalGroupSession session = SparkZLocalGroupSession.Get();
        RefreshLocalGroupIdentityFields(session);

        if (session.IsActive())
        {
            string groupAuthority = "LOCAL SQUAD";
            if (session.IsServerBacked())
                groupAuthority = "SERVER SQUAD";

            SetTextSafe(m_PageBody, groupAuthority + ": ACTIVE\nTAG: " + session.GetChatPrefix() + "\nMEMBERS: " + session.GetMemberCount() + "\nSQUAD CHAT: READY");
            SetTextSafe(m_GroupSessionText, session.GetGroupName() + " " + session.GetChatPrefix() + "\nLEADER: " + session.GetLeaderName() + "\nIDENTITY LOCKED UNTIL LEAVE\nCLICK A ROSTER MEMBER TO MANAGE.");
            RefreshGroupRosterList(session);
            RefreshGroupInviteControls();
            RefreshSelectedGroupMemberPanel();

            if (m_GroupCreateButton)
                m_GroupCreateButton.Show(false);

            if (m_GroupLeaveButton)
                m_GroupLeaveButton.Show(true);

            ShowWidgetSafe(m_GroupNameLabel, false);
            ShowWidgetSafe(m_GroupTagLabel, false);
            ShowWidgetSafe(m_GroupNameEdit, false);
            ShowWidgetSafe(m_GroupTagEdit, false);

            if (m_GroupIdentityApplyButton)
                m_GroupIdentityApplyButton.Show(false);

            return;
        }

        SetTextSafe(m_PageBody, "SQUAD: OFF\nNETWORK: READY\nROSTER: READY\nCHAT TAG: RESERVED");
        if (SparkZGroupInviteState.Get().HasInvite())
            SetTextSafe(m_GroupSessionText, "PENDING SQUAD INVITE\nACCEPT OR DECLINE ON THE LEFT.");
        else
            SetTextSafe(m_GroupSessionText, "NO ACTIVE SQUAD.\nSET NAME/TAG, THEN CREATE SQUAD.");
        ClearGroupRosterList();
        RefreshGroupInviteControls();

        if (m_GroupCreateButton)
            m_GroupCreateButton.Show(true);

        if (m_GroupLeaveButton)
            m_GroupLeaveButton.Show(false);

        ShowWidgetSafe(m_GroupNameLabel, true);
        ShowWidgetSafe(m_GroupTagLabel, true);
        ShowWidgetSafe(m_GroupNameEdit, true);
        ShowWidgetSafe(m_GroupTagEdit, true);

        if (m_GroupIdentityApplyButton)
            m_GroupIdentityApplyButton.Show(false);
    }

    protected void RefreshGroupInviteControls()
    {
        bool activeGroup = SparkZLocalGroupSession.Get().IsActive();
        bool hasInvite = SparkZGroupInviteState.Get().HasInvite();
        bool canManage = activeGroup && !hasInvite && IsLocalGroupLeader() && m_SelectedGroupMemberId != "" && m_SelectedGroupMemberId != SparkZLocalGroupSession.Get().GetLocalMemberId();

        ShowWidgetSafe(m_GroupInviteLabel, activeGroup || hasInvite);
        ShowWidgetSafe(m_GroupInviteNameEdit, activeGroup && !hasInvite);
        ShowWidgetSafe(m_GroupInviteButton, activeGroup && !hasInvite);
        ShowWidgetSafe(m_GroupKickButton, canManage);
        ShowWidgetSafe(m_GroupPromoteButton, false);
        ShowWidgetSafe(m_GroupRosterList, activeGroup);
        ShowWidgetSafe(m_GroupSelectedMemberText, activeGroup && !hasInvite);
        ShowWidgetSafe(m_GroupPermBuildButton, canManage);
        ShowWidgetSafe(m_GroupPermDoorButton, canManage);
        ShowWidgetSafe(m_GroupPermStorageButton, canManage);
        ShowWidgetSafe(m_GroupPermApplyButton, canManage);
        ShowWidgetSafe(m_GroupPermStatusText, activeGroup && !hasInvite);

        if (hasInvite)
        {
            SetTextSafe(m_GroupInviteLabel, "SQUAD INVITE");
            SetTextSafe(m_GroupInvitePendingText, SparkZGroupInviteState.Get().BuildInviteText());
            if (m_GroupInviteNameEdit)
                m_GroupInviteNameEdit.SetText("");

            SetTextSafe(m_GroupSelectedMemberText, "");
            SetTextSafe(m_GroupPermStatusText, "");
        }
        else
        {
            SetTextSafe(m_GroupInviteLabel, "INVITE PLAYER");
            if (activeGroup)
            {
                string networkStatus = SparkZGroupInviteState.Get().GetStatusText();
                if (networkStatus != "")
                    SetTextSafe(m_GroupInvitePendingText, networkStatus);
                else if (m_GroupInviteStatusOverride != "")
                    SetTextSafe(m_GroupInvitePendingText, m_GroupInviteStatusOverride);
                else
                    SetTextSafe(m_GroupInvitePendingText, "TYPE EXACT PLAYER NAME, THEN INVITE.");

                RefreshSelectedGroupMemberPanel();
            }
            else
            {
                SetTextSafe(m_GroupInvitePendingText, "");
                SetTextSafe(m_GroupSelectedMemberText, "");
                SetTextSafe(m_GroupPermStatusText, "");
            }
        }

        ShowWidgetSafe(m_GroupInvitePendingText, activeGroup || hasInvite);

        if (m_GroupInviteAcceptButton)
            m_GroupInviteAcceptButton.Show(hasInvite);

        if (m_GroupInviteDeclineButton)
            m_GroupInviteDeclineButton.Show(hasInvite);
    }

    protected void RefreshGroupRosterList(SparkZLocalGroupSession session)
    {
        if (!m_GroupRosterIds)
            m_GroupRosterIds = new array<string>();

        m_GroupRosterIds.Clear();

        if (m_GroupRosterList)
            m_GroupRosterList.ClearItems();

        if (!session || !session.IsActive())
        {
            m_SelectedGroupMemberId = "";
            m_SelectedGroupMemberName = "";
            m_SelectedGroupMemberRoleFlags = 0;
            return;
        }

        bool selectionStillExists = false;
        int selectedRow = -1;
        int firstManageableRow = -1;
        string localMemberId = session.GetLocalMemberId();
        array<ref SparkZCoreGroupMemberPayload> members = session.GetMembers();

        for (int index = 0; index < members.Count(); index++)
        {
            SparkZCoreGroupMemberPayload member = members.Get(index);
            if (!member || !member.IsValid())
                continue;

            m_GroupRosterIds.Insert(member.PlainId);
            if (m_GroupRosterList)
                m_GroupRosterList.AddItem(BuildGroupRosterListLabel(member, localMemberId), null, 0);

            if (member.PlainId == m_SelectedGroupMemberId)
            {
                selectionStillExists = true;
                selectedRow = m_GroupRosterIds.Count() - 1;
            }

            if (firstManageableRow < 0 && member.PlainId != localMemberId)
                firstManageableRow = m_GroupRosterIds.Count() - 1;
        }

        if (!selectionStillExists)
        {
            if (firstManageableRow >= 0)
                SelectGroupRosterMember(firstManageableRow);
            else
            {
                m_SelectedGroupMemberId = "";
                m_SelectedGroupMemberName = "";
                m_SelectedGroupMemberRoleFlags = 0;
            }
        }
        else if (m_GroupRosterList && selectedRow >= 0)
        {
            m_GroupRosterList.SelectRow(selectedRow);
        }
    }

    protected void ClearGroupRosterList()
    {
        if (!m_GroupRosterIds)
            m_GroupRosterIds = new array<string>();

        m_GroupRosterIds.Clear();
        if (m_GroupRosterList)
            m_GroupRosterList.ClearItems();

        m_SelectedGroupMemberId = "";
        m_SelectedGroupMemberName = "";
        m_SelectedGroupMemberRoleFlags = 0;
    }

    protected void SelectGroupRosterMember(int row)
    {
        if (!m_GroupRosterIds || row < 0 || row >= m_GroupRosterIds.Count())
            return;

        SparkZLocalGroupSession session = SparkZLocalGroupSession.Get();
        SparkZCoreGroupMemberPayload member = session.FindMemberByPlainId(m_GroupRosterIds.Get(row));
        if (!member)
            return;

        m_SelectedGroupMemberId = member.PlainId;
        m_SelectedGroupMemberName = member.Name;
        m_SelectedGroupMemberRoleFlags = member.RoleFlags;

        if (m_GroupRosterList)
            m_GroupRosterList.SelectRow(row);

        RefreshSelectedGroupMemberPanel();
        RefreshGroupInviteControls();
    }

    protected string BuildGroupRosterListLabel(SparkZCoreGroupMemberPayload member, string localMemberId)
    {
        string label = member.Name;
        if (label == "")
            label = "Survivor";

        if (member.PlainId == localMemberId)
            label = label + "  / YOU";

        if (SparkZLocalGroupSession.HasRoleFlag(member.RoleFlags, SparkZGroupConstants.SQUAD_ROLE_LEADER))
            label = label + "  / LEADER";

        string permissions = SparkZLocalGroupSession.BuildPermissionText(member.RoleFlags);
        if (permissions != "")
            label = label + "  / " + permissions;

        if (!member.Online)
            label = label + "  / OFFLINE";

        return label;
    }

    protected bool IsLocalGroupLeader()
    {
        SparkZLocalGroupSession session = SparkZLocalGroupSession.Get();
        SparkZCoreGroupMemberPayload localMember = session.FindMemberByPlainId(session.GetLocalMemberId());
        if (!localMember)
            return false;

        return SparkZLocalGroupSession.HasRoleFlag(localMember.RoleFlags, SparkZGroupConstants.SQUAD_ROLE_LEADER);
    }

    protected void RefreshSelectedGroupMemberPanel()
    {
        if (m_SelectedGroupMemberId == "")
        {
            SetTextSafe(m_GroupSelectedMemberText, "SELECT A ROSTER MEMBER TO MANAGE.");
            SetTextSafe(m_GroupPermStatusText, "");
            SetPermissionButtonStates();
            return;
        }

        SparkZCoreGroupMemberPayload member = SparkZLocalGroupSession.Get().FindMemberByPlainId(m_SelectedGroupMemberId);
        if (member)
        {
            // Only refresh the name here - the role flags are the local, possibly-unsaved
            // edit in progress (set by SelectGroupRosterMember on selection and mutated by
            // ToggleSelectedGroupPermission). Overwriting them from the synced session data
            // on every refresh would silently discard every toggle click before Save Perms
            // ever sends it.
            m_SelectedGroupMemberName = member.Name;
        }

        string permissions = SparkZLocalGroupSession.BuildPermissionText(m_SelectedGroupMemberRoleFlags);
        if (permissions == "")
            permissions = "NO BASE PERMISSIONS";

        SetTextSafe(m_GroupSelectedMemberText, "SELECTED: " + m_SelectedGroupMemberName + "\n" + permissions);
        SetTextSafe(m_GroupPermStatusText, "BASE ACCESS: BUILD, DISMANTLE, DOORS, STORAGE\nBASE MANAGER: CLAIM AND PAY BASE UPKEEP.");
        SetPermissionButtonStates();
    }

    protected void SetPermissionButtonStates()
    {
        SetToggleWidgetColor(m_GroupPermBuildButton, SparkZLocalGroupSession.HasBaseAccess(m_SelectedGroupMemberRoleFlags));
        SetToggleWidgetColor(m_GroupPermDoorButton, SparkZLocalGroupSession.HasRoleFlag(m_SelectedGroupMemberRoleFlags, SparkZGroupConstants.SQUAD_PERMISSION_BASE_MANAGER));
        SetToggleWidgetColor(m_GroupPermStorageButton, SparkZLocalGroupSession.HasRoleFlag(m_SelectedGroupMemberRoleFlags, SparkZGroupConstants.SQUAD_ROLE_LEADER));
    }

    protected void SetToggleWidgetColor(Widget widget, bool active)
    {
        if (!widget)
            return;

        if (active)
            widget.SetColor(ARGB(255, 216, 154, 36));
        else
            widget.SetColor(ARGB(255, 90, 94, 96));
    }

    protected void ApplyLocalGroupIdentity()
    {
        string groupName = GetLocalGroupNameInput();
        string groupTag = GetLocalGroupTagInput();

        SparkZLocalGroupSession.Get().SetIdentity(groupName, groupTag);
        if (SparkZLocalGroupSession.Get().IsActive())
            SparkZGroupNetworkClient.Get().RequestCreateGroup(groupName, groupTag);

        m_GroupIdentityDraftDirty = false;
        m_GroupIdentityFieldsInitialized = false;
        RefreshLocalGroupSessionControls();
    }

    protected void SendGroupInvite()
    {
        if (!m_GroupInviteNameEdit)
            return;

        string targetName = m_GroupInviteNameEdit.GetText();
        if (targetName == "")
        {
            m_GroupInviteStatusOverride = "TYPE PLAYER NAME FIRST.";
            RefreshLocalGroupSessionControls();
            return;
        }

        if (!SparkZLocalGroupSession.Get().IsActive())
        {
            m_GroupInviteStatusOverride = "CREATE SQUAD FIRST.";
            RefreshLocalGroupSessionControls();
            return;
        }

        SparkZGroupInviteState.Get().ClearStatusText();
        SparkZGroupNetworkClient.Get().RequestInvite(targetName);
        m_GroupInviteStatusOverride = "CHECKING PLAYER: " + targetName;
        RefreshLocalGroupSessionControls();
    }

    protected void KickGroupMember()
    {
        string targetToken = GetGroupManageTargetToken();
        if (targetToken == "")
            return;

        SparkZGroupNetworkClient.Get().RequestKick(targetToken);
        m_GroupInviteStatusOverride = "KICK REQUEST SENT: " + GetSelectedGroupMemberDisplayName(targetToken);
        RefreshLocalGroupSessionControls();
    }

    protected void PromoteGroupMember()
    {
        string targetToken = GetGroupManageTargetToken();
        if (targetToken == "")
            return;

        SparkZGroupNetworkClient.Get().RequestPromote(targetToken);
        m_GroupInviteStatusOverride = "PROMOTE REQUEST SENT: " + GetSelectedGroupMemberDisplayName(targetToken);
        RefreshLocalGroupSessionControls();
    }

    protected void ToggleSelectedGroupPermission(int permissionFlag)
    {
        if (m_SelectedGroupMemberId == "")
        {
            m_GroupInviteStatusOverride = "SELECT A MEMBER FIRST.";
            RefreshLocalGroupSessionControls();
            return;
        }

        if (SparkZLocalGroupSession.HasRoleFlag(m_SelectedGroupMemberRoleFlags, permissionFlag))
            m_SelectedGroupMemberRoleFlags = m_SelectedGroupMemberRoleFlags & ~permissionFlag;
        else
            m_SelectedGroupMemberRoleFlags = m_SelectedGroupMemberRoleFlags | permissionFlag;

        if (permissionFlag == SparkZGroupConstants.SQUAD_PERMISSION_BASE_MANAGER)
            m_SelectedGroupMemberRoleFlags = m_SelectedGroupMemberRoleFlags | SparkZGroupConstants.SQUAD_PERMISSION_BASE_ACCESS;

        // Manager always implies Access server-side (NormalizeBaseRoleFlags forces Access back on
        // whenever Manager is present) - so unchecking only Access while Manager stayed checked
        // silently got overridden on save, which looked exactly like "revoking doesn't work."
        // Clearing Manager here too keeps the UI and the server's own rule in sync.
        if (permissionFlag == SparkZGroupConstants.SQUAD_PERMISSION_BASE_ACCESS && !SparkZLocalGroupSession.HasRoleFlag(m_SelectedGroupMemberRoleFlags, SparkZGroupConstants.SQUAD_PERMISSION_BASE_ACCESS))
            m_SelectedGroupMemberRoleFlags = m_SelectedGroupMemberRoleFlags & ~SparkZGroupConstants.SQUAD_PERMISSION_BASE_MANAGER;

        RefreshSelectedGroupMemberPanel();
    }

    protected void ApplySelectedGroupPermissions()
    {
        string targetToken = GetGroupManageTargetToken();
        if (targetToken == "")
            return;

        SparkZGroupNetworkClient.Get().RequestPermissions(targetToken, m_SelectedGroupMemberRoleFlags);
        m_GroupInviteStatusOverride = "PERMISSIONS SENT: " + GetSelectedGroupMemberDisplayName(targetToken);
        RefreshLocalGroupSessionControls();
    }

    protected string GetGroupManageTargetToken()
    {
        if (!SparkZLocalGroupSession.Get().IsActive())
        {
            m_GroupInviteStatusOverride = "CREATE SQUAD FIRST.";
            RefreshLocalGroupSessionControls();
            return "";
        }

        if (m_SelectedGroupMemberId != "")
            return m_SelectedGroupMemberId;

        string targetToken = "";
        if (m_GroupInviteNameEdit)
            targetToken = m_GroupInviteNameEdit.GetText();

        if (targetToken == "")
        {
            m_GroupInviteStatusOverride = "SELECT A MEMBER FIRST.";
            RefreshLocalGroupSessionControls();
            return "";
        }

        return targetToken;
    }

    protected string GetSelectedGroupMemberDisplayName(string fallback)
    {
        if (m_SelectedGroupMemberName != "")
            return m_SelectedGroupMemberName;

        return fallback;
    }

    protected void RespondToGroupInvite(bool accepted)
    {
        SparkZGroupNetworkClient.Get().RespondToInvite(accepted);
        m_GroupInviteStatusOverride = "";
        RefreshLocalGroupSessionControls();
    }

    protected void RefreshLocalGroupIdentityFields(SparkZLocalGroupSession session)
    {
        if (!session)
            return;

        if (m_TextInputActive || m_GroupIdentityDraftDirty)
            return;

        if (session.IsActive())
        {
            if (m_GroupNameEdit)
                m_GroupNameEdit.SetText(session.GetGroupName());

            if (m_GroupTagEdit)
                m_GroupTagEdit.SetText(session.GetGroupTag());

            m_GroupIdentityFieldsInitialized = true;
            return;
        }

        if (m_GroupIdentityFieldsInitialized)
            return;

        if (m_GroupNameEdit)
            m_GroupNameEdit.SetText(session.GetGroupName());

        if (m_GroupTagEdit)
            m_GroupTagEdit.SetText(session.GetGroupTag());

        m_GroupIdentityFieldsInitialized = true;
    }

    protected string GetLocalGroupNameInput()
    {
        if (!m_GroupNameEdit)
            return "SparkZ Squad";

        string groupName = m_GroupNameEdit.GetText();
        if (groupName == "")
            return "SparkZ Squad";

        return groupName;
    }

    protected string GetLocalGroupTagInput()
    {
        if (!m_GroupTagEdit)
            return "SPZ";

        string groupTag = m_GroupTagEdit.GetText();
        if (groupTag == "")
            return "SPZ";

        return groupTag;
    }

    protected void RefreshQuickPingProfileControls()
    {
        int colorARGB = SparkZQuickPingProfile.Get().GetColor();

        m_SuppressQuickPingProfileEvents = true;

        if (m_QuickPingRedSlider)
            m_QuickPingRedSlider.SetCurrent(GetRed(colorARGB));

        if (m_QuickPingGreenSlider)
            m_QuickPingGreenSlider.SetCurrent(GetGreen(colorARGB));

        if (m_QuickPingBlueSlider)
            m_QuickPingBlueSlider.SetCurrent(GetBlue(colorARGB));

        m_SuppressQuickPingProfileEvents = false;

        SetTextSafe(m_QuickPingRedValue, "R " + GetRed(colorARGB));
        SetTextSafe(m_QuickPingGreenValue, "G " + GetGreen(colorARGB));
        SetTextSafe(m_QuickPingBlueValue, "B " + GetBlue(colorARGB));
        UpdateQuickPingProfilePreview();
    }

    protected void ApplyQuickPingProfileColorFromSliders()
    {
        int colorARGB = ARGB(255, GetSliderValue(m_QuickPingRedSlider), GetSliderValue(m_QuickPingGreenSlider), GetSliderValue(m_QuickPingBlueSlider));
        SparkZQuickPingProfile.Get().SetColor(colorARGB);
        SparkZGroupMarkerCache.Get().ApplyQuickPingProfileToActivePing();

        SetTextSafe(m_QuickPingRedValue, "R " + GetRed(colorARGB));
        SetTextSafe(m_QuickPingGreenValue, "G " + GetGreen(colorARGB));
        SetTextSafe(m_QuickPingBlueValue, "B " + GetBlue(colorARGB));
        UpdateQuickPingProfilePreview();
    }

    protected void StepQuickPingProfileIcon(int direction)
    {
        SparkZQuickPingProfile.Get().StepIcon(direction);
        SparkZGroupMarkerCache.Get().ApplyQuickPingProfileToActivePing();
        UpdateQuickPingProfilePreview();
    }

    protected void UpdateQuickPingProfilePreview()
    {
        int colorARGB = SparkZQuickPingProfile.Get().GetColor();

        if (m_QuickPingColorPreview)
            m_QuickPingColorPreview.SetColor(colorARGB);

        if (m_QuickPingIconPreview)
        {
            m_QuickPingIconPreview.LoadImageFile(0, SparkZQuickPingProfile.Get().GetIconPath());
            m_QuickPingIconPreview.SetColor(colorARGB);
        }

        SetTextSafe(m_QuickPingIconName, SparkZQuickPingProfile.Get().GetIconName());
        SetTextSafe(m_QuickPingProfileText, "COLOR AND ICON USED FOR FAST SQUAD PINGS.\nCURRENT STYLE UPDATES ACTIVE PINGS TOO.");
    }

    protected void RefreshGroupHudProfileControls()
    {
        int colorARGB = SparkZGroupHudProfile.Get().GetNameTagColor();

        m_SuppressGroupHudProfileEvents = true;

        if (m_GroupHudRedSlider)
            m_GroupHudRedSlider.SetCurrent(GetRed(colorARGB));

        if (m_GroupHudGreenSlider)
            m_GroupHudGreenSlider.SetCurrent(GetGreen(colorARGB));

        if (m_GroupHudBlueSlider)
            m_GroupHudBlueSlider.SetCurrent(GetBlue(colorARGB));

        m_SuppressGroupHudProfileEvents = false;

        SetTextSafe(m_GroupHudRedValue, "R " + GetRed(colorARGB));
        SetTextSafe(m_GroupHudGreenValue, "G " + GetGreen(colorARGB));
        SetTextSafe(m_GroupHudBlueValue, "B " + GetBlue(colorARGB));
        UpdateGroupHudProfilePreview();
    }

    protected void ApplyGroupHudProfileColorFromSliders()
    {
        int colorARGB = ARGB(255, GetSliderValue(m_GroupHudRedSlider), GetSliderValue(m_GroupHudGreenSlider), GetSliderValue(m_GroupHudBlueSlider));
        SparkZGroupHudProfile.Get().SetNameTagColor(colorARGB);

        SetTextSafe(m_GroupHudRedValue, "R " + GetRed(colorARGB));
        SetTextSafe(m_GroupHudGreenValue, "G " + GetGreen(colorARGB));
        SetTextSafe(m_GroupHudBlueValue, "B " + GetBlue(colorARGB));
        UpdateGroupHudProfilePreview();
    }

    protected void UpdateGroupHudProfilePreview()
    {
        int colorARGB = SparkZGroupHudProfile.Get().GetNameTagColor();

        if (m_GroupHudColorPreview)
            m_GroupHudColorPreview.SetColor(colorARGB);

        SetTextSafe(m_GroupHudProfileText, "COLOR USED FOR SQUAD HUD NAMES AND TEAMMATE TAGS.\nDISTANCE AND HEALTH STAY CLEAN FOR FAST READS.");

        if (m_GroupHudPreviewName0)
            m_GroupHudPreviewName0.SetColor(colorARGB);

        if (m_GroupHudPreviewName1)
            m_GroupHudPreviewName1.SetColor(colorARGB);

        if (m_GroupHudPreviewName2)
            m_GroupHudPreviewName2.SetColor(colorARGB);
    }

    protected void ApplySelectedMarkerColorFromSliders()
    {
        if (m_SelectedMarkerIndex < 0 && m_SelectedServerMarkerIndex >= 0)
        {
            UpdateServerMarkerSelectionDetail(m_SelectedServerMarkerIndex);
            return;
        }

        int colorARGB = GetCurrentEditorColor();
        SetColorSliderTexts(colorARGB);

        if (m_MarkerColorPreview)
            m_MarkerColorPreview.SetColor(colorARGB);

        if (m_MarkerIconPreview)
            m_MarkerIconPreview.SetColor(colorARGB);

        SetSelectedMarkerColor(colorARGB);
    }

    protected void SetColorSlidersFromColor(int colorARGB)
    {
        int red = GetRed(colorARGB);
        int green = GetGreen(colorARGB);
        int blue = GetBlue(colorARGB);

        m_SuppressMarkerEditorEvents = true;

        if (m_MarkerRedSlider)
            m_MarkerRedSlider.SetCurrent(red);

        if (m_MarkerGreenSlider)
            m_MarkerGreenSlider.SetCurrent(green);

        if (m_MarkerBlueSlider)
            m_MarkerBlueSlider.SetCurrent(blue);

        m_SuppressMarkerEditorEvents = false;

        SetColorSliderTexts(colorARGB);

        if (m_MarkerColorPreview)
            m_MarkerColorPreview.SetColor(colorARGB);

        if (m_MarkerIconPreview)
            m_MarkerIconPreview.SetColor(colorARGB);
    }

    protected void SetColorSliderTexts(int colorARGB)
    {
        SetTextSafe(m_MarkerRedValue, "R " + GetRed(colorARGB));
        SetTextSafe(m_MarkerGreenValue, "G " + GetGreen(colorARGB));
        SetTextSafe(m_MarkerBlueValue, "B " + GetBlue(colorARGB));
    }

    protected int GetCurrentEditorColor()
    {
        return ARGB(255, GetSliderValue(m_MarkerRedSlider), GetSliderValue(m_MarkerGreenSlider), GetSliderValue(m_MarkerBlueSlider));
    }

    protected int GetSliderValue(SliderWidget slider)
    {
        if (!slider)
            return 0;

        int value = slider.GetCurrent();
        if (value < 0)
            return 0;

        if (value > 255)
            return 255;

        return value;
    }

    protected int GetRed(int colorARGB)
    {
        return (colorARGB >> 16) & 255;
    }

    protected int GetGreen(int colorARGB)
    {
        return (colorARGB >> 8) & 255;
    }

    protected int GetBlue(int colorARGB)
    {
        return colorARGB & 255;
    }

    protected void UpdatePositionReadout()
    {
        if (!m_CoordinateReadout)
            return;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (!player)
        {
            m_CoordinateReadout.SetText("POS -- / --");
            return;
        }

        vector position = player.GetPosition();
        m_CoordinateReadout.SetText("POS " + FormatCoordinate(position[0]) + " / " + FormatCoordinate(position[2]));
    }

    protected string FormatCoordinate(float value)
    {
        string text = "" + value;
        int dotIndex = text.IndexOf(".");
        if (dotIndex > 0)
            return text.Substring(0, dotIndex);

        return text;
    }

    protected void SetTextSafe(TextWidget widget, string text)
    {
        if (widget)
            widget.SetText(text);
    }

    protected void ShowWidgetSafe(Widget widget, bool visible)
    {
        if (widget)
            widget.Show(visible);
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
}
