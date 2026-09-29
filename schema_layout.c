#include "dsne.h"
#include <assert.h>

int main(void)
{
    unsigned char bytes[4];
    dsne_header_wire header = {0};
    dsne_node_wire node = {0};
    const uint32_t values[] = {
        UINT32_C(0), UINT32_C(1), UINT32_C(255),
        UINT32_C(256), UINT32_C(0x12345678), UINT32_MAX
    };
    size_t i;
    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        dsne_store_u32_le(bytes, values[i]);
        assert(dsne_load_u32_le(bytes) == values[i]);
    }
    dsne_store_u32_le(bytes, UINT32_C(0x12345678));
    assert(bytes[0] == 0x78 && bytes[1] == 0x56);
    assert(bytes[2] == 0x34 && bytes[3] == 0x12);
    dsne_store_u32_le(header.node_stride_le, DSNE_NODE_SIZE);
    assert(((unsigned char *)&header)[24] == 32);
    dsne_store_u32_le(node.true_successor_le, UINT32_C(7));
    assert(((unsigned char *)&node)[16] == 7);
    return 0;
}
