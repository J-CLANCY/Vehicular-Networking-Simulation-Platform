import os
import sys
import lxc


class Container:
    dist_config = {
        "dist": "fedora",
        "release": "33",
        "arch": "amd64"
    }

    def __init__(self, name, ip):
        configPath = os.environ["HOME"] + "nuigcar/cont_conf/lxc-{}.conf".format(name)

        configFile = open(configPath, "w")
        configFile.write("lxc.net.0.type = veth\n")
        configFile.write("lxc.net.0.flags = up\n")
        configFile.write("lxc.net.0.link = br-{}\n".format(name))
        configFile.write("lxc.net.0.ipv4.address = {}/24\n".format(ip))
        configFile.write("lxc.net.0.veth.pair = {}-veth\n".format(name))
        configFile.close()

        self.container = lxc.Container(name, configPath)

    def start(self):
        if not self.container.defined:
            self.create()

        if not self.container.start():
            print("Failed to start the container", file=sys.stderr)
            sys.exit(1)

    def create(self):
        if not self.container.create("download", lxc.LXC_CREATE_QUIET, self.dist_config):
            print("Failed to create the container rootfs, dist_config is probably out of date, increment release "
                  "number until it works lol", file=sys.stderr)
            sys.exit(1)

    def stop(self):
        if not self.container.shutdown(30):
            print("Failed to cleanly shutdown the container, forcing.")
            if not self.container.stop():
                print("Failed to kill the container", file=sys.stderr)
                sys.exit(1)

    def destroy(self):
        if not self.container.destroy():
            print("Failed to destroy the container.", file=sys.stderr)
            sys.exit(1)
