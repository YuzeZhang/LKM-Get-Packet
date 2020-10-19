#pragma once

// loadable kernel module
#include <linux/module.h>
#include <linux/kernel.h>

// net filter
#include <linux/net.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/skbuff.h>
#include <linux/if_vlan.h>
#include <linux/if_ether.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <net/ip.h>
#include <net/tcp.h>
#include <net/icmp.h>
#include <net/protocol.h>

#define PACKET_INFO_LEN 100

// output dotted decimal IP addr
#define NIPQUAD(addr)                \
    ((unsigned char *)&addr)[0],     \
        ((unsigned char *)&addr)[1], \
        ((unsigned char *)&addr)[2], \
        ((unsigned char *)&addr)[3]

// Convert a string IP addr to a dotted decimal format (little-endian)
static unsigned int ip_atoi(char *ip_Str);
// Convert a dotted decimal IP addr to a string
static void ip_itoa(char *ip_str, unsigned int ip_num);
// Get the info of the specified IP address
static unsigned int getPacketInfo(void *priv, struct sk_buff *skb, const struct nf_hook_state *state);
