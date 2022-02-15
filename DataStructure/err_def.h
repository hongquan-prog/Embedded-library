//
// Created by lhq on 2022/2/15.
//

#ifndef DATA_STRUCTURE_ERR_DEF_H
#define DATA_STRUCTURE_ERR_DEF_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef int32_t err_t;

// 模块数量
#define MODULE_COUNT 1
// 将最高位设为1构成错误码
#define ERR_CONSTRUCT(_error_) ((_error_) | (1 << 31))
// 根据模块标识确认第一个错误标识的值
#define ERR_CODE_BEGIN(_module_) ((_module_) << 16)
// 获取模块ID
#define ERR_GET_MODULE_ID(_error_) (((_error_) >> 16) & 0x7fff)
// 获取错误标识在数组中的下标
#define ERR_GET_ERROR_INDEX(_error_) ((_error_)&0xffff)
// 获取错误标识
#define ERR_GET_ERROR_CODE(_error_) ((_error_)&0x7fffffff)

#endif //ERR_DEF_H
