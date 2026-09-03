// Build menu: category tabs -> a scrollable grid of recipes (each shown with
// a real live 3D preview of its output kit, via ItemPreviewWidget - no icon
// art needed at all, it renders whatever model the class already has) ->
// selecting one shows a big preview plus required materials/tools (each
// with their own small live preview) and a Build Now button.
class SPKZ_WorkbenchMenu extends UIScriptedMenu
{
 protected static ref SPKZ_WorkbenchMenu s_ActiveMenu;

 // Typed as the generic, universally-known Object rather than the concrete
 // SPKZ_Workbench - this 5_Mission class cannot reference that 4_World-only
 // type at all (World and Mission are sibling script modules; see Pitfall
 // #8 in docs/CODING_STANDARDS.md). Passed straight through to ScriptRPC.Send,
 // which itself takes a plain Object.
 protected Object m_Workbench;
 protected ref SPKZ_WorkbenchRecipeCatalog m_Catalog;
 protected ref array<ref SPKZ_WorkbenchStockEntry> m_Stock;
 protected ref array<string> m_Categories;
 protected string m_SelectedCategory;
 protected SPKZ_WorkbenchRecipe m_SelectedRecipe;
 protected Widget m_SelectedRecipeWidget;

 protected Widget m_Root;
 protected Widget m_TabsSpacer;
 protected Widget m_ItemsGridSpacer;
 protected Widget m_MaterialsGridSpacer;
 protected Widget m_ToolsGridSpacer;
 protected Widget m_NoSelectionText;
 protected Widget m_DetailContainer;
 protected ItemPreviewWidget m_DetailPreview;
 protected TextWidget m_DetailName;
 protected ButtonWidget m_BuildButton;
 protected ButtonWidget m_CloseButton;
 protected TextWidget m_StatusText;

 // Every ItemPreviewWidget needs a real (but purely local, never networked -
 // see GetGame().CreateObject's create_local param) EntityAI instance to
 // render. Tracked here so they get deleted rather than leaking every time
 // the grid/detail panel refreshes or the menu closes.
 protected ref array<EntityAI> m_GridPreviewItems;
 protected ref array<EntityAI> m_DetailPreviewItems;

 void SPKZ_WorkbenchMenu(Object workbench)
 {
  m_Workbench = workbench;
  m_Catalog = new SPKZ_WorkbenchRecipeCatalog();
  m_Stock = new array<ref SPKZ_WorkbenchStockEntry>();
  m_Categories = new array<string>();
  m_GridPreviewItems = new array<EntityAI>();
  m_DetailPreviewItems = new array<EntityAI>();

  // Hide the vanilla hotbar/quickbar so it doesn't show through the menu -
  // same technique vanilla's own full-screen menus use (see e.g.
  // scripts/5_mission/gui/invitemenu.c's constructor/destructor pair).
  if (GetGame() && GetGame().GetMission() && GetGame().GetMission().GetHud())
  {
   GetGame().GetMission().GetHud().ShowHudUI(false);
   GetGame().GetMission().GetHud().ShowQuickbarUI(false);
  }
 }

 void ~SPKZ_WorkbenchMenu()
 {
  if (GetGame() && GetGame().GetMission() && GetGame().GetMission().GetHud())
  {
   GetGame().GetMission().GetHud().ShowHudUI(true);
   GetGame().GetMission().GetHud().ShowQuickbarUI(true);
  }
 }

 override Widget Init()
 {
  m_Root = GetGame().GetWorkspace().CreateWidgets("SparkZBaseBuilding/gui/layouts/sparkz_workbench_menu.layout");
  layoutRoot = m_Root;

  m_TabsSpacer = m_Root.FindAnyWidget("TabsSpacer");
  m_ItemsGridSpacer = m_Root.FindAnyWidget("ItemsGridSpacer");
  m_MaterialsGridSpacer = m_Root.FindAnyWidget("MaterialsGridSpacer");
  m_ToolsGridSpacer = m_Root.FindAnyWidget("ToolsGridSpacer");
  m_NoSelectionText = m_Root.FindAnyWidget("NoSelectionText");
  m_DetailContainer = m_Root.FindAnyWidget("DetailContainer");
  m_DetailPreview = ItemPreviewWidget.Cast(m_Root.FindAnyWidget("DetailPreview"));
  m_DetailName = TextWidget.Cast(m_Root.FindAnyWidget("DetailName"));
  m_BuildButton = ButtonWidget.Cast(m_Root.FindAnyWidget("BuildButton"));
  m_CloseButton = ButtonWidget.Cast(m_Root.FindAnyWidget("CloseButton"));
  m_StatusText = TextWidget.Cast(m_Root.FindAnyWidget("StatusText"));

  if (m_CloseButton) { m_CloseButton.SetName("SPKZ_CloseButton"); }
  if (m_BuildButton) { m_BuildButton.SetName("SPKZ_BuildButton"); m_BuildButton.Enable(false); }
  if (m_StatusText) { m_StatusText.SetText("Loading..."); }

  s_ActiveMenu = this;
  SPKZ_SendOpenRequest();
  return m_Root;
 }

