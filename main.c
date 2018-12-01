#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <memory.h>

int main(int argc, char **argv) {

    // Name of Serial Port
    char *serial_name = NULL;

    // Get command line arguments
    if (argc != 2) {
        // Print error message
        printf("Invalid command line arguments!");

        // Quit program
        return 0;
    }
    else {
        // Save name of serial port
        serial_name = *(argv + 1);
    }

    // Open Serial Port in read only mode
    // and synchronous mode
    int serial = open(serial_name, O_RDONLY | O_NOCTTY | O_SYNC);

    // Check for errors
    if (serial == -1) {
        // Print error message
        printf("Failed to open serial port!");

        // Quit program
        return 0;
    }

    // Set Serial Port speed
    struct termios settings;

    // Get current settings of Serial Port
    if (tcgetattr(serial, &settings)) {
        // Print error message
        printf("Failed to get serial port settings!");

        // Quit program
        return 0;
    }

    // Disable parity flag
    settings.c_cflag &= ~(CSIZE | PARENB);

    // Force 8-bit input
    settings.c_cflag |= CS8;

    // Set RX/TX speed to 57600
    cfsetspeed(&settings, B57600);

    // Save changes
    tcsetattr(serial, TCSANOW, &settings);

    // Allocate buffer for
    // incoming data
    float buffer;

    // Begin endless read() loop
    for (long w;;) {
        // Receive bytes from Serial Port
        w = read(serial, &buffer, sizeof(float));

        // Check for errors
        if (w >= 0) {
            // Print value
            printf("Received double: %lf\n", buffer);
        } else {
            // Print error message
            printf("Failed to read from serial port!");
            break;
        }
    }

    // Close Serial Port
    if (close(serial)) {
        printf("Failed to close serial port!");
    }

    return 0;
}