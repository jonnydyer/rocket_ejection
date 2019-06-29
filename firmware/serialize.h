
/*
 * serialize.h
 * Header file for a library to serialize data.
 *
 * Author: Jonny Dyer
 * Date: 4/21/18
 */

#ifndef __SERIALIZE_H__
#define __SERIALIZE_H__

#include <stdint.h>
#include <avrlib/buffer.h>

int16_t serialize_uint8(cBuffer *buff, uint8_t *val);
int16_t serialize_uint16(cBuffer *buff, uint16_t *val);
int16_t serialize_uint32(cBuffer *buff, uint32_t *val);
int16_t serialize_int8(cBuffer *buff, int8_t *val);
int16_t serialize_int16(cBuffer *buff, int16_t *val);
int16_t serialize_int32(cBuffer *buff, int32_t *val);
int16_t serialize_float32(cBuffer *buff, float *val);

#endif
