// A snapshot of how much of one material a specific workbench's cargo holds
// right now, sent server->client so the build menu can highlight material
// costs red/green. Purely for display - the server always re-checks the
// real cargo contents independently before actually consuming anything.
class SPKZ_WorkbenchStockEntry
{
 string ClassName;
 int Quantity;

 void SPKZ_WorkbenchStockEntry(string className = "", int quantity = 0)
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

 static int FindQuantity(array<ref SPKZ_WorkbenchStockEntry> stock, string className)
 {
  if (!stock) return 0;

  for (int index = 0; index < stock.Count(); index++)
  {
   SPKZ_WorkbenchStockEntry entry = stock.Get(index);
   if (entry && entry.ClassName == className)
    return entry.Quantity;
  }

  return 0;
 }

 static void WriteArray(ParamsWriteContext ctx, array<ref SPKZ_WorkbenchStockEntry> stock)
 {
  int count = stock.Count();
  ctx.Write(count);
  for (int index = 0; index < count; index++)
  {
   stock.Get(index).WriteToContext(ctx);
  }
 }

 static bool ReadArray(ParamsReadContext ctx, out array<ref SPKZ_WorkbenchStockEntry> stock)
 {
  stock = new array<ref SPKZ_WorkbenchStockEntry>();
  int count;
  if (!ctx.Read(count)) return false;

  for (int index = 0; index < count; index++)
  {
   SPKZ_WorkbenchStockEntry entry = new SPKZ_WorkbenchStockEntry();
   if (!entry.ReadFromContext(ctx)) return false;
   stock.Insert(entry);
  }

  return true;
 }
}
