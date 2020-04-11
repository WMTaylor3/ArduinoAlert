
/*
   UIPEthernet TcpClient example.

   UIPEthernet is a TCP/IP stack that can be used with a enc28j60 based
   Ethernet-shield.

   UIPEthernet uses the fine uIP stack by Adam Dunkels <adam@sics.se>

        -----------------

   This TcpClient example gets its local ip-address via dhcp and sets
   up a tcp socket-connection to 192.168.0.1 port 5000 every 5 Seconds.
   After sending a message it waits for a response. After receiving the
   response the client disconnects and tries to reconnect after 5 seconds.

   Copyright (C) 2013 by Norbert Truchsess <norbert.truchsess@t-online.de>
*/

#include <UIPEthernet.h>
#include <ArduinoHttpClient.h>
#include "utility/logging.h"
EthernetClient client;
unsigned long next;
char serverAddress[] = "maker.ifttt.com";  // server address
int port = 80;

void setup() {
    Serial.begin(9600);
    uint8_t mac[6] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
    Ethernet.init(10); Serial.println("cs pin");
    while (!Ethernet.begin(mac))
    { //Configure IP address via DHCP
        Serial.println("not Initiliazed");
    }
    Serial.println("Initiliazed");
    Serial.print(("localIP: "));
    Serial.println(Ethernet.localIP());
    Serial.print(("subnetMask: "));
    Serial.println(Ethernet.subnetMask());
    Serial.print(("gatewayIP: "));
    Serial.println(Ethernet.gatewayIP());
    Serial.print(("dnsServerIP: "));
    Serial.println(Ethernet.dnsServerIP());
    next = 0;
}
HttpClient client1 = HttpClient(client, serverAddress, port);

void loop()
{
    if (digitalRead(12) == HIGH) {
        sendRequest();
    }
}

void sendRequest()
{
    Serial.println("making GET request");

    client1.get("/trigger/murry_alert/with/key/cfRgKZsJBWPAZu5kFDD2XwWuqpDf9rp_UjqR3P4eVl2");
    //client.print(String("GET ") + "/trigger/murry_alert/with/key/cfRgKZsJBWPAZu5kFDD2XwWuqpDf9rp_UjqR3P4eVl2" + " HTTP/1.1\r\n" +
    //    "Host: " + "maker.ifttt.com" + "\r\n" +
    //    "Connection: close\r\n\r\n");
    int statusCode = client1.responseStatusCode();
    String response = client1.responseBody();
    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
    Serial.println("Wait five seconds");
    delay(5000);
}