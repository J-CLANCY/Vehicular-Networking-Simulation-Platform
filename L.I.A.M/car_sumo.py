#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Script that handles the SUMO simulation of the NUI Galway CAR network simulation

Essentially runs the simulation and collects the positions of the relevant vehicles
"""

__author__ = "Joseph Clancy"
__version__ = "1.0.0"
__maintainer__ = "Joseph Clancy"
__email__ = "J.CLANCY6@nuigalway.ie"
__status__ = "Development"

import os
import sys
import optparse
import subprocess

# Need to import python modules from the $SUMO_HOME/tools directory, this is the check
if 'SUMO_HOME' in os.environ:
    tools = os.path.join(os.environ['SUMO_HOME'], 'tools')
    sys.path.append(tools)
else:
    sys.exit("please declare environment variable 'SUMO_HOME'")

from sumolib import checkBinary
import traci


def run():
    """Execute the TraCI control loop

    Extracts the coordinates of the vehicles in question every time step - not really used right now
    """
    print("Running SUMO...")

    while "auto_car" not in traci.simulation.getArrivedIDList():
        # for veh_id in traci.vehicle.getIDList():
        #    if veh_id == "auto_car":
        #        x, y = traci.vehicle.getPosition(veh_id)
        #        lon, lat = traci.simulation.convertGeo(x, y)
        traci.simulationStep()
    traci.close()


def convert_static_coords():
    """Pre-processing of the static nodes in the network simulation.

    Uses Sumo/TracI facility to do this because we want the coordinates of the static nodes to be in the same
    coordinate system as the mobile nodes

    'static_nodes_latlng' contains the real lat/lng coordinates of the static nodes
    """
    print("Converting static node coordinates...")

    with open("../../staticNodes.csv", "r") as static_latlng:
        with open("../../covt_staticNodes.csv", "w") as static_cart:
            for line in static_latlng:
                if line != "Name,Type,Latitude,Longitude,Altitude,Neighbours":
                    line = line.rsplit(",")

                    x, y = traci.simulation.convertGeo(float(line[3]), float(line[2]), fromGeo=True)
                    new_line = line[0] + "," + line[1] + "," + str(x) + "," + str(y) + "," + line[4] + "," + line[5]

                    print(new_line)
                    static_cart.write(new_line + "\n")


def get_options():
    """Options parser
    """
    opt_parser = optparse.OptionParser()
    opt_parser.add_option("--nogui", action="store_true",
                          default=False, help="run the commandline version of sumo")
    opt, args = opt_parser.parse_args()

    return opt


def main(args):
    """This script has been called from the command line.

        It will start sumo as a server, then connect and run
        """
    if args.nogui:
        sumoBinary = checkBinary('sumo')
    else:
        sumoBinary = checkBinary('sumo-gui')

    # This is the normal way of using TraCI
    # Sumo is started as a subprocess and then the python script connects and runs
    # osm.sumocfg contains references to all relevant Sumo files
    # Output files 'tripinfo.xml', 'sumoTrace.xml' give useful information on the previously run simulation
    traci.start([sumoBinary, "-c", "$HOME/nuigcar/car_track/osm.sumocfg",
                 "--tripinfo-output", "$HOME/nuigcar/car_track/tripinfo.xml",
                 "--fcd-output", "$HOME/nuigcar/car_track/sumoTrace.xml"])

    # Pre-processing of static node coordinates for the simulation
    convert_static_coords()

    # Actual running of the Sumo simulation
    run()

    # Post-processing of the data collected
    r_code = subprocess.call("cd ~/nuigcar && traceExporter.py --fcd-input sumoTrace.xml "
                             "--ns2activity-output ns2act.tcl "
                             "--ns2config-output ns2config.tcl --ns2mobility-output ns2mobility.tcl",
                             shell=True, executable='/bin/bash')


if __name__ == "__main__":
    options = get_options()
    main(options)
