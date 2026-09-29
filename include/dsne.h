#ifndef DSNE_H_INCLUDED
#define DSNE_H_INCLUDED

/* DSNE version-1 wire schema, corresponding to manuscript Table II.
 * All integers are little-endian byte arrays, not native integer fields.
 * Accessors require a caller-validated four-byte range.
 */
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#if CHAR_BIT != 8
#error "DSNE requires eight-bit bytes"
#endif

#define DSNE_VERSION UINT32_C(1)
#define DSNE_HEADER_SIZE 64u
#define DSNE_NODE_SIZE 32u
#define DSNE_NODE_ARRAY_OFFSET 64u
#define DSNE_MAGIC "DSNE"

enum dsne_opcode {
    DSNE_OP_REJECT = 0,
    DSNE_OP_ACTION = 1,
    DSNE_OP_EQUAL = 2,
    DSNE_OP_LESS_THAN = 3
};

enum dsne_feature_source {
    DSNE_SOURCE_INPUT = 0,
    DSNE_SOURCE_STATE = 1
};

typedef struct dsne_header_wire {
    unsigned char magic[4];                  /*  0 */
    unsigned char version_le[4];             /*  4 */
    unsigned char file_length_le[4];         /*  8 */
    unsigned char node_count_le[4];          /* 12 */
    unsigned char root_index_le[4];          /* 16 */
    unsigned char node_offset_le[4];         /* 20 */
    unsigned char node_stride_le[4];         /* 24 */
    unsigned char payload_offset_le[4];      /* 28 */
    unsigned char payload_length_le[4];      /* 32 */
    unsigned char input_word_count_le[4];    /* 36 */
    unsigned char state_word_count_le[4];    /* 40 */
    unsigned char action_library_version_le[4]; /* 44 */
    unsigned char reserved[16];              /* 48 */
} dsne_header_wire;

typedef struct dsne_node_wire {
    unsigned char opcode;                   /*  0 */
    unsigned char feature_source;           /*  1 */
    unsigned char reserved[2];              /*  2 */
    unsigned char feature_index_le[4];      /*  4 */
    unsigned char immediate_le[4];          /*  8 */
    unsigned char false_successor_le[4];    /* 12 */
    unsigned char true_successor_le[4];     /* 16 */
    unsigned char action_id_le[4];          /* 20 */
    unsigned char payload_offset_le[4];     /* 24: relative to payload */
    unsigned char payload_length_le[4];     /* 28 */
} dsne_node_wire;

/* C99 compile-time assertions: no packing extensions are needed. */
#define DSNE_ASSERT(name, condition) \
    typedef char dsne_assert_##name[(condition) ? 1 : -1]
#define DSNE_OFFSET(type, field, value) \
    DSNE_ASSERT(type##_##field, offsetof(type, field) == (value))

DSNE_ASSERT(header_size, sizeof(dsne_header_wire) == DSNE_HEADER_SIZE);
DSNE_ASSERT(node_size, sizeof(dsne_node_wire) == DSNE_NODE_SIZE);
DSNE_OFFSET(dsne_header_wire, magic, 0);
DSNE_OFFSET(dsne_header_wire, version_le, 4);
DSNE_OFFSET(dsne_header_wire, file_length_le, 8);
DSNE_OFFSET(dsne_header_wire, node_count_le, 12);
DSNE_OFFSET(dsne_header_wire, root_index_le, 16);
DSNE_OFFSET(dsne_header_wire, node_offset_le, 20);
DSNE_OFFSET(dsne_header_wire, node_stride_le, 24);
DSNE_OFFSET(dsne_header_wire, payload_offset_le, 28);
DSNE_OFFSET(dsne_header_wire, payload_length_le, 32);
DSNE_OFFSET(dsne_header_wire, input_word_count_le, 36);
DSNE_OFFSET(dsne_header_wire, state_word_count_le, 40);
DSNE_OFFSET(dsne_header_wire, action_library_version_le, 44);
DSNE_OFFSET(dsne_header_wire, reserved, 48);
DSNE_OFFSET(dsne_node_wire, opcode, 0);
DSNE_OFFSET(dsne_node_wire, feature_source, 1);
DSNE_OFFSET(dsne_node_wire, reserved, 2);
DSNE_OFFSET(dsne_node_wire, feature_index_le, 4);
DSNE_OFFSET(dsne_node_wire, immediate_le, 8);
DSNE_OFFSET(dsne_node_wire, false_successor_le, 12);
DSNE_OFFSET(dsne_node_wire, true_successor_le, 16);
DSNE_OFFSET(dsne_node_wire, action_id_le, 20);
DSNE_OFFSET(dsne_node_wire, payload_offset_le, 24);
DSNE_OFFSET(dsne_node_wire, payload_length_le, 28);

#undef DSNE_OFFSET
#undef DSNE_ASSERT

static inline uint32_t dsne_load_u32_le(const unsigned char *p)
{
    return (uint32_t)p[0]
        | ((uint32_t)p[1] << 8)
        | ((uint32_t)p[2] << 16)
        | ((uint32_t)p[3] << 24);
}

static inline void dsne_store_u32_le(unsigned char *p, uint32_t value)
{
    p[0] = (unsigned char)(value & UINT32_C(255));
    p[1] = (unsigned char)((value >> 8) & UINT32_C(255));
    p[2] = (unsigned char)((value >> 16) & UINT32_C(255));
    p[3] = (unsigned char)((value >> 24) & UINT32_C(255));
}

#endif /* DSNE_H_INCLUDED */
