// Build menu: category list -> recipe list -> cost breakdown -> Build Now.
// v1 UI intentionally keeps every recipe on one shared placeholder icon
// (SPKZ_CardboardKit_co.paa) since no per-part icon art exists yet - see the
// open asset-source question in docs/BRIEF.md. The icon widget and the
// per-recipe IconPath field are already wired end to end, so real art drops
// in later with no script changes.
class SPKZ_WorkbenchMenu extends UIScriptedMenu
{
 // At most this many material lines are shown per recipe. Recipes with more
 // materials than this need a v2 layout with a scrollable cost list.
 static const int MAX_COST_ROWS = 6;

 protected static ref SPKZ_WorkbenchMenu s_ActiveMenu;

 protected SPKZ_Workbench m_Workbench;
 protected ref SPKZ_WorkbenchRecipeCatalog m_Catalog;
 protected ref array<ref SPKZ_WorkbenchStockEntry> m_Stock;
 protected ref array<string> m_Categories;
 protected string m_SelectedCategory;
 protected string m_SelectedRecipeId;

 protected Widget m_Root;
 protected TextListboxWidget m_CategoryList;
 protected TextListboxWidget m_ItemList;
 protected TextWidget m_DetailName;
 protected ImageWidget m_DetailIcon;
 protected TextWidget m_StatusText;
 protected ButtonWidget m_BuildButton;
 protected ButtonWidget m_CloseButton;
 protected TextWidget m_CostRows[6];

 void SPKZ_WorkbenchMenu(SPKZ_Workbench workbench)
 {
  m_Workbench = workbench;
  m_Catalog = new SPKZ_WorkbenchRecipeCatalog();
  m_Stock = new array<ref SPKZ_WorkbenchStockEntry>();
  m_Categories = new array<string>();
 }

 override Widget Init()
 {
  m_Root = GetGame().GetWorkspace().CreateWidgets("SparkZBaseBuilding/gui/layouts/sparkz_workbench_menu.layout");
  layoutRoot = m_Root;

  m_CategoryList = TextListboxWidget.Cast(m_Root.FindAnyWidget("CategoryList"));
  m_ItemList = TextListboxWidget.Cast(m_Root.FindAnyWidget("ItemList"));
  m_DetailName = TextWidget.Cast(m_Root.FindAnyWidget("DetailName"));
  m_DetailIcon = ImageWidget.Cast(m_Root.FindAnyWidget("DetailIcon"));
  m_StatusText = TextWidget.Cast(m_Root.FindAnyWidget("StatusText"));
  m_BuildButton = ButtonWidget.Cast(m_Root.FindAnyWidget("BuildButton"));
  m_CloseButton = ButtonWidget.Cast(m_Root.FindAnyWidget("CloseButton"));
  for (int index = 0; index < MAX_COST_ROWS; index++)
  {
   m_CostRows[index] = TextWidget.Cast(m_Root.FindAnyWidget("CostRow" + index.ToString()));
   if (m_CostRows[index]) { m_CostRows[index].Show(false); }
  }

  if (m_StatusText) { m_StatusText.SetText("Loading..."); }
  if (m_BuildButton) { m_BuildButton.Enable(false); }

  s_ActiveMenu = this;
  SPKZ_SendOpenRequest();
  return m_Root;
 }

 override void OnHide()
 {
  super.OnHide();
  if (s_ActiveMenu == this) { s_ActiveMenu = null; }
 }

 protected void SPKZ_SendOpenRequest()
 {
  if (!m_Workbench) return;
  ScriptRPC rpc = new ScriptRPC();
  rpc.Send(m_Workbench, SPKZ_WorkbenchRPCId.OPEN_REQUEST, true, null);
 }

 // Called from SPKZ_WorkbenchNetworking.c when this workbench's
 // OPEN_RESPONSE arrives.
 static void SPKZ_OnOpenResponse(SPKZ_Workbench workbench, SPKZ_WorkbenchOpenResponse response)
 {
  if (!s_ActiveMenu || s_ActiveMenu.m_Workbench != workbench) return;
  s_ActiveMenu.SPKZ_ApplyOpenResponse(response);
 }

