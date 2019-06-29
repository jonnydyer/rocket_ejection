
/*
 * serialize.c
 * Source file for a library to serialize data.
 *
 * Author: Jonny Dyer
 * Date: 4/21/18
 */

#include <stdint.h>
#include "serialize.h"

int16_t serialize_uint8(cBuffer *buff, uint8_t *val){

    return bufferAddToEnd(buff, *val);
}

int16_t serialize_uint16(cBuffer *buff, uint16_t *val){

    bufferAddToEnd(buff, (*val & 0xFF00)>>8);
    return bufferAddToEnd(buff, (*val & 0x00FF));
}

int16_t serialize_uint32(cBuffer *buff, uint32_t *val){

    bufferAddToEnd(buff, (*val & 0xFF000000)>>24);
    bufferAddToEnd(buff, (*val & 0x00FF0000)>>16);
    bufferAddToEnd(buff, (*val & 0x0000FF00)>>8);
    return bufferAddToEnd(buff, (*val & 0x000000FF));
}

int16_t serialize_int8(cBuffer *buff, int8_t *val){
    return serialize_uint8(buff, (uint8_t *)val);
}

int16_t serialize_int16(cBuffer *buff, int16_t *val){
    return serialize_uint16(buff, (uint16_t *)val);
}

int16_t serialize_int32(cBuffer *buff, int32_t *val){
    return serialize_uint32(buff, (uint32_t *)val);
}

int16_t serialize_float32(cBuffer *buff, float *val){
    return serialize_uint32(buff, (uint32_t *)val);
}


