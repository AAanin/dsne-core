# DSNE Core: Version-1 Wire Schema

Reference binary schema for **Deterministic Symbolic-Neural Engine (DSNE): Zero-Copy Directed Acyclic Graph Compilation for Sub-Millisecond Edge Inference**, by Mahamodul Anin, Royalx LLC.

This artifact defines the C99 wire representation from Table II of the manuscript. It provides schema declarations and compile-time layout checks. The declarations are not a complete execution runtime or an SMT verification implementation.

## Files

- `include/dsne.h`: exact 64-byte container header, 32-byte node records, opcodes, field offsets, and little-endian scalar accessors.
- `tests/schema_layout.c`: a small executable check of scalar encoding and representative layout offsets. The header itself checks every field offset at compile time.

## Encoding

All multibyte integers are unsigned and little-endian. Struct fields use byte arrays to avoid host-endian and alignment dependencies. Do not cast untrusted buffers to native integer structs. The accessor functions operate on bytes; callers must establish that four bytes are available before calling them.

The header begins with ASCII `DSNE`, version 1, and the total file length. Nodes start at byte 64 with stride 32. The payload follows at `64 + 32 * node_count`. Root and successor indices refer to the node array. Every test successor must satisfy `source_index < successor_index < node_count`.

Opcodes are 0 (reject), 1 (action), 2 (unsigned equality), and 3 (unsigned less-than). Source selectors are 0 (input) and 1 (state). Test nodes use the source, feature index, immediate, and both successors. Action nodes use the action identifier and payload slice. Unused fields and reserved bytes must be zero.

## Validation contract

Before execution, validate magic, version, file length, dimensions, action-library version, reserved bytes, all record ranges, feature and action identifiers, and both successors of every test node. For a slice `(offset, length)` inside a buffer of size `L`, require `offset <= L` and `length <= L - offset` before pointer construction. Check multiplication and addition for overflow. Payload end must equal file end. Target object-size limits also apply.

Structural validation is separate from proving application invariants and action contracts. The graph and backing storage must remain immutable and alive for the lifetime of all borrowed views. A read-only mapping alone does not prevent changes to its backing file.

## Build the schema check

With a C99 compiler, from the repository root:

```sh
cc -std=c99 -Wall -Wextra -Werror -pedantic -Iinclude tests/schema_layout.c -o schema_layout
./schema_layout
```

Compilation verifies the sizes and every field offset through C99-compatible static assertions. The executable checks little-endian round trips and representative offsets. These checks establish layout conformance, not formal verification of a runtime.

## Scope of resource claims

The manuscript's 3,833,920-byte configuration is structural accounting with declared code and workspace allowances. Its latency figures are parametric cost-model results. This repository does not present those figures as hardware benchmark measurements.

## Author

Mahamodul Anin — Royalx LLC — anin.bd@gmail.com
