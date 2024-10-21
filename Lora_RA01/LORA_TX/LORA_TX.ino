#include <SPI.h>
#include <LoRa.h>
bool bit_phim=true;
int counter = 0;
int button=3;
void setup() {
  Serial.begin(9600);
  while (!Serial);
  pinMode(button,INPUT_PULLUP);
  Serial.println("LoRa Sender");

  if (!LoRa.begin(428E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  if ((digitalRead(button)==0)&bit_phim)
  {bit_phim=false;
    Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello");
  // LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
  Serial.println("send done!");}
  else
  {
    bit_phim=true;
    delay(100);

  }
}
