#pragma once

#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/udp.h>
#include <bpf/bpf_endian.h>

#define memcpy __builtin_memcpy

#define DNS_PORT 53

#define IS_IPV4(P) P == __bpf_htons(ETH_P_IP)
#define IS_IPV6(P) P == __bpf_htons(ETH_P_IPV6)
#define IS_VLAN(P) P == __bpf_htons(ETH_P_8021Q) || P == __bpf_htons(ETH_P_8021AD)

#define IS_PORT(U, P) (__bpf_htons(U->source) == P || __bpf_htons(U->dest) == P)
#define IS_DNS(U)     IS_PORT(U, DNS_PORT)

struct vlanhdr {
	__u16 tci;
	__u16 encap_proto;
};

struct dnshdr {
	__u16 id;
	union {
		struct {
			__u8  rd     : 1;
			__u8  tc     : 1;
			__u8  aa     : 1;
			__u8  opcode : 4;
			__u8  qr     : 1;

			__u8  rcode  : 4;
			__u8  cd     : 1;
			__u8  ad     : 1;
			__u8  z      : 1;
			__u8  ra     : 1;
		}        as_bits_and_pieces;
		__u16 as_value;
	} flags;
	__u16 qdcount;
	__u16 ancount;
	__u16 nscount;
	__u16 arcount;
};

struct cursor
{
    void *pos;
    void *end;
};

static __always_inline
void cursor_init(struct cursor *c, struct xdp_md *ctx)
{
    c->end = (void *)(long)ctx->data_end;
    c->pos = (void *)(long)ctx->data;
}


#define PARSE_FUNC_DECLARATION(STRUCT)			    \
static __always_inline                              \
struct STRUCT *parse_ ## STRUCT (struct cursor *c)	\
{							                        \
	struct STRUCT *ret = c->pos;			        \
	if (c->pos + sizeof(struct STRUCT) > c->end)	\
		return 0;				                    \
	c->pos += sizeof(struct STRUCT);		        \
	return ret;					                    \
}

PARSE_FUNC_DECLARATION(ethhdr)
PARSE_FUNC_DECLARATION(vlanhdr)
PARSE_FUNC_DECLARATION(iphdr)
PARSE_FUNC_DECLARATION(ipv6hdr)
PARSE_FUNC_DECLARATION(udphdr)
PARSE_FUNC_DECLARATION(dnshdr)

static __always_inline
struct ethhdr *parse_eth(struct cursor *c, __u16 *eth_proto)
{
	struct ethhdr  *eth;

	if (!(eth = parse_ethhdr(c)))
		return 0;

	*eth_proto = eth->h_proto;
	if (IS_VLAN(*eth_proto)) {
		struct vlanhdr *vlan;

		if (!(vlan = parse_vlanhdr(c)))
			return 0;

		*eth_proto = vlan->encap_proto;
		if (IS_VLAN(*eth_proto)) {
			if (!(vlan = parse_vlanhdr(c)))
				return 0;

			*eth_proto = vlan->encap_proto;
		}
	}
	return eth;
}
