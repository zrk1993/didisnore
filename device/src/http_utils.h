#include <Arduino.h>
#include <WiFi.h>
#include <SD.h>
#include <HTTPClient.h>

int httpDownloadFile(const char *url, const char *fileName) {
	File fd = SD.open(fileName, FILE_READ);
	if (!fd) {
		Serial.printf("Can't open %s !\r\n", fileName);
		return -1;
	}
	Serial.printf("DownloadFile %s to %s\n", url, fileName);
	HTTPClient http;
	WiFiClientSecure client;

	http.setTimeout(10000);
	http.begin(client, url);

	int httpCode = http.GET();
	Serial.printf("GET... code: %d\n", httpCode);

	if (httpCode != HTTP_CODE_OK)
	{
		Serial.printf("GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
		return -2;
	}

	// int len = http.getSize();
	int len = 26797;
	Serial.printf("download file size: %d\r\n", len);

	uint8_t buff[128] = {0};
	WiFiClient *stream = http.getStreamPtr();

	while (http.connected() && (len > 0 || len == -1))
	{
		size_t size = stream->available();
		if (size) {
			int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
			fd.write(buff, c);
			if (len > 0) {len -= c;}
		}
		delay(1);
	}

	Serial.printf("save file size: %d\n", fd.size());

	if (len != fd.size()) {
		Serial.printf("save file data len error\n");
	}

	Serial.print("[HTTP] connection closed or file end.\n");

	fd.close();
	http.end();
	return 1;
}

String randomChr(int size)
{
	String ret = "";
	for (int i = 0; i < size; i++)
	{
		if (i % 3 == 0)
		{
			ret += (char)random(48, 57);
		}
		else if (i % 3 == 1)
		{
			ret += (char)random(65, 90);
		}
		else if (i % 3 == 2)
		{
			ret += (char)random(97, 122);
		}
	}
	return ret;
}

int httpUploadFile(const char *fileName, String host, int port, String path)
{
	File fd = SD.open(fileName, FILE_READ);
	if (!fd) {
		Serial.print("open file failed\n");
		return -1;
	}
	
	String boundary = randomChr(32);

	String header = "POST " + path + " HTTP/1.1\r\n";
	header += "Host: " + host + "\r\n";
	header += "Content-Type: multipart/form-data;boundary=" + boundary + "\r\n";
	header += "Connection: keep-alive\r\n";

	String body = "--" + boundary + "\r\n";
	body += "Content-Disposition: form-data;name=\"file\";filename=\"" + String(fileName) + "\"\r\n";
	body += "Content-Type: application/octet-stream\r\n";
	body += "\r\n";

	String bodyend = "\r\n--" + boundary + "--\r\n";

	header += "Content-Length: " + String(body.length() + fd.size() + bodyend.length()) + "\r\n";
	header += "\r\n";

	WiFiClient client;
	Serial.println("start upload ");
	if (!client.connect(host.c_str(), port)) {
		Serial.print("open file failed\n");
		return -1;
	}
	client.print(header);
	client.print(body);
	uint8_t *buf = (uint8_t *)malloc(4096);
	int len = 0;
	while (fd.available()) {
		int ret = fd.read(buf, 4096);
		client.write(buf, ret);
		len += ret;
		Serial.print(".");
	}
	free(buf);
	
	Serial.printf("write data len: %d\n", len);

	client.print(bodyend);
	Serial.print("response:\n");

	String response_status = client.readStringUntil('\n');

	Serial.print("code:");
	Serial.println(response_status);

	if (response_status.indexOf("200 OK") < 0) {
		Serial.print("response code is not 200\n");
		return 1;
	}else{
		Serial.print("httpUploadFile success\n");
	}

	// while (client.available()) {
	// 	uint8_t ret = client.read();
	// 	Serial.write(ret);
	// }

	Serial.println();

	return 1;
}