 // Locks player movement/look input to the menu while it's open - the same
 // technique used by a real, tested workbench UI in an installed third-party
 // mod (structure/technique inspection only, per docs/BRIEF.md - see
 // AftermathBaseBuilding.pbo's WorkbenchMenu.OnShow/OnHide). Without this the
 // player keeps walking/looking around underneath the full-screen menu.
 override void OnShow()
 {
  super.OnShow();
  if (GetGame().GetPlayer())
  {
   GetGame().GetInput().ChangeGameFocus(1);
  }
 }

 override void OnHide()
 {
  super.OnHide();
  if (s_ActiveMenu == this) { s_ActiveMenu = null; }
  SPKZ_ClearPreviewItems(m_GridPreviewItems);
  SPKZ_ClearPreviewItems(m_DetailPreviewItems);
  if (GetGame() && GetGame().GetInput())
  {
   GetGame().GetInput().ResetGameFocus();
  }
 }

 protected void SPKZ_SendOpenRequest()
 {
  if (!m_Workbench) return;
  ScriptRPC rpc = new ScriptRPC();
  rpc.Send(m_Workbench, SPKZ_WorkbenchRPCId.OPEN_REQUEST, true, null);
 }

 // Polls SPKZ_WorkbenchClientBridge (3_Game) for responses SPKZ_Workbench
 // (4_World) delivered - this Mission-tier class cannot be called into
 // directly from World, so it checks each frame instead, the same pattern
 // scripts/5_Mission/SPKZ_PlacementLegend.c already uses for Hologram state.
 override void Update(float timeslice)
 {
  super.Update(timeslice);

  if (SPKZ_WorkbenchClientBridge.s_HasNewOpenResponse && SPKZ_WorkbenchClientBridge.s_ResponseWorkbench == m_Workbench)
  {
   SPKZ_WorkbenchClientBridge.s_HasNewOpenResponse = false;
   SPKZ_ApplyOpenResponse(SPKZ_WorkbenchClientBridge.s_LatestOpenResponse);
  }

  if (SPKZ_WorkbenchClientBridge.s_HasNewBuildResponse)
  {
   SPKZ_WorkbenchClientBridge.s_HasNewBuildResponse = false;
   SPKZ_ApplyBuildResponse(SPKZ_WorkbenchClientBridge.s_LatestBuildResponse);
  }
 }

 protected void SPKZ_ApplyOpenResponse(SPKZ_WorkbenchOpenResponse response)
 {
  m_Catalog = response.Catalog;
  m_Stock = response.Stock;

  m_Categories.Clear();
  for (int index = 0; index < m_Catalog.Recipes.Count(); index++)
  {
   string category = m_Catalog.Recipes.Get(index).Category;
   if (m_Categories.Find(category) == -1)
   {
    m_Categories.Insert(category);
   }
  }

  if (m_SelectedCategory == "" && m_Categories.Count() > 0)
  {
   m_SelectedCategory = m_Categories.Get(0);
  }

  if (m_StatusText) { m_StatusText.SetText(response.Message); }
  SPKZ_RefreshTabs();
  SPKZ_RefreshRecipeGrid();
  SPKZ_RefreshDetailPanel();
 }

 protected void SPKZ_ApplyBuildResponse(SPKZ_WorkbenchBuildResponse response)
 {
  m_Stock = response.Stock;
  if (m_StatusText) { m_StatusText.SetText(response.Message); }
  SPKZ_RefreshRecipeGrid();
  SPKZ_RefreshDetailPanel();
 }

 protected void SPKZ_ClearChildren(Widget parent)
 {
  if (!parent) return;
  Widget child = parent.GetChildren();
  while (child)
  {
   Widget next = child.GetSibling();
   child.Unlink();
   child = next;
  }
 }

 protected void SPKZ_ClearPreviewItems(array<EntityAI> items)
 {
  for (int index = 0; index < items.Count(); index++)
  {
   EntityAI item = items.Get(index);
   if (item) { GetGame().ObjectDelete(item); }
  }
  items.Clear();
 }

