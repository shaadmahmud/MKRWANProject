#include <SPI.h>
#include <LoRa.h>
#include <MKRWAN.h>

int counter = 0;
const int UNIQUE_NODES = 4;
const String id [UNIQUE_NODES] = {"a8610a3432276904", "a8610a3432359408", "a8610a3432398b08", "a8610a3432218b04"};

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("LoRa Reciever Hub");

  //using the basic connection to recieve any packets over the NA frequency band
  if(!LoRa.begin(915E6))
  {
    Serial.println("Starting LoRa failed! --> update firmware and try again");
    while(1);  
  }
  //now we define the device EUIs we want to use. These are not generated by any program, and need to be input here by the user to ensure security
  //id = {"a8610a3432276904", "a8610a3432359408", "a8610a3432398b08", "a8610a3432218b04"};//so far we have been working with these 4
  //the 0 index is the hub EUI
  
  Serial.println("Connected to LoRa Network. Waiting for packets...");
  //if we have gotten here, our board is connected to the LoRa network and can begin receiving packets
  //we want to predefine the device EUIs that we will be using in a header file for security
  
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  int localID;
  String eui = "";
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read first 16 bytes and save for compare
    while (LoRa.available() && counter < 16) {
      char cur = (char)LoRa.read();
      eui += cur;
      Serial.print(cur);
      counter++;//count the bytes read
    }
    int valid = 0;
    for(int i=0; i<UNIQUE_NODES; i++) {
        if(id[i] == eui){
            valid = 1;
            localID = i;
          }
    }
    if(valid == 0)//invalid sender, do not store any info from packet
    {
      Serial.println("\nInvalid Sender --> Please Register new devices with the Hub");
    }
    else//valid sender, store data
    {
      while(LoRa.available())
      {
        Serial.print((char)LoRa.read());
        counter++;
      }
    }
    //print the local ID
    Serial.print("' from device ");
    Serial.print(localID);
    // print RSSI of packet
    Serial.print(" with RSSI ");
    Serial.println(LoRa.packetRssi());
    Serial.print("Packet Size(bytes): ");
    Serial.println(counter);
    counter=0;
}
}