 static void SPKZ_OnBuildResponse(SPKZ_Workbench workbench, SPKZ_WorkbenchBuildResponse response)
 {
  if (!s_ActiveMenu || s_ActiveMenu.m_Workbench != workbench) return;
  s_ActiveMenu.SPKZ_ApplyBuildResponse(response);
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

  if (m_StatusText) { m_StatusText.SetText(response.Message); }
  SPKZ_RefreshCategoryList();
 }

 protected void SPKZ_ApplyBuildResponse(SPKZ_WorkbenchBuildResponse response)
 {
  m_Stock = response.Stock;
  if (m_StatusText) { m_StatusText.SetText(response.Message); }
  SPKZ_RefreshItemList();
  SPKZ_RefreshDetailPanel();
 }

 protected void SPKZ_RefreshCategoryList()
 {
  if (!m_CategoryList) return;
  m_CategoryList.ClearItems();
  for (int index = 0; index < m_Categories.Count(); index++)
  {
   m_CategoryList.AddItem(m_Categories.Get(index), null, 0);
  }

  if (m_Categories.Count() > 0)
  {
   m_SelectedCategory = m_Categories.Get(0);
   m_CategoryList.SelectRow(0);
  }
  SPKZ_RefreshItemList();
 }

 protected void SPKZ_RefreshItemList()
 {
  if (!m_ItemList) return;
  m_ItemList.ClearItems();
  if (m_SelectedCategory == "") return;

  for (int index = 0; index < m_Catalog.Recipes.Count(); index++)
  {
   SPKZ_WorkbenchRecipe recipe = m_Catalog.Recipes.Get(index);
   if (recipe.Category != m_SelectedCategory) continue;

   int row = m_ItemList.AddItem(recipe.DisplayName, null, 0);
   if (!SPKZ_CanAffordRecipe(recipe))
   {
    m_ItemList.SetItemColor(row, 0, ARGB(255, 200, 90, 90));
   }
  }
 }

 protected SPKZ_WorkbenchRecipe SPKZ_FindRecipeByDisplayName(string displayName)
 {
  for (int index = 0; index < m_Catalog.Recipes.Count(); index++)
  {
   SPKZ_WorkbenchRecipe recipe = m_Catalog.Recipes.Get(index);
   if (recipe.Category == m_SelectedCategory && recipe.DisplayName == displayName)
    return recipe;
  }
  return null;
 }

 protected bool SPKZ_CanAffordRecipe(SPKZ_WorkbenchRecipe recipe)
 {
  for (int index = 0; index < recipe.Materials.Count(); index++)
  {
   SPKZ_WorkbenchMaterialCost cost = recipe.Materials.Get(index);
   if (SPKZ_WorkbenchStockEntry.FindQuantity(m_Stock, cost.ClassName) < cost.Quantity)
    return false;
  }
  for (int toolIndex = 0; toolIndex < recipe.Tools.Count(); toolIndex++)
  {
   if (SPKZ_WorkbenchStockEntry.FindQuantity(m_Stock, recipe.Tools.Get(toolIndex).ClassName) < 1)
    return false;
  }
  return true;
 }

