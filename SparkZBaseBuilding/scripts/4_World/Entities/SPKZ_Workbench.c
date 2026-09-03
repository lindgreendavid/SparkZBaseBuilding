// Crafting workbench: 500-slot cargo (20x25 grid) holding raw materials.
// Anyone can access the build menu (matching this addon's existing rule that
// ordinary interactions - open/close doors and windows - are not owner-
// gated, only dismantle is; see SPKZ_ActionAccessWorkbench). Full squad-role
// gating (Base Access holders only) is future work once this addon has a
// plot-pole/squad-permission system to check against - see docs/WORKFLOW.md's
// list of not-yet-implemented features and docs/BRIEF.md.
class SPKZ_Workbench extends ItemBase
{
 protected string m_SPKZOwnerId;
 protected int m_SPKZOwnerHash;
 // Only set server-side and never networked to every client - each client
 // only receives the specific OPEN_RESPONSE/BUILD_RESPONSE addressed to it.
 protected ref SPKZ_WorkbenchRecipeCatalog m_SPKZCatalog;

 void SPKZ_Workbench()
 {
  RegisterNetSyncVariableInt("m_SPKZOwnerHash");
  SetAllowDamage(false);
  SetCanBeDestroyed(false);
 }

 override void EEInit()
 {
  super.EEInit();
  if (GetGame().IsServer())
  {
   m_SPKZCatalog = SPKZ_WorkbenchRecipeCatalog.Load();
  }
 }

 void SPKZ_RefreshLifetime()
 {
  if (!GetGame().IsServer()) return;
  // Native CE owns persistence, matching SPKZ_WoodWallDoor's convention -
  // refresh cleanup lifetime, never recreate a deleted workbench.
  SetLifetimeMax(315360000);
  SetLifetime(315360000);
 }

 override void OnCEUpdate()
 {
  super.OnCEUpdate();
  SPKZ_RefreshLifetime();
 }

 void SPKZ_SetOwner(PlayerBase player)
 {
  if (!GetGame().IsServer() || !player || !player.GetIdentity()) return;
  m_SPKZOwnerId = player.GetIdentity().GetId();
  m_SPKZOwnerHash = m_SPKZOwnerId.Hash();
  SPKZ_RefreshLifetime();
  SetSynchDirty();
 }

 bool SPKZ_CanDismantle(PlayerBase player, ItemBase tool)
 {
  if (!player || !player.GetIdentity() || !tool) return false;
  if (!tool.IsKindOf("Screwdriver") || tool.IsRuined()) return false;
  string id = player.GetIdentity().GetId();
  if (GetGame().IsServer())
  {
   return m_SPKZOwnerId != "" && m_SPKZOwnerId == id;
  }
  return m_SPKZOwnerHash != 0 && m_SPKZOwnerHash == id.Hash();
 }

 void SPKZ_Dismantle(PlayerBase player, ItemBase tool)
 {
  if (!GetGame().IsServer() || !SPKZ_CanDismantle(player, tool)) return;
  // Refuse to dismantle a workbench that still has materials in it - avoids
  // silently deleting whatever was stored inside.
  if (GetInventory().GetCargo() && GetInventory().GetCargo().GetItemCount() > 0)
  {
   player.MessageStatus("[SparkZBaseBuilding] Empty the workbench before dismantling it.");
   return;
  }

  EntityAI kit = EntityAI.Cast(GetGame().CreateObjectEx("SPKZ_WorkbenchKit", player.GetPosition(), ECE_PLACE_ON_SURFACE));
  if (!kit) return;
  tool.AddHealth("", "Health", -5);
  GetGame().ObjectDelete(this);
 }

 override void OnStoreSave(ParamsWriteContext ctx)
 {
  super.OnStoreSave(ctx);
  ctx.Write(m_SPKZOwnerId);
 }

 override bool OnStoreLoad(ParamsReadContext ctx, int version)
 {
  if (!super.OnStoreLoad(ctx, version))
  {
   return false;
  }

  // Workbenches saved before owner tracking existed have no stored owner.
  // Do not assign them to a stranger.
  if (!ctx.Read(m_SPKZOwnerId)) { m_SPKZOwnerId = ""; }
  if (m_SPKZOwnerId != "") { m_SPKZOwnerHash = m_SPKZOwnerId.Hash(); }
  SPKZ_RefreshLifetime();
  return true;
 }