 protected EntityAI SPKZ_CreatePreviewItem(string className, array<EntityAI> tracker)
 {
  if (className == "") return null;
  EntityAI item = EntityAI.Cast(GetGame().CreateObject(className, Vector(0, 0, 0), true, false, false));
  if (item) { tracker.Insert(item); }
  return item;
 }

 protected void SPKZ_ApplyPreview(ItemPreviewWidget preview, string className, array<EntityAI> tracker)
 {
  if (!preview) return;
  EntityAI item = SPKZ_CreatePreviewItem(className, tracker);
  if (!item) return;
  preview.SetItem(item);
 }

 protected void SPKZ_StyleTab(Widget tabWidget, bool selected)
 {
  Widget background = tabWidget.FindAnyWidget("TabBackground");
  TextWidget label = TextWidget.Cast(tabWidget.FindAnyWidget("TabLabel"));
  if (selected)
  {
   if (background) { background.SetColor(ARGB(255, 242, 158, 38)); }
   if (label) { label.SetColor(ARGB(255, 20, 20, 20)); }
  }
  else
  {
   if (background) { background.SetColor(ARGB(255, 36, 36, 40)); }
   if (label) { label.SetColor(ARGB(255, 200, 200, 205)); }
  }
 }

 protected void SPKZ_RefreshTabs()
 {
  if (!m_TabsSpacer) return;
  SPKZ_ClearChildren(m_TabsSpacer);

  for (int index = 0; index < m_Categories.Count(); index++)
  {
   string category = m_Categories.Get(index);
   Widget tabWidget = GetGame().GetWorkspace().CreateWidgets("SparkZBaseBuilding/gui/components/sparkz_workbench_tab.layout", m_TabsSpacer);
   tabWidget.SetName("SPKZ_Tab_" + category);

   TextWidget label = TextWidget.Cast(tabWidget.FindAnyWidget("TabLabel"));
   if (label) { label.SetText(category); }

   SPKZ_StyleTab(tabWidget, category == m_SelectedCategory);
  }
 }

 // A recipe is "affordable" only if every material AND every tool it needs
 // is currently satisfied - used to color the grid card and its name text so
 // players can tell what they can build at a glance, without opening it.
 protected bool SPKZ_IsRecipeAffordable(SPKZ_WorkbenchRecipe recipe)
 {
  for (int materialIndex = 0; materialIndex < recipe.Materials.Count(); materialIndex++)
  {
   SPKZ_WorkbenchMaterialCost cost = recipe.Materials.Get(materialIndex);
   if (SPKZ_WorkbenchStockEntry.FindQuantity(m_Stock, cost.ClassName) < cost.Quantity) return false;
  }
  for (int toolIndex = 0; toolIndex < recipe.Tools.Count(); toolIndex++)
  {
   SPKZ_WorkbenchToolRequirement toolReq = recipe.Tools.Get(toolIndex);
   if (SPKZ_WorkbenchStockEntry.FindQuantity(m_Stock, toolReq.ClassName) < 1) return false;
  }
  return true;
 }

 protected void SPKZ_StyleRecipeItem(Widget itemWidget, bool selected)
 {
  Widget background = itemWidget.FindAnyWidget("RecipeItemBackground");
  Widget accent = itemWidget.FindAnyWidget("RecipeItemAccent");
  if (background) { background.SetColor(ARGB(235, 30, 30, 34)); }
  if (accent) { accent.Show(selected); }
 }

