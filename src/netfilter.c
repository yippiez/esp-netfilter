#include "netfilter.h"
#include "lwip/netif.h"

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
