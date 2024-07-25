# Vehicular Networking Simulation Platform

This repository was created as an attempt to create and use a vehicular networking simulation platform for my Ph.D. program. The COVID-19 pandemic began not long after I began the research programme, cutting off access to potential communications equipment and real-world testing. In conjunction with conducting a literature review, I went through several versions of this to create a platform for myself and my research group to conduct variable large-scale vehicular communications simulations. In particular, specific functionality to utilise fixed infrastructure sensor nodes was implemented, this is known as _Vehicle-to-Infrastructure (V2I)_ communications. _Network Simulator 3 (NS-3)_ [1] and _Simulation of Urban Mobility (SUMO)_ [2] were chosen as a result of their open-source nature and the fact that the NS-3 community was the first to adopt the early 5G NR specifications from the _3rd Generation Partnership Project (3GPP)_ [3]. Ultimately, this simulation platform did not make it into my dissertation as a significant focus of my research surrounded the effects of different automotive environments on the wireless channel experienced by vehicles while travelling. Instead of wrestling with wireless channel models, as the pandemic ended I was able to acquire equipment which allowed me to conduct actual tests on real wireless channels. More information on those real-world tests can be found in [4],[5].

## Project Structure

```
├── "".idea"" => Config from PyCharm  
├── ""old"" => Old code that I wanted to keep
│    ├── ""L.I.A.M"" => A series of helper scripts to run the simulation platform. I was losing my mind during the pandemic and named it after my housemate at the time.
│    ├── ""osm_car_track"" => Contains the files needed for the University of Galway campus route in SUMO.
│    ├── ""osm_car_track"" => NS-3 C++ code for the old version of the vehicular communications system based on the University of Galway campus route.
├── ""sims"" => Where the git submodules for NS-3 and SUMO are kept.
│    ├── ""ns-3"" => Runs after _compile.sh_ to assemble and link RISC-V hex output for use in the RISC-V DUT.  
│    ├── ""sumo"" => Runs the gcc compiler on a given C file.  
├── ""src"" => My code for the vehicular communications simulation platform
│    ├── ""config"" => Config files for the simulation platform
│    ├── ""ns-3-code"" => Contains the NS-3 C++ for the vehicular communications system that is based around the University of Galway campus route.
│    ├── ""python-code"" => Contains some of the Python code necessary to handle the simulators and the LXC/LXD containers.
│    ├── ""route"" => Contains the files necessary to describe the map and route around the University of Galway campus.
│    ├── ""helper.py" => Helper script for using the simulation platform.
├── "".gitmodules"" => List of github submodiles i.e. NS-3, SUMO etc.  
```

## Project Overview

This vehicular networking simulation platform was designed with two major goals in mind:
-	Only the network connections were simulated, and real-world data should be used to test the network.
-	The platform should allow for either simulated traffic mobility or real-world GPS traces of traffic mobility.

As previously mentioned, the two open-source simulation tools were chosen for this platform. NS-3 was chosen because, at the time, the Centre Tecnològic de Telecomunicacions de Catalunya (CTTC) had developed the most up-to-date 5G NR simulation tool, known as 5G-LENA [6], an extension of NS-3. SUMO was chosen because it was the only other open-source mobility simulator at the time (or at least that I could find). In the initial 9 months of this project, a preliminary simulation platform was developed, see Figure 1. This platform was hosted on a virtual Linux environment with VirtualBox, where Linux (LXC/LXD) container technology is used to provide the functionality allowing a connection point between the real world and the simulation. This connection point leverages Linux’s TAP interface to enable the simulation to code to send and receive data from the host machine's networking firmware. It should be noted that Docker was considered for the container functionality, however, the networking capabilities of Docker are limited and where not suitable for this application.

![Figure 1](/images/VirtualTestPlatform.png)
Figure 1 - Vehicular Networking Simulation Platform

The process of running a test via this simulation platform can be seen in Figure 2. The desired scenario is initially defined by:
-	The routes to be taken by relevant traffic e.g., vehicles, pedestrians.
-	The placement of static nodes in the network e.g., servers, base stations, fixed sensor nodes.
-	The data to be transmitted.

![Figure 2](/images/SimulationProcess.png)
Figure 2 - Simulation Platform Execution Flowchart

If the mobility traces of traffic for the desired scenario have been organically collected, then the SUMO simulation can be skipped. However, if the mobility traces of traffic require simulation, the SUMO simulation is then run using the road network of the NUI Galway test route, see Figure 3. All the GPS coordinates of the relevant infrastructure and road users, defined as parameters, are taken and converted to a local cartesian coordinate system, followed by the generation of the mobility traces for every relevant traffic entity in the scenario. Following either the skipping or the completion of the SUMO simulation, the relevant Linux virtual networking facilities instantiated based on the configured simulation parameters. With the necessary facilities and inputs set and collected, the NS-3 simulation is run. 

![Figure 3](/images/UofGalwayTestRoute)
Figure 3 - University of Galway Campus Route

Within the NS-3 simulation, the topology of the preliminary communications network is created (connecting optic fibres and setting up the wireless network), and the relevant nodes are connected to the outside world via their TAP interfaces. Each mobile stakeholder is given a starting position defined by their mobility traces and are then connected wirelessly to their nearest base station. The NS-3 simulation then applies the mobility models provided to the traffic entities and carries the data streams sent from the real world. The outputs from this simulation are PCAP files, this they can then be analysed by various packet tracing tools.

This simulation platform is limited in a few ways:
-	The characteristics of the wireless channel are not accurate to real communications links.
-	There was no way for the behaviours of the communications network to influence the mobility of the traffic entities e.g. vehicles change their direction based on communications data.

These limitations slowed the development of this simulation platform sufficiently until the pandemic had ended, at which point communications equipment was finally acquired for real-world testing.

## References
[1] https://www.nsnam.org/
[2] https://eclipse.dev/sumo/
[3] https://www.3gpp.org/
[4] J. Clancy et al., "Feasibility Study of V2X Communications in Initial 5G NR Deployments," in IEEE Access, vol. 11, pp. 75269-75284, 2023, doi: 10.1109/ACCESS.2023.3296089.
[5} J. Clancy et al., "Investigating the Effect of Handover on Latency in Early 5G NR Deployments for C-V2X Network Planning," in IEEE Access, vol. 11, pp. 129124-129143, 2023, doi: 10.1109/ACCESS.2023.3334162.
[6] https://5g-lena.cttc.es/