 protected void SPKZ_RefreshRecipeGrid()
 {
  if (!m_ItemsGridSpacer) return;
  SPKZ_ClearChildren(m_ItemsGridSpacer);
  SPKZ_ClearPreviewItems(m_GridPreviewItems);
  m_SelectedRecipeWidget = null;

  for (int index = 0; index < m_Catalog.Recipes.Count(); index++)
  {
   SPKZ_WorkbenchRecipe recipe = m_Catalog.Recipes.Get(index);
   if (recipe.Category != m_SelectedCategory) continue;

   bool selected = m_SelectedRecipe && m_SelectedRecipe.RecipeId == recipe.RecipeId;
   bool affordable = SPKZ_IsRecipeAffordable(recipe);

   Widget itemWidget = GetGame().GetWorkspace().CreateWidgets("SparkZBaseBuilding/gui/components/sparkz_workbench_recipe_item.layout", m_ItemsGridSpacer);
   itemWidget.SetName("SPKZ_RecipeItem");
   itemWidget.SetUserData(recipe);
   SPKZ_StyleRecipeItem(itemWidget, selected);
   if (selected) { m_SelectedRecipeWidget = itemWidget; }

   TextWidget nameWidget = TextWidget.Cast(itemWidget.FindAnyWidget("RecipeItemName"));
   if (nameWidget) { nameWidget.SetText(recipe.DisplayName); }

   TextWidget affordWidget = TextWidget.Cast(itemWidget.FindAnyWidget("RecipeItemAfford"));
   if (affordWidget)
   {
    affordWidget.SetColor(SPKZ_RequirementColor(affordable));
    if (affordable) { affordWidget.SetText("READY TO BUILD"); }
    else { affordWidget.SetText("MISSING REQUIREMENTS"); }
   }

   // Preview the finished piece (e.g. the wall itself), not the generic
   // cardboard kit box every recipe's output kit shares.
   ItemPreviewWidget preview = ItemPreviewWidget.Cast(itemWidget.FindAnyWidget("RecipeItemPreview"));
   SPKZ_ApplyPreview(preview, recipe.PreviewClassName, m_GridPreviewItems);
  }
 }

 protected void SPKZ_RefreshDetailPanel()
 {
  SPKZ_ClearPreviewItems(m_DetailPreviewItems);

  if (!m_SelectedRecipe)
  {
   if (m_NoSelectionText) { m_NoSelectionText.Show(true); }
   if (m_DetailContainer) { m_DetailContainer.Show(false); }
   if (m_BuildButton) { m_BuildButton.Enable(false); }
   return;
  }

  if (m_NoSelectionText) { m_NoSelectionText.Show(false); }
  if (m_DetailContainer) { m_DetailContainer.Show(true); }
  if (m_DetailName) { m_DetailName.SetText(m_SelectedRecipe.DisplayName); }

  // Preview the finished piece, not the generic cardboard kit box.
  SPKZ_ApplyPreview(m_DetailPreview, m_SelectedRecipe.PreviewClassName, m_DetailPreviewItems);

  bool affordable = true;

  SPKZ_ClearChildren(m_MaterialsGridSpacer);
  for (int materialIndex = 0; materialIndex < m_SelectedRecipe.Materials.Count(); materialIndex++)
  {
   SPKZ_WorkbenchMaterialCost cost = m_SelectedRecipe.Materials.Get(materialIndex);
   int have = SPKZ_WorkbenchStockEntry.FindQuantity(m_Stock, cost.ClassName);
   bool enough = have >= cost.Quantity;
   if (!enough) { affordable = false; }

   Widget materialRow = GetGame().GetWorkspace().CreateWidgets("SparkZBaseBuilding/gui/components/sparkz_workbench_required_item.layout", m_MaterialsGridSpacer);
   TextWidget materialText = TextWidget.Cast(materialRow.FindAnyWidget("RequiredItemText"));
   if (materialText)
   {
    materialText.SetText(cost.ClassName + " (" + have.ToString() + "/" + cost.Quantity.ToString() + ")");
    materialText.SetColor(SPKZ_RequirementColor(enough));
   }
   Widget materialDot = materialRow.FindAnyWidget("RequiredItemStatusDot");
   if (materialDot) { materialDot.SetColor(SPKZ_RequirementColor(enough)); }
   ItemPreviewWidget materialPreview = ItemPreviewWidget.Cast(materialRow.FindAnyWidget("RequiredItemPreview"));
   SPKZ_ApplyPreview(materialPreview, cost.ClassName, m_DetailPreviewItems);
  }

  SPKZ_ClearChildren(m_ToolsGridSpacer);
  for (int toolIndex = 0; toolIndex < m_SelectedRecipe.Tools.Count(); toolIndex++)
  {
   SPKZ_WorkbenchToolRequirement toolReq = m_SelectedRecipe.Tools.Get(toolIndex);
   bool present = SPKZ_WorkbenchStockEntry.FindQuantity(m_Stock, toolReq.ClassName) >= 1;
   if (!present) { affordable = false; }

   string status = "MISSING";
   if (present) { status = "AVAILABLE"; }

   Widget toolRow = GetGame().GetWorkspace().CreateWidgets("SparkZBaseBuilding/gui/components/sparkz_workbench_required_item.layout", m_ToolsGridSpacer);
   TextWidget toolText = TextWidget.Cast(toolRow.FindAnyWidget("RequiredItemText"));
   if (toolText)
   {
    toolText.SetText(toolReq.ClassName + " (" + status + ")");
    toolText.SetColor(SPKZ_RequirementColor(present));
   }
   Widget toolDot = toolRow.FindAnyWidget("RequiredItemStatusDot");
   if (toolDot) { toolDot.SetColor(SPKZ_RequirementColor(present)); }
   ItemPreviewWidget toolPreview = ItemPreviewWidget.Cast(toolRow.FindAnyWidget("RequiredItemPreview"));
   SPKZ_ApplyPreview(toolPreview, toolReq.ClassName, m_DetailPreviewItems);
  }

  if (m_BuildButton) { m_BuildButton.Enable(affordable); }
  Widget buildButtonBackground = null;
  if (m_BuildButton) { buildButtonBackground = m_BuildButton.FindAnyWidget("BuildButtonBackground"); }
  if (buildButtonBackground)
  {
   if (affordable) { buildButtonBackground.SetColor(ARGB(255, 242, 158, 38)); }
   else { buildButtonBackground.SetColor(ARGB(255, 40, 40, 44)); }
  }
  TextWidget buildButtonLabel = null;
  if (m_BuildButton) { buildButtonLabel = TextWidget.Cast(m_BuildButton.FindAnyWidget("BuildButtonLabel")); }
  if (buildButtonLabel)
  {
   if (affordable) { buildButtonLabel.SetColor(ARGB(255, 20, 20, 20)); }
   else { buildButtonLabel.SetColor(ARGB(255, 130, 130, 135)); }
  }
 }

