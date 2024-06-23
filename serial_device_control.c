#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#define PORTNAME "/dev/ttyACM0" // Change this to your port 
#define BAUDRATE B9600

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
    printf("Usage: %s <command> [options]\n", progname);
    printf("Commands:\n");
    printf("  on                  - Turn the LED on\n");
    printf("  off                 - Turn the LED off\n");
    printf("  toggle              - Toggle the LED state\n");
    printf("  read                - Read the LED state\n");
    printf("  blink <count>       - Blink the LED specified number of times\n");
    printf("  set_interval <ms>   - Set the blink interval in milliseconds\n");
    printf("  get_interval        - Get the current blink interval\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    int fd = open(PORTNAME, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    set_interface_attribs(fd, BAUDRATE);
    set_blocking(fd, 0);

    char command[256];
    if (strcmp(argv[1], "on") == 0) {
        snprintf(command, sizeof(command), "on\n");
    } else if (strcmp(argv[1], "off") == 0) {
        snprintf(command, sizeof(command), "off\n");
    } else if (strcmp(argv[1], "toggle") == 0) {
        snprintf(command, sizeof(command), "toggle\n");
    } else if (strcmp(argv[1], "read") == 0) {
        snprintf(command, sizeof(command), "read\n");
        write(fd, command, strlen(command));
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
        close(fd);
        return 0;
    } else if (strcmp(argv[1], "blink") == 0) {
        if (argc < 3) {
            print_usage(argv[0]);
            close(fd);
            return 1;
        }
        snprintf(command, sizeof(command), "blink %s\n", argv[2]);
    } else if (strcmp(argv[1], "set_interval") == 0) {
        if (argc < 3) {
            print_usage(argv[0]);
            close(fd);
            return 1;
        }
        snprintf(command, sizeof(command), "set_interval %s\n", argv[2]);
    } else if (strcmp(argv[1], "get_interval") == 0) {
        snprintf(command, sizeof(command), "get_interval\n");
        write(fd, command, strlen(command));
        sleep(1); 
        char buf[256];
        int n = read(fd, buf, sizeof buf - 1);
        if (n > 0) {
            buf[n] = 0; 
            printf("Current blink interval: %s ms\n", buf);
        } else {
            printf("Failed to get blink interval\n");
        }
        close(fd);
        return 0;
    } else {
        print_usage(argv[0]);
        close(fd);
        return 1;
    }

    write(fd, command, strlen(command));
    close(fd);
    return 0;
}
