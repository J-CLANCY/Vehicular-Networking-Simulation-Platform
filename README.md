# Vehicular Networking Simulation Platform

This repo contains a poor attempt at creating a V2X network simulation for the CAR Group at University of Galway. The intent was to use the SUMO mobility simulator to generate mobility traces, which then fed into NS-3 for network simulation. 

NS-3 was chosen due to it's ability to connect to the outsite world if needed, given that a significant amount of the research completed in the CAR Group surround sensors and deep learning performance.

## Project Structure

```
├── ""Documentation"" => Contains documentation for this project.  
│    ├── ""Diagrams"" => Diagrams created for thesis document.  
│    ├── ""FYP_Thesis_Joseph_Clancy.pdf"" => Bachelor's thesis document.  
│    ├── ""RISC-V Compiler Installation Guide.docx"" => Guide written to use RISC-V gcc toolchain (Winter 2019).  
│    ├── ""RISC-V FYP.docx"" => Minor document with a few overview notes.  
│    ├── ""RISC-V User Guide"" => Minor document meant to accompany the course content and online content on Vicilogic.  
├── ""RISCV_Top/vhdl"" => Contains the VHDL source code and Xilinx Vivado project files.  
│    ├── ""HDLModel"" => Contains VHDL source code for the RISC-V Device-Under-Test (DUT) and testbench.  
│    ├── ""xilinxprj"" => Contains the Xilinx project files.  
├── ""Scripts"" => Contains a series of convenience scripts for using the RISC-V DUT.  
│    ├── ""assemble.sh"" => Runs after _compile.sh_ to assemble and link RISC-V hex output for use in the RISC-V DUT.  
│    ├── ""compile.sh"" => Runs the gcc compiler on a given C file.  
│    ├── ""hex_to_TB.py"" => Converts RISC-V toolchain hex output into a usable format for VHDL testbench.  
│    ├── ""hex_to_viciMacro.py"" => Converts RISC-V toolchain hex output into a usable format for the ViciLogic platform.  
│    ├── ""little_to_big.py"" => Converts from little to big-endian.  
```

## Project Overview