 protected int SPKZ_RequirementColor(bool met)
 {
  if (met) return ARGB(255, 140, 220, 140);
  return ARGB(255, 220, 90, 90);
 }

 override bool OnKeyPress(Widget w, int x, int y, int key)
 {
  if (key == KeyCode.KC_ESCAPE)
  {
   Close();
   return true;
  }
  return super.OnKeyPress(w, x, y, key);
 }

 override bool OnClick(Widget w, int x, int y, int button)
 {
  if (w.GetName() == "SPKZ_CloseButton")
  {
   Close();
   return true;
  }

  if (w.GetName() == "SPKZ_BuildButton")
  {
   SPKZ_SendBuildRequest();
   return true;
  }

  if (w.GetName() == "SPKZ_RecipeItem")
  {
   SPKZ_WorkbenchRecipe recipe;
   w.GetUserData(recipe);
   if (recipe)
   {
    if (m_SelectedRecipeWidget && m_SelectedRecipeWidget != w) { SPKZ_StyleRecipeItem(m_SelectedRecipeWidget, false); }
    m_SelectedRecipe = recipe;
    m_SelectedRecipeWidget = w;
    SPKZ_StyleRecipeItem(w, true);
    SPKZ_RefreshDetailPanel();
   }
   return true;
  }

  if (w.GetName().IndexOf("SPKZ_Tab_") == 0)
  {
   string category = w.GetName().Substring(9, w.GetName().Length() - 9);
   if (category != m_SelectedCategory)
   {
    m_SelectedCategory = category;
    m_SelectedRecipe = null;
    SPKZ_RefreshTabs();
    SPKZ_RefreshRecipeGrid();
    SPKZ_RefreshDetailPanel();
   }
   return true;
  }

  return super.OnClick(w, x, y, button);
 }

 override bool OnMouseEnter(Widget w, int x, int y)
 {
  if (w.GetName() == "SPKZ_RecipeItem")
  {
   Widget background = w.FindAnyWidget("RecipeItemBackground");
   if (background) { background.SetColor(ARGB(235, 48, 48, 54)); }
  }
  return super.OnMouseEnter(w, x, y);
 }

 override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
 {
  if (w.GetName() == "SPKZ_RecipeItem")
  {
   Widget background = w.FindAnyWidget("RecipeItemBackground");
   if (background) { background.SetColor(ARGB(235, 30, 30, 34)); }
  }
  return super.OnMouseLeave(w, enterW, x, y);
 }

 protected void SPKZ_SendBuildRequest()
 {
  if (!m_Workbench || !m_SelectedRecipe) return;

  SPKZ_WorkbenchBuildRequest request = new SPKZ_WorkbenchBuildRequest();
  request.RecipeId = m_SelectedRecipe.RecipeId;

  ScriptRPC rpc = new ScriptRPC();
  request.WriteToContext(rpc);
  rpc.Send(m_Workbench, SPKZ_WorkbenchRPCId.BUILD_REQUEST, true, null);

  if (m_StatusText) { m_StatusText.SetText("Building..."); }
  if (m_BuildButton) { m_BuildButton.Enable(false); }
 }
}
