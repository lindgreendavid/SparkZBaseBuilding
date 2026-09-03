// Self-contained RPC ids for the workbench. SparkZGroup/SparkZCore are not a
// dependency of this standalone addon (see docs/WORKFLOW.md), so this does
// not reuse SparkZCoreRPCId - these are dispatched entirely through
// SPKZ_Workbench's own EntityAI.OnRPC override, so collision with any other
// mod's RPC ids is not possible (dispatch is per-object, not global).
class SPKZ_WorkbenchRPCId
{
 static const int OPEN_REQUEST = 27201;
 static const int OPEN_RESPONSE = 27202;
 static const int BUILD_REQUEST = 27203;
 static const int BUILD_RESPONSE = 27204;
}

// Server -> client, sent in response to OPEN_REQUEST.
class SPKZ_WorkbenchOpenResponse
{
 ref SPKZ_WorkbenchRecipeCatalog Catalog;
 ref array<ref SPKZ_WorkbenchStockEntry> Stock;
 string Message;

 void SPKZ_WorkbenchOpenResponse()
 {
  Catalog = new SPKZ_WorkbenchRecipeCatalog();
  Stock = new array<ref SPKZ_WorkbenchStockEntry>();
  Message = "";
 }

 void WriteToContext(ParamsWriteContext ctx)
 {
  Catalog.WriteToContext(ctx);
  SPKZ_WorkbenchStockEntry.WriteArray(ctx, Stock);
  ctx.Write(Message);
 }

 bool ReadFromContext(ParamsReadContext ctx)
 {
  if (!Catalog.ReadFromContext(ctx)) return false;
  if (!SPKZ_WorkbenchStockEntry.ReadArray(ctx, Stock)) return false;
  if (!ctx.Read(Message)) return false;
  return true;
 }
}

// Client -> server, sent when the player clicks "Build Now".
class SPKZ_WorkbenchBuildRequest
{
 string RecipeId;

 void WriteToContext(ParamsWriteContext ctx)
 {
  ctx.Write(RecipeId);
 }

 bool ReadFromContext(ParamsReadContext ctx)
 {
  if (!ctx.Read(RecipeId)) return false;
  return true;
 }
}

// Server -> client, sent in response to BUILD_REQUEST.
class SPKZ_WorkbenchBuildResponse
{
 bool Success;
 string Message;
 ref array<ref SPKZ_WorkbenchStockEntry> Stock;

 void SPKZ_WorkbenchBuildResponse()
 {
  Success = false;
  Message = "";
  Stock = new array<ref SPKZ_WorkbenchStockEntry>();
 }

 void WriteToContext(ParamsWriteContext ctx)
 {
  ctx.Write(Success);
  ctx.Write(Message);
  SPKZ_WorkbenchStockEntry.WriteArray(ctx, Stock);
 }

 bool ReadFromContext(ParamsReadContext ctx)
 {
  if (!ctx.Read(Success)) return false;
  if (!ctx.Read(Message)) return false;
  if (!SPKZ_WorkbenchStockEntry.ReadArray(ctx, Stock)) return false;
  return true;
 }
}
