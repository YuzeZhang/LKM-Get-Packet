#include "lkm.h"
#include "lkm_mmap.h"

// dest ip from user
unsigned int filter_ip = 0; // 2887057415

unsigned int count = 0;

// Convert a string IP addr to a dotted decimal format (little-endian)
unsigned int ip_atoi(char *ip_str)
{
    unsigned int val = 0, part = 0;
    int i = 0;
    char c;
    for (i = 0; i < 4; ++i)
    {
        part = 0;
        while ((c = *ip_str++) != '\0' && c != '.')
        {
            if (c < '0' || c > '9')
                return -1;
            part = part * 10 + (c - '0');
        }
        if (part > 255)
            return -1;
        val = ((val << 8) | part);
        if (i == 3)
        {
            if (c != '\0')
                return -1;
        }
        else
        {
            if (c == '\0')
                return -1;
        }
    }
    return val;
}

// Convert a dotted decimal IP addr to a string
void ip_itoa(char *ip_str, unsigned int ip_num)
{
    unsigned char *p = (unsigned char *)(&ip_num);
    sprintf(ip_str, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);
}

static struct nf_hook_ops nfho = {
    .hook = getPacketInfo,
    .pf = PF_INET,
    .hooknum = NF_INET_LOCAL_OUT,
    .priority = NF_IP_PRI_FIRST,
};

void sendPacketInfo(char *packetInfo)
{
    // printk("%s\n",packetInfo);
    if (infop->size + strlen(packetInfo) < infop->cap)
    {
        strcat(infop->data, packetInfo);
        infop->size += strlen(packetInfo);
    }
    else
    {
        unsigned char *tmp = malloc_reserved_mem(infop->cap * 10);
        strcpy(tmp, infop->data);
        kfree(infop->data);
        infop->data = tmp;
        sendPacketInfo(packetInfo);
    }
}

// Get the info of the specified IP address
unsigned int getPacketInfo(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    struct iphdr *iph = ip_hdr(skb);
    struct tcphdr *tcph;
    struct udphdr *udph;
    int header = 0;
    char packetInfo[PACKET_INFO_LEN] = {0};
    if (MAX_IP_COUNT == 0 || (count < MAX_IP_COUNT && ntohl(iph->saddr) == filter_ip))
    {
        count++;
        // printk("Found a packet with the same srcIP as the user request!\n");
        // printk("==========5-Tuple=========\n");
        // printk("srcIP: %u.%u.%u.%u\n", NIPQUAD(iph->saddr));
        // printk("dstIP: %u.%u.%u.%u\n", NIPQUAD(iph->daddr));
        // Determine the transport layer protocol, currently only TCP and UDP are supported
        if (likely(iph->protocol == IPPROTO_TCP))
        {
            tcph = tcp_hdr(skb);
            // skb has data
            if (skb->len - header > 0)
            {
                // printk("srcPORT:%d\n", ntohs(tcph->source));
                // printk("dstPORT:%d\n", ntohs(tcph->dest));
                // printk("PROTOCOL:TCP\n");

                sprintf(packetInfo,
                        "srcIP:%u.%u.%u.%u\ndstIP:%u.%u.%u.%u\nsrcPORT:%d\ndstPORT:%d\nPROTOCOL:%s\n\n",
                        NIPQUAD(iph->saddr),
                        NIPQUAD(iph->daddr),
                        ntohs(tcph->source),
                        ntohs(tcph->dest),
                        "TCP");
                // send msg to user via shm
                sendPacketInfo(packetInfo);
            }
        }
        else if (likely(iph->protocol == IPPROTO_UDP))
        {
            udph = udp_hdr(skb);
            // skb has data
            if (skb->len - header > 0)
            {
                // printk("srcPORT:%d\n", ntohs(udph->source));
                // printk("dstPORT:%d\n", ntohs(udph->dest));
                // printk("PROTOCOL:UDP\n");

                sprintf(packetInfo,
                        "srcIP:%u.%u.%u.%u\ndstIP:%u.%u.%u.%u\nsrcPORT:%d\ndstPORT:%d\nPROTOCOL:%s\n\n",
                        NIPQUAD(iph->saddr),
                        NIPQUAD(iph->daddr),
                        ntohs(udph->source),
                        ntohs(udph->dest),
                        "UDP");
                // send msg to user via shm
                sendPacketInfo(packetInfo);
            }
        }
        // printk("========5-Tuple end=======\n\n");
    }
    return NF_ACCEPT;
}

static int __init lkm_init(void)
{
    if (lkm_mmap_init() != 0)
    {
        printk("mmap_init error!\n");
    }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,13,0)
    // register hook function
    nf_register_net_hook(&init_net, &nfho);
#else
        nf_register_hook(&nfho);
#endif
    printk("Start getting 5-Tuple with specific IP addr...\n");
    return 0;
}

static void __exit lkm_exit(void)
{
    // unregister hook function
    nf_unregister_net_hook(&init_net, &nfho);
    lkm_mmap_exit();
    printk("Exit mod!\n");
}

module_init(lkm_init);
module_exit(lkm_exit);
MODULE_AUTHOR("z2z23n0");
MODULE_LICENSE("GPL");
