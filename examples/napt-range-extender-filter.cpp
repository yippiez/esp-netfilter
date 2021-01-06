#define STASSID "SSID"
#define STAPSK  "PASS"

#define APSSID  "TESTEST"

#include <ESP8266WiFi.h>
#include <IPAddress.h>
#include <dhcpserver.h>
#include <lwip/napt.h>

extern "C"{
  #include "netfilter.h"
}

/*============  SOFTAP ===========*/
IPAddress SOFTAP_LOCAL_IP(192,168,2,1);
#define SOFTAP_GATEWAY_IP SOFTAP_LOCAL_IP
IPAddress SOFTAP_SUBNET_IP(255,255,255,0);

struct netfilter n; // <-- GLOBAL

err_t test_link_output(netif *netif, pbuf *p){

  Serial.println("Function executed");
  return netfilter_out(n, netif, p);

}

void setup() {

  Serial.begin(115200);

  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(STASSID, STAPSK);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }

  WiFi.softAPConfig(SOFTAP_LOCAL_IP, SOFTAP_GATEWAY_IP, SOFTAP_SUBNET_IP);

  #ifdef APPSK
    WiFi.softAP(APSSID, APPSK);
  #else
    WiFi.softAP(APSSID);
  #endif

  /* NAPT */
  Serial.println("Enable nat");

  ip_napt_init(IP_NAPT_MAX, IP_PORTMAP_MAX);
  ip_napt_enable_no(1, 1); // enable napt for ap interface

  dhcps_set_dns(0, WiFi.dnsIP(0));
  dhcps_set_dns(1, WiFi.dnsIP(1));


  n = netfilter_new(test_link_output);
  netfilter_start(&n, NETIF_AP);

  Serial.printf("\n%p : %p function pointers old vs new linkoutput", n.old_output_fn, n.new_output_fn);

}

void loop() {
  delay(500);
}