 override void SetActions()
 {
  super.SetActions();
  AddAction(SPKZ_ActionAccessWorkbench);
 }

 // Every distinct material AND tool classname referenced by any recipe
 // currently known to this workbench's catalog - the set we scan cargo for
 // and report back to the client so the menu can compute red/green
 // highlighting for both material costs and tool availability.
 protected void SPKZ_CollectMaterialClassNames(out array<string> outNames)
 {
  outNames = new array<string>();
  for (int index = 0; index < m_SPKZCatalog.Recipes.Count(); index++)
  {
   SPKZ_WorkbenchRecipe recipe = m_SPKZCatalog.Recipes.Get(index);
   for (int materialIndex = 0; materialIndex < recipe.Materials.Count(); materialIndex++)
   {
    string className = recipe.Materials.Get(materialIndex).ClassName;
    if (outNames.Find(className) == -1)
    {
     outNames.Insert(className);
    }
   }
   for (int toolIndex = 0; toolIndex < recipe.Tools.Count(); toolIndex++)
   {
    string toolClassName = recipe.Tools.Get(toolIndex).ClassName;
    if (outNames.Find(toolClassName) == -1)
    {
     outNames.Insert(toolClassName);
    }
   }
  }
 }

 // Ruined items (materials or tools) never count as usable stock.
 protected int SPKZ_CountItemsOfType(string className)
 {
  int total = 0;
  array<EntityAI> items = new array<EntityAI>();
  GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
  for (int index = 0; index < items.Count(); index++)
  {
   ItemBase item = ItemBase.Cast(items.Get(index));
   if (!item || !item.IsKindOf(className) || item.IsRuined()) continue;

   if (item.HasQuantity())
   {
    total += item.GetQuantity();
   }
   else
   {
    total += 1;
   }
  }

  return total;
 }

 // A tool "counts" for a recipe if at least one non-ruined instance is
 // anywhere in the workbench's inventory. This is a stand-in for the real
 // model's designated tool-attachment slots (see docs/BRIEF.md and the
 // pending workbench model) - once that model exists with named slots for
 // hacksaw/saw/hammer/shovel/screwdriver/pliers/sledgehammer, this should
 // check that specific slot instead of scanning all cargo, and the check
 // shown on-model becomes real rather than just a script-side rule.
 protected bool SPKZ_HasUsableTool(string className)
 {
  return SPKZ_FindUsableTool(className) != null;
 }

 protected ItemBase SPKZ_FindUsableTool(string className)
 {
  array<EntityAI> items = new array<EntityAI>();
  GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
  for (int index = 0; index < items.Count(); index++)
  {
   ItemBase item = ItemBase.Cast(items.Get(index));
   if (item && item.IsKindOf(className) && !item.IsRuined())
    return item;
  }

  return null;
 }

 // Applies a flat health-point loss to one matching tool. Does not consume
 // or remove it - a ruined tool remains in the workbench until a player
 // takes it out and repairs or replaces it. If a sharpening stone is
 // present (see SPKZ_WorkbenchRecipeCatalog.SharpeningStoneClassName), the
 // loss is immediately offset - net effect: tools stay maintained for free
 // as long as a stone is loaded. Placeholder behaviour pending the model
 // update that actually adds the stone's designated slot.
 protected void SPKZ_DamageToolOfType(string className, int healthLossPoints)
 {
  ItemBase tool = SPKZ_FindUsableTool(className);
  if (!tool || healthLossPoints <= 0) return;

  tool.AddHealth("", "Health", -healthLossPoints);
  if (SPKZ_HasUsableSharpeningStone())
  {
   tool.AddHealth("", "Health", healthLossPoints);
  }
 }

 protected bool SPKZ_HasUsableSharpeningStone()
 {
  if (!m_SPKZCatalog || m_SPKZCatalog.SharpeningStoneClassName == "") return false;
  return SPKZ_HasUsableTool(m_SPKZCatalog.SharpeningStoneClassName);
 }

