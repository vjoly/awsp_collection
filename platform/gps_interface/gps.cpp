#include <chrono>
#include <errno.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "gps_interface.hpp"

void print_position(position position)
{
    std::cout << "POSITION"                 << std::endl;
    std::cout << "========================" << std::endl;
    std::cout << "Message --------------> " << position.message              << std::endl;
    std::cout << "Timestamp ------------> " << position.timestamp            << std::endl;
    std::cout << "Latitude -------------> " << position.latitude             << std::endl;
    std::cout << "Longitude ------------> " << position.longitude            << std::endl;
    std::cout << "Fix ------------------> " << position.fix                  << std::endl;
    std::cout << "Number of satelites --> " << position.number_of_satelites  << std::endl;
    std::cout << "Horizontal Precision -> " << position.horizontal_precision << std::endl;
    std::cout << "Altitude -------------> " << position.altitude             << std::endl;
}

int64_t get_epoch()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main() 
{
    GPSInterface gps;

    if (gps.open_connection("/dev/ttyS0", 9600))
        std::cout << "Connected to port: " << gps.get_port() << std::endl;
    else
    {
        std::cout << "Cannot connect" << std::endl;
        return 1;
    }

    int num_lines = 0;
    int64_t last_position_time = get_epoch();
    int64_t position_time = get_epoch();
    while (true)
    {
        num_lines = gps.read_lines();
        position last_position = gps.get_position();

        if (num_lines > 0)
        {
            position_time = get_epoch();
            std::cout << std::endl;
            std::cout << "********************************** NEW POSITION **********************************" << std::endl;
            std::cout << "Ellapsed time --------> " << position_time - last_position_time << std::endl;
            std::cout << "Number of lines read -> " << num_lines                          << std::endl;
            std::cout << "----------------------------------------------------------------------------------" << std::endl;
            print_position(last_position);
            last_position_time = get_epoch();
        }
    }
    gps.close_connection();

    return 0;
}