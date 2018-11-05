#include "gps_interface.hpp"

// ******************************** CONSTRUCTORS-DESTRUCTORS *******************************
GPSInterface::GPSInterface()
{
    read_line_ = "";
    port_ = 0;
}

GPSInterface::~GPSInterface() { }

// **************************************** PRIVATE ****************************************
std::vector<std::string> GPSInterface::break_string_(std::string str, char separator)
{
    std::string result = "";
    std::stringstream ss(str);
    std::string substr;
    std::vector<std::string> content;

    while (ss.good())
    {
        getline(ss, substr, separator);
        if (substr != "")
            content.push_back(substr);
    }

    return content;
}

float GPSInterface::parse_to_degrees_(std::string str)
{
    std::vector<std::string> content = break_string_(str, '.');

    std::string minutes = content[0].substr(content[0].size() - 2);
    minutes += "." + content[1];

    std::string::size_type idx;
    float minutes_float = std::stof(minutes, &idx);
    minutes_float = minutes_float / 60;

    content[0].erase(content[0].end() - 2, content[0].end());
    idx = 0;
    float degrees = std::stof(content[0], &idx) + minutes_float;

    return degrees;
}

bool GPSInterface::parse_raw_line_(std::string line)
{
    if (strncmp(line.c_str(), POSITION_START_.c_str(), POSITION_START_.size()) == 0)
        return populate_position_(line);
    else
        return false;
}

bool GPSInterface::populate_position_(std::string position_line)
{
    std::vector<std::string> content = break_string_(position_line, ',');

    if (content.size() >= 10)
    {
        position_.message = position_line;
        position_.timestamp = content[1];
        position_.latitude = parse_to_degrees_(content[2]);
        position_.longitude = parse_to_degrees_(content[4]);
        if (content[3] != "N") position_.latitude = -position_.latitude;
        if (content[5] != "E") position_.longitude = -position_.longitude;

        std::string::size_type idx;
        position_.fix = std::stoi(content[6], &idx);
        idx = 0;
        position_.number_of_satelites = std::stoi(content[7], &idx);
        idx = 0;
        position_.horizontal_precision = std::stof(content[8], &idx);
        idx = 0;
        position_.altitude = std::stof(content[9], &idx);

        return true;
    }
    else return false;
}

std::vector<std::string> GPSInterface::read_raw_lines_()
{
    char received;
    std::vector<std::string> lines;
    while (serialDataAvail(port_))
    {
        received = serialGetchar(port_);
        if (received != '\n')
            read_line_ += received;
        else
        {
            lines.push_back(read_line_);
            read_line_ = "";
        }
    }

    return lines;
}

// **************************************** PUBLIC *****************************************
bool GPSInterface::close_connection()
{
    serialClose(port_);
    return true;
}

int GPSInterface::get_port()
{
    return port_;
}

position GPSInterface::get_position()
{
    return position_;
}

bool GPSInterface::open_connection(const char* serial_port, long baud_rate)
{
    port_ = serialOpen(serial_port, baud_rate);

    if (port_ >= 0)
    {
        read_raw_lines_();
        return true;
    }
    else return false;
}

int GPSInterface::read_lines()
{
    std::vector<std::string> raw_lines = read_raw_lines_();
    int num_lines = 0;

    for (int i = 0; i < int(raw_lines.size()); i++)
        if (parse_raw_line_(raw_lines[i])) num_lines++;

    return num_lines;
}