// Client-side half of the workbench's networking. Split from
// scripts/4_World/Entities/SPKZ_Workbench.c because both pieces here need
// SPKZ_WorkbenchMenu, which is a 5_Mission-only class - 4_World code cannot
// reference it directly. See the comments on SPKZ_Workbench's
// SPKZ_OnAccessRequested and OnRPC for the 4_World side of this split.
modded class SPKZ_Workbench
{
 override void SPKZ_OnAccessRequested(PlayerBase player)
 {
  super.SPKZ_OnAccessRequested(player);
  GetGame().GetUIManager().ShowScriptedMenu(new SPKZ_WorkbenchMenu(this), null);
 }

 override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
 {
  super.OnRPC(sender, rpc_type, ctx);

  if (GetGame().IsServer()) return;

  if (rpc_type == SPKZ_WorkbenchRPCId.OPEN_RESPONSE)
  {
   SPKZ_WorkbenchOpenResponse response = new SPKZ_WorkbenchOpenResponse();
   if (response.ReadFromContext(ctx))
   {
    SPKZ_WorkbenchMenu.SPKZ_OnOpenResponse(this, response);
   }
  }
  else if (rpc_type == SPKZ_WorkbenchRPCId.BUILD_RESPONSE)
  {
   SPKZ_WorkbenchBuildResponse response = new SPKZ_WorkbenchBuildResponse();
   if (response.ReadFromContext(ctx))
   {
    SPKZ_WorkbenchMenu.SPKZ_OnBuildResponse(this, response);
   }
  }
 }
}
