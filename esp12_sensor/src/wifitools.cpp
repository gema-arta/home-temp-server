// Wi-Fi credentials.
#ifndef STASSID
#define STASSID "u_pasovcu"
#define STAPSK "pasovec1"
#endif

#define MAX_CONNECT_TRIES 500

#include <ESP8266WiFi.h>
#include <chrono>

#include "wifitools.h"

bool connectToWifi()
{
  auto time_start = std::chrono::system_clock::now();

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

  Serial.printf("\nConnected to Wi-Fi! Established in %f seconds. IP address: '%s'\n",
                elapsed_seconds.count(),
                WiFi.localIP().toString().c_str());
  return true;
}