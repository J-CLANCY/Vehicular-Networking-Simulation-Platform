#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Main coordination script for the NUI Galway CAR network simulations
"""

import os
import subprocess

__author__ = "Joseph Clancy"
__version__ = "1.0.0"
__maintainer__ = "Joseph Clancy"
__email__ = "J.CLANCY6@nuigalway.ie"
__status__ = "Development"


def main():
    print("NUI Galway CAR Network Simulation")


def test(args):
    print("Setting up the simulation to run with nodes: {}".format(args.nodes))

    # Setup TAP interfaces
    # Setup the data centre node TAP

    r_code = subprocess.call("sudo tunctl -t tap-dc && sudo ifconfig tap-dc 0.0.0.0 promisc up",
                             stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    # Setup the car node TAP
    r_code = subprocess.call("sudo tunctl -t tap-car && sudo ifconfig tap-car 0.0.0.0 promisc up",
                             stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    # If user specifies certain nodes to be used, setup their TAPs, otherwise, setup up TAPs for every node
    if args.nodes:
        for node in args.nodes:
            r_code = subprocess.call(
                "sudo tunctl -t tap-{} && sudo ifconfig tap-{} 0.0.0.0 promisc up".format(node, node),
                stdout=subprocess.PIPE, shell=True, executable='/bin/bash')
    else:
        for i in range(1, 17):
            r_code = subprocess.call(
                "sudo tunctl -t tap-n{}c1 && sudo ifconfig tap-n{}c1 0.0.0.0 promisc up".format(i, i),
                stdout=subprocess.PIPE, shell=True, executable='/bin/bash')
            if not i == 9 or not i == 12:
                r_code = subprocess.call(
                    "sudo tunctl -t tap-n{}c2 && sudo ifconfig tap-n{}c2 0.0.0.0 promisc up".format(i, i),
                    stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    # Setup L2 virtual bridges
    # Setup the data centre node V-Bridge
    r_code = subprocess.call("sudo brctl addbr br-dc && sudo brctl addif br-dc tap-dc && sudo ifconfig br-dc up",
                             stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    # Setup the car node V-Bridge
    r_code = subprocess.call("sudo brctl addbr br-car && sudo brctl addif br-car tap-car && sudo ifconfig br-car up",
                             stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    # As with TAPs, setup requested V-Bridges, otherwise, setup all
    if args.nodes:
        for node in args.nodes:
            r_code = subprocess.call(
                "sudo brctl addbr br-{} && sudo brctl addif br-{} tap-{} && sudo ifconfig br-{} up".format(node, node,
                                                                                                           node, node),
                stdout=subprocess.PIPE, shell=True, executable='/bin/bash')
    else:
        for i in range(1, 17):
            r_code = subprocess.call(
                "sudo brctl addbr br-n{}c1 && sudo brctl addif br-n{}c1 tap-n{}c1 && sudo ifconfig br-n{}c1 up".format(
                    i, i, i, i),
                stdout=subprocess.PIPE, shell=True, executable='/bin/bash')
            if not i == 9 or not i == 12:
                r_code = subprocess.call(
                    "sudo brctl addbr br-n{}c2 && sudo brctl addif br-n{}c2 tap-n{}c2 && sudo ifconfig br-n{}c2 up".format(
                        i, i, i, i),
                    stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    # Setup and Start Containers
    # Setup IPs and configs
    subnet = "192.168.1.0/24"
    if args.network:
        subnet = args.network

    prefix = subnet[:-4]

    # Setup data centre container config
    dc_conf = open(os.environ["NS3_HOME"] + "/../cont_conf/lxc-dc.conf", "w")
    dc_conf.write("lxc.net.0.type = veth\n")
    dc_conf.write("lxc.net.0.flags = up\n")
    dc_conf.write("lxc.net.0.link = br-dc\n")
    dc_conf.write("lxc.net.0.ipv4.address = {}1/24\n".format(prefix))
    dc_conf.write("lxc.net.0.veth.pair = dc-veth\n")
    dc_conf.close()

    # Setup car container config
    car_conf = open(os.environ["NS3_HOME"] + "/../cont_conf/lxc-car.conf", "w")
    car_conf.write("lxc.net.0.type = veth\n")
    car_conf.write("lxc.net.0.flags = up\n")
    car_conf.write("lxc.net.0.link = br-car\n")
    car_conf.write("lxc.net.0.ipv4.address = {}1/24\n".format(prefix))
    car_conf.write("lxc.net.0.veth.pair = car-veth\n")
    car_conf.close()

    # Setup selected or all node container configs
    contCount = 2
    if args.nodes:
        for node in args.nodes:
            conf = open(os.environ["NS3_HOME"] + "/../cont_conf/lxc-{}.conf".format(node), "w")
            conf.write("lxc.net.0.type = veth\n")
            conf.write("lxc.net.0.flags = up\n")
            conf.write("lxc.net.0.link = br-{}\n".format(node))
            conf.write("lxc.net.0.ipv4.address = {}{}/24\n".format(prefix, contCount))
            conf.write("lxc.net.0.veth.pair = {}-veth\n".format(node))
            conf.close()
            contCount += 1

    else:
        for i in range(1, 17):
            conf = open(os.environ["NS3_HOME"] + "/../cont_conf/lxc-n{}c1.conf".format(i), "w")
            conf.write("lxc.net.0.type = veth\n")
            conf.write("lxc.net.0.flags = up\n")
            conf.write("lxc.net.0.link = br-n{}c1\n".format(i))
            conf.write("lxc.net.0.ipv4.address = {}{}/24\n".format(prefix, contCount))
            conf.write("lxc.net.0.veth.pair = n{}c1-veth\n".format(i))
            conf.close()
            contCount += 1

            if not i == 9 or not i == 12:
                conf = open(os.environ["NS3_HOME"] + "/../cont_conf/lxc-n{}c2.conf".format(i), "w")
                conf.write("lxc.net.0.type = veth\n")
                conf.write("lxc.net.0.flags = up\n")
                conf.write("lxc.net.0.link = br-n{}c2\n".format(i))
                conf.write("lxc.net.0.ipv4.address = {}{}/24\n".format(prefix, contCount))
                conf.write("lxc.net.0.veth.pair = n{}c2-veth\n".format(i))
                conf.close()
                contCount += 1

    # Create data centre container
    r_code = subprocess.call(
        "cd $NS3_HOME/../cont_conf && sudo lxc-create -f lxc-dc.conf -t download -n dc -- -d fedora -r 31 -a amd64",
        stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    # Create car container
    r_code = subprocess.call(
        "cd $NS3_HOME/../cont_conf && sudo lxc-create -f lxc-car.conf -t download -n car -- -d fedora -r 31 -a amd64",
        stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    # Create remaining containers
    if args.nodes:
        for node in args.nodes:
            r_code = subprocess.call(
                "cd $NS3_HOME/../cont_conf && sudo lxc-create -f lxc-{}.conf -t download -n {} -- -d fedora -r 31 -a amd64".format(
                    node, node),
                stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    else:
        for i in range(1, 17):
            r_code = subprocess.call(
                "cd $NS3_HOME/../cont_conf && sudo lxc-create -f lxc-n{}c1.conf -t download -n n{}c1 -- -d fedora -r 31 "
                "-a amd64".format(
                    i, i),
                stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

            if not i == 9 or not i == 12:
                r_code = subprocess.call(
                    "cd $NS3_HOME/../cont_conf && sudo lxc-create -f lxc-n{}c2.conf -t download -n n{}c2 -- -d fedora -r "
                    "31 -a amd64".format(
                        i, i),
                    stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    # Start Containers
    r_code = subprocess.call(
        "sudo lxc-start -n dc".format(node),
        stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    r_code = subprocess.call(
        "sudo lxc-start -n car".format(node),
        stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    if args.nodes:
        for node in args.nodes:
            r_code = subprocess.call(
                "sudo lxc-start -n {}".format(node),
                stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    else:
        for i in range(1, 17):
            r_code = subprocess.call(
                "sudo lxc-start -n n{}c1".format(i),
                stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

            if not i == 9 or not i == 12:
                r_code = subprocess.call(
                    "sudo lxc-start -n n{}c2".format(i),
                    stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    # Create a file with list of nodes to use if selected, so the C++ code can interpret them
    useNodes = False
    if args.nodes:
        useNodes = True
        nodeFile = open(os.environ["NS3_HOME"] + "/nodesToUse.txt", "w")
        for node in args.nodes:
            nodeFile.write(node)
        nodeFile.close()

    simTime = 300
    if args.time:
        simTime = args.time

    # Run NS-3 simulation
    r_code = subprocess.call(
        "cd $NS3_HOME && sudo ./waf --run ''scratch/car_track' --verbose={} --tracing={} --simTime={}"
        " --nodes={}'".format(args.verbose, args.trace, simTime, useNodes),
        shell=True, executable='/bin/bash')

    # Tear everything down after the simulation
    # Stop Containers
    r_code = subprocess.call(
        "sudo lxc-stop -n dc".format(node),
        stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    r_code = subprocess.call(
        "sudo lxc-stop -n car".format(node),
        stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    if args.nodes:
        for node in args.nodes:
            r_code = subprocess.call(
                "sudo lxc-stop -n {}".format(node),
                stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    else:
        for i in range(1, 17):
            r_code = subprocess.call(
                "sudo lxc-stop -n n{}c1".format(i),
                stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

            if not i == 9 or not i == 12:
                r_code = subprocess.call(
                    "sudo lxc-stop -n n{}c2".format(i),
                    stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    # Tear down L2 virtual bridges
    # Tear down the data centre node V-Bridge
    r_code = subprocess.call("sudo ifconfig br-dc down && sudo brctl delif br-dc tap-dc && sudo brctl delbr br-dc",
                             stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    # Tear down the car node V-Bridge
    r_code = subprocess.call("sudo ifconfig br-car down && sudo brctl delif br-car tap-car && sudo brctl delbr br-car",
                             stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    # Tear down remaining bridges
    if args.nodes:
        for node in args.nodes:
            r_code = subprocess.call(
                "sudo ifconfig br-{} down && sudo brctl delif br-{} tap-{} && sudo brctl delbr br-{}".format(node, node,
                                                                                                             node,
                                                                                                             node),
                stdout=subprocess.PIPE, shell=True, executable='/bin/bash')
    else:
        for i in range(1, 17):
            r_code = subprocess.call(
                "sudo ifconfig br-n{}c1 down && sudo brctl delif br-n{}c1 tap-n{}c1 && sudo brctl delbr br-n{}c1".format(
                    i, i, i, i),
                stdout=subprocess.PIPE, shell=True, executable='/bin/bash')
            if not i == 9 or not i == 12:
                r_code = subprocess.call(
                    "sudo ifconfig br-n{}c2 down && sudo brctl delif br-n{}c2 tap-n{}c2 && sudo brctl delbr br-n{}c2".format(
                        i, i, i, i),
                    stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    # Tear down TAP interfaces
    # Tear down the data centre node TAP
    r_code = subprocess.call("sudo ifconfig tap-dc down && sudo tunctl -d tap-dc",
                             stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    r_code = subprocess.call("sudo ifconfig tap-car down && sudo tunctl -d tap-car",
                             stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    # Tear down remaining TAPs
    if args.nodes:
        for node in args.nodes:
            r_code = subprocess.call(
                "sudo ifconfig tap-{} down && sudo tunctl -d tap-d{}".format(node, node),
                stdout=subprocess.PIPE, shell=True, executable='/bin/bash')
    else:
        for i in range(1, 17):
            r_code = subprocess.call(
                "sudo ifconfig tap-n{}c1 down && sudo tunctl -d tap-n{}c1".format(i, i),
                stdout=subprocess.PIPE, shell=True, executable='/bin/bash')
            if not i == 9 or not i == 12:
                r_code = subprocess.call(
                    "sudo ifconfig tap-n{}c2 down && sudo tunctl -d tap-n{}c2".format(i, i),
                    stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    # Destroy Containers
    r_code = subprocess.call(
        "sudo lxc-destroy -n dc".format(node),
        stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    r_code = subprocess.call(
        "sudo lxc-destroy -n car".format(node),
        stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    if args.nodes:
        for node in args.nodes:
            r_code = subprocess.call(
                "sudo lxc-destroy -n {}".format(node),
                stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

    else:
        for i in range(1, 17):
            r_code = subprocess.call(
                "sudo lxc-destroy -n n{}c1".format(i),
                stdout=subprocess.PIPE, shell=True, executable='/bin/bash')

            if not i == 9 or not i == 12:
                r_code = subprocess.call(
                    "sudo lxc-destroy -n n{}c2".format(i),
                    stdout=subprocess.PIPE, shell=True, executable='/bin/bash')


if __name__ == '__main__':
    main()
