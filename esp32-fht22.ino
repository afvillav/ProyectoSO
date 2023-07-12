/**
   ESP32 + DHT22 Example for Wokwi
   
   https://wokwi.com/arduino/projects/322410731508073042
*/
#include <lwip/sockets.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "DHTesp.h"

#define NGROK_AUTH_TOKEN "2SRmzsZPCByqxyVixwAhBdKkuBA_3kmZPBZpowDtFrhHkjStG" 
#define NGROK_ADDRESS "0.tcp.sa.ngrok.io:16152"
#define NGROK_PORT 80

const int DHT_PIN = 15;

DHTesp dhtSensor;

void setup() {
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
}

void loop() {
  char datos[50];
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  String temperature = String(data.temperature,2);
  String humidity = String(data.humidity,1);
  sprintf(datos, "Temperatura: %.2f°C, Humedad: %.2f%%", temperature, humidity);
  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  Serial.println("---");
  

  int sockfd;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    printf("Error al abrir el socket\n");
    return;
  }

  memset((char *)&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(NGROK_PORT);

  if (inet_pton(AF_INET, NGROK_ADDRESS, &(serv_addr.sin_addr)) <= 0) {
    printf("Error al convertir la dirección\n");
    return;
  }
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("Error al establecer la conexión\n");
    return;
  }

  // Enviar los datos al servidor a través del socket
  int n = write(sockfd, datos, strlen(datos));
  if (n < 0) {
    printf("Error al escribir en el socket\n");
    return;
  }

  close(sockfd);
  delay(50000); // Wait for a new reading from the sensor (DHT22 has ~0.5Hz sample rate)
}
