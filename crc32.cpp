#include "crc32.h"

uint32_t Crc32::calculate(const uint32_t *data, size_t length){
    // 1. 初始化为复位值 (0xFFFFFFFF)
    uint32_t crc = init_value;

    // 2. 依次处理每个 32 位字 (模拟硬件逐个写入 CRC_DR)
    for (size_t i = 0; i < length; i++) {
        // 注意：此处直接读取数组元素意味着使用平台的字节序解释内存中的值。
        // 如果 data 中的字节序与平台不同，需要在这里或之前转换每个字。
        crc = update(crc, data[i]);
    }

    // 3. 返回最终计算值 (等效最后一次 CRC 计算后的 CRC_DR 读取)
    return crc;
}

uint32_t Crc32::update(uint32_t crc, uint32_t data) {
    // 1. 将输入数据与当前 CRC 进行异或 (XOR, Reflect in?)
    crc ^= data;

    // 2. 处理 32 位 (4 字节) - 每次处理 1 bit (bit-by-bit 参考实现)
    for (int i = 0; i < 32; i++) {
        if (crc & 0x80000000) { // 检查最高位 (MSb) 是否为 1
            crc = (crc << 1) ^ polynomial; // 左移一位并异或多项式
        } else {
            crc = (crc << 1); // 左移一位
        }
    }
    return crc; // 结果直接输出 (等效硬件读操作)
}
