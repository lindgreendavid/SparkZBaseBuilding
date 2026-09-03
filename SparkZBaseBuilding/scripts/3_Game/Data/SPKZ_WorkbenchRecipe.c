// Shared (client+server) recipe data model. Lives in 3_Game because both the
// server (loads/validates recipes) and the client (renders the build menu)
// need the exact same class definitions to read/write these over RPC.
class SPKZ_WorkbenchMaterialCost
{
 string ClassName;
 int Quantity;

 void SPKZ_WorkbenchMaterialCost(string className = "", int quantity = 0)
 {
  ClassName = className;
  Quantity = quantity;
 }

 void WriteToContext(ParamsWriteContext ctx)
 {
  ctx.Write(ClassName);
  ctx.Write(Quantity);
 }

 bool ReadFromContext(ParamsReadContext ctx)
 {
  if (!ctx.Read(ClassName)) return false;
  if (!ctx.Read(Quantity)) return false;
  return true;
 }
}

class SPKZ_WorkbenchRecipe
{
 string RecipeId;
 string Category;
 string DisplayName;
 // Path to a picture shown for this recipe in the build menu. All recipes
 // currently point at the same placeholder kit icon - see docs/BRIEF.md's
 // open asset-source question; wire in per-recipe art here once it exists.
 string IconPath;
 string OutputKitClassName;
 ref array<ref SPKZ_WorkbenchMaterialCost> Materials;

 void SPKZ_WorkbenchRecipe()
 {
  Materials = new array<ref SPKZ_WorkbenchMaterialCost>();
 }

 void AddMaterial(string className, int quantity)
 {
  Materials.Insert(new SPKZ_WorkbenchMaterialCost(className, quantity));
 }

 void WriteToContext(ParamsWriteContext ctx)
 {
  ctx.Write(RecipeId);
  ctx.Write(Category);
  ctx.Write(DisplayName);
  ctx.Write(IconPath);
  ctx.Write(OutputKitClassName);
  int count = Materials.Count();
  ctx.Write(count);
  for (int index = 0; index < count; index++)
  {
   Materials.Get(index).WriteToContext(ctx);
  }
 }

 bool ReadFromContext(ParamsReadContext ctx)
 {
  if (!ctx.Read(RecipeId)) return false;
  if (!ctx.Read(Category)) return false;
  if (!ctx.Read(DisplayName)) return false;
  if (!ctx.Read(IconPath)) return false;
  if (!ctx.Read(OutputKitClassName)) return false;

  int count;
  if (!ctx.Read(count)) return false;

  Materials.Clear();
  for (int index = 0; index < count; index++)
  {
   SPKZ_WorkbenchMaterialCost cost = new SPKZ_WorkbenchMaterialCost();
   if (!cost.ReadFromContext(ctx)) return false;
   Materials.Insert(cost);
  }

  return true;
 }
}

// Server-loaded, admin-tunable recipe list. Only the server ever calls Load()
// (it reads/writes a profile-relative JSON file); the client's copy of this
// class is populated purely from the OPEN_RESPONSE RPC sent by the workbench
// it's looking at, never by loading its own local file.
class SPKZ_WorkbenchRecipeCatalog
{
 static const string CONFIG_DIR = "$profile:SparkZBaseBuilding";
 static const string CONFIG_FILE = "$profile:SparkZBaseBuilding/WorkbenchRecipes.json";

 ref array<ref SPKZ_WorkbenchRecipe> Recipes;

 void SPKZ_WorkbenchRecipeCatalog()
 {
  Recipes = new array<ref SPKZ_WorkbenchRecipe>();
 }

 SPKZ_WorkbenchRecipe FindRecipe(string recipeId)
 {
  for (int index = 0; index < Recipes.Count(); index++)
  {
   SPKZ_WorkbenchRecipe recipe = Recipes.Get(index);
   if (recipe && recipe.RecipeId == recipeId)
    return recipe;
  }

  return null;
 }

 void WriteToContext(ParamsWriteContext ctx)
 {
  int count = Recipes.Count();
  ctx.Write(count);
  for (int index = 0; index < count; index++)
  {
   Recipes.Get(index).WriteToContext(ctx);
  }
 }

 bool ReadFromContext(ParamsReadContext ctx)
 {
  int count;
  if (!ctx.Read(count)) return false;

  Recipes.Clear();
  for (int index = 0; index < count; index++)
  {
   SPKZ_WorkbenchRecipe recipe = new SPKZ_WorkbenchRecipe();
   if (!recipe.ReadFromContext(ctx)) return false;
   Recipes.Insert(recipe);
  }

  return true;
 }

