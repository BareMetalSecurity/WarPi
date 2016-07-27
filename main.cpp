#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <boost/thread/thread.hpp>

#include "Manager/Manager.h"


using namespace std;

void check_root(){
    if(geteuid() != 0){
        cout << "You need root permissions to do this." << endl;
        exit(0);
    }
}

bool file_exists(const string& name){
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

void install(){
    if(file_exists("/bin/warpi")){
        cout << "WarPi is already installed" << endl;
        exit(0);
    }
    struct stat stat_buf;
    int read_fd = open("warpi", O_RDONLY);
    fstat(read_fd, &stat_buf);
    int write_fd = open("/bin/warpi", O_WRONLY | O_CREAT, stat_buf.st_mode);
    close(read_fd);
    close(write_fd);
}

void uninstall(){
    if(!file_exists("/bin/warpi")){
        cout << "WarPi does not exist in /bin/" << endl;
        cout << "I can not delete what does not exist." << endl;
        exit(0);
    }
    remove("/bin/warpi");
}

void enable(){
    if(file_exists("/etc/init.d/warpi.sh")){
        cout << "WarPi should already run at boot" << endl;
        exit(0);
    }
    ofstream warpi("/etc/init.d/warpi.sh");
    warpi << "#!/bin/bash\n";
    warpi << "screen warpi start\n";
    warpi.close();
}

void disable(){
    if(!file_exists("/etc/init.d/warpi.sh")){
        cout << "WarPi already does not run at boot" << endl;
        exit(0);
    }
    remove("/etc/init.d/warpi.sh");
}

void usage(){
    cout << "Usage: ./warpi {option}" << endl;
    cout << "\tOptions:" << endl;
    cout << "\tstart\t\t-\tStarts the Program" << endl;
    cout << "\tinstall\t\t-\tInstalls the Program for usage from /bin/" << endl;
    cout << "\tuninstall\t-\tRemoves the Program from /bin/" << endl;
    cout << "\tenable\t\t-\tEnable running this program at boot" << endl;
    cout << "\tdisable\t\t-\tDisable running this program at boot" << endl;
    cout << "Got it? Ok. Bye." << endl;
    exit(0);
}

int main(int argc, char ** argv) {
    if(argc == 2){
        if(argv[1] == "install"){
            check_root();
            install();
        } else if(argv[1] == "enable"){
            check_root();
            enable();
        } else if(argv[1] == "uninstall"){
            check_root();
            uninstall();
        } else if(argv[1] == "disable"){
            check_root();
            disable();
        } else if(argv[1] == "start"){
            boost::thread* manager_thread = new boost::thread(ManagerFunction::run);
            manager_thread->join();
        } else{
            usage();
        }
    } else {
        usage();
    }
}

/*
 * TODO:
 * WiFi Authentication (Get IWLib encryption codes(ieee 802.11))
 *
 * Set scanning rate accordingly to moving speed ( eg. walking -> 2 seconds, driving -> 0.2 seconds )
 *
 * Network Object (eg.: Store information about if connected to network and IP ranges)
 * Network Function (eg.: Get Details for above Object, execute scans)
 *
 * Scanning and Spoofing
 *
 * WiFi Karma HotSpot (eg.: get most probed ssid -> open hotspot -> scan & poison)
 * Number Plate recognition (eg.: libpng -> take picture every x seconds -> openalpr -> plate, coords to db)
 *
 * Bluetooth (eg.: get near devices, put mac and services to db, act malicious)
 * GPS Map (eg.: Tangram Library for rendering)
 *
 */