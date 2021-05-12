/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

/**
 * @file car_track.cc
 *
 * @brief NUI Galway CAR NS-3 C++ script
 *
 * @author Joseph Clancy
 * Contact: J.CLANCY6@nuigalway.ie
 */

// ----------------------------------------------------------------------
// Includes and nonsense
// ----------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/bridge-module.h"
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"
#include "ns3/tap-bridge-module.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/nr-point-to-point-epc-helper.h"
#include "ns3/ns2-mobility-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CarTrack");

// ----------------------------------------------------------------------
// Helper functions etc.
// ----------------------------------------------------------------------

void
printAddress(Ipv4InterfaceContainer &inter, std::string const &node){
	Ipv4Address addr = inter.GetAddress (0,0);
	NS_LOG_INFO("IP Address of " << node << ": " << ((addr.Get() >> 24) & 0xff) << "."
	                             << ((addr.Get() >> 16) & 0xff) << "."
	                             << ((addr.Get() >> 8) & 0xff) << "."
	                             << ((addr.Get() >> 0) & 0xff));
}

// ----------------------------------------------------------------------
// Main portion of the simulation code
// ----------------------------------------------------------------------

int
main (int argc, char *argv[])
{
  // Default configuration parameters
  bool        verbose            = true;
  bool        tracing            = true;
  int         simDurationSeconds = 300;
  bool        useFromFile        = true;
  std::string mobilityFile       = "../../../sumo/car_track/ns2mobility.tcl";
  std::string logFile            = "car_logs.txt";

  // ----------------------------------------------------------------------
  // Parse CLI for configuration parameters
  // ----------------------------------------------------------------------

  CommandLine cmd;
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing",                   tracing);
  cmd.AddValue ("simTime", "Simulation Duration in Seconds",        simDurationSeconds);
  cmd.AddValue ("nodes",   "Simulation Duration in Seconds",        useFromFile);
  cmd.AddValue ("mobility","Simulation Duration in Seconds",        mobilityFile);
  cmd.AddValue ("log",     "Simulation Duration in Seconds",        logFile);

  cmd.Parse (argc,argv);

  if (verbose)
    {
      LogComponentEnable ("CarTrack", LOG_LEVEL_INFO);
      //LogComponentEnable ("MmWaveUePhy", LOG_LEVEL_ALL);
    }

  // Binding these globals allows us to connect to the outside world
  GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));
  GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

  // ----------------------------------------------------------------------
  // Create the nodes in the network i.e. car, switches, cameras, masts etc.
  // ----------------------------------------------------------------------

  NS_LOG_INFO ("Creating permenant nodes...");        
  // WIRED DEVICES ======================== // - - - - - - - - - - - - - - - -
                                            //              :
  // BACKBONE                               //              :
  Ptr<Node> bck = CreateObject<Node> ();    // <no IP>      : Backbone switch
  Ptr<Node> dc  = CreateObject<Node> ();    // <Assigned>   : Data Centre Node
                                            //              :
  // BUILDINGS                              //              :
  Ptr<Node> eng = CreateObject<Node> ();    // <no IP>      : Engineering Building Switch
  Ptr<Node> pre = CreateObject<Node> ();    // <no IP>      : Prefab. Switch
  Ptr<Node> cai = CreateObject<Node> ();    // <no IP>      : Cairns Building Switch
  Ptr<Node> bio = CreateObject<Node> ();    // <no IP>      : Bio. Science Building Switch
  Ptr<Node> ila = CreateObject<Node> ();    // <no IP>      : ILAS Building Switch
  Ptr<Node> hut = CreateObject<Node> ();    // <no IP>      : Dangan Car Park Hut Switch
                                            //              :
  // REPEATERS                              //              :
  Ptr<Node> r56 = CreateObject<Node> ();    // <no IP>      : "Repeater" from n5 to n6
  Ptr<Node> r67 = CreateObject<Node> ();    // <no IP>      : "Repeater" from n6 to n7
  Ptr<Node> r78 = CreateObject<Node> ();    // <no IP>      : "Repeater" from n7 to n8
  Ptr<Node> r89 = CreateObject<Node> ();    // <no IP>      : "Repeater" from n8 to n9
                                            //              :
  // NODE SWITCHES                          //              :
  Ptr<Node> n1  = CreateObject<Node> ();    // <no IP>      : Node 1 Switch
  Ptr<Node> n2  = CreateObject<Node> ();    // <no IP>      : Node 2 Switch
  Ptr<Node> n3  = CreateObject<Node> ();    // <no IP>      : Node 3 Switch
  Ptr<Node> n4  = CreateObject<Node> ();    // <no IP>      : Node 4 Switch
  Ptr<Node> n5  = CreateObject<Node> ();    // <no IP>      : Node 5 Switch
  Ptr<Node> n6  = CreateObject<Node> ();    // <no IP>      : Node 6 Switch
  Ptr<Node> n7  = CreateObject<Node> ();    // <no IP>      : Node 7 Switch
  Ptr<Node> n8  = CreateObject<Node> ();    // <no IP>      : Node 8 Switch
  Ptr<Node> n9  = CreateObject<Node> ();    // <no IP>      : Node 9 Switch
  Ptr<Node> n10 = CreateObject<Node> ();    // <no IP>      : Node 10 Switch
  Ptr<Node> n11 = CreateObject<Node> ();    // <no IP>      : Node 11 Switch
  Ptr<Node> n12 = CreateObject<Node> ();    // <no IP>      : Node 12 Switch
  Ptr<Node> n13 = CreateObject<Node> ();    // <no IP>      : Node 13 Switch
  Ptr<Node> n14 = CreateObject<Node> ();    // <no IP>      : Node 14 Switch
  Ptr<Node> n15 = CreateObject<Node> ();    // <no IP>      : Node 15 Switch
  Ptr<Node> n16 = CreateObject<Node> ();    // <no IP>      : Node 16 Switch
                                            //              :
  // CAMERAS                                //              :
  Ptr<Node> n1c1 = CreateObject<Node> ();   // <Assigned>   : Node 1 Camera 1
  Ptr<Node> n1c2 = CreateObject<Node> ();   // <Assigned>   : Node 1 Camera 2
  Ptr<Node> n2c1 = CreateObject<Node> ();   // <Assigned>   : Node 2 Camera 1
  Ptr<Node> n2c2 = CreateObject<Node> ();   // <Assigned>   : Node 2 Camera 2
  Ptr<Node> n3c1 = CreateObject<Node> ();   // <Assigned>   : Node 3 Camera 1
  Ptr<Node> n3c2 = CreateObject<Node> ();   // <Assigned>   : Node 3 Camera 2
  Ptr<Node> n4c1 = CreateObject<Node> ();   // <Assigned>   : Node 4 Camera 1
  Ptr<Node> n4c2 = CreateObject<Node> ();   // <Assigned>   : Node 4 Camera 2
  Ptr<Node> n5c1 = CreateObject<Node> ();   // <Assigned>   : Node 5 Camera 1
  Ptr<Node> n5c2 = CreateObject<Node> ();   // <Assigned>   : Node 5 Camera 2
  Ptr<Node> n6c1 = CreateObject<Node> ();   // <Assigned>   : Node 6 Camera 1
  Ptr<Node> n6c2 = CreateObject<Node> ();   // <Assigned>   : Node 6 Camera 2
  Ptr<Node> n7c1 = CreateObject<Node> ();   // <Assigned>   : Node 7 Camera 1
  Ptr<Node> n7c2 = CreateObject<Node> ();   // <Assigned>   : Node 7 Camera 2
  Ptr<Node> n8c1 = CreateObject<Node> ();   // <Assigned>   : Node 8 Camera 1
  Ptr<Node> n8c2 = CreateObject<Node> ();   // <Assigned>   : Node 8 Camera 2
  Ptr<Node> n9c1 = CreateObject<Node> ();   // <Assigned>   : Node 9 Camera 1
  Ptr<Node> n10c1 = CreateObject<Node> ();  // <Assigned>   : Node 10 Camera 1
  Ptr<Node> n10c2 = CreateObject<Node> ();  // <Assigned>   : Node 10 Camera 2
  Ptr<Node> n11c1 = CreateObject<Node> ();  // <Assigned>   : Node 11 Camera 1
  Ptr<Node> n11c2 = CreateObject<Node> ();  // <Assigned>   : Node 11 Camera 2
  Ptr<Node> n12c1 = CreateObject<Node> ();  // <Assigned>   : Node 12 Camera 1
  Ptr<Node> n13c1 = CreateObject<Node> ();  // <Assigned>   : Node 13 Camera 1
  Ptr<Node> n13c2 = CreateObject<Node> ();  // <Assigned>   : Node 13 Camera 2
  Ptr<Node> n14c1 = CreateObject<Node> ();  // <Assigned>   : Node 14 Camera 1
  Ptr<Node> n14c2 = CreateObject<Node> ();  // <Assigned>   : Node 14 Camera 2
  Ptr<Node> n15c1 = CreateObject<Node> ();  // <Assigned>   : Node 15 Camera 1
  Ptr<Node> n15c2 = CreateObject<Node> ();  // <Assigned>   : Node 15 Camera 2
  Ptr<Node> n16c1 = CreateObject<Node> ();  // <Assigned>   : Node 16 Camera 1
  Ptr<Node> n16c2 = CreateObject<Node> ();  // <Assigned>   : Node 16 Camera 2
                                            //              :
  // WIRELESS DEVICES                       //              :
  Ptr<Node> car = CreateObject<Node> ();    // <no IP>      : Backbone switch
  Ptr<Node> gnb = CreateObject<Node> ();    // <Assigned>   : Data Centre Node
											                      // - - - - - - - - - - - - - - - -

  // ----------------------------------------------------------------------
  // Give the nodes names (no functional purpose, for ease of grouping)
  // ----------------------------------------------------------------------

  NS_LOG_INFO ("Adding nodes to name registry...");

  Names::Add ("car", car);
  Names::Add ("gnb", gnb);

  Names::Add ("bck", bck);
  Names::Add ("dc",  dc);

  Names::Add ("eng", eng);
  Names::Add ("pre", pre);
  Names::Add ("cai", cai);
  Names::Add ("bio", bio);
  Names::Add ("ila", ila);
  Names::Add ("hut", hut);

  Names::Add ("r56", r56);
  Names::Add ("r67", r67);
  Names::Add ("r78", r78);
  Names::Add ("r89", r89);

  Names::Add ("n1", n1);
  Names::Add ("n2", n2);
  Names::Add ("n3", n3);
  Names::Add ("n4", n4);
  Names::Add ("n5", n5);
  Names::Add ("n6", n6);
  Names::Add ("n7", n7);
  Names::Add ("n8", n8);
  Names::Add ("n9", n9);
  Names::Add ("n10", n10);
  Names::Add ("n11", n11);
  Names::Add ("n12", n12);
  Names::Add ("n13", n13);
  Names::Add ("n14", n14);
  Names::Add ("n15", n15);
  Names::Add ("n16", n16);

  Names::Add ("n1c1", n1c1);
  Names::Add ("n1c2", n1c2);
  Names::Add ("n2c1", n2c1);
  Names::Add ("n2c2", n2c2);
  Names::Add ("n3c1", n3c1);
  Names::Add ("n3c2", n3c2);
  Names::Add ("n4c1", n4c1);
  Names::Add ("n4c2", n4c2);
  Names::Add ("n5c1", n5c1);
  Names::Add ("n5c2", n5c2);
  Names::Add ("n6c1", n6c1);
  Names::Add ("n6c2", n6c2);
  Names::Add ("n7c1", n7c1);
  Names::Add ("n7c2", n7c2);
  Names::Add ("n8c1", n8c1);
  Names::Add ("n8c2", n8c2);
  Names::Add ("n9c1", n9c1);
  Names::Add ("n10c1", n10c1);
  Names::Add ("n10c2", n10c2);
  Names::Add ("n11c1", n11c1);
  Names::Add ("n11c2", n11c2);
  Names::Add ("n12c1", n12c1);
  Names::Add ("n13c1", n13c1);
  Names::Add ("n13c2", n13c2);
  Names::Add ("n14c1", n14c1);
  Names::Add ("n14c2", n14c2);
  Names::Add ("n15c1", n15c1);
  Names::Add ("n15c2", n15c2);
  Names::Add ("n16c1", n16c1);
  Names::Add ("n16c2", n16c2);

  // ----------------------------------------------------------------------
  // Group Nodes
  // ----------------------------------------------------------------------

  NS_LOG_INFO ("Grouping switch and camera nodes...");

  NodeContainer switches (bck,eng,cai,pre);
  switches.Add (bio);
  switches.Add (ila);
  switches.Add (hut);
  switches.Add (r56);
  switches.Add (r67);
  switches.Add (r78);
  switches.Add (r89);
  switches.Add (n1);
  switches.Add (n2);
  switches.Add (n3);
  switches.Add (n4);
  switches.Add (n5);
  switches.Add (n6);
  switches.Add (n7);
  switches.Add (n8);
  switches.Add (n9);
  switches.Add (n10);
  switches.Add (n11);
  switches.Add (n12);
  switches.Add (n13);
  switches.Add (n14);
  switches.Add (n15);
  switches.Add (n16);

  NodeContainer cameras;
  cameras.Add (n1c1);
  cameras.Add (n1c2);
  cameras.Add (n2c1);
  cameras.Add (n2c2);
  cameras.Add (n3c1);
  cameras.Add (n3c2);
  cameras.Add (n4c1);
  cameras.Add (n4c2);
  cameras.Add (n5c1);
  cameras.Add (n5c2);
  cameras.Add (n6c1);
  cameras.Add (n6c2);
  cameras.Add (n7c1);
  cameras.Add (n7c2);
  cameras.Add (n8c1);
  cameras.Add (n8c2);
  cameras.Add (n9c1);
  cameras.Add (n10c1);
  cameras.Add (n10c2);
  cameras.Add (n11c1);
  cameras.Add (n11c2);
  cameras.Add (n12c1);
  cameras.Add (n13c1);
  cameras.Add (n13c2);
  cameras.Add (n14c1);
  cameras.Add (n14c2);
  cameras.Add (n15c1);
  cameras.Add (n15c2);
  cameras.Add (n16c1);
  cameras.Add (n16c2);

  std::map<std::string, int> nodeDict;

  for(uint32_t i=0; i<cameras.GetN(); i++){
  		  nodeDict[Names::FindName(cameras.Get(i))] = i;
  }

  // ---------------------------------------------------------------------
  // Create WIRED network
  // ----------------------------------------------------------------------

  // ---------------------------------------------------------------------
  // Create Ethernet links to use for connecting nodes together
  // ----------------------------------------------------------------------

  NS_LOG_INFO ("Creating a 1Gbps/6.56us Ethernet link...");
  CsmaHelper Eth1G;
  Eth1G.SetChannelAttribute ("DataRate", StringValue ("1Gbps"));
  Eth1G.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  NS_LOG_INFO ("Creating a 10Gbps/6.56us Ethernet link...");
  CsmaHelper Eth10G;
  Eth10G.SetChannelAttribute ("DataRate", StringValue ("10Gbps"));
  Eth10G.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  // ----------------------------------------------------------------------
  // Connect the nodes together with Ethernet links.
  // ----------------------------------------------------------------------

  NS_LOG_INFO ("Connecting nodes together using Ethernet links...");

  // DATACENTRE-BACKBONE
  NetDeviceContainer link_dc_bck = Eth10G.Install (NodeContainer (dc, bck));

  // BUILDINGS-BACKBONE
  NetDeviceContainer link_eng_bck = Eth10G.Install (NodeContainer (eng, bck));
  NetDeviceContainer link_cai_bck = Eth10G.Install (NodeContainer (cai, bck));
  NetDeviceContainer link_pre_cai = Eth10G.Install (NodeContainer (pre, cai));
  NetDeviceContainer link_bio_bck = Eth10G.Install (NodeContainer (bio, bck));
  NetDeviceContainer link_ila_bio = Eth10G.Install (NodeContainer (ila, bio));
  NetDeviceContainer link_hut_cai = Eth10G.Install (NodeContainer (hut, cai));

  // NODES-BUILDINGS
  NetDeviceContainer link_n2_eng = Eth10G.Install (NodeContainer (n2,  eng));
  NetDeviceContainer link_n3_cai = Eth10G.Install (NodeContainer (n3,  cai));
  NetDeviceContainer link_n4_pre = Eth10G.Install (NodeContainer (n4,  pre));
  NetDeviceContainer link_n10_bio = Eth10G.Install (NodeContainer (n10, bio));
  NetDeviceContainer link_n11_ila = Eth10G.Install (NodeContainer (n11, ila));
  NetDeviceContainer link_n12_hut = Eth10G.Install (NodeContainer (n12, hut));
  NetDeviceContainer link_n13_hut = Eth10G.Install (NodeContainer (n13, hut));
  NetDeviceContainer link_n16_hut = Eth10G.Install (NodeContainer (n16, hut));

  // NODES-REPEATERS
  NetDeviceContainer link_n5_r56 = Eth10G.Install (NodeContainer (n5,  r56));
  NetDeviceContainer link_r56_n6 = Eth10G.Install (NodeContainer (r56, n6));
  NetDeviceContainer link_n6_r67 = Eth10G.Install (NodeContainer (n6,  r67));
  NetDeviceContainer link_r67_n7 = Eth10G.Install (NodeContainer (r67, n7));
  NetDeviceContainer link_n7_r78 = Eth10G.Install (NodeContainer (n7,  r78));
  NetDeviceContainer link_r78_n8 = Eth10G.Install (NodeContainer (r78, n8));
  NetDeviceContainer link_n8_r89 = Eth10G.Install (NodeContainer (n8,  r89));
  NetDeviceContainer link_r89_n9 = Eth10G.Install (NodeContainer (r89, n9));

  // NODES-NODES
  NetDeviceContainer link_n1_n2   = Eth10G.Install (NodeContainer (n1,  n2));
  NetDeviceContainer link_n9_n10  = Eth10G.Install (NodeContainer (n9,  n10));
  NetDeviceContainer link_n14_n13 = Eth10G.Install (NodeContainer (n14, n13));
  NetDeviceContainer link_n15_n16 = Eth10G.Install (NodeContainer (n15, n16));

  // CAMERAS-NODES
  NetDeviceContainer link_n1c1_n1 = Eth10G.Install (NodeContainer (n1c1,  n1));
  NetDeviceContainer link_n1c2_n1 = Eth10G.Install (NodeContainer (n1c2,  n1));
  NetDeviceContainer link_n2c1_n2 = Eth10G.Install (NodeContainer (n2c1, n2));
  NetDeviceContainer link_n2c2_n2 = Eth10G.Install (NodeContainer (n2c2, n2));
  NetDeviceContainer link_n3c1_n3 = Eth10G.Install (NodeContainer (n3c1, n3));
  NetDeviceContainer link_n3c2_n3 = Eth10G.Install (NodeContainer (n3c2, n3));
  NetDeviceContainer link_n4c1_n4 = Eth10G.Install (NodeContainer (n4c1,  n4));
  NetDeviceContainer link_n4c2_n4 = Eth10G.Install (NodeContainer (n4c2,  n4));
  NetDeviceContainer link_n5c1_n5 = Eth1G.Install  (NodeContainer (n5c1, n5));
  NetDeviceContainer link_n5c2_n5 = Eth1G.Install  (NodeContainer (n5c2, n5));
  NetDeviceContainer link_n6c1_n6 = Eth1G.Install  (NodeContainer (n6c1, n6));
  NetDeviceContainer link_n6c2_n6 = Eth1G.Install  (NodeContainer (n6c2, n6));
  NetDeviceContainer link_n7c1_n7 = Eth1G.Install  (NodeContainer (n7c1,  n7));
  NetDeviceContainer link_n7c2_n7 = Eth1G.Install  (NodeContainer (n7c2,  n7));
  NetDeviceContainer link_n8c1_n8 = Eth1G.Install  (NodeContainer (n8c1, n8));
  NetDeviceContainer link_n8c2_n8 = Eth1G.Install  (NodeContainer (n8c2, n8));
  NetDeviceContainer link_n9c1_n9 = Eth10G.Install (NodeContainer (n9c1, n9));
  NetDeviceContainer link_n10c1_n10 = Eth10G.Install (NodeContainer (n10c1, n10));
  NetDeviceContainer link_n10c2_n10 = Eth10G.Install (NodeContainer (n10c2,  n10));
  NetDeviceContainer link_n11c1_n11 = Eth10G.Install (NodeContainer (n11c1,  n11));
  NetDeviceContainer link_n11c2_n11 = Eth10G.Install (NodeContainer (n11c2, n11));
  NetDeviceContainer link_n12c1_n12 = Eth10G.Install (NodeContainer (n12c1, n12));
  NetDeviceContainer link_n13c1_n13 = Eth10G.Install (NodeContainer (n13c1, n13));
  NetDeviceContainer link_n13c2_n13 = Eth10G.Install (NodeContainer (n13c2, n13));
  NetDeviceContainer link_n14c1_n14 = Eth10G.Install (NodeContainer (n14c1,  n14));
  NetDeviceContainer link_n14c2_n14 = Eth1G.Install  (NodeContainer (n14c2,  n14));
  NetDeviceContainer link_n15c1_n15 = Eth1G.Install  (NodeContainer (n15c1, n15));
  NetDeviceContainer link_n15c2_n15 = Eth10G.Install (NodeContainer (n15c2, n15));
  NetDeviceContainer link_n16c1_n16 = Eth10G.Install (NodeContainer (n16c1, n16));
  NetDeviceContainer link_n16c2_n16 = Eth1G.Install  (NodeContainer (n16c2, n16));

  // ---------------------------------------------------------------------
  // Group together the network devices of the cameras for IP assignment
  // ---------------------------------------------------------------------

  NS_LOG_INFO ("Grouping camera network devices...");

  NetDeviceContainer cameraDevices;         // - - - - - -- - - - - - -
  cameraDevices.Add (link_n1c1_n1.Get (0)); // NOTE: order matters here
  cameraDevices.Add (link_n1c2_n1.Get (0)); //       for IP address
  cameraDevices.Add (link_n2c1_n2.Get (0)); //       assignment
  cameraDevices.Add (link_n2c2_n2.Get (0)); // - - - - - -- - - - - -
  cameraDevices.Add (link_n3c1_n3.Get (0));
  cameraDevices.Add (link_n3c2_n3.Get (0));
  cameraDevices.Add (link_n4c1_n4.Get (0));
  cameraDevices.Add (link_n4c2_n4.Get (0));
  cameraDevices.Add (link_n5c1_n5.Get (0));
  cameraDevices.Add (link_n5c2_n5.Get (0));
  cameraDevices.Add (link_n6c1_n6.Get (0));
  cameraDevices.Add (link_n6c2_n6.Get (0));
  cameraDevices.Add (link_n7c1_n7.Get (0));
  cameraDevices.Add (link_n7c2_n7.Get (0));
  cameraDevices.Add (link_n8c1_n8.Get (0));
  cameraDevices.Add (link_n8c2_n8.Get (0));
  cameraDevices.Add (link_n9c1_n9.Get (0));
  cameraDevices.Add (link_n10c1_n10.Get (0));
  cameraDevices.Add (link_n10c2_n10.Get (0));
  cameraDevices.Add (link_n11c1_n11.Get (0));
  cameraDevices.Add (link_n11c2_n11.Get (0));
  cameraDevices.Add (link_n12c1_n12.Get (0));
  cameraDevices.Add (link_n13c1_n13.Get (0));
  cameraDevices.Add (link_n13c2_n13.Get (0));
  cameraDevices.Add (link_n14c1_n14.Get (0));
  cameraDevices.Add (link_n14c2_n14.Get (0));
  cameraDevices.Add (link_n15c1_n15.Get (0));
  cameraDevices.Add (link_n15c2_n15.Get (0));
  cameraDevices.Add (link_n16c1_n16.Get (0));
  cameraDevices.Add (link_n16c2_n16.Get (0));

  // ----------------------------------------------------------------------
  // Manually create the list of NetDevices for each switch
  // ----------------------------------------------------------------------

  NS_LOG_INFO ("Creating NetDevices for switches...");

  NetDeviceContainer bckND;
  bckND.Add (link_dc_bck.Get  (1));
  bckND.Add (link_eng_bck.Get (1));
  bckND.Add (link_cai_bck.Get (1));
  bckND.Add (link_bio_bck.Get (1));

  NetDeviceContainer engND;
  engND.Add (link_eng_bck.Get (0));
  engND.Add (link_n2_eng. Get (1));

  NetDeviceContainer caiND;
  caiND.Add (link_cai_bck.Get (0));
  caiND.Add (link_n3_cai. Get (1));
  caiND.Add (link_pre_cai. Get(1));
  caiND.Add (link_hut_cai. Get(1));

  NetDeviceContainer preND;
  preND.Add (link_pre_cai.Get (0));
  preND.Add (link_n4_pre. Get (1));

  NetDeviceContainer bioND;
  bioND.Add (link_bio_bck.Get (0));
  bioND.Add (link_n10_bio.Get (1));
  bioND.Add (link_ila_bio.Get (1));

  NetDeviceContainer ilaND;
  ilaND.Add (link_ila_bio.Get (0));
  ilaND.Add (link_n11_ila.Get (1));

  NetDeviceContainer hutND;
  hutND.Add (link_hut_cai.Get (0));
  hutND.Add (link_n12_hut.Get (1));
  hutND.Add (link_n13_hut.Get (1));
  hutND.Add (link_n16_hut.Get (1));

  NetDeviceContainer r56ND;
  r56ND.Add (link_n5_r56.Get (1));
  r56ND.Add (link_r56_n6.Get (0));

  NetDeviceContainer r67ND;
  r67ND.Add (link_n6_r67.Get (1));
  r67ND.Add (link_r67_n7.Get (0));

  NetDeviceContainer r78ND;
  r78ND.Add (link_n7_r78.Get (1));
  r78ND.Add (link_r78_n8.Get (0));

  NetDeviceContainer r89ND;
  r89ND.Add (link_n8_r89.Get (1));
  r89ND.Add (link_r89_n9.Get (0));

  NetDeviceContainer n1ND;
  n1ND.Add (link_n1_n2.  Get (0));
  n1ND.Add (link_n1c1_n1.Get (1));
  n1ND.Add (link_n1c2_n1.Get (1));

  NetDeviceContainer n2ND;
  n2ND.Add (link_n2_eng. Get (0));
  n2ND.Add (link_n1_n2.  Get (1));
  n2ND.Add (link_n2c1_n2.Get (1));
  n2ND.Add (link_n2c2_n2.Get (1));

  NetDeviceContainer n3ND;
  n3ND.Add (link_n3_cai. Get (0));
  n3ND.Add (link_n3c1_n3.Get (1));
  n3ND.Add (link_n3c2_n3.Get (1));

  NetDeviceContainer n4ND;
  n4ND.Add (link_n4_pre. Get (0));
  n4ND.Add (link_n4c1_n4.Get (1));
  n4ND.Add (link_n4c2_n4.Get (1));

  NetDeviceContainer n5ND;
  n5ND.Add (link_n5_r56. Get (0));
  n5ND.Add (link_n5c1_n5.Get (1));
  n5ND.Add (link_n5c2_n5.Get (1));

  NetDeviceContainer n6ND;
  n6ND.Add (link_r56_n6. Get (1));
  n6ND.Add (link_n6_r67. Get (0));
  n6ND.Add (link_n6c1_n6.Get (1));
  n6ND.Add (link_n6c2_n6.Get (1));

  NetDeviceContainer n7ND;
  n7ND.Add (link_r67_n7. Get (1));
  n7ND.Add (link_n7_r78. Get (0));
  n7ND.Add (link_n7c1_n7.Get (1));
  n7ND.Add (link_n7c2_n7.Get (1));

  NetDeviceContainer n8ND;
  n8ND.Add (link_r78_n8. Get (1));
  n8ND.Add (link_n8_r89. Get (0));
  n8ND.Add (link_n8c1_n8.Get (1));
  n8ND.Add (link_n8c2_n8.Get (1));

  NetDeviceContainer n9ND;
  n9ND.Add (link_r89_n9. Get (1));
  n9ND.Add (link_n9_n10. Get (0));
  n9ND.Add (link_n9c1_n9.Get (1));

  NetDeviceContainer n10ND;
  n10ND.Add (link_n10_bio.  Get (0));
  n10ND.Add (link_n9_n10.   Get (1));
  n10ND.Add (link_n10c1_n10.Get (1));
  n10ND.Add (link_n10c2_n10.Get (1));

  NetDeviceContainer n11ND;
  n11ND.Add (link_n11_ila.  Get (0));
  n11ND.Add (link_n11c1_n11.Get (1));
  n11ND.Add (link_n11c2_n11.Get (1));

  NetDeviceContainer n12ND;
  n12ND.Add (link_n12_hut.  Get (0));
  n12ND.Add (link_n12c1_n12.Get (1));

  NetDeviceContainer n13ND;
  n13ND.Add (link_n13_hut.  Get (0));
  n13ND.Add (link_n14_n13.  Get (1));
  n13ND.Add (link_n13c1_n13.Get (1));
  n13ND.Add (link_n13c2_n13.Get (1));

  NetDeviceContainer n14ND;
  n14ND.Add (link_n14_n13.  Get (0));
  n14ND.Add (link_n14c1_n14.Get (1));
  n14ND.Add (link_n14c2_n14.Get (1));

  NetDeviceContainer n15ND;
  n15ND.Add (link_n15_n16.  Get (0));
  n15ND.Add (link_n15c1_n15.Get (1));
  n15ND.Add (link_n15c2_n15.Get (1));

  NetDeviceContainer n16ND;
  n16ND.Add (link_n15_n16.  Get (1));
  n16ND.Add (link_n16_hut.  Get (0));
  n16ND.Add (link_n16c1_n16.Get (1));
  n16ND.Add (link_n16c2_n16.Get (1));

  // ----------------------------------------------------------------------
  // Install bridging code on each switch
  // ----------------------------------------------------------------------

  NS_LOG_INFO ("Installing bridging code on each switch...");

  BridgeHelper bridge;

  bridge.Install (eng, engND);
  bridge.Install (cai, caiND);
  bridge.Install (pre, preND);
  bridge.Install (bio, bioND);
  bridge.Install (ila, ilaND);
  bridge.Install (hut, hutND);

  bridge.Install (r56, r56ND);
  bridge.Install (r67, r67ND);
  bridge.Install (r78, r78ND);
  bridge.Install (r89, r89ND);

  bridge.Install (n1,  n1ND);
  bridge.Install (n2,  n2ND);
  bridge.Install (n3,  n3ND);
  bridge.Install (n4,  n4ND);
  bridge.Install (n5,  n5ND);
  bridge.Install (n6,  n6ND);
  bridge.Install (n7,  n7ND);
  bridge.Install (n8,  n8ND);
  bridge.Install (n9,  n9ND);
  bridge.Install (n10,  n10ND);
  bridge.Install (n11,  n11ND);
  bridge.Install (n12,  n12ND);
  bridge.Install (n13,  n13ND);
  bridge.Install (n14,  n14ND);
  bridge.Install (n15,  n15ND);
  bridge.Install (n16,  n16ND);

  // ---------------------------------------------------------------------
  // Create WIRELESS network
  // ----------------------------------------------------------------------

  // ---------------------------------------------------------------------
  // Create 5G network elements
  // ----------------------------------------------------------------------

  NS_LOG_INFO ("Creating 5G network links...");

  Ptr<MmWaveHelper> mmWaveHelper = CreateObject<MmWaveHelper> ();
  mmWaveHelper->SetAttribute ("PathlossModel", StringValue ("ns3::MmWave3gppPropagationLossModel"));
  mmWaveHelper->SetAttribute ("ChannelModel", StringValue ("ns3::MmWave3gppChannel"));

  Ptr<NrPointToPointEpcHelper> epcHelper = CreateObject<NrPointToPointEpcHelper> ();

  mmWaveHelper->SetEpcHelper (epcHelper);
  mmWaveHelper->Initialize();

  Ptr<Node> pgw = epcHelper->GetPgwNode ();
  Ptr<Node> sgw = epcHelper->GetSgwNode ();

  Names::Add ("pgw", pgw);
  Names::Add ("sgw", sgw);
  Names::Add ("mme", NodeList::GetNode(62));


  // ------------------------------------------------------------------
  // Install mobility models on all nodes
  // ------------------------------------------------------------------

  NS_LOG_INFO ("Setting initial positions and attatching mobility models...");

  NS_LOG_INFO ("Creating position allocator...");

  MobilityHelper mobility;
  Ptr<ListPositionAllocator> posAlloc = CreateObject<ListPositionAllocator> ();

  std::string line;
  std::ifstream staticPosFile("../../../sumo/car_track/static_nodes_cart.txt");

  while (staticPosFile >> line){
	  float x;
	  float y;
	  std::vector<std::string> result;
	  std::string temp;
	  std::istringstream ss(line);

	  while (std::getline(ss, temp, ',')) {
	          result.push_back(temp);
	  }
	  x = std::stof(result[1]);
	  y = std::stof(result[2]);

	  if (result[0] == "gnb"){
		  posAlloc->Add(Vector(x,y,10));
	  }
	  else{
		  posAlloc->Add(Vector(x,y,1.5));
	  }
  }

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (posAlloc);

  NS_LOG_INFO ("Installing mobility models on static nodes...");

  mobility.Install (dc);
  mobility.Install (gnb);
  mobility.Install (pgw);
  mobility.Install (sgw);
  mobility.Install (NodeList::GetNode(62));
  mobility.Install (switches);
  mobility.Install (cameras);

  NS_LOG_INFO ("Setting non-static mobility models...");

  Ns2MobilityHelper ns2 = Ns2MobilityHelper (mobilityFile);
  ns2.Install ();

  // ---------------------------------------------------------------------
  // Connect nodes together using 5G network
  // ----------------------------------------------------------------------

  NS_LOG_INFO ("Connecting nodes to 5G network...");

  NetDeviceContainer gnbND = mmWaveHelper->InstallEnbDevice (gnb);
  NetDeviceContainer carND = mmWaveHelper->InstallUeDevice (car);

  // ---------------------------------------------------------------------
  // Connect 5G wireless network to the Ethernet wired network
  // ----------------------------------------------------------------------

  NS_LOG_INFO ("Connecting 5G network to Ethernet network...");

  NetDeviceContainer link_pgw_bck = Eth10G.Install (NodeContainer (pgw, bck));

  bckND.Add (link_pgw_bck.Get (1));
  bridge.Install (bck, bckND);

  // ------------------------------------------------------------------
  // Install Internet stack on devices
  // ------------------------------------------------------------------

  NS_LOG_INFO ("Installing Internet stack on devices...");

  InternetStackHelper internet;
  internet.Install (car);
  Ipv4InterfaceContainer carIpIface;
  carIpIface = epcHelper->AssignUeIpv4Address (NetDeviceContainer (carND));

  internet.Install (dc);
  internet.Install (cameras);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer dcInterface;
  Ipv4InterfaceContainer pgwInterface;
  Ipv4InterfaceContainer cameraInterfaces;

  dcInterface = address.Assign(link_dc_bck.Get(0));
  pgwInterface = address.Assign(link_pgw_bck.Get(0));
  cameraInterfaces = address.Assign (cameraDevices);

  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> pgwStaticRouting = ipv4RoutingHelper.GetStaticRouting (pgw->GetObject<Ipv4> ());
  pgwStaticRouting->AddNetworkRouteTo (Ipv4Address ("10.1.1.0"), Ipv4Mask ("255.255.255.0"), 1);

  mmWaveHelper->AttachToClosestEnb (carND, gnbND);

  // ------------------------------------------------------------------
  // Install Tap Bridges on relevant devices
  // ------------------------------------------------------------------
  NS_LOG_INFO ("Configuring TAP Bridges...");

  TapBridgeHelper tapBridge;
  tapBridge.SetAttribute ("Mode", StringValue ("UseBridge"));

  tapBridge.SetAttribute ("DeviceName", StringValue ("tap-dc"));
  tapBridge.Install (dc, link_dc_bck.Get (0));

  tapBridge.SetAttribute ("DeviceName", StringValue ("tap-car"));
  tapBridge.Install (car, carND.Get(0));

  if(useFromFile){
	  // Parse selected nodes
	  std::string line;
	  std::ifstream nodeFile("nodesToUse.txt");

	  while (nodeFile >> line){
		  tapBridge.SetAttribute ("DeviceName", StringValue ("tap-" + Names::FindName(cameras.Get(nodeDict[line]))));
		  tapBridge.Install (cameras.Get(nodeDict[line]), cameraDevices.Get(nodeDict[line]));
	  }

  }
  else{
	  /*for(uint32_t i=0; i<cameras.GetN(); i++){
		  tapBridge.SetAttribute ("DeviceName", StringValue ("tap-" + Names::FindName(cameras.Get(i))));
		  tapBridge.Install (cameras.Get(i), cameraDevices.Get(i));
	  }*/
	  tapBridge.SetAttribute ("DeviceName", StringValue ("tap-n1c1"));
	  tapBridge.Install (n1c1, link_n1c1_n1.Get (0));

	  tapBridge.SetAttribute ("DeviceName", StringValue ("tap-n1c2"));
	  tapBridge.Install (n1c2, link_n1c2_n1.Get (0));
  }

  // ------------------------------------------------------------------
  // Install UDP echo server
  // ------------------------------------------------------------------
  /*NS_LOG_INFO ("Installing UDP server...");

  UdpEchoServerHelper server (udpEchoPort);

  ApplicationContainer serverApp = server.Install (dc);
  serverApp.Start (Seconds (0.5));
  serverApp.Stop  (Seconds (simDurationSeconds));

  // ------------------------------------------------------------------
  // Install UDP echo client
  // ------------------------------------------------------------------
  NS_LOG_INFO ("Installing UDP client...");

  Time     interPacketInterval = Seconds (0.005);
  uint32_t packetSize          = 1000;
  uint32_t maxPacketCount      = (simDurationSeconds - 2.0) / 0.005;

  UdpEchoClientHelper client (dcInter.GetAddress(0,0), udpEchoPort);

  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client.SetAttribute ("Interval",   TimeValue     (interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (packetSize));

  ApplicationContainer clientApp = client.Install (n5c1);
  clientApp.Start (Seconds (0.6));
  clientApp.Stop  (Seconds (simDurationSeconds));

  for(uint32_t i=0; i<cameras.GetN(); i++){
	  ApplicationContainer clientApp = client.Install (cameras.Get(i));
	  clientApp.Start (Seconds (0.6));
	  clientApp.Stop  (Seconds (simDurationSeconds));
  }*/

  // ----------------------------------------------------------------------
  // Configure PCAP traces
  // ----------------------------------------------------------------------

  NS_LOG_INFO ("Configuring PCAP Tracing...");

  if (tracing) {
    // Server
    Eth10G.EnablePcap ("dc.pcap",   link_dc_bck. Get (0), true, true);

    mmWaveHelper->EnableTraces();

      // Clients
    if(useFromFile){
        // Parse selected nodes
        std::string line;
        std::ifstream nodeFile("nodesToUse.txt");

        while (nodeFile >> line){
          Eth10G.EnablePcap (line + ".pcap", cameraDevices.Get(nodeDict[line]), true, true);
        }

      }
      else{
        /*for(uint32_t i=0; i<cameras.GetN(); i++){
          Eth10G.EnablePcap (Names::FindName(cameras.Get(i)) + ".pcap", cameraDevices.Get(i), true, true);
        }*/
        Eth10G.EnablePcap ("n1c1.pcap",   link_dc_bck. Get (0), true, true);
        Eth10G.EnablePcap ("n1c2.pcap",   link_dc_bck. Get (0), true, true);
      }

  }

  // ----------------------------------------------------------------------
  // Output to animator file
  // ----------------------------------------------------------------------
  NS_LOG_INFO ("Configuring animation file...");

  // Set output file
  AnimationInterface anim ("car_animation");

  // Set background
  anim.SetBackgroundImage ("HorizontalMap.png", 0, 0, 0.1, 0.1, 0.5);

  // Enable metadata
  anim.EnablePacketMetadata ();

  // Colour nodes
  anim.UpdateNodeColor(dc, 0, 255, 0);

  for(uint32_t i=0; i<cameras.GetN(); i++){
	  anim.UpdateNodeColor(cameras.Get(i), 0, 0, 255);
  }

  // Update node sizes
  for(uint32_t i=0; i<61; i++){
	  anim.UpdateNodeSize(i, 4, 4);
  }

  // Give nodes names
  anim.UpdateNodeDescription(dc, "dc");
  anim.UpdateNodeDescription(bck, "bck");

  anim.UpdateNodeDescription(car, "car");
  anim.UpdateNodeDescription(gnb, "gnb");
  anim.UpdateNodeDescription(pgw, "pgw");

  anim.UpdateNodeDescription(eng, "eng");
  anim.UpdateNodeDescription(cai, "cai");
  anim.UpdateNodeDescription(pre, "pre");
  anim.UpdateNodeDescription(bio, "bio");
  anim.UpdateNodeDescription(ila, "ila");
  anim.UpdateNodeDescription(hut, "hut");

  anim.UpdateNodeDescription(r56, "r56");
  anim.UpdateNodeDescription(r67, "r67");
  anim.UpdateNodeDescription(r78, "r78");
  anim.UpdateNodeDescription(r89, "r89");

  anim.UpdateNodeDescription(n1, "n1");
  anim.UpdateNodeDescription(n2, "n2");
  anim.UpdateNodeDescription(n3, "n3");
  anim.UpdateNodeDescription(n4, "n4");
  anim.UpdateNodeDescription(n5, "n5");
  anim.UpdateNodeDescription(n6, "n6");
  anim.UpdateNodeDescription(n7, "n7");
  anim.UpdateNodeDescription(n8, "n8");
  anim.UpdateNodeDescription(n9, "n9");
  anim.UpdateNodeDescription(n10, "n10");
  anim.UpdateNodeDescription(n11, "n11");
  anim.UpdateNodeDescription(n12, "n12");
  anim.UpdateNodeDescription(n13, "n13");
  anim.UpdateNodeDescription(n14, "n14");
  anim.UpdateNodeDescription(n15, "n15");
  anim.UpdateNodeDescription(n16, "n16");

  anim.UpdateNodeDescription(n1c1, "n1c1");
  anim.UpdateNodeDescription(n1c2, "n1c2");
  anim.UpdateNodeDescription(n2c1, "n2c1");
  anim.UpdateNodeDescription(n2c2, "n2c2");
  anim.UpdateNodeDescription(n3c1, "n3c1");
  anim.UpdateNodeDescription(n3c2, "n3c2");
  anim.UpdateNodeDescription(n4c1, "n4c1");
  anim.UpdateNodeDescription(n4c2, "n4c2");
  anim.UpdateNodeDescription(n5c1, "n5c1");
  anim.UpdateNodeDescription(n5c2, "n5c2");
  anim.UpdateNodeDescription(n6c1, "n6c1");
  anim.UpdateNodeDescription(n6c2, "n6c2");
  anim.UpdateNodeDescription(n7c1, "n7c1");
  anim.UpdateNodeDescription(n7c2, "n7c2");
  anim.UpdateNodeDescription(n8c1, "n8c1");
  anim.UpdateNodeDescription(n8c2, "n8c2");
  anim.UpdateNodeDescription(n9c1, "n9c1");
  anim.UpdateNodeDescription(n10c1, "n10c1");
  anim.UpdateNodeDescription(n10c2, "n10c2");
  anim.UpdateNodeDescription(n11c1, "n11c1");
  anim.UpdateNodeDescription(n11c2, "n11c2");
  anim.UpdateNodeDescription(n12c1, "n12c1");
  anim.UpdateNodeDescription(n13c1, "n13c1");
  anim.UpdateNodeDescription(n13c2, "n13c2");
  anim.UpdateNodeDescription(n14c1, "n14c1");
  anim.UpdateNodeDescription(n14c2, "n14c2");
  anim.UpdateNodeDescription(n15c1, "n15c1");
  anim.UpdateNodeDescription(n15c2, "n15c2");
  anim.UpdateNodeDescription(n16c1, "n16c1");
  anim.UpdateNodeDescription(n16c2, "n16c2");

  // ----------------------------------------------------------------------
  // Now, run the actual simulation and generate Flow Monitor output
  // ----------------------------------------------------------------------
  //for(uint32_t i=0; i<NodeList::GetNNodes(); i++){
  	 // NS_LOG_INFO("Node Name: " + Names::FindName(NodeList::GetNode(i)) + ", Node ID: " + std::to_string(i));
  //  }

  NS_LOG_INFO ("Running Simulation for " << simDurationSeconds << " seconds...");

  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll();

  NS_LOG_INFO("HERE");

  Simulator::Stop (Seconds (simDurationSeconds));
  Simulator::Run ();

  NS_LOG_INFO("HERE2");

  std::string xmlName = "CAR_TRACK.xml";
  monitor->SerializeToXmlFile(xmlName, false, false );

  NS_LOG_INFO("HERE3");

  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");

}
