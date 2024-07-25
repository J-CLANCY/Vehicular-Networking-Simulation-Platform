# Vehicular Networking Simulation Platform

This repository was created as an attempt to create and use a vehicular networking simulation platform for my Ph.D. program. The COVID-19 pandemic began not long after I began the research programme, cutting off access to potential communications equipment and real-world testing. In conjunction with conducting a literature review, I went through several versions of this to create a platform for myself and my research group to conduct variable large-scale vehicular communications simulations. In particular, specific functionality to utilise fixed infrastructure sensor nodes was implemented. _Network Simulator 3 (NS-3)_ [1] and _Simulation of Urban Mobility (SUMO)_ [2] were chosen as a result of their open-source nature and the fact that the NS-3 community was the first to adopt the early 5G NR specifications from the _3rd Generation Partnership Project (3GPP)_ [3]. Ultimately, this simulation platform did not make it into my dissertation as a significant focus of my research surrounded the effects of different automotive environments on the wireless channel experienced by vehicles while travelling. Instead of wrestling with wireless channel models, as the pandemic ended I was able to acquire equipment which allowed me to conduct actual tests on real wireless channels. More information on those real-world tests can be found in [4],[5].

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

## References
[1] https://www.nsnam.org/
[2] https://eclipse.dev/sumo/
[3] https://www.3gpp.org/
[4] J. Clancy et al., "Feasibility Study of V2X Communications in Initial 5G NR Deployments," in IEEE Access, vol. 11, pp. 75269-75284, 2023, doi: 10.1109/ACCESS.2023.3296089.
[5} J. Clancy et al., "Investigating the Effect of Handover on Latency in Early 5G NR Deployments for C-V2X Network Planning," in IEEE Access, vol. 11, pp. 129124-129143, 2023, doi: 10.1109/ACCESS.2023.3334162.
