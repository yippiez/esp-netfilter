#ifndef NETFILTER_H
#define NETFILTER_H

#include "lwip/netif.h"
#include "lwip/etharp.h"
#include "lwip/prot/ethernet.h"
#include "lwip/ip4.h"
#include "lwip/udp.h"
#include "lwip/prot/dns.h"

typedef enum NetifType {

    NETIF_STA = 0,
    NETIF_AP = 1

} NetifType_t;

struct netfilter{

  netif_linkoutput_fn new_output_fn;
  netif_linkoutput_fn old_output_fn;

};

#define DNS_GET_QNAME(p, off) ( ((const char *)p->payload) + off )

/*
  netif_input_fn input; <-- function called when phisycal layer recives ethernet frame
  netif_output_fn output; <-- function called when ip packet want to be sent
  netif_linkoutput_fn linkoutput; <-- calls phisycal driver to output ethernet packets
*/

struct netfilter netfilter_new(netif_linkoutput_fn func);
int netfilter_start(struct netfilter *n, NetifType_t type);
int netfilter_stop (struct netfilter n, NetifType_t type);
err_t netfilter_out(struct netfilter n, struct netif* netif, struct pbuf *p);

struct eth_hdr* get_ethernet_header(struct pbuf *p, uint16_t offset);
struct ip_hdr*  get_ip_header(struct pbuf *p, uint16_t offset);
struct udp_hdr* get_udp_header(struct pbuf *p, uint16_t offset);
struct dns_hdr* get_dns_header(struct pbuf *p, uint16_t offset);

#endif // NETFILTER_H