 // Removes up to `amount` units of `className` from this workbench's cargo.
 // Returns false (and removes nothing) if the workbench does not currently
 // hold at least `amount` - callers must check this before consuming any
 // material for a recipe, so a build never partially consumes materials.
 protected bool SPKZ_ConsumeItemsOfType(string className, int amount)
 {
  if (SPKZ_CountItemsOfType(className) < amount) return false;

  array<EntityAI> items = new array<EntityAI>();
  GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
  int remaining = amount;
  for (int index = 0; index < items.Count() && remaining > 0; index++)
  {
   ItemBase item = ItemBase.Cast(items.Get(index));
   if (!item || !item.IsKindOf(className)) continue;

   if (item.HasQuantity())
   {
    int quantity = item.GetQuantity();
    int take = Math.Min(quantity, remaining);
    item.AddQuantity(-take);
    remaining -= take;
   }
   else
   {
    GetGame().ObjectDelete(item);
    remaining -= 1;
   }
  }

  return true;
 }

 protected array<ref SPKZ_WorkbenchStockEntry> SPKZ_BuildStockSnapshot()
 {
  array<string> names;
  SPKZ_CollectMaterialClassNames(names);

  array<ref SPKZ_WorkbenchStockEntry> stock = new array<ref SPKZ_WorkbenchStockEntry>();
  for (int index = 0; index < names.Count(); index++)
  {
   string className = names.Get(index);
   stock.Insert(new SPKZ_WorkbenchStockEntry(className, SPKZ_CountItemsOfType(className)));
  }

  return stock;
 }

 protected void SPKZ_HandleOpenRequest(PlayerIdentity sender)
 {
  if (!GetGame().IsServer()) return;

  SPKZ_WorkbenchOpenResponse response = new SPKZ_WorkbenchOpenResponse();
  response.Catalog = m_SPKZCatalog;
  response.Stock = SPKZ_BuildStockSnapshot();

  ScriptRPC rpc = new ScriptRPC();
  response.WriteToContext(rpc);
  rpc.Send(this, SPKZ_WorkbenchRPCId.OPEN_RESPONSE, true, sender);
 }

 protected void SPKZ_HandleBuildRequest(PlayerIdentity sender, ParamsReadContext ctx)
 {
  if (!GetGame().IsServer()) return;

  SPKZ_WorkbenchBuildRequest request = new SPKZ_WorkbenchBuildRequest();
  SPKZ_WorkbenchBuildResponse response = new SPKZ_WorkbenchBuildResponse();

  if (!request.ReadFromContext(ctx))
  {
   response.Message = "Invalid request.";
   SPKZ_SendBuildResponse(sender, response);
   return;
  }

  SPKZ_WorkbenchRecipe recipe = m_SPKZCatalog.FindRecipe(request.RecipeId);
  if (!recipe)
  {
   response.Message = "Unknown recipe.";
   SPKZ_SendBuildResponse(sender, response);
   return;
  }

  // Re-check every material AND every required tool server-side - the
  // client's red/green display is cosmetic only and is never trusted here.
  for (int index = 0; index < recipe.Materials.Count(); index++)
  {
   SPKZ_WorkbenchMaterialCost cost = recipe.Materials.Get(index);
   if (SPKZ_CountItemsOfType(cost.ClassName) < cost.Quantity)
   {
    response.Message = "Not enough materials.";
    response.Stock = SPKZ_BuildStockSnapshot();
    SPKZ_SendBuildResponse(sender, response);
    return;
   }
  }

  for (int toolCheckIndex = 0; toolCheckIndex < recipe.Tools.Count(); toolCheckIndex++)
  {
   SPKZ_WorkbenchToolRequirement toolReq = recipe.Tools.Get(toolCheckIndex);
   if (!SPKZ_HasUsableTool(toolReq.ClassName))
   {
    response.Message = "Missing tool: " + toolReq.ClassName;
    response.Stock = SPKZ_BuildStockSnapshot();
    SPKZ_SendBuildResponse(sender, response);
    return;
   }
  }

  for (int consumeIndex = 0; consumeIndex < recipe.Materials.Count(); consumeIndex++)
  {
   SPKZ_WorkbenchMaterialCost consumeCost = recipe.Materials.Get(consumeIndex);
   SPKZ_ConsumeItemsOfType(consumeCost.ClassName, consumeCost.Quantity);
  }

  for (int toolDamageIndex = 0; toolDamageIndex < recipe.Tools.Count(); toolDamageIndex++)
  {
   SPKZ_WorkbenchToolRequirement damageToolReq = recipe.Tools.Get(toolDamageIndex);
   SPKZ_DamageToolOfType(damageToolReq.ClassName, damageToolReq.HealthLossPoints);
  }

  EntityAI kit = EntityAI.Cast(GetGame().CreateObjectEx(recipe.OutputKitClassName, GetPosition(), ECE_PLACE_ON_SURFACE));
  if (!kit)
  {
   response.Message = "Failed to create kit.";
   response.Stock = SPKZ_BuildStockSnapshot();
   SPKZ_SendBuildResponse(sender, response);
   return;
  }

  response.Success = true;
  response.Message = recipe.DisplayName + " built.";
  response.Stock = SPKZ_BuildStockSnapshot();
  SPKZ_SendBuildResponse(sender, response);
 }

