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
#include <vector>
#include <sstream>

#include "static-node.h"

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
#include "ns3/ns2-mobility-helper.h"
#include "ns3/nr-module.h"
#include "ns3/lte-module.h"
#include "ns3/point-to-point-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CarTrack");

// ----------------------------------------------------------------------
// Helper functions, constants etc.
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

int main (int argc, char *argv[])
{
  // Default configuration parameters
  bool        verbose            = true;
  bool        tracing            = true;
  int         simDurationSeconds = 5;
  int         numMobileNodes     = 1;
  std::string staticNodesConfig  = "../../../covt_staticNodes.csv";
  std::string mobilityFile       = "../../../car_track/ns2mobility.tcl";
  //std::string logFile            = "car_logs.txt";

  // Static-node type groups
  std::vector<StaticNode> staticNodes;
  MobilityHelper mobility;

  NodeContainer coreNodes;
  NodeContainer switches;
  NodeContainer sensorNodes;
  NodeContainer baseStations;
  NodeContainer cameras;

  NetDeviceContainer cameraDevices; 

  // ----------------------------------------------------------------------
  // Parse CLI for configuration parameters
  // ----------------------------------------------------------------------

  CommandLine cmd;
  cmd.AddValue ("verbose",        "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing",        "Enable pcap tracing",                   tracing);
  cmd.AddValue ("simTime",        "Simulation Duration in Seconds",        simDurationSeconds);
  cmd.AddValue ("numMobileNodes", "Number of Mobile Nodes",                numMobileNodes);

  cmd.Parse (argc,argv);

  if (verbose)
    {
      LogComponentEnable ("CarTrack", LOG_LEVEL_INFO);
    }

  // Binding these globals allows us to connect to the outside world
  GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));
  GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

  // ------------------------------------------------------------------
  // Create mobile nodes and install their mobility models first due
  // to the way NS-3 handles mobility trace files
  // ------------------------------------------------------------------

  NS_LOG_INFO ("Creating mobile nodes...");

  NodeContainer mobileNodes;
  mobileNodes.Create(numMobileNodes);  

  Ns2MobilityHelper ns2 = Ns2MobilityHelper (mobilityFile);
  ns2.Install ();

  // ---------------------------------------------------------------------
  // Create Ethernet links to use for connecting static nodes together
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
  // Create and organise the static nodes in the network i.e. server, switches, cameras, masts etc.
  // ----------------------------------------------------------------------

  NS_LOG_INFO ("Creating permenant nodes...");        
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

  NS_LOG_INFO ("Adding permenant nodes to name registry...");

  Names::Add ("bck", bck);
  Names::Add ("dc",  dc);

  Names::Add ("eng", eng);
  Names::Add ("pre", pre);
  Names::Add ("cai", cai);
  Names::Add ("bio", bio);
  Names::Add ("ila", ila);
  Names::Add ("hut", hut);

  NS_LOG_INFO ("Connecting permenant nodes together using Ethernet links...");

  // DATACENTRE-BACKBONE
  NetDeviceContainer link_dc_bck = Eth10G.Install (NodeContainer (dc, bck));

  // BUILDINGS-BACKBONE
  NetDeviceContainer link_eng_bck = Eth10G.Install (NodeContainer (eng, bck));
  NetDeviceContainer link_cai_bck = Eth10G.Install (NodeContainer (cai, bck));
  NetDeviceContainer link_pre_cai = Eth10G.Install (NodeContainer (pre, cai));
  NetDeviceContainer link_bio_bck = Eth10G.Install (NodeContainer (bio, bck));
  NetDeviceContainer link_ila_bio = Eth10G.Install (NodeContainer (ila, bio));
  NetDeviceContainer link_hut_cai = Eth10G.Install (NodeContainer (hut, cai));

  NS_LOG_INFO ("Creating NetDevices for permenant switches...");

  NetDeviceContainer bckND;
  bckND.Add (link_dc_bck.Get  (1));
  bckND.Add (link_eng_bck.Get (1));
  bckND.Add (link_cai_bck.Get (1));
  bckND.Add (link_bio_bck.Get (1));

  NetDeviceContainer engND;
  engND.Add (link_eng_bck.Get (0));

  NetDeviceContainer caiND;
  caiND.Add (link_cai_bck.Get (0));
  caiND.Add (link_pre_cai. Get(1));
  caiND.Add (link_hut_cai. Get(1));

  NetDeviceContainer preND;
  preND.Add (link_pre_cai.Get (0));

  NetDeviceContainer bioND;
  bioND.Add (link_bio_bck.Get (0));
  bioND.Add (link_ila_bio.Get (1));

  NetDeviceContainer ilaND;
  ilaND.Add (link_ila_bio.Get (0));

  NetDeviceContainer hutND;
  hutND.Add (link_hut_cai.Get (0));

  NS_LOG_INFO ("Creating static nodes from configuration file...");

  std::vector<std::string> row;
  std::string line, word;

  std::ifstream staticNodesFile(staticNodesConfig);

  while (staticNodesFile >> line){
    std::stringstream ss(line);
    if(line.compare("Name,Type,Latitude,Longitude,Altitude,Neighbours") == 0){
      NS_LOG_INFO("GOT HEADER");
      continue;
    }
    else{
      row.clear();
	    while(std::getline(ss, word, ',')){
        row.push_back(word);
       }
      staticNodes.push_back(StaticNode(row));
    }
  }

  NS_LOG_INFO ("Organise and further fill out static node groupings...");

  Ptr<ListPositionAllocator> posAlloc = CreateObject<ListPositionAllocator> ();
  Ptr<Node> newNode;
  Ptr<Node> neighbour; 
  Ptr<Node> c1;
  Ptr<Node> c2; 

  for(auto node: staticNodes){
    NS_LOG_INFO("Node: " + node.toString());
    if(Names::Find<Node>(node.name) == 0){
      newNode = CreateObject<Node> ();
      Names::Add(node.name, newNode);
    }
    else{
      newNode = Names::Find<Node>(node.name);
    }
    posAlloc->Add(Vector(node.x,node.y,node.z));

    if (node.type == "CN"){
      coreNodes.Add(newNode);
    }
    else if (node.type == "BS"){
      baseStations.Add(newNode);
    }
    else if (node.type == "SW"){
      switches.Add(newNode);
    }
    else if (node.type == "SN"){
      sensorNodes.Add(newNode);

      c1 = CreateObject<Node> ();
      c2 = CreateObject<Node> ();
      Names::Add(node.name + "c1", c1);
      Names::Add(node.name + "c2", c2);

      cameras.Add(c1);
      cameras.Add(c2);

      NetDeviceContainer link_node_c1 = Eth1G.Install (NodeContainer (newNode, c1));
      NetDeviceContainer link_node_c2 = Eth1G.Install (NodeContainer (newNode, c2));
      cameraDevices.Add (link_node_c1.Get(1));
      cameraDevices.Add (link_node_c2.Get(1));
      node.addDevice(link_node_c1.Get(0));
      node.addDevice(link_node_c2.Get(0));

      if(Names::Find<Node>(node.neighbour) == 0){
        neighbour = CreateObject<Node> ();
        Names::Add(node.neighbour, neighbour);
      }
      else{
        neighbour = Names::Find<Node>(node.neighbour);
      }

      NetDeviceContainer link_node_neigh = Eth10G.Install (NodeContainer (newNode, neighbour));
      node.addDevice(link_node_neigh.Get(0));

      for(auto neigh: staticNodes){
        if(neigh.name == node.neighbour){
          neigh.addDevice(link_node_neigh.Get(1));
        }
      }

    }
    else{
      NS_LOG_ERROR("Node: " + node.toString() + " is not a valid node.");
    }
  }

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (posAlloc);
  mobility.InstallAll();

  // ----------------------------------------------------------------------
  // Install bridging code on each switch
  // ----------------------------------------------------------------------

  NS_LOG_INFO ("Installing bridging code on each switch...");

  BridgeHelper bridge;

  for(auto node: staticNodes){
    if (node.type == "SN" || node.type == "SW"){
      bridge.Install (Names::Find<Node>(node.name), node.devices);
    }
  }

  // ---------------------------------------------------------------------
  // Create wireless links to use for connecting mobile nodes to network
  // ----------------------------------------------------------------------

  NS_LOG_INFO ("Creating cellular network...");

  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  NetDeviceContainer enbDevs;
  enbDevs = lteHelper->InstallEnbDevice (baseStations);
  

  // ------------------------------------------------------------------
  // Connect mobile nodes to the cellular network
  // ------------------------------------------------------------------

  NS_LOG_INFO ("Connecting mobile nodes...");

  NetDeviceContainer ueDevs;
  ueDevs = lteHelper->InstallUeDevice (mobileNodes);

  lteHelper->AttachToClosestEnb(ueDevs, enbDevs);

  // Activate a data radio bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lteHelper->ActivateDataRadioBearer (ueDevs, bearer);
  lteHelper->EnableTraces ();

  /*Ptr<NrPointToPointEpcHelper> epcHelper = CreateObject<NrPointToPointEpcHelper> ();
  Ptr<IdealBeamformingHelper> idealBeamformingHelper = CreateObject<IdealBeamformingHelper>();
  Ptr<NrHelper> nrHelper = CreateObject<NrHelper> ();

  // Put the pointers inside nrHelper
  nrHelper->SetBeamformingHelper (idealBeamformingHelper);
  nrHelper->SetEpcHelper (epcHelper);

  Ptr<Node> pgw = epcHelper->GetPgwNode ();

  NS_LOG_INFO ("Connecting cellular network to rest of the system...");

  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (2500));
  p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.000)));
  NetDeviceContainer link_pgw_bck = p2ph.Install (pgw, bck);*/

  // ------------------------------------------------------------------
  // Install Internet stack on devices
  // ------------------------------------------------------------------

  //NS_LOG_INFO ("Installing Internet stack on devices...");

  // ------------------------------------------------------------------
  // Install Tap Bridges on relevant devices
  // ------------------------------------------------------------------

  //NS_LOG_INFO ("Configuring TAP Bridges...");

  // ----------------------------------------------------------------------
  // Configure PCAP traces
  // ----------------------------------------------------------------------

  //NS_LOG_INFO ("Configuring PCAP Tracing...");

  // ----------------------------------------------------------------------
  // Output to animator file
  // ----------------------------------------------------------------------
  //NS_LOG_INFO ("Configuring animation file...");

  // ----------------------------------------------------------------------
  // Now, run the actual simulation and generate Flow Monitor output
  // ----------------------------------------------------------------------

  NS_LOG_INFO ("Running Simulation for " << simDurationSeconds << " seconds...");

  //FlowMonitorHelper flowmon;
  //Ptr<FlowMonitor> monitor = flowmon.InstallAll();

  Simulator::Stop (Seconds (simDurationSeconds));
  Simulator::Run ();

  //std::string xmlName = "CAR_TRACK.xml";
  //monitor->SerializeToXmlFile(xmlName, false, false );

  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
