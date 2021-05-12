#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Helper script for development activities on NUI Galway CAR network simulations

Based on the nuances of housemate during COVID-19 pandemic
"""

import argparse
import subprocess

from car_sim import main as sim

__author__ = "Joseph Clancy"
__version__ = "1.3.0"
__maintainer__ = "Joseph Clancy"
__email__ = "J.CLANCY6@nuigalway.ie"
__status__ = "Development"


def main():
    """Essentially just one large command line parsing function
    """

    print("Hello-seph!")

    # Setup CLI parser
    parser = argparse.ArgumentParser(prog="liam",
                                     description="Liam In A Machine - L.I.A.M")

    # Enable sub-parsers
    subparsers = parser.add_subparsers(title="Commands",
                                       dest="command")

    # Setup parser for run command
    parser_run = subparsers.add_parser("run", help="Run a specific program/application")
    parser_run.add_argument("program", action='store', help="The program/application to run e.g. eclipse, netanim")
    parser_run.set_defaults(func=run)

    # Setup parser for carsim command
    parser_car_sim = subparsers.add_parser("carsim", help="Run the NUI Galway CAR NS-3 simulation")
    parser_car_sim.set_defaults(func=car_sim)

    # Setup parser for save command
    parser_save = subparsers.add_parser("save", help="Backup important files to github repo.")
    parser_save.set_defaults(func=save)

    # Joke
    parser_save = subparsers.add_parser("howareyou", help="How are you?")
    parser_save.set_defaults(func=how)

    parser.add_argument('-v', '--version', action='version', version=__version__)
    args = parser.parse_args()
    args.func(args)


def run(args):
    """Parsing the arguments from the run command.

    Using subprocess library to run linux commands in the shell that called this helper script, very handy
    """
    print("Ah yes, the run command is being executed, as it should be!")

    if args.program == "eclipse":
        print("Running our finest IDE ...")
        r_code = subprocess.call("cd $HOME/eclipse/cpp-2019-12/eclipse && ./eclipse &> /dev/null & disown",
                                 shell=True, executable='/bin/bash')
    elif args.program == "netanim":
        print("Running Netanim...")
        r_code = subprocess.call("cd $HOME/ns-3/source/netanim-3.108 && ./NetAnim &> /dev/null & disown",
                                 shell=True, executable='/bin/bash')
    elif args.program == "pycharm":
        print("Running PyCharm...")
        r_code = subprocess.call("cd $HOME/pycharm-community-2019.3.4/bin && ./pycharm.sh &> /dev/null & disown",
                                 shell=True, executable='/bin/bash')


def car_sim(args):
    """Calls the car_sim.py script that contains the network simulation management code
    """
    print("Ah yes, the carsim command is being executed, as it should be!")

    # See imports
    sim(args)


def save(args):
    """Takes care of collecting all the important files etc. from around the workspace and saving them to the github
    repo.
    """
    print("Ah yes, the save command is being executed, as it should be!")

    r_code = subprocess.call(
        "cd ~/nuigcar && sudo git commit -u && sudo git push",
        shell=True, executable='/bin/bash')


def how(args):
    """I'm not going insane I swear.
    """
    print("I am simply excellent.")


if __name__ == '__main__':
    main()
