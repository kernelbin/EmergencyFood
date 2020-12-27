/*
 * Simple MD5 implementation
 * Source: https://github.com/pod32g/MD5
 */



#pragma once

#include <stdint.h>

extern "C" void md5(const uint8_t *initial_msg, size_t initial_len, uint8_t *digest);
