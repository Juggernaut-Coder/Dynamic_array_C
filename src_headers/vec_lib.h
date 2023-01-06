#pragma once /* VECTOR LIBRARY */
#include "vector.h"
#include "../../Macros/custom_types/custom_types.h"

/* Vector result handler */
typedef int vec_bool;

/* Integer types */
typedef vector(int8_t)  i8_vec;
typedef vector(int16_t) i16_vec;
typedef vector(int32_t) i32_vec;
typedef vector(int64_t) i64_vec;

/* Unsigned integer types */
typedef vector(uint8_t)  u8_vec;
typedef vector(uint16_t) u16_vec;
typedef vector(uint32_t) u32_vec;
typedef vector(uint64_t) u64_vec;

/* Float vector */
typedef vector(float)  f32_vec;
typedef vector(double) f64_vec;

/* Char vector */
typedef vector(char)          char_vec;
typedef vector(unsigned char) uchar_vec;

/* String vector */
typedef vector(const char*) str_vec;

/* Void vector */
typedef vector(void*) void_vec;


/** Q sort functions for i64, f64, u64, char and String 
 * a - is acending
 * d - is decending
*/
/* i64 */
static inline int cmp_i64_a(const void *a, const void *b) {
    return ( *(i64*)a  -  *(i64*)b );    
}
static inline int cmp_i64_d(const void *a, const void *b) {
    return ( *(i64*)b  -  *(i64*)a );    
}
/* u64 */
static inline int cmp_u64_a(const void *a, const void *b) {
    return ( *(u64*)a  -  *(u64*)b );    
}
static inline int cmp_u64_d(const void *a, const void *b) {
    return ( *(u64*)b  -  *(u64*)a );  
}  
/* f64 */
static inline int cmp_f64_a(const void *a, const void *b) {
    return ( *(f64*)a  -  *(f64*)b );    
}
static inline int cmp_f64_d(const void *a, const void *b) {
    return ( *(f64*)b  -  *(f64*)a );    
}
/* char */
static inline int cmp_char_a(const void *a, const void *b) {
    return ( *(char*)a  -  *(char*)b );    
}
static inline int cmp_char_d(const void *a, const void *b) {
    return ( *(char*)b  -  *(char*)a );    
}
/* String */
static inline int cmp_str_a(const void *a, const void *b) {
    return strcmp( *(String*)a,  *(String*)b );    
}
static inline int cmp_str_d(const void *a, const void *b) {
    return strcmp( *(String*)b,  *(String*)a );    
}