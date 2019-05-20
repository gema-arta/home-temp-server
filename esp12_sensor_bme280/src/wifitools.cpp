// Wi-Fi credentials.
#ifndef STASSID
#define STASSID "Rotterovi"
#define STAPSK "bertopes"
#endif

#define SENSOR_IP 192, 168, 44, 200
#define GATEWAY_IP 192, 168, 44, 1
#define MAX_CONNECT_TRIES 500

#include <ESP8266WiFi.h>
#include <chrono>

#include "wifitools.h"

bool connectToWifi(double* seconds_to_join)
{
  auto time_start = std::chrono::system_clock::now();

  WiFi.config(IPAddress(SENSOR_IP), IPAddress(GATEWAY_IP), IPAddress(255, 255, 255, 0));
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);

  int conect_tries = MAX_CONNECT_TRIES;

  // Wait while connecting to network.
  while (WiFi.status() != WL_CONNECTED)
  {
    if (conect_tries-- <= 0)
    {
      Serial.println(F("\nConnection to Wi-Fi failed!!!"));
      return false;
    }

    Serial.print('.');
    delay(10);
  }

  auto time_end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = time_end - time_start;

  *seconds_to_join = elapsed_seconds.count();

  Serial.printf("\nConnected to Wi-Fi! Established in %f seconds. IP address: '%s'\n",
                *seconds_to_join,
                WiFi.localIP().toString().c_str());
  return true;
}