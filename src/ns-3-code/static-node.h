#include <string>
#include <vector>

#include "ns3/core-module.h"
#include "ns3/network-module.h"

using namespace std;

class StaticNode {
    public:
        string name;
        string type;
        int x;
        int y;
        int z;
        string neighbour;
        ns3::NetDeviceContainer devices;

        StaticNode(vector<string> row){
            name = row[0];
            type = row[1];
            x = std::stoi(row[2]);
            y = std::stoi(row[3]);
            z = std::stoi(row[4]);
            neighbour = row[5];
        }

        void addDevice(ns3::Ptr<ns3::NetDevice> device){
            devices.Add(device);
        }

        string toString(){
            return name + "," + type + "," + to_string(x) + "," + to_string(y) + "," + to_string(z) + "," + neighbour;
        }
};