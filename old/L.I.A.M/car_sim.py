#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Main coordination script for the NUI Galway CAR network simulations
"""
import argparse
import ipaddress
import subprocess

from Container import *
from car_sumo import main as sumo

__author__ = "Joseph Clancy"
__version__ = "1.0.0"
__maintainer__ = "Joseph Clancy"
__email__ = "J.CLANCY6@nuigalway.ie"
__status__ = "Development"

staticNodeContainers = []
mobileNodeContainers = []


def create_taps(config):
    """Setup of TAP interfaces
            """
    # Static Node TAPs
    for staticNode in config["staticNodes"]:
        r_code = subprocess.call(
            "sudo tunctl -t tap-{} && sudo ifconfig tap-{} 0.0.0.0 promisc up".format(staticNode, staticNode),
            stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    # Mobile Node TAPs
    for mobileNode in range(1, config["numMobileNodes"] + 1):
        r_code = subprocess.call(
            "sudo tunctl -t tap-{} && sudo ifconfig tap-{} 0.0.0.0 promisc up".format(mobileNode, mobileNode),
            stdout=subprocess.PIPE, shell=True, executable='/bin/bash')


def remove_taps(config):
    for staticNode in config["staticNodes"]:
        r_code = subprocess.call(
            "sudo ifconfig tap-{} down && sudo tunctl -d tap-{}".format(staticNode, staticNode),
            stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    # Mobile Node TAPs
    for mobileNode in range(1, config["numMobileNodes"] + 1):
        r_code = subprocess.call(
            "sudo ifconfig tap-{} down && sudo tunctl -d tap-{}".format(mobileNode, mobileNode),
            stdout=subprocess.PIPE, shell=True, executable='/bin/bash')


def create_bridges(config):
    """Setup of L2 Virtual Bridges
    """
    for staticNode in config["staticNodes"]:
        r_code = subprocess.call(
            "sudo brctl addbr br-{} && sudo brctl addif br-{} tap-{} && sudo ifconfig br-{} up"
                .format(staticNode, staticNode, staticNode, staticNode),
            stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    for mobileNode in range(1, config["numMobileNodes"] + 1):
        r_code = subprocess.call(
            "sudo brctl addbr br-{} && sudo brctl addif br-{} tap-{} && sudo ifconfig br-{} up"
                .format(mobileNode, mobileNode, mobileNode, mobileNode),
            stdout=subprocess.PIPE, shell=True, executable='/bin/bash')


def remove_bridges(config):
    for staticNode in config["staticNodes"]:
        r_code = subprocess.call(
            "sudo ifconfig br-{} down && sudo brctl delif br-{} tap-{} && sudo brctl delbr br-{}"
                .format(staticNode, staticNode, staticNode, staticNode),
            stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    for mobileNode in range(1, config["numMobileNodes"] + 1):
        r_code = subprocess.call(
            "sudo ifconfig br-{} down && sudo brctl delif br-{} tap-{} && sudo brctl delbr br-{}"
                .format(mobileNode, mobileNode, mobileNode, mobileNode),
            stdout=subprocess.PIPE, shell=True, executable='/bin/bash')


def start_cont(config, startIP):
    # Setup and Start Containers
    # Setup IPs and configs
    currentIP = ipaddress.ip_address(startIP)

    # Create static node containers
    for staticNode in config["staticNodes"]:
        staticNodeContainers.append(Container(staticNode, currentIP))
        currentIP += 1

    # Create mobile node containers
    for mobileNode in range(1, config["numMobileNodes"] + 1):
        mobileNodeContainers.append(Container(mobileNode, currentIP))
        currentIP += 1

    # Start static node containers
    for staticNode in staticNodeContainers:
        staticNode.start()

    # Start mobile node containers
    for mobileNode in mobileNodeContainers:
        mobileNode.start()

    return staticNodeContainers, mobileNodeContainers


def stop_cont(staticNodes, mobileNodes):
    # Stop static node containers
    for staticNode in staticNodes:
        staticNode.stop()

    # Stop mobile node containers
    for mobileNode in mobileNodes:
        mobileNode.stop()


def parse_config():
    """Parse config file
        """
    mobileConfigFile = open("car_track/ns2config.tcl", 'r')
    staticConfigFile = open("../../covt_staticNodes.csv", 'r')
    config = {
        "numMobileNodes": 0,
        "staticNodes": [],
        "simTime": 300
    }
    try:
        for line in mobileConfigFile:
            if line[1] == 'opt(nn) ':
                config["numMobileNodes"] = int(line[2])
            elif line[1] == 'opt(stop)':
                config["simTime"] = int(line[2])
    finally:
        mobileConfigFile.close()

    try:
        for line in mobileConfigFile:
            config["numStaticNodes"].append(line[0])
    finally:
        staticConfigFile.close()

    return config


def parse_args():
    """CLI parsing function
        """
    parser = argparse.ArgumentParser(prog="car_sim",
                                     description="NUI Galway CAR Network Simulation")

    parser.add_argument('-v', '--version', action='version', version=__version__)
    parser.add_argument('-V', '--verbose', action='store_true', default=False, help="Verbose output")
    parser.add_argument('-t', '--tracing', action='store_true', default=False, help="Enable tracing")
    parser.add_argument('-n', '--nogui', action='store_true', default=False, help="Run the commandline version of sumo")
    parser.add_argument('-i', '--ipaddr', action='store', default="192.168.0.1", help="Starting IP address")

    args = parser.parse_args()

    return args


def main(args):
    # Run SUMO mobility simulation first
    sumo(args)

    # Parse the NS-2 config file outputted from SUMO for running NS-3 simulation
    config = parse_config()

    # Setup the various virtual networking devices and the node containers
    create_taps(config)
    create_bridges(config)
    static, mobile = start_cont(config, args.ipaddr)

    # Run NS-3 simulation
    r_code = subprocess.call(
        "cd $NS3_HOME && sudo ./waf --run ''scratch/car_track' --verbose={} --tracing={} --simTime={}"
        " --mobileNodes={}'".format(args.verbose, args.trace, config["simTime"], config["numMobileNodes"]),
        shell=True, executable='/bin/bash')

    stop_cont(static, mobile)
    remove_bridges(config)
    remove_taps(config)


if __name__ == '__main__':
    # Parse CLI parameters
    args = parse_args()
    main(args)
