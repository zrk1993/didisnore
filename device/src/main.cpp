#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <WAVFileWriter.h>
#include <ADCSampler.h>
#include "config.h"

void record(I2SSampler *input, const char *fname)
{
  int16_t *samples = (int16_t *)malloc(sizeof(int16_t) * 1024);
  Serial.println("Start recording");
  input->start();
  // open the file on the sdcard
  File file = SD.open("/test.wav", FILE_WRITE);

  // create a new wave file writer
  WAVFileWriter *writer = new WAVFileWriter(file, input->sample_rate());
  // keep writing until the user releases the button
  while (digitalRead(GPIO_NUM_2))
  {
    int samples_read = input->read(samples, 1024);
    int64_t start = esp_timer_get_time();
    writer->write(samples, samples_read);
    int64_t end = esp_timer_get_time();
    Serial.printf("Wrote %d samples in %lld microseconds", samples_read, end - start);
  }
  // stop the input
  input->stop();
  // and finish the writing
  writer->finish();
  file.close();
  delete writer;
  free(samples);
  Serial.printf("Finished recording");
}


void setup() {
  Serial.begin(115200);
  Serial.println("start!");
  delay(3000);
  if (!SD.begin(5)) {
    Serial.println("SD init failed!");
    return;
  }
  Serial.printf("SD.cardType = %d \r\n", SD.cardType());

  I2SSampler *input = new ADCSampler(ADC_UNIT_1, ADC1_CHANNEL_7, i2s_adc_config);
  record(input, "/test.wav");
}

void loop() {
  // put your main code here, to run repeatedly:
}