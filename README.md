# Vehicular Networking Simulation Platform

This repo contains a poor attempt at creating a V2X network simulation for the CAR Group at University of Galway. The intent was to use the SUMO mobility simulator to generate mobility traces, which then fed into NS-3 for network simulation. 

NS-3 was chosen due to it's ability to connect to the outsite world if needed, given that a significant amount of the research completed in the CAR Group surround sensors and deep learning performance.

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
│    ├── ""ns-3-code"" => 
│    ├── ""python-code"" => Runs after _compile.sh_ to assemble and link RISC-V hex output for use in the RISC-V DUT.  
│    ├── ""route"" => Runs the gcc compiler on a given C file.
│    ├── ""helper.py" => Runs after _compile.sh_ to assemble and link RISC-V hex output for use in the RISC-V DUT.  
├── "".gitmodules"" => List of github submodiles i.e. NS-3, SUMO etc.  
```

## Project Overview

