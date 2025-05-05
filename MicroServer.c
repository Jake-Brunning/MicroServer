#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/sockets.h"
#include "lwip/err.h"
#include "lwip/ip_addr.h"


int main()
{
    stdio_init_all();
    pico_led_init();

    connectToANetwork("SSID", "PASSWORD"); // Connect to the Wi-Fi network

    startReceiving(); // Start receiving UDP packets
    return 0;
}

//starts listening for UDP packets on a port.
void startReceiving(){
    const int portToBindTo = 1234; // Port to bind to for receiving UDP packets


    //bind the port to the UDP PCB
    struct udp_pcb* reciever = UDPbindToPort(portToBindTo); 
    
    if (reciever == NULL) {
        printf("Failed to start listeing on port:  %d\n", portToBindTo);
        return;
    }

    //lisen.
    udp_recv(reciever, onReceive, NULL); // Set the receive callback function

    while(true) {
        sleep_ms(1000); //so program doesnt shut down.
    }

    udp_remove(reciever);
    return 0;
}

// Perform led initialisation
int pico_led_init(void) {
    #if defined(PICO_DEFAULT_LED_PIN)
        gpio_init(PICO_DEFAULT_LED_PIN);
        gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
        return PICO_OK;
    #elif defined(CYW43_WL_GPIO_LED_PIN)
        // For Pico W devices we need to initialise the driver etc
        return cyw43_arch_init();
    #endif
}

// Turn the led on or off
void pico_set_led(bool led_on) {
    #if defined(PICO_DEFAULT_LED_PIN)
        // Just set the GPIO on or off
        gpio_put(PICO_DEFAULT_LED_PIN, led_on);
    #elif defined(CYW43_WL_GPIO_LED_PIN)
        // Ask the wifi "driver" to set the GPIO on or off
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
    #endif
}

//connects to a specified network.
int connectToANetwork(const char* ssid, const char* password) {
    //initilise wifi module
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    // Connect to the Wi-Fi network
    if (cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Failed to connect to %s\n", ssid);
        return -1;
    } else {
        printf("Connected to %s\n", ssid);
        return 0;
    }
}


struct udp_pcb* UDPbindToPort(const int portToBindTo) {
    struct udp_pcb *pcbReciever = udp_new(); // create a new UDP PCB for receiving

    if(!pcbReciever) { //if pcb is 0 it means it was unssuccessful
        printf("Failed to create PCB\n");
        return NULL;
    }

    
    //bind to a port to listen to
    err_t err = udp_bind(pcbReciever, IP_ADDR_ANY, portToBindTo); // bind the PCB to the specified port

    if (err != ERR_OK) { //if we faild to make bind. Probably bc the port is in use.
        printf("Failed to bind PCB\n");
        udp_remove(pcbReciever); // clean up the PCB
        return NULL;
    }

    return pcbReciever; //return the binded port info. for udp only.
}


void onReceive(struct udp_pcb *pcb, struct pbuf *p, const struct ip_addr *addr, u16_t port) {
    // Handle incoming UDP packets here
    printf("Received packet from %s:%d\n", ipaddr_ntoa(addr), port);
    printf("Data: %s\n", (char*)p->payload); // Print the received data

    //process the receieved packet
    //extract payload
    char buffer[1024];
    memcpy(buffer, p->payload, p->len); 
    buffer[p->len] = '\0';

    printf("Payload: %s\n", buffer); // Print the payload

    handlePayload(buffer);
    pbuf_free(p); // Free the received packet buffer
}

void handlePayload(const char* payload) {
    //split payload into commands
    char** commands = splitPayload(payload, ';', 50);

    //go through and execute each command
    int i = 0;
    while (commands[i] != NULL) {
        printf("Executing command: %s\n", commands[i]);
        identifyAndExecuteCommand(commands[i]); // Execute the command
        i++;
    }
}


//split string by delimiter. I dont actually think this is reentrant because strtok does seem so.
//i have NO clue who made some of these C functions but they suck.
char** splitPayload(const char* payload, const char delimiter, const int maxCommands) {
    //split payload into commands
    char* command = strtok(payload, delimiter);
    char** commandList[maxCommands]; // Assuming a maximum of 50 commands

    //go through all commands and add them to the list.
    int i = 0;
    while (command != NULL && i < maxCommands) {
        commandList[i] = command;
        command = strtok(NULL, delimiter);
        i++;
    }

    commandList[i] = NULL;
    return commandList; // Return the list of commands
}


//checks if the start of a string matches another string.
bool startMatches(const char* str1, const char * str2) {
    
    //go through characters until there not equal.
    int i = 0;
    while(str1[i] == str2[i]){
        i++;
    }

    //if we have fully consumed the second string then str2 is a prefix of str1.
    if(str2[i] == '\0'){
        return true; 
    }
    else{
        return false; 
    }
}


//ids and executes a command.
char** identifyAndExecuteCommand(const char* command){
    if(strcmp(command, "LED_ON") == 0){
        LED_ON_Command(); // Turn the LED on
    }
    else if(strcmp(command, "LED_OFF") == 0){
        LED_OFF_Command();
    }
    else{
        printf("Unknown command: %s\n", command);
    }
}


//youll never guess what this does
void LED_ON_Command(){
    pico_set_led(true); // Turn the LED on
}

void LED_OFF_Command(){
    pico_set_led(false); // Turn the LED off
}