 // Placeholder starting costs - tune freely by editing the generated JSON on
 // the server; these defaults only seed a fresh file the first time it's
 // created. All items referenced here are real vanilla DayZ classnames.
 protected void SeedDefaults()
 {
  Recipes.Clear();
  string placeholderIcon = "SparkZBaseBuilding\\Data\\SparkZKit\\SPKZ_CardboardKit_co.paa";

  SPKZ_WorkbenchRecipe wall = new SPKZ_WorkbenchRecipe();
  wall.RecipeId = "wood_wall";
  wall.Category = "Wood";
  wall.DisplayName = "Wood Wall Kit";
  wall.IconPath = placeholderIcon;
  wall.OutputKitClassName = "SPKZ_WoodWallKit";
  wall.AddMaterial("WoodenLog", 2);
  wall.AddMaterial("WoodenPlank", 5);
  Recipes.Insert(wall);

  SPKZ_WorkbenchRecipe doorWall = new SPKZ_WorkbenchRecipe();
  doorWall.RecipeId = "wood_door_wall";
  doorWall.Category = "Wood";
  doorWall.DisplayName = "Wood Wall Door Kit";
  doorWall.IconPath = placeholderIcon;
  doorWall.OutputKitClassName = "SPKZ_WoodWallDoorKit";
  doorWall.AddMaterial("WoodenLog", 2);
  doorWall.AddMaterial("WoodenPlank", 6);
  doorWall.AddMaterial("Nail", 5);
  Recipes.Insert(doorWall);

  SPKZ_WorkbenchRecipe windowWall = new SPKZ_WorkbenchRecipe();
  windowWall.RecipeId = "wood_window_wall";
  windowWall.Category = "Wood";
  windowWall.DisplayName = "Wood Window Kit";
  windowWall.IconPath = placeholderIcon;
  windowWall.OutputKitClassName = "SPKZ_WoodWindowKit";
  windowWall.AddMaterial("WoodenLog", 2);
  windowWall.AddMaterial("WoodenPlank", 5);
  windowWall.AddMaterial("Nail", 4);
  Recipes.Insert(windowWall);

  SPKZ_WorkbenchRecipe floor = new SPKZ_WorkbenchRecipe();
  floor.RecipeId = "wood_floor";
  floor.Category = "Wood";
  floor.DisplayName = "Wood Floor Kit";
  floor.IconPath = placeholderIcon;
  floor.OutputKitClassName = "SPKZ_WoodFloorKit";
  floor.AddMaterial("WoodenLog", 1);
  floor.AddMaterial("WoodenPlank", 8);
  Recipes.Insert(floor);

  SPKZ_WorkbenchRecipe glassWindow = new SPKZ_WorkbenchRecipe();
  glassWindow.RecipeId = "wood_glass_window";
  glassWindow.Category = "Wood";
  glassWindow.DisplayName = "Wood Glass Window Kit";
  glassWindow.IconPath = placeholderIcon;
  glassWindow.OutputKitClassName = "SPKZ_WoodGlassWindowKit";
  glassWindow.AddMaterial("WoodenLog", 2);
  glassWindow.AddMaterial("WoodenPlank", 5);
  glassWindow.AddMaterial("MetalPlate", 1);
  Recipes.Insert(glassWindow);

  SPKZ_WorkbenchRecipe garage = new SPKZ_WorkbenchRecipe();
  garage.RecipeId = "wood_garage";
  garage.Category = "Metal";
  garage.DisplayName = "Wood Garage Door Kit";
  garage.IconPath = placeholderIcon;
  garage.OutputKitClassName = "SPKZ_WoodGarageKit";
  garage.AddMaterial("WoodenLog", 3);
  garage.AddMaterial("WoodenPlank", 10);
  garage.AddMaterial("MetalPlate", 4);
  garage.AddMaterial("Nail", 8);
  Recipes.Insert(garage);
 }

 static ref SPKZ_WorkbenchRecipeCatalog Load()
 {
  if (!FileExist(CONFIG_DIR))
   MakeDirectory(CONFIG_DIR);

  SPKZ_WorkbenchRecipeCatalog catalog = new SPKZ_WorkbenchRecipeCatalog();
  if (FileExist(CONFIG_FILE))
  {
   JsonFileLoader<SPKZ_WorkbenchRecipeCatalog>.JsonLoadFile(CONFIG_FILE, catalog);
  }
  else
  {
   catalog.SeedDefaults();
  }

  JsonFileLoader<SPKZ_WorkbenchRecipeCatalog>.JsonSaveFile(CONFIG_FILE, catalog);
  return catalog;
 }
}