 protected void SPKZ_SendBuildResponse(PlayerIdentity sender, SPKZ_WorkbenchBuildResponse response)
 {
  ScriptRPC rpc = new ScriptRPC();
  response.WriteToContext(rpc);
  rpc.Send(this, SPKZ_WorkbenchRPCId.BUILD_RESPONSE, true, sender);
 }

 // Client-side responses are handed to SPKZ_WorkbenchClientBridge (3_Game)
 // rather than to SPKZ_WorkbenchMenu directly - 4_World cannot reference
 // 5_Mission's custom classes (Mission can't reference World's either; see
 // Pitfall #8 in docs/CODING_STANDARDS.md), so the bridge is the only
 // channel between them.
 override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
 {
  super.OnRPC(sender, rpc_type, ctx);

  if (GetGame().IsServer())
  {
   if (rpc_type == SPKZ_WorkbenchRPCId.OPEN_REQUEST)
   {
    SPKZ_HandleOpenRequest(sender);
   }
   else if (rpc_type == SPKZ_WorkbenchRPCId.BUILD_REQUEST)
   {
    SPKZ_HandleBuildRequest(sender, ctx);
   }
   return;
  }

  if (rpc_type == SPKZ_WorkbenchRPCId.OPEN_RESPONSE)
  {
   SPKZ_WorkbenchOpenResponse openResponse = new SPKZ_WorkbenchOpenResponse();
   if (openResponse.ReadFromContext(ctx))
   {
    SPKZ_WorkbenchClientBridge.DeliverOpenResponse(this, openResponse);
   }
  }
  else if (rpc_type == SPKZ_WorkbenchRPCId.BUILD_RESPONSE)
  {
   SPKZ_WorkbenchBuildResponse buildResponse = new SPKZ_WorkbenchBuildResponse();
   if (buildResponse.ReadFromContext(ctx))
   {
    SPKZ_WorkbenchClientBridge.DeliverBuildResponse(buildResponse);
   }
  }
 }
}

class SPKZ_WorkbenchKit extends ItemBase
{
 override bool IsBasebuildingKit() { return true; }
 override bool IsDeployable() { return true; }

 override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
 {
  super.OnPlacementComplete(player, position, orientation);
  if (!GetGame().IsServer()) return;

  Object workbench = GetGame().CreateObjectEx("SPKZ_Workbench", position, ECE_OBJECT_SWAP);
  if (!workbench) return;

  SPKZ_Workbench placed = SPKZ_Workbench.Cast(workbench);
  if (placed) { placed.SPKZ_SetOwner(PlayerBase.Cast(player)); }
  workbench.SetPosition(position);
  workbench.SetOrientation(orientation);
  // ActionPlaceObject deletes base-building kits at the end of the action -
  // matching SPKZ_WoodWallDoorKit's own convention, never delete it here.
 }

 override void SetActions()
 {
  super.SetActions();
  AddAction(ActionTogglePlaceObject);
  AddAction(ActionPlaceObject);
 }
}
