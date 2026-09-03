modded class MissionGameplay
{
 protected Widget m_SPKZPlacementLegend;
 protected TextWidget m_SPKZPlacementTitle;
 protected TextWidget m_SPKZPlacementState;
 protected TextWidget m_SPKZPlacementOptions;
 protected TextWidget m_SPKZRotationReadout;
 protected TextWidget m_SPKZPositionReadout;
 override void OnUpdate(float timeslice)
 {
  super.OnUpdate(timeslice);
  if(Hologram.SPKZ_IsActive())
  {
   if(!m_SPKZPlacementLegend)
   {
    m_SPKZPlacementLegend=GetGame().GetWorkspace().CreateWidgets("SparkZBaseBuilding/gui/layouts/sparkz_placement_legend.layout");
    if(!m_SPKZPlacementLegend)return;
    array<string> labels={"Title","State","Rotation","Position","Options","Controls1","Controls2"};
    foreach(string label: labels)
    {
     TextWidget textWidget=TextWidget.Cast(m_SPKZPlacementLegend.FindAnyWidget(label));
     if(textWidget){textWidget.SetTextExactSize(14);}
    }
    m_SPKZPlacementState=TextWidget.Cast(m_SPKZPlacementLegend.FindAnyWidget("State"));
    m_SPKZPlacementTitle=TextWidget.Cast(m_SPKZPlacementLegend.FindAnyWidget("Title"));
    m_SPKZPlacementOptions=TextWidget.Cast(m_SPKZPlacementLegend.FindAnyWidget("Options"));
    m_SPKZRotationReadout=TextWidget.Cast(m_SPKZPlacementLegend.FindAnyWidget("Rotation"));
    m_SPKZPositionReadout=TextWidget.Cast(m_SPKZPlacementLegend.FindAnyWidget("Position"));
   }
   m_SPKZPlacementLegend.Show(true);
   if(m_SPKZPlacementTitle){m_SPKZPlacementTitle.SetText(Hologram.SPKZ_KitTitle());}
   string frozen="";if(Hologram.SPKZ_IsFrozen()){frozen=" [FROZEN]";}
   if(m_SPKZPlacementState){m_SPKZPlacementState.SetText(Hologram.SPKZ_AxisName()+Hologram.SPKZ_AdjustmentReadout()+frozen);}
   if(m_SPKZRotationReadout){m_SPKZRotationReadout.SetText(Hologram.SPKZ_RotationReadout());}
   if(m_SPKZPositionReadout){m_SPKZPositionReadout.SetText(Hologram.SPKZ_PositionReadout());}
   string placementMode="GROUND";
   string snapMode="ON";
   if(Hologram.SPKZ_FreeLook()){placementMode="FREE LOOK";}
   if(!Hologram.SPKZ_SnapEnabled()){snapMode="OFF";}
   if(m_SPKZPlacementOptions){m_SPKZPlacementOptions.SetText(placementMode+" | SNAP: "+snapMode);}
  }
  else if(m_SPKZPlacementLegend)m_SPKZPlacementLegend.Show(false);
 }
}