 protected void SPKZ_RefreshDetailPanel()
 {
  SPKZ_WorkbenchRecipe recipe = m_Catalog.FindRecipe(m_SelectedRecipeId);
  if (!recipe)
  {
   if (m_DetailName) { m_DetailName.SetText(""); }
   for (int emptyRowIndex = 0; emptyRowIndex < MAX_COST_ROWS; emptyRowIndex++)
   {
    if (m_CostRows[emptyRowIndex]) { m_CostRows[emptyRowIndex].Show(false); }
   }
   if (m_BuildButton) { m_BuildButton.Enable(false); }
   return;
  }

  if (m_DetailName) { m_DetailName.SetText(recipe.DisplayName); }
  if (m_DetailIcon) { m_DetailIcon.LoadImageFile(0, recipe.IconPath); }

  // Materials and tools share the same row list - materials show "have/need"
  // and are consumed on build; tools show "AVAILABLE"/"MISSING" (or
  // "RUINED" - a present-but-ruined tool reports as absent, see
  // SPKZ_Workbench.SPKZ_CountItemsOfType) and are only damaged, not consumed.
  bool affordable = true;
  int row = 0;
  for (int materialIndex = 0; materialIndex < recipe.Materials.Count() && row < MAX_COST_ROWS; materialIndex++)
  {
   SPKZ_WorkbenchMaterialCost cost = recipe.Materials.Get(materialIndex);
   int have = SPKZ_WorkbenchStockEntry.FindQuantity(m_Stock, cost.ClassName);
   bool enough = have >= cost.Quantity;
   if (!enough) { affordable = false; }

   m_CostRows[row].Show(true);
   m_CostRows[row].SetText(cost.ClassName + "  " + have.ToString() + " / " + cost.Quantity.ToString());
   m_CostRows[row].SetColor(SPKZ_RequirementColor(enough));
   row++;
  }

  for (int toolIndex = 0; toolIndex < recipe.Tools.Count() && row < MAX_COST_ROWS; toolIndex++)
  {
   SPKZ_WorkbenchToolRequirement toolReq = recipe.Tools.Get(toolIndex);
   bool present = SPKZ_WorkbenchStockEntry.FindQuantity(m_Stock, toolReq.ClassName) >= 1;
   if (!present) { affordable = false; }

   string status = "MISSING";
   if (present) { status = "AVAILABLE"; }

   m_CostRows[row].Show(true);
   m_CostRows[row].SetText(toolReq.ClassName + " (tool)  " + status);
   m_CostRows[row].SetColor(SPKZ_RequirementColor(present));
   row++;
  }

  for (int clearIndex = row; clearIndex < MAX_COST_ROWS; clearIndex++)
  {
   if (m_CostRows[clearIndex]) { m_CostRows[clearIndex].Show(false); }
  }

  if (m_BuildButton) { m_BuildButton.Enable(affordable); }
 }

 protected int SPKZ_RequirementColor(bool met)
 {
  if (met) return ARGB(255, 140, 220, 140);
  return ARGB(255, 220, 90, 90);
 }

 override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn)
 {
  if (w == m_CategoryList)
  {
   if (row >= 0 && row < m_Categories.Count())
   {
    m_SelectedCategory = m_Categories.Get(row);
    m_SelectedRecipeId = "";
    SPKZ_RefreshItemList();
    SPKZ_RefreshDetailPanel();
   }
   return true;
  }

  if (w == m_ItemList)
  {
   string displayName;
   if (m_ItemList.GetItemText(row, 0, displayName))
   {
    SPKZ_WorkbenchRecipe recipe = SPKZ_FindRecipeByDisplayName(displayName);
    if (recipe)
    {
     m_SelectedRecipeId = recipe.RecipeId;
     SPKZ_RefreshDetailPanel();
    }
   }
   return true;
  }

  return super.OnItemSelected(w, x, y, row, column, oldRow, oldColumn);
 }

 override bool OnClick(Widget w, int x, int y, int button)
 {
  if (w == m_BuildButton)
  {
   SPKZ_SendBuildRequest();
   return true;
  }

  if (w == m_CloseButton)
  {
   Close();
   return true;
  }

  return super.OnClick(w, x, y, button);
 }

 protected void SPKZ_SendBuildRequest()
 {
  if (!m_Workbench || m_SelectedRecipeId == "") return;

  SPKZ_WorkbenchBuildRequest request = new SPKZ_WorkbenchBuildRequest();
  request.RecipeId = m_SelectedRecipeId;

  ScriptRPC rpc = new ScriptRPC();
  request.WriteToContext(rpc);
  rpc.Send(m_Workbench, SPKZ_WorkbenchRPCId.BUILD_REQUEST, true, null);

  if (m_StatusText) { m_StatusText.SetText("Building..."); }
  if (m_BuildButton) { m_BuildButton.Enable(false); }
 }
}
