#include "netfilter.h"
#include "lwip/netif.h"
#include "lwip/etharp.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/prot/ethernet.h"
#include "lwip/prot/dns.h"

struct netfilter netfilter_new(netif_linkoutput_fn func){

    struct netfilter n;
    n.new_output_fn = func;
    return n;

}


int netfilter_start(struct netfilter *n, NetifType_t type) {

    struct netif *netif = NULL;

    for (netif = netif_list; netif; netif = netif->next) {

        if (netif->num == type)
            break;
    
    }

    if(!netif)
        return -1;

    n->old_output_fn = netif->linkoutput;
    netif->linkoutput = n->new_output_fn;

    return 1;
}

err_t netfilter_out(struct netfilter n, struct netif* netif,struct pbuf *p){
    
    return (* n.old_output_fn)(netif, p);

}


struct eth_hdr* get_ethernet_header(struct pbuf *p, uint16_t offset){
    
    // not an ethernet packet
    if(p->len < offset + sizeof(struct eth_hdr))
        return NULL;

    struct eth_hdr* eth = (struct eth_hdr*)( (char *)p->payload + offset );
    
    return eth;
}

struct ip_hdr*  get_ip_header(struct pbuf *p, uint16_t offset){

    if(p->len < offset + sizeof(struct ip_hdr))
        return NULL; // ip truncated

    struct ip_hdr* ip_header = (struct ip_hdr*)( (char *)p->payload + offset );

    return ip_header;
}

struct udp_hdr* get_udp_header(struct pbuf *p, uint16_t offset){

    if(p->len < offset + sizeof(struct udp_hdr))
        return NULL;

    struct udp_hdr* udp_header = (struct udp_hdr*)( (char *)p->payload + offset );

    return udp_header;
}

struct dns_hdr* get_dns_header(struct pbuf *p, uint16_t offset){
    
    if(p->len < offset + SIZEOF_DNS_HDR)
        return NULL;

    struct dns_hdr* dns_header = (struct dns_hdr*)( (char *)p->payload + offset );

    return dns_header;

}

