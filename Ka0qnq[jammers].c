// By Ka01nq
// Lisensi Creative Commons BY-NC-ND
// ambil boleh raname jangan
/*
⣿⣿⣿⡇⢩⠘⣴⣿⣥⣤⢦⢁⠄⠉⡄⡇⠛⠛⠛⢛⣭⣾⣿⣿⡏
⣿⣿⣿⡇⠹⢇⡹⣿⣿⣛⣓⣿⡿⠞⠑⣱⠄⢀⣴⣿⣿⣿⣿⡟
⣿⣿⣿⣧⣸⡄⣿⣪⡻⣿⠿⠋⠄⠄⣀⣀⢡⣿⣿⣿⣿⡿⠋
⠘⣿⣿⣿⣿⣷⣭⣓⡽⡆⡄⢀⣤⣾⣿⣿⣿⣿⣿⡿⠋
⠄⢨⡻⡇⣿⢿⣿⣿⣭⡶⣿⣿⣿⣜⢿⡇⡿⠟⠉
⠄⠸⣷⡅⣫⣾⣿⣿⣿⣷⣙⢿⣿⣿⣷⣦⣚⡀
⠄⠄⢉⣾⡟⠙⠶⠖⠈⢻⣿⣷⣅⢻⣿⣿⣿⣿⣿⣶⣶⡆⠄⣤⡀
⠄⢠⣿⣿⣧⣀⣀⣀⣀⣼⣿⣿⣿⡎⢿⣿⣿⣿⣿⣿⣿⣇⠄⠈⠁
⠄⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢇⣎⢿⣿⣿⣿⣿⣿⣿⣿⣶⣶
⠄⠄⠻⢿⣿⣿⣿⣿⣿⣿⣿⢟⣫⣾⣿⣷⡹⣿⣿⣿⣿⣿⣿⣿⡟
⠄⠄⠄⠄⢮⣭⣍⡭⣭⡵⣾⣿⣿⣿⡎⣿⣿⣌⠻⠿⠿⠿⠟⠋
⠄⠄⠄⠄⠈⠻⣿⣿⣿⣿⣹⣿⣿⣿⡇⣿⣿⡿
⠄⠄⣀⣴⣾⣶⡞⣿⣿⣿⣿⣿⣿⣿⣾⣿⡿⠃
*/

/* 
⠀⠀⠀⠀⠀⠀⠀⢀⡤⠤⣄⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⣾⠀⠀⢸⡇⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⡏⠀⠀⢸⡇⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⢸⡇⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⢸⡇⠀⠀⢸⡇⠀⠀⠀⠀⠀⠀
⠀⠀⠀⢀⡾⠋⠻⡇⠀⠀⢸⣧⣀⡀⠀⠀⠀⠀
⠀⠀⢀⣾⠁⠀⠀⡇⠀⠀⢸⠁⠀⢹⣀⠀⠀⠀
⢀⡴⠋⡟⠀⠀⢠⡇⠀⠀⢸⠀⠀⠀⡇⠉⢆⠀
⡎⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⠀⠈⣆
⢷⡀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸
⠀⠻⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾
⠀⠀⠈⠻⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠞⠁
⠀⠀⠀⠀⠈⣷⠀⠀⠀⠀⠀⠀⠀⠀⢰⠋⠀⠀
⠀⠀⠀⠀⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⡏⠀⠀⠀
⠀⠀⠀⠀⠀⠛⠒⠒⠒⠒⠒⠒⠒⠚⠃⠀⠀⠀
*/

#include <SPI.h>
#include <RF24.h>
// >\\\<
#define CE1 4
#define CSN1 5
#define CE2 16
#define CSN2 17
#define BUTTON_PIN 15
#define LED_PIN 2

RF24 radio1(CE1, CSN1);
RF24 radio2(CE2, CSN2);

bool jamming = false;
bool lastButtonState = HIGH;
unsigned long lastDebounce = 0;
const unsigned long debounceDelay = 50;

const int channelCount = 126;
int currentChannel = 0;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  if (!radio1.begin()) {
    Serial.println("radio1 ggl");
  } else {
    radio1.setPALevel(RF24_PA_MAX);
    radio1.setDataRate(RF24_2MBPS);
    radio1.setAutoAck(false);
    radio1.setPayloadSize(32);
    radio1.stopListening();
  }

  if (!radio2.begin()) {
    Serial.println("radio2 sama gll");
  } else {
    radio2.setPALevel(RF24_PA_MAX);
    radio2.setDataRate(RF24_2MBPS);
    radio2.setAutoAck(false);
    radio2.setPayloadSize(32);
    radio2.stopListening();
    /* Hiduplah dengan gayamu sendiri karena setiap tempat pasti ada manusia yang membenci */
  }

  Serial.println("tekan tbl unuk mula");
}

void loop() {
  bool reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonState) {
    lastDebounce = millis();
  }
  if ((millis() - lastDebounce) > debounceDelay && reading == LOW && lastButtonState == HIGH) {
    jamming = !jamming;
    digitalWrite(LED_PIN, jamming ? HIGH : LOW);
    if (jamming) {
      Serial.println("Ka01nq Jamer urep");
      currentChannel = 0;
    } else {
      Serial.println("Ka01nq Jamer mati");
    }
    delay(100);
  }
  lastButtonState = reading;
  // ka01nq
  if (jamming) {
    int ch1 = currentChannel;
    int ch2 = (currentChannel + 63) % channelCount;
    
    radio1.setChannel(ch1);
    radio2.setChannel(ch2);
    
    uint8_t payload[32];
    for (int i = 0; i < 32; i++) payload[i] = random(0, 256);
    
    radio1.write(payload, 32);
    radio2.write(payload, 32);
    
    currentChannel = (currentChannel + 1) % channelCount;
    
    delayMicroseconds(100);
  }
}
