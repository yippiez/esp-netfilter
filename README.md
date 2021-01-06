# ESP netfilter

esp netfilter class that allows blocking/changing/reading any packets that goes through
any of the interfaces.

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

