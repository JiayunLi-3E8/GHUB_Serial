#ifndef CRC32_H
#define CRC32_H

#include <cstdint>

class Crc32
{
public:
    // 计算 - 逐字 (每次处理1个uint32_t，length表示word数量)
    static uint32_t calculate(const uint32_t* data, size_t length);

private:
    static constexpr uint32_t init_value = 0xFFFFFFFF;
    static constexpr uint32_t polynomial = 0x04C11DB7;

    static uint32_t update(uint32_t crc, uint32_t data);
};

#endif // CRC32_H
