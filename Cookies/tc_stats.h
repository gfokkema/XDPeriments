//#include <iproute2/bpf_elf.h> /* for struct bpf_elf_map */
//#include <linux/bpf.h>        /* of bpf_helpers.h */
//#include <bpf_helpers.h>      /* for SEC */
//#include "bpf-dns.h"

//struct rcode_key {
//    uint8_t rcode;
//};

struct bpf_elf_map SEC("maps") rcodes_v4 = {
    .type = BPF_MAP_TYPE_ARRAY,
    .size_key = sizeof(uint32_t),
    .size_value = sizeof(uint64_t),
    .max_elem = 16,
    .pinning = PIN_GLOBAL_NS
};

struct bpf_elf_map SEC("maps") response_sizes_v4 = {
    .type = BPF_MAP_TYPE_ARRAY,
    .size_key = sizeof(uint32_t),
    .size_value = sizeof(uint64_t),
    .max_elem = 10000,
    .pinning = PIN_GLOBAL_NS
};

struct bpf_elf_map SEC("maps") rcodes_v6 = {
    .type = BPF_MAP_TYPE_ARRAY,
    .size_key = sizeof(uint32_t),
    .size_value = sizeof(uint64_t),
    .max_elem = 16,
    .pinning = PIN_GLOBAL_NS
};

struct bpf_elf_map SEC("maps") response_sizes_v6 = {
    .type = BPF_MAP_TYPE_ARRAY,
    .size_key = sizeof(uint32_t),
    .size_value = sizeof(uint64_t),
    .max_elem = 10000,
    .pinning = PIN_GLOBAL_NS
};

struct bpf_elf_map SEC("maps") dnames_v4 = {
    .type = BPF_MAP_TYPE_LRU_PERCPU_HASH,
    //.type = BPF_MAP_TYPE_PERCPU_HASH,
    .size_key = 255, //TODO make this constant/configurable and consistent with tc_stats.c
    .size_value = sizeof(uint64_t),
    .max_elem = 1 * 1 * 1000,
    .pinning = PIN_GLOBAL_NS
};
struct bpf_elf_map SEC("maps") dnames_v6 = {
    .type = BPF_MAP_TYPE_LRU_PERCPU_HASH,
    //.type = BPF_MAP_TYPE_PERCPU_HASH,
    .size_key = 255, //TODO make this constant/configurable and consistent with tc_stats.c
    .size_value = sizeof(uint64_t),
    .max_elem = 1 * 1 * 1000,
    .pinning = PIN_GLOBAL_NS
};

struct dname {
    char full[255];
    char tld[10];
    uint8_t len;
};

struct bpf_elf_map SEC("maps") tlds = {
    .type = BPF_MAP_TYPE_PERCPU_HASH,
    .size_key = sizeof(char[9]), //TODO make this constant/configurable and consistent with tc_stats.c
    .size_value = sizeof(uint64_t),
    .max_elem = 1 * 1 * 1000,
    .pinning = PIN_GLOBAL_NS
};
