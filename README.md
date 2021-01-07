# ESP netfilter

esp netfilter is a library for both esp8266 and esp32 that allows blocking/changing/reading/filtering of any packets that goes through
any of the interfaces ( Station or AP ) regardless of any of the services that are running.

## This library can be used for:
* capture network packets
* filter/drop network packets
* modify network packets
* reinject network packets
### In conjuction with napt feature:
* dns filtering ( ad removal )
* data logging  ( you can make a small repeater that shows which packets going where )
* firewall
* honeypot
* vpn           ( maybe esp32 is fast enough for this if coded clever )

# Basic Usage
    struct netfilter n; // <-- GLOBAL
    
    // callback function
    err_t test_link_output(netif *netif, pbuf *p){
    
      Serial.println("Function executed");
      return netfilter_out(n, netif, p);
    
    }
    
    // this is in main or setup
     n = netfilter_new(test_link_output);
     netfilter_start(&n, NETIF_AP);

# How it works
in lwip there are 3 input/output functions every netif (network interface) has which are:

      netif_input_fn input; <-- called when phisycal layer recives ethernet frame
      netif_output_fn output; <-- called when ip packet want to be sent
      netif_linkoutput_fn linkoutput; <-- calls phisycal driver to output ethernet packets

the netfilter_new function stores the callback function addres in struct n
netfilter_start function changes the linkoutput function of a netif with the callback
netfilter_out sends the given data to the original linkoutput function so in the end
function order changes from:data->linkoutput to data->callback->linkoutput

# Overview of the functions

    // CALL BACK FUNCTION TYPE (err_t)(struct netif*, struct pbuf*)

    // Creates a new netfilter struct and returns it ( will be combined with netfilter_start )
    struct netfilter netfilter_new(netif_linkoutput_fn func);

    // Starts filtering by linking the functions together for the right netif.
    int netfilter_start(struct netfilter *n, NetifType_t type);


    // outputs packets to the correct function using netfilter object
    // n struct musts be the same one thats used in netfilter_new
    err_t netfilter_out(struct netfilter n, struct netif* netif, struct pbuf *p);

    // Returns the specified header from given pbuf ( offset are calculated manually )
    struct eth_hdr* get_ethernet_header(struct pbuf *p, uint16_t offset); // offset += sizeof(eth_hdr) 
    struct ip_hdr*  get_ip_header(struct pbuf *p, uint16_t offset);       // offset += IP_HL(ip_header) << 2
    struct udp_hdr* get_udp_header(struct pbuf *p, uint16_t offset);

# Installation
Download bot netfilter.h and netfilter.c and put them in the same directory as your project.