// #include <Arduino.h>
// #include <WiFi.h>
// #include <SPI.h>
// #include <SD.h>
// #include <WAVFileWriter.h>
// #include <ADCSampler.h>
// #include "config.h"
// #include "http_utils.h"

// const char* ssid = "ChinaNet-3199";
// const char* passwd = "12345678";

// void connectWifi() {
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, passwd);
//   WiFi.setAutoReconnect(true);
//   WiFi.persistent(true);

//   Serial.println("connecting to router... ");
//   //等待wifi连接成功
//   while (!WiFi.isConnected()) {
//       Serial.print(".");
//       delay(1000);
//   }
//   Serial.print("\nWiFi connected, local IP address:");
//   Serial.println(WiFi.localIP());
// }

// void record(const char *fname, int duration) {
//   I2SSampler *input = new ADCSampler(ADC_UNIT_1, ADC1_CHANNEL_7, i2s_adc_config);
//   int16_t *samples = (int16_t *)malloc(sizeof(int16_t) * 1024);
//   // char* samples = (char*) calloc(1024, sizeof(char));
//   Serial.println("Start recording");
//   input->start();
//   // open the file on the sdcard
//   File file = SD.open(fname, FILE_WRITE);
//   // create a new wave file writer
//   WAVFileWriter *writer = new WAVFileWriter(file, input->sample_rate());
//   // keep writing until the user releases the button
//   int64_t start = millis();
//   size_t count = 0;
//   while (millis() < start + duration) {
//     count += 1;
//     int samples_read = input->read(samples, 1024);
//     writer->write(samples, samples_read);
//     Serial.printf(".");
//   }
//   // Serial.printf("\n %d,  %d", count, count * 1024);
//   // stop the input
//   input->stop();
//   // and finish the writing
//   writer->finish();
//   file.close();
//   delete writer;
//   free(samples);
//   Serial.printf("\nFinished recording, duration: %d", millis() - start);
// }


// void setup() {
//   Serial.begin(115200);
//   Serial.println("start!");
//   delay(1000);
//   if (!SD.begin(5)) {
//     Serial.println("SD init failed!");
//     return;
//   }
//   Serial.printf("SD.cardType = %d \r\n", SD.cardType());

//   connectWifi();

//   delay(1000 * 2);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   if (Serial.available()) {
//     char input = Serial.read();
//     Serial.printf("input: %c", input);
//     switch (input) {
//       case '0': Serial.println("ok");break;
//       case '1': httpUploadFile("/test.wav", "192.168.1.7", 3005, "/wav/upload");
//                 break;
//       case 'a': record("/test.wav", 1000 * 2);
//                 break;
//       case 'b': record("/test.wav", 1000 * 3);
//                 break;
//       case 'c': record("/test.wav", 1000 * 5);
//                 break;
//       case 'd': record("/test.wav", 1000 * 10);
//                 break;
//     }
//   }
// }