// Bridges scripts/4_World (SPKZ_Workbench, which knows the concrete type)
// and scripts/5_Mission (SPKZ_WorkbenchMenu, which opens the UI) without
// either tier ever needing to name the other's custom class - World and
// Mission are sibling script modules and cannot reference each other's own
// newly-declared types at all (only pre-existing vanilla classes, which are
// globally known regardless of which tier's source file declares them).
// Lives in 3_Game because that tier IS visible to both World and Mission.
// World writes into these static fields (using only the universally-known
// `Object` type); Mission polls them once per frame, exactly like the
// existing SPKZ_PlacementLegend.c already polls Hologram's static state -
// no other bridging mechanism worked for two addon-local custom classes.
class SPKZ_WorkbenchClientBridge
{
 static Object s_PendingAccessWorkbench;
 static bool s_HasPendingAccessRequest;

 static Object s_ResponseWorkbench;
 static ref SPKZ_WorkbenchOpenResponse s_LatestOpenResponse;
 static bool s_HasNewOpenResponse;

 static ref SPKZ_WorkbenchBuildResponse s_LatestBuildResponse;
 static bool s_HasNewBuildResponse;

 static void RequestAccess(Object workbench)
 {
  s_PendingAccessWorkbench = workbench;
  s_HasPendingAccessRequest = true;
 }

 static void DeliverOpenResponse(Object workbench, SPKZ_WorkbenchOpenResponse response)
 {
  s_ResponseWorkbench = workbench;
  s_LatestOpenResponse = response;
  s_HasNewOpenResponse = true;
 }

 static void DeliverBuildResponse(SPKZ_WorkbenchBuildResponse response)
 {
  s_LatestBuildResponse = response;
  s_HasNewBuildResponse = true;
 }
}
