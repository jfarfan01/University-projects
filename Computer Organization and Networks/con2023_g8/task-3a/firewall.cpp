#include "framework/userspace.h"
#include <cstdio>
#include <cinttypes>
#include <arpa/inet.h>

int outgoing_ICMP_PE = 0;
int incoming_ICMP_PE = 0;
int port80_count = 0;

struct IPv4Frame 
{
    uint8_t ip_hl_v;

    uint8_t ip_tos;
    unsigned short ip_len;
    unsigned short ip_id;
    unsigned short ip_off;

    uint8_t ip_ttl;
    uint8_t ip_p;
    unsigned short ip_sum;
    uint8_t ip_src[4], ip_dst[4];

    uint8_t options_and_payload[];
}__attribute__((__packed__));

static void hexdump(uint8_t const* data, size_t size)
{
    for (size_t bi=0; bi<size; bi += 16)
    {
        // hex values
        for (size_t di=0; di<16; ++di)
        {
            size_t i = bi+di;
            if (i < size)
                printf("%02X ", data[i]);
            else
                printf("   ");
        }
        printf("        ");
        
        // printable characters
        for (size_t di=0; di<16; ++di)
        {
            size_t i = bi+di;
            if (i < size)
            {
                uint8_t c = data[i];
                if ((32 <= c) && (c <= 126))
                    printf(" %c ", c);
                else
                    printf(" . ");
            }
            else
                printf("  ");
        }
        printf("\n");
    }
}


FilterResponse processPacket(uint8_t const* packetData, size_t packetSize, Direction direction)
{
    struct _PNAP { ~_PNAP() { printf("\n"); } } printNewlineAfterPacket;
    
    switch (direction)
    {
        case INCOMING: printf("Packet (size %zu) received:\n", packetSize); break;
        case OUTGOING: printf("Packet (size %zu) sent:\n", packetSize); break;
    }
    
    hexdump(packetData, packetSize);

    FilterResponse response = ACCEPT;

#define IP_FMT "%hhu.%hhu.%hhu.%hhu"
#define IP_ARG(a) ((a)[0]), ((a)[1]), ((a)[2]), ((a)[3])
#define IP_ARG2(a) ((a)[0]), ((a)[1])
#define IP_ARG3(a,b) ((a)[2+b]), ((a)[3+b])
#define FIRST_BYTE(a) ((a)[0])
#define SECOND_BYTE(a) ((a)[1])
#define THIRD_BYTE(a) ((a)[2])
#define FOURTH_BYTE(a) ((a)[3])
#define FIRST_PLUS_OFFSET(a,b) ((a)[0+b])
#define IP_ARG4(a,b) ((a)[2+b] << 8) | ((a)[3+b])



// TODO: Implement this method!
    auto &ip = *reinterpret_cast<const IPv4Frame*>(packetData);
    printf("  IP source: " IP_FMT " IP destination: " IP_FMT "\n", IP_ARG(ip.ip_src), IP_ARG(ip.ip_dst));
    printf("offset and flag %d \n",ntohs(ip.ip_off));
    printf("Protocol: %d \n", ip.ip_p);
    int header_length = 15 & ip.ip_hl_v;
    printf("Header length: %d\n", header_length);
    int offset = 0;
    if(header_length> 5)
    {
        offset = (header_length - 5) * 4;
    }
    auto inter = (IP_ARG4(ip.options_and_payload, offset));
    printf("  port:  %d   \n",inter);//offset (from header size) not taken care of here
    if(ip.ip_p == 1)
    {
        printf("ICMP type:%d \n", FIRST_BYTE(ip.options_and_payload));
    }

	if(FIRST_BYTE(ip.ip_dst) == 8 && SECOND_BYTE(ip.ip_dst) == 8 && THIRD_BYTE(ip.ip_dst) == 4 && FOURTH_BYTE(ip.ip_dst) == 4)
	{
		response = DROP;
		return response;
	}

    if(FIRST_BYTE(ip.ip_dst) == 127 || FIRST_BYTE(ip.ip_src) == 127)
    {
        response = ACCEPT;
        return response;
    }

    if((ntohs(ip.ip_off) & 8191) > 0) //frag offset
    {
        response = DROP;
        return response;
    }

     if((ntohs(ip.ip_off) & 8192)) //MF
    {
        response = DROP;
        return response;
    }


    // if(ntohs(ip.ip_off) != 16384)
    // {
    //     response = DROP;
    //     return response;
    // }

    if(ip.ip_p != 1 && ip.ip_p != 6 && ip.ip_p != 17) //rule 4
    {
        response = DROP;
        return response;
    }


    if(ip.ip_p == 1 && FIRST_PLUS_OFFSET(ip.options_and_payload,offset) == 8 && direction == OUTGOING)
    {   
        if(FIRST_BYTE(ip.ip_dst) == 10 || (FIRST_BYTE(ip.ip_dst) == 192 && SECOND_BYTE(ip.ip_dst) == 168) || //first and third subnets
        (FIRST_BYTE(ip.ip_dst) == 172 && (SECOND_BYTE(ip.ip_dst) >= 16 && SECOND_BYTE(ip.ip_dst) <= 31)))
        {
            outgoing_ICMP_PE ++;
            // printf("we on that bullshit number: %d\n",outgoing_ICMP_PE);
            if(outgoing_ICMP_PE % 3 ==  0 && outgoing_ICMP_PE > 0)
            {
                // printf("this is happening\n");
                response = DROP;
                return response;
            }
        }
    }

    if(ip.ip_p == 1 && FIRST_PLUS_OFFSET(ip.options_and_payload, offset) == 8 && direction == INCOMING)
    {   
        if(FIRST_BYTE(ip.ip_src) == 10 || (FIRST_BYTE(ip.ip_src) == 192 && SECOND_BYTE(ip.ip_src) == 168) || //first and third subnets
        (FIRST_BYTE(ip.ip_src) == 172 && (SECOND_BYTE(ip.ip_src) >= 16 && SECOND_BYTE(ip.ip_src) <= 31)))
        {
            incoming_ICMP_PE ++;
            // printf("we on that bullshit number: %d\n",incoming_ICMP_PE);
            if(incoming_ICMP_PE % 4 ==  0 && outgoing_ICMP_PE > 0)
            {
                // printf("this is happening\n");
                response = DROP;
                return response;
            }
        }
    }

    if(ip.ip_p == 1)
    {
        response = ACCEPT;
        return response;
    }

    if(direction == OUTGOING && inter == 80)
    {
        port80_count++;
        if(port80_count % 2 == 0 && port80_count > 0)
        {
            response = DROP;
            return response;
        }
    }

    if((direction == OUTGOING) && inter == 443)
    {
        response = DROP;
        return response;
    }


    // printf("accpeted\n");
    return response;
}
