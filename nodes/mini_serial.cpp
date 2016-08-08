#include <boost/asio.hpp> 
#include "ros/ros.h"
#include "sensor_msgs/Imu.h"

#include <iostream>

using namespace::boost::asio;  
using std::cin;

const char *PORT = "dev/ttyS3";

serial_port_base::baud_rate baud(115200);
// how big is each "packet" of data (default is 8 bits)
serial_port_base::character_size csize(8);
// what flow control is used (default is none)
serial_port_base::flow_control flow(serial_port_base::flow_control::none);
// what parity is used (default is none)
serial_port_base::parity parity(serial_port_base::parity::none);
// how many stop bits are used (default is one)
serial_port_base::stop_bits stop(serial_port_base::stop_bits::one);

int main()
{
    // create the I/O service that talks to the serial device
    io_service io;
    // create the serial device, note it takes the io service and the port name
    serial_port port(io, PORT);

    port.set_option(baud);
    port.set_option(csize);
    port.set_option(flow);
    port.set_option(parity);
    port.set_option(stop);

    // buffer to store commands
    // this device reads 8 bits, meaning an unsigned char, as instructions
    // varies with the device, check the manual first
    unsigned char command[1] = {0};

    // read in user value to be sent to device
    int input;
    cin >> input;

    // Simple loop, since the only good values are [0,255]
    //  break when a negative number is entered.
    // The cast will convert too big numbers into range.
    while(input >= 0) {
        // convert our read in number into the target data type
        command[0] = static_cast<unsigned char>(input);

        // this is the command that sends the actual bits over the wire
        // note it takes a stream and a asio::buffer
        // the stream is our serial_port
        // the buffer is constructed using our command buffer and
        //  the number of instructions to send
        write(port, buffer(command, 1));

        // read in the next input value
        cin >> input;
    }
    return 0;
}