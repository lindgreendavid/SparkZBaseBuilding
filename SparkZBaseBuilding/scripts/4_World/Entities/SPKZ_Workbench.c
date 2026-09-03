// Crafting workbench: extends SPKZ_WoodWallDoor/SPKZ_WoodWallDoorKit (see
// SPKZ_WoodWallDoor.c) purely to reuse its owner tracking, lifetime refresh,
// save/load and dismantle-to-kit machinery - a workbench is not a wall, so
// SPKZ_HasDoor/IsOpen/SetActions are overridden below to drop door behaviour.
// Real named attachment slots (SPKZ_WB_Hacksaw, SPKZ_WB_HandSaw,
// SPKZ_WB_Hammer, SPKZ_WB_Shovel, SPKZ_WB_Screwdriver, SPKZ_WB_Pliers,
// SPKZ_WB_SledgeHammer, SPKZ_WB_SharpeningStone - see workbench_slots.hpp)
// come from the real model; the earlier "scan all cargo for a tool anywhere"
// placeholder logic is gone. Anyone can access the build menu (matching this
// addon's existing rule that ordinary interactions - open/close doors - are
// not owner-gated, only dismantle is; see SPKZ_ActionAccessWorkbench). Full
// squad-role gating (Base Access holders only) is future work once this
// addon has a plot-pole/squad-permission system to check against - see
// docs/WORKFLOW.md's list of not-yet-implemented features and docs/BRIEF.md.
class SPKZ_WorkbenchCollision extends BuildingSuper
{
 // Matches the model's separate invisible collision mesh - block player
 // movement without blocking inventory access, same as a working open
 // wardrobe.
 override bool CanObstruct() { return false; }
}

class SPKZ_Workbench extends SPKZ_WoodWallDoor
{
 protected Object m_SPKZBenchCollision;
 // Only set server-side and never networked to every client - each client
 // only receives the specific OPEN_RESPONSE/BUILD_RESPONSE addressed to it.
 protected ref SPKZ_WorkbenchRecipeCatalog m_SPKZCatalog;

 override void EEInit()
 {
  super.EEInit();
  if (GetGame().IsServer())
  {
   m_SPKZCatalog = SPKZ_WorkbenchRecipeCatalog.Load();
   GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SPKZ_EnsureBenchCollision, 100, false);
  }
 }

 override void AfterStoreLoad()
 {
  super.AfterStoreLoad();
  if (GetGame().IsServer())
  {
   GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SPKZ_EnsureBenchCollision, 100, false);
  }
 }

 void SPKZ_EnsureBenchCollision()
 {
  if (!GetGame() || !GetGame().IsServer()) return;
  if (!m_SPKZBenchCollision)
  {
   int flags = ECE_CREATEPHYSICS | ECE_KEEPHEIGHT | ECE_NOSURFACEALIGN | ECE_NOLIFETIME | ECE_NOPERSISTENCY_WORLD | ECE_NOPERSISTENCY_CHAR;
   m_SPKZBenchCollision = GetGame().CreateObjectEx("SPKZ_WorkbenchCollision", GetPosition(), flags);
  }
  if (m_SPKZBenchCollision)
  {
   m_SPKZBenchCollision.SetPosition(GetPosition());
   m_SPKZBenchCollision.SetOrientation(GetOrientation());
  }
 }

 override void EEDelete(EntityAI parent)
 {
  if (GetGame())
  {
   GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(SPKZ_EnsureBenchCollision);
   if (GetGame().IsServer() && m_SPKZBenchCollision) { GetGame().ObjectDelete(m_SPKZBenchCollision); }
  }
  super.EEDelete(parent);
 }

 override bool SPKZ_HasDoor() { return false; }
 override bool IsOpen() { return true; }
 override string SPKZ_ReturnKitType() { return "SPKZ_WorkbenchKit"; }
 override bool CanDisplayCargo() { return true; }

 // Once placed, a workbench is a fixed structure, not a loose item - it
 // must never be pickable into hands or another container's cargo (only
 // dismantling it with a screwdriver, via SPKZ_Dismantle, produces a
 // portable kit again). EntityAI.CanPutIntoHands/CanPutInCargo/IsTakeable
 // all default to true regardless of the config's itemSize - the wall/door/
 // window family never needed to override these because their House-rooted
 // config ancestor made them non-pickable at the engine level already; the
 // workbench's Container_Base ancestor (needed for real cargo - see
 // workbench_vehicles.hpp) does not, so it must be blocked here explicitly,
 // matching the exact pattern the installed game's own TentBase uses to
 // stay non-pickable once deployed.
 override bool IsTakeable() { return false; }
 override bool CanPutIntoHands(EntityAI parent) { return false; }
 override bool CanPutInCargo(EntityAI parent) { return false; }
 override bool CanRemoveFromCargo(EntityAI parent) { return false; }
 override bool CanRemoveFromHands(EntityAI parent) { return false; }

 override bool CanReceiveItemIntoCargo(EntityAI item)
 {
  // The parent (SPKZ_WoodWallDoor) deliberately blocks cargo for walls - the
  // bench is a storage object, so it needs to allow it.
  if (!item || item == this || SPKZ_WoodWallDoor.Cast(item)) return false;
  return true;
 }

 override bool CanReceiveAttachment(EntityAI attachment, int slotId)
 {
  if (!attachment || !super.CanReceiveAttachment(attachment, slotId)) return false;
  if (slotId == InventorySlots.GetSlotIdFromString("SPKZ_WB_Hacksaw")) return attachment.IsKindOf("Hacksaw");
  if (slotId == InventorySlots.GetSlotIdFromString("SPKZ_WB_HandSaw")) return attachment.IsKindOf("HandSaw");
  if (slotId == InventorySlots.GetSlotIdFromString("SPKZ_WB_Hammer")) return attachment.IsKindOf("Hammer");
  if (slotId == InventorySlots.GetSlotIdFromString("SPKZ_WB_Shovel")) return attachment.IsKindOf("Shovel");
  if (slotId == InventorySlots.GetSlotIdFromString("SPKZ_WB_Screwdriver")) return attachment.IsKindOf("Screwdriver");
  if (slotId == InventorySlots.GetSlotIdFromString("SPKZ_WB_Pliers")) return attachment.IsKindOf("Pliers");
  if (slotId == InventorySlots.GetSlotIdFromString("SPKZ_WB_SledgeHammer")) return attachment.IsKindOf("SledgeHammer");
  if (slotId == InventorySlots.GetSlotIdFromString("SPKZ_WB_SharpeningStone")) return attachment.IsKindOf("Whetstone");
  return false;
 }

 override bool SPKZ_CanDismantle(PlayerBase player, ItemBase tool)
 {
  if (!super.SPKZ_CanDismantle(player, tool)) return false;
  if (GetInventory().AttachmentCount() > 0) return false;
  CargoBase cargo = GetInventory().GetCargo();
  return !cargo || cargo.GetItemCount() == 0;
 }

 override void SetActions()
 {
  // No door actions (SPKZ_HasDoor() is false) - normal inventory interaction
  // already exposes cargo/attachments without a dedicated action. This one
  // custom action opens the crafting menu.
  AddAction(SPKZ_ActionAccessWorkbench);
 }

 // Maps a recipe's tool classname to the real named attachment slot it must
 // sit in on the workbench model. A fixed table, not a string transform,
 // since Whetstone (the sharpening stone) doesn't follow the same
 // "SPKZ_WB_" + classname pattern as the rest.
 protected string SPKZ_ToolSlotName(string toolClassName)
 {
  if (toolClassName == "Hacksaw") return "SPKZ_WB_Hacksaw";
  if (toolClassName == "HandSaw") return "SPKZ_WB_HandSaw";
  if (toolClassName == "Hammer") return "SPKZ_WB_Hammer";
  if (toolClassName == "Shovel") return "SPKZ_WB_Shovel";
  if (toolClassName == "Screwdriver") return "SPKZ_WB_Screwdriver";
  if (toolClassName == "Pliers") return "SPKZ_WB_Pliers";
  if (toolClassName == "SledgeHammer") return "SPKZ_WB_SledgeHammer";
  if (toolClassName == "Whetstone") return "SPKZ_WB_SharpeningStone";
  return "";
 }

 // A tool "counts" for a recipe only if it sits in its own designated
 // attachment slot - matching what the model actually shows mounted on the
 // pegboard, rather than the old "anywhere in cargo" placeholder.
 protected ItemBase SPKZ_FindUsableTool(string toolClassName)
 {
  string slotName = SPKZ_ToolSlotName(toolClassName);
  if (slotName == "") return null;

  ItemBase tool = ItemBase.Cast(GetInventory().FindAttachmentByName(slotName));
  if (!tool || !tool.IsKindOf(toolClassName) || tool.IsRuined()) return null;
  return tool;
 }

 protected bool SPKZ_HasUsableTool(string toolClassName)
 {
  return SPKZ_FindUsableTool(toolClassName) != null;
 }

 // Applies a flat health-point loss to one matching tool. Does not consume
 // or remove it - a ruined tool remains mounted until a player takes it out
 // and repairs or replaces it. If a sharpening stone is present in its own
 // slot, the loss is immediately offset - net effect: tools stay maintained
 // for free as long as a stone is mounted.
 protected void SPKZ_DamageToolOfType(string toolClassName, int healthLossPoints)
 {
  ItemBase tool = SPKZ_FindUsableTool(toolClassName);
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

 // Every distinct material classname referenced by any recipe currently
 // known to this workbench's catalog - the set counted in cargo and reported
 // back to the client so the menu can compute red/green affordability. Tools
 // are checked separately via their designated attachment slots, not listed
 // here.
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
  }
 }

 // Materials live in cargo - tools own their own named attachment slots and
 // are never counted here. Ruined items never count as usable stock.
 protected int SPKZ_CountItemsOfType(string className)
 {
  int total = 0;
  CargoBase cargo = GetInventory().GetCargo();
  if (!cargo) return 0;

  for (int index = 0; index < cargo.GetItemCount(); index++)
  {
   ItemBase item = ItemBase.Cast(cargo.GetItem(index));
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

 // Removes up to `amount` units of `className` from this workbench's cargo.
 // Returns false (and removes nothing) if the workbench does not currently
 // hold at least `amount` - callers must check this before consuming any
 // material for a recipe, so a build never partially consumes materials.
 protected bool SPKZ_ConsumeItemsOfType(string className, int amount)
 {
  if (SPKZ_CountItemsOfType(className) < amount) return false;

  CargoBase cargo = GetInventory().GetCargo();
  if (!cargo) return false;

  int remaining = amount;
  for (int index = 0; index < cargo.GetItemCount() && remaining > 0; index++)
  {
   ItemBase item = ItemBase.Cast(cargo.GetItem(index));
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

class SPKZ_WorkbenchKit extends SPKZ_WoodWallDoorKit
{
 override string SPKZ_PlacedType() { return "SPKZ_Workbench"; }
 override string SPKZ_ProjectionType() { return "SPKZ_Workbench_Hologram"; }
}
