#pragma once
#include <endian.h>

#include <cstdint>
#include <type_traits>

namespace util
{
// Generic host-to-big-endian
template <typename T> inline T htobe_integral(T v);

// Generic big-endian-to-host
template <typename T> inline T betoh_integral(T v);

// --- Unsigned specializations ---
template <> inline uint16_t htobe_integral<uint16_t>(uint16_t v)
{
    return htobe16(v);
}
template <> inline uint32_t htobe_integral<uint32_t>(uint32_t v)
{
    return htobe32(v);
}
template <> inline uint64_t htobe_integral<uint64_t>(uint64_t v)
{
    return htobe64(v);
}

template <> inline uint16_t betoh_integral<uint16_t>(uint16_t v)
{
    return be16toh(v);
}
template <> inline uint32_t betoh_integral<uint32_t>(uint32_t v)
{
    return be32toh(v);
}
template <> inline uint64_t betoh_integral<uint64_t>(uint64_t v)
{
    return be64toh(v);
}

// --- Signed specializations ---
template <> inline int16_t htobe_integral<int16_t>(int16_t v)
{
    auto u = static_cast<uint16_t>(v);
    return static_cast<int16_t>(htobe_integral(u));
}

template <> inline int32_t htobe_integral<int32_t>(int32_t v)
{
    auto u = static_cast<uint32_t>(v);
    return static_cast<int32_t>(htobe_integral(u));
}

template <> inline int64_t htobe_integral<int64_t>(int64_t v)
{
    auto u = static_cast<uint64_t>(v);
    return static_cast<int64_t>(htobe_integral(u));
}

template <> inline int16_t betoh_integral<int16_t>(int16_t v)
{
    auto u = static_cast<uint16_t>(v);
    return static_cast<int16_t>(betoh_integral(u));
}

template <> inline int32_t betoh_integral<int32_t>(int32_t v)
{
    auto u = static_cast<uint32_t>(v);
    return static_cast<int32_t>(betoh_integral(u));
}

template <> inline int64_t betoh_integral<int64_t>(int64_t v)
{
    auto u = static_cast<uint64_t>(v);
    return static_cast<int64_t>(betoh_integral(u));
}
// For enums
template <typename T>
inline T betoh_integral(T v) {
    auto u = static_cast<uint32_t>(v);
    return static_cast<T>(be32toh(u));
}
} // namespace util
