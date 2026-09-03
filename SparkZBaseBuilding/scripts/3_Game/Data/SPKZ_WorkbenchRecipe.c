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

// A tool that must be present in the workbench (not consumed) to build a
// given recipe. Real DayZ health values are absolute points, not percent
// (see SPKZ_Workbench.SPKZ_DamageToolOfType), matching the flat -5 already
// used for the screwdriver in SPKZ_ActionDismantleWorkbench - so this is a
// flat point loss per craft, not a percentage.
class SPKZ_WorkbenchToolRequirement
{
 string ClassName;
 int HealthLossPoints;

 void SPKZ_WorkbenchToolRequirement(string className = "", int healthLossPoints = 0)
 {
  ClassName = className;
  HealthLossPoints = healthLossPoints;
 }

 void WriteToContext(ParamsWriteContext ctx)
 {
  ctx.Write(ClassName);
  ctx.Write(HealthLossPoints);
 }

 bool ReadFromContext(ParamsReadContext ctx)
 {
  if (!ctx.Read(ClassName)) return false;
  if (!ctx.Read(HealthLossPoints)) return false;
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
 // Tools required to be present (and non-ruined) in the workbench to build
 // this recipe. Not consumed - each one takes HealthLossPoints of damage per
 // successful build instead. Which tool(s) a given recipe needs, and how
 // much damage per craft, is a placeholder pending real design (the user's
 // own framing: "we gotta find a nice way of checking what items use what
 // tools") - tune freely in the generated JSON, no code changes needed.
 ref array<ref SPKZ_WorkbenchToolRequirement> Tools;

 void SPKZ_WorkbenchRecipe()
 {
  Materials = new array<ref SPKZ_WorkbenchMaterialCost>();
  Tools = new array<ref SPKZ_WorkbenchToolRequirement>();
 }

 void AddMaterial(string className, int quantity)
 {
  Materials.Insert(new SPKZ_WorkbenchMaterialCost(className, quantity));
 }

 void AddTool(string className, int healthLossPoints)
 {
  Tools.Insert(new SPKZ_WorkbenchToolRequirement(className, healthLossPoints));
 }

 void WriteToContext(ParamsWriteContext ctx)
 {
  ctx.Write(RecipeId);
  ctx.Write(Category);
  ctx.Write(DisplayName);
  ctx.Write(IconPath);
  ctx.Write(OutputKitClassName);

  int materialCount = Materials.Count();
  ctx.Write(materialCount);
  for (int index = 0; index < materialCount; index++)
  {
   Materials.Get(index).WriteToContext(ctx);
  }

  int toolCount = Tools.Count();
  ctx.Write(toolCount);
  for (int toolIndex = 0; toolIndex < toolCount; toolIndex++)
  {
   Tools.Get(toolIndex).WriteToContext(ctx);
  }
 }

 bool ReadFromContext(ParamsReadContext ctx)
 {
  if (!ctx.Read(RecipeId)) return false;
  if (!ctx.Read(Category)) return false;
  if (!ctx.Read(DisplayName)) return false;
  if (!ctx.Read(IconPath)) return false;
  if (!ctx.Read(OutputKitClassName)) return false;

  int materialCount;
  if (!ctx.Read(materialCount)) return false;

  Materials.Clear();
  for (int index = 0; index < materialCount; index++)
  {
   SPKZ_WorkbenchMaterialCost cost = new SPKZ_WorkbenchMaterialCost();
   if (!cost.ReadFromContext(ctx)) return false;
   Materials.Insert(cost);
  }

  int toolCount;
  if (!ctx.Read(toolCount)) return false;

  Tools.Clear();
  for (int toolIndex = 0; toolIndex < toolCount; toolIndex++)
  {
   SPKZ_WorkbenchToolRequirement tool = new SPKZ_WorkbenchToolRequirement();
   if (!tool.ReadFromContext(ctx)) return false;
   Tools.Insert(tool);
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
 // Classname of the item that goes in the workbench's designated sharpening-
 // stone slot (per the pending model update: 500-cell cargo + one slot each
 // for hacksaw/saw/hammer/shovel/screwdriver/pliers/sledgehammer + this one -
 // all of those, and this, are real vanilla DayZ items: "Whetstone" is the
 // real vanilla classname, confirmed against the installed game's own
 // gear_tools.pbo). While present, it fully offsets the durability a tool
 // loses per craft (see SPKZ_Workbench.SPKZ_DamageToolOfType) - a real,
 // exact repair amount is a design decision to make later; full offset is
 // the simplest placeholder ("keeps tools maintained for free").
 string SharpeningStoneClassName;

 void SPKZ_WorkbenchRecipeCatalog()
 {
  Recipes = new array<ref SPKZ_WorkbenchRecipe>();
  SharpeningStoneClassName = "Whetstone";
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

 // Placeholder starting costs/tools - tune freely by editing the generated
 // JSON on the server; these defaults only seed a fresh file the first time
 // it's created. All items referenced here are real vanilla DayZ classnames.
 // Per direction, every material cost is a flat 1-of-each for now (one log,
 // one plank, one nail, etc. - whatever set of materials makes sense for
 // that piece), not tuned real quantities - easy to test with, easy to
 // retune in the JSON once real costs are decided. Nail is included on
 // every recipe since nails are essential across all wall/door/window/
 // floor/garage construction. Each recipe also gets one placeholder tool
 // requirement, deliberately varied (Hammer for wood, Pliers for the glass
 // frame, Hacksaw for the garage's metal sheets) to demonstrate the system
 // supports different tools per recipe - the real mapping is a design
 // decision still to be made.
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
  wall.AddMaterial("WoodenLog", 1);
  wall.AddMaterial("WoodenPlank", 1);
  wall.AddMaterial("Nail", 1);
  wall.AddTool("Hammer", 5);
  Recipes.Insert(wall);

  SPKZ_WorkbenchRecipe doorWall = new SPKZ_WorkbenchRecipe();
  doorWall.RecipeId = "wood_door_wall";
  doorWall.Category = "Wood";
  doorWall.DisplayName = "Wood Wall Door Kit";
  doorWall.IconPath = placeholderIcon;
  doorWall.OutputKitClassName = "SPKZ_WoodWallDoorKit";
  doorWall.AddMaterial("WoodenLog", 1);
  doorWall.AddMaterial("WoodenPlank", 1);
  doorWall.AddMaterial("Nail", 1);
  doorWall.AddTool("Hammer", 5);
  Recipes.Insert(doorWall);

  SPKZ_WorkbenchRecipe windowWall = new SPKZ_WorkbenchRecipe();
  windowWall.RecipeId = "wood_window_wall";
  windowWall.Category = "Wood";
  windowWall.DisplayName = "Wood Window Kit";
  windowWall.IconPath = placeholderIcon;
  windowWall.OutputKitClassName = "SPKZ_WoodWindowKit";
  windowWall.AddMaterial("WoodenLog", 1);
  windowWall.AddMaterial("WoodenPlank", 1);
  windowWall.AddMaterial("Nail", 1);
  windowWall.AddTool("Hammer", 5);
  Recipes.Insert(windowWall);

  SPKZ_WorkbenchRecipe floor = new SPKZ_WorkbenchRecipe();
  floor.RecipeId = "wood_floor";
  floor.Category = "Wood";
  floor.DisplayName = "Wood Floor Kit";
  floor.IconPath = placeholderIcon;
  floor.OutputKitClassName = "SPKZ_WoodFloorKit";
  floor.AddMaterial("WoodenLog", 1);
  floor.AddMaterial("WoodenPlank", 1);
  floor.AddMaterial("Nail", 1);
  floor.AddTool("Hammer", 5);
  Recipes.Insert(floor);

  SPKZ_WorkbenchRecipe glassWindow = new SPKZ_WorkbenchRecipe();
  glassWindow.RecipeId = "wood_glass_window";
  glassWindow.Category = "Wood";
  glassWindow.DisplayName = "Wood Glass Window Kit";
  glassWindow.IconPath = placeholderIcon;
  glassWindow.OutputKitClassName = "SPKZ_WoodGlassWindowKit";
  glassWindow.AddMaterial("WoodenLog", 1);
  glassWindow.AddMaterial("WoodenPlank", 1);
  glassWindow.AddMaterial("MetalPlate", 1);
  glassWindow.AddMaterial("Nail", 1);
  glassWindow.AddTool("Pliers", 5);
  Recipes.Insert(glassWindow);

  SPKZ_WorkbenchRecipe garage = new SPKZ_WorkbenchRecipe();
  garage.RecipeId = "wood_garage";
  garage.Category = "Metal";
  garage.DisplayName = "Wood Garage Door Kit";
  garage.IconPath = placeholderIcon;
  garage.OutputKitClassName = "SPKZ_WoodGarageKit";
  garage.AddMaterial("WoodenLog", 1);
  garage.AddMaterial("WoodenPlank", 1);
  garage.AddMaterial("MetalPlate", 1);
  garage.AddMaterial("Nail", 1);
  garage.AddTool("Hacksaw", 8);
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
