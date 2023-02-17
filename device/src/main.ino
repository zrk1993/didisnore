#include <Arduino.h>
#include <WiFi.h>
#include <driver/i2s.h>
#include "esp_task_wdt.h"

#define I2S_WS 15
#define I2S_SD 32
#define I2S_SCK 14
#define I2S_PORT I2S_NUM_0
#define I2S_SAMPLE_RATE (16000)
#define I2S_SAMPLE_BITS (16)
#define I2S_READ_LEN (1 * 16 * 1024)

int upload_buff_index = 0;
int i2s_read_buff_index = 1;
char *i2s_read_buff1 = (char *)calloc(I2S_READ_LEN, sizeof(char));
char *i2s_read_buff2 = (char *)calloc(I2S_READ_LEN, sizeof(char));

const char *ssid = "helloword";
const char *passwd = "zxcvbnm8";

const char *tcp_host = "150.158.27.240";
const int tcp_port = 8106;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    connectWifi();
    i2sInit();

    xTaskCreatePinnedToCore(i2s_adc, "i2s_adc", 1024 * 2, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(upload, "upload", 1024 * 2, NULL, 1, NULL, 1);
}

void loop()
{
    // put your main code here, to run repeatedly:
    // Serial.println("loop");
}

void connectWifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, passwd);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    Serial.println("connecting to router... ");
    // 等待wifi连接成功
    while (!WiFi.isConnected())
    {
        Serial.print(".");
        delay(1000);
    }
    Serial.print("\nWiFi connected, local IP address:");
    Serial.println(WiFi.localIP());
}

void i2sInit()
{
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = I2S_SAMPLE_RATE,
        .bits_per_sample = i2s_bits_per_sample_t(I2S_SAMPLE_BITS),
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = 0,
        .dma_buf_count = 64,
        .dma_buf_len = 1024,
        .use_apll = 1};

    i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);

    const i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK,
        .ws_io_num = I2S_WS,
        .data_out_num = -1,
        .data_in_num = I2S_SD};

    i2s_set_pin(I2S_PORT, &pin_config);
}

void example_disp_buf(uint8_t* buf, int length)
{
    printf("======\n");
    for (int i = 0; i < length; i++) {
        printf("%02x ", buf[i]);
        if ((i + 1) % 8 == 0) {
            printf("\n");
        }
    }
    printf("======\n");
}

void i2s_adc_data_scale(uint8_t *d_buff, uint8_t *s_buff, uint32_t len)
{
    uint32_t j = 0;
    uint32_t dac_value = 0;
    for (int i = 0; i < len; i += 2)
    {
        dac_value = ((((uint16_t)(s_buff[i + 1] & 0xf) << 8) | ((s_buff[i + 0]))));
        d_buff[j++] = 0;
        d_buff[j++] = dac_value * 256 / 4096;
    }
}

void i2s_adc(void *arg)
{
    int i2s_read_len = I2S_READ_LEN;
    size_t bytes_read;
    Serial.println(" *** Recording Start *** ");
    i2s_read(I2S_PORT, (void*) i2s_read_buff1, i2s_read_len, &bytes_read, portMAX_DELAY);
    i2s_read(I2S_PORT, (void*) i2s_read_buff1, i2s_read_len, &bytes_read, portMAX_DELAY);
    while (true)
    {
        Serial.print('-');
        while (upload_buff_index == i2s_read_buff_index)
        {
            Serial.println("* waiting upload *");
            delay(10);
        }
        char *i2s_read_buff = i2s_read_buff_index == 1 ? i2s_read_buff1 : i2s_read_buff2;
        i2s_read(I2S_PORT, (void *)i2s_read_buff, i2s_read_len, &bytes_read, portMAX_DELAY);
        i2s_adc_data_scale((uint8_t *)i2s_read_buff, (uint8_t *)i2s_read_buff, i2s_read_len);
        upload_buff_index = i2s_read_buff_index;
        i2s_read_buff_index = i2s_read_buff_index == 1 ? 2 : 1;
    }
    vTaskDelete(NULL);
}

void upload(void *arg)
{
    esp_task_wdt_init(10, true);
    esp_task_wdt_add(NULL);
    WiFiClient client;
    while (true)
    {
        if (client.connected())
        {
            Serial.print("=");
            esp_task_wdt_reset();
            if (upload_buff_index == 0)
            {
                Serial.println("* no data upload *");
                delay(100);
                continue;
            }
            if (upload_buff_index == -1) {
                Serial.print("$");
            }
            while (upload_buff_index == -1)
            {
                delay(10);
            }
            char *i2s_read_buff = upload_buff_index == 1 ? i2s_read_buff1 : i2s_read_buff2;
            //example_disp_buf((uint8_t *)i2s_read_buff, I2S_READ_LEN);
            client.write(i2s_read_buff, I2S_READ_LEN);
            upload_buff_index = -1;
            Serial.print('*');
        }
        else
        {
            Serial.print("client connect ...");
            while (!client.connect(tcp_host, tcp_port))
            {
                delay(1000);
                Serial.print(".");
            }
        }
    }
    vTaskDelete(NULL);
}
