#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

int set_interface_attribs(int fd, int speed) {
    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        perror("tcgetattr");
        return -1;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; 
    tty.c_iflag &= ~IGNBRK; 
    tty.c_lflag = 0; 
    tty.c_oflag = 0; 
    tty.c_cc[VMIN]  = 0; 
    tty.c_cc[VTIME] = 5; 

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); 

    tty.c_cflag |= (CLOCAL | CREAD); 
    tty.c_cflag &= ~(PARENB | PARODD); 
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        perror("tcsetattr");
        return -1;
    }
    return 0;
}

void set_blocking(int fd, int should_block) {
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) != 0) {
        perror("tcgetattr");
        return;
    }

    tty.c_cc[VMIN]  = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5; 

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
        perror("tcsetattr");
}

void print_usage(const char *progname) {
    printf("Usage: %s <command>\n", progname);
    printf("Commands:\n");
    printf("  on     - Turn the LED on\n");
    printf("  off    - Turn the LED off\n");
    printf("  toggle - Toggle the LED state\n");
    printf("  read   - Read the LED state\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char *portname = "/dev/ttyACM0"; // Change this to your port
    int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    set_interface_attribs(fd, B9600); 
    set_blocking(fd, 0);              

    const char *command = argv[1];
    if (strcmp(command, "on") == 0) {
        write(fd, "1", 1);
    } else if (strcmp(command, "off") == 0) {
        write(fd, "0", 1);
    } else if (strcmp(command, "toggle") == 0) {
        write(fd, "t", 1);
    } else if (strcmp(command, "read") == 0) {
        write(fd, "r", 1);
        sleep(1); 
        char buf[1];
        int n = read(fd, buf, sizeof buf);
        if (n > 0) {
            if (buf[0] == '1') {
                printf("LED is on\n");
            } else if (buf[0] == '0') {
                printf("LED is off\n");
            } else {
                printf("Unknown state: %c\n", buf[0]);
            }
        } else {
            printf("Failed to read LED state\n");
        }
    } else {
        print_usage(argv[0]);
    }

    close(fd);
    return 0;
}
