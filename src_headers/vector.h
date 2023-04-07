#pragma once /* vector.h */

/* Required standard library */
#include <stdlib.h>
#include <string.h>

/* OK or ERR */
#define OK   1
#define ERR  0

/* CAPCITY GROWTH CALCULATOR */
#define CAPACITY_GROWTH( CAP ) ( ( CAP<<1 ) - ( CAP>>1 ) )
#define CAP_ARR_GROWTH( CAP ) (CAP*1.25)
/* Macro to create vector type using typedef keyword */
#define vector(type)               \
    struct {                       \
        type *data;                \
        size_t len, cap;           \
    }

/* Vector unpacking macros */
/* v stands for vector pointer thoughout this code */
#define vlen(v)   ( (v)->len )  /* Unpacks vector length */
#define vcap(v)   ( (v)->cap )  /* Unpacks vector capacity */
#define vdat(v)   ( (v)->data ) /* Unpacks vector data */
#define vsize(v)  ( sizeof(*(v)) ) /* Gets size of the vector */
#define vdsize(v) ( sizeof( *(vdat(v)) ) ) /* Gets size of the vector data */
#define _unwrap(v) \
    ( ( void ** )( &(vdat(v)) ) ), ( &(vlen(v)) ), ( &(vcap(v)) ), ( vdsize(v) ) /* Unpacks the entire vector */

/**
 * Vector resizing static inline functions
 * @data    : Pointer to an array
 * @len     : Pointer to array length
 * @cap     : Pointer to array capacity
 * @n       : New value for capacity
 * @returns : OK for success and ERR for failure  
*/
/* Vector resize. Change capacity to n */
static inline int _vec_resize_n(void **data, size_t *len, size_t *cap, size_t memsize, size_t n) {
    (void) len;
    void *ptr = realloc(*data, n * memsize);
    if(!ptr) return ERR;
    *data = ptr;
    *cap = n;
    return OK;
}
/* Vector expand. Increase capacity to n */
static inline int _vec_expand_n(void **data, size_t *len, size_t *cap, size_t memsize, size_t n) {
    if(n>*cap) {
        return _vec_resize_n(data,len,cap,memsize,n);
    }
    return OK;
}
/* Vector expand. Expand when more space need for pushing a element */
static inline int _vec_expand(void **data, size_t *len, size_t *cap, size_t memsize) {
    if(*len + 1 > *cap) {
        size_t n = CAPACITY_GROWTH(*cap);
        return _vec_resize_n(data,len,cap,memsize,n);
    }
    return OK;
} 
/* Vector shrink capacity to length */
static inline int _vec_shrink(void **data, size_t *len, size_t *cap, size_t memsize) {
    if(*len < *cap) return _vec_resize_n(data,len,cap,memsize,*len);
    return OK;
}
/* Vector shrink capacity to n */
static inline int _vec_shrink_n(void **data, size_t *len, size_t *cap, size_t memsize, size_t n) {
    if(n<*cap) {
        int ret = _vec_resize_n(data,len,cap,memsize,n);
        if(ret && n < *len) *len = n;
        return ret; 
    }
    return OK;
}
/** 
 * Vector swap data_elements between idx1 and idx2
 * @data    : Pointer to an array
 * @len     : Pointer to array length
 * @cap     : Pointer to array capacity
 * @idx1    : size_t idx 1 
 * @idx2    : size_t idx 2
*/
static inline void _vec_swap(void **data, size_t *len, size_t *cap, size_t memsize, 
                            size_t idx1, size_t idx2) {
    (void) len;
    (void) cap;
    unsigned char *a = (unsigned char*) *data + idx1 * memsize,
                  *b = (unsigned char*) *data + idx2 * memsize,
                  tmp;
    while(memsize--) {
        tmp = *a;
        *a  = *b;
        *b  = tmp;
        a++, b++;
    }
}
/** 
 * Vector insert/splice static inline fucntion 
 * @data    : Pointer to an array
 * @len     : Pointer to array length
 * @cap     : Pointer to array capacity
 * @idx     : a size_t idx (insert/splice location)
 * @count : number of elements to intert/spice
 * @returns : OK for success and ERR for failure
*/
static inline int _vec_splice(void **data, size_t *len, size_t *cap, size_t memsize, 
                                size_t idx, size_t count) {
    if(idx + count > *len) return ERR;
    (void) cap;
    unsigned char **d = (unsigned char **) data;
    memmove( (*d) + idx * memsize,
             (*d) + (idx + count) * memsize,
             (*len - idx - count) * memsize
    );
    return OK;
}

static inline int _vec_insert(void **data, size_t *len, size_t *cap, size_t memsize, 
                                size_t idx, size_t count) {
    size_t n = *len + count;
    int ret = n > *cap ? _vec_resize_n(data,len,cap,memsize,CAP_ARR_GROWTH(n)) : OK;
    if(idx > *len || !(ret)) return ERR;
    unsigned char **d = (unsigned char **) data;
    memmove((*d) + (idx+count) * memsize,
            (*d) + idx * memsize,
            (*len - idx) * memsize
    );
    (*len) += count;
    return OK;
}

/** VECTOR API 
 * Usage: API(&vec, ...)
*/
/* API - init and free */
#define vec_init(v, capacity) ( __extension__ ({      \
    vcap(v) = ( capacity );                           \
    vlen(v) = (0);                                    \
    vdat(v) = malloc( (capacity) * vdsize(v) )        \
    int (ret) = (OK);                                 \
    if(!vdat(v)) (ret) = (ERR);                       \
    (ret);                                            \
}))

#define vec_free(v)                                   \
    free( vdat(v) );                                  \
    memset( (v), (0), vsize(v) )                      \

/* API - resizing of vector */
#define vec_expand_n(v, n)                            \
    _vec_expand_n( _unwrap(v), (n) )

#define vec_push(v, val)                              \
    ( _vec_expand( _unwrap(v) ) ? ( ( vdat(v)[(vlen(v)++)] = (val) ), (OK) ) : (ERR) )

#define vec_shrink(v)                                 \
   ( ( vlen(v) <= (0) ) ? ( (vec_free(v)) , (OK) )    \
   : ( _vec_shrink( _unwrap(v) ) ) )

#define vec_shrink_n(v, n)                            \
   ( ( vlen(v) <= (0) ) ? ( (vec_free(v)) , (OK) )    \
   : ( _vec_shrink( _unwrap(v), (n) ) ) )

/* API - pop  */ 
#define vec_pop(v)                                    \
    vdat(v)[(--vlen(v))]   /* Usage data_elem_type data_elem = vec_pop(&vec) */

#define vec_pop_ptr_s(v)                              \
    ( ( vlen(v) > 0 ) ? (&vec_pop(v)) : (NULL) ) /* data_elem_type &data_elem = vec_pop_s(v) */      

/* API - Vector truncate  */
#define vec_truncate(v, length)                       \
  ( vlen(v) = ( (length) < vlen(v) ) ? (length) : vlen(v) )

/* API - Vector clear */
#define vec_clear(v)                                  \
    vlen(v) = (0) 

/* API - Vector position swapping */
#define vec_swap(v, idx1, idx2)                       \
    _vec_swap( _unwrap(v), (idx1), (idx2) )

#define vec_reverse(v) {                              \
    size_t (i) = ( vlen(v)>>(1) );                    \
    while(i--) {                                      \
        vec_swap( (v), (i), (vlen(v) - (i+1)) )       \
    }                                                 \
}

/* API - Find vector element by value */
#define vec_find(v, val, idx) ( __extension__ ({      \
    int (ret) = (ERR);                                \
    for( (*idx)=0; (*idx) < vlen(v); ((*idx)++) ) {   \
        if( ( vdat(v)[(*idx)] ) == (val) ) {          \
            (ret) = (OK);                             \
            break;                                    \
        }                                             \
    }                                                 \
    (ret);                                            \
}))

/* API related to insert/splice */
#define vec_splice(v, idx, count) (                   \
    _vec_splice( _unwrap(v), (idx), (count) ) ?       \
    ( ( vlen(v) -= (count) ) , (OK) ) : (ERR)         \
)

#define vec_rm(v,val) ( __extension__  ({             \
    size_t (idx);                                     \
    int (ret) = vec_find( (v), (val), (&idx) );       \
    if( (ret)  )                                      \
        (ret) = vec_splice( (v), (idx), 1 );          \
    (ret);                                            \
}))

#define vec_insert(v, val, idx) (                     \
    _vec_insert( _unwrap(v), (idx), (1) ) ?           \
    ( vdat(v)[idx] = (val) , (OK) ) : (ERR)           \
)

#define __insert_arr(v, arr, idx, count, ret)              \
    if(ret){                                               \
        for( size_t (i) = (0); (i) < (count); (i++) ) {    \
            vdat(v)[(idx++)] = (arr)[(i)];                 \
        }                                                  \
    }                                                      

#define vec_insert_arr(v, arr, idx, count) ( __extension__ ({  \
    int (ret) = _vec_insert( _unwrap(v), (idx), (count) );     \
    size_t (j) = (idx);                                        \
    __insert_arr( (v), (arr), (j), (count), (ret) );           \
    (ret);                                                     \
}))

#define vec_push_arr(v, arr, count) ( __extension__  ({          \
    int (ret) = (OK);                                            \
    size_t (n)  = (count) + vlen(v);                             \
    if( (n) > vcap(v) )                                          \
        (ret) = _vec_resize_n( _unwrap(v), CAP_ARR_GROWTH(n) );  \
    __insert_arr( (v), (arr), vlen(v), (count), (ret) );         \
    (ret);                                                       \
}))

/* Vector Sort */
#define vec_sort(v,sort_fn) \
    qsort( vdat(v), vlen(v), vdsize(v), (sort_fn) )

/* Vector Clone */
#define vec_clone(dest, src) /* clones on an uninitialized vector variable */ ( __extension__ ({ \
    int (ret) = vec_init( (dest) , (vlen(src)) )                                                  \
    if(ret) {                                                                                     \
        memcpy( vdat(dest), vdat(src), ((vlen(src)) * vdsize(src)) );                             \
        vlen(dest) = vlen(src);                                                                   \
    }                                                                                             \
    (ret);                                                                                        \
}))

#define vec_wipe_clone(dest, src) ( __extension__ ({ /* clones on an initialized vector variable */   \
    int (ret) = _vec_expand_n(_unwrap(dest),vlen(src));                                                \
    if(ret) {                                                                                          \
        memcpy( vdat(dest), vdat(src), ((vlen(src)) * vdsize(src)) );                                  \
        vlen(dest) = vlen(src);                                                                        \
    }                                                                                                  \
    (ret);                                                                                             \
}))  

/** 
 * Some for loop iteration schemes are provided
 * Safety and bound checking is on the USER. For example,  
 * if(condition) vec_for( &v, start, count ) 
 *      --User code-- 
 *      Do something with variable elem
 * END_vec_for 
 * However, can also be used without if conditions
*/

#define vec_for(v,start,count)                                   \
    { size_t (vec_len) = (count);                                \
    __typeof__(vdat(v)[0]) (elem);                               \
    for( size_t (i) = (start); ( ( (i) < (vec_len) ) &&          \
    ( ( (elem) = vdat(v)[(i)] ) , OK ) );                        \
    (i++)) {

#define vec_for_each(v)                                          \
    vec_for( (v), (0), vlen(v) )

#define vec_for_rev(v,start, count)                              \
    { __typeof__(vdat(v)[0]) (elem);                             \
    size_t (vec_pos) = ((start)-(count)+1);                      \
    for( size_t (i) = (start); ( ( (i) >= (vec_pos) ) &&         \
    ( ( (elem) = vdat(v)[(i)] ) , OK ) );                        \
    (i--)) {

#define vec_for_each_rev(v)                                      \
    vec_for_rev(v, (vlen(v)-1), vlen(v))                         

#define vec_for_ptr(v,start,count)                               \
    { size_t (vec_len) = (count);                                \
    __typeof__(vdat(v)[0]) *(elem);                              \
    for( size_t (i) = (start); ( ( (i) < (vec_len) ) &&          \
    ( ( (elem) = &(vdat(v)[(i)]) ) , OK ) );                     \
    (i++)) {

#define vec_for_ptr_rev(v,start, count)                          \
    { __typeof__(vdat(v)[0]) *(elem);                            \
    size_t (vec_pos) = ((start)-(count)+1);                      \
    for( size_t (i) = (start); ( ( (i) >= (vec_pos) ) &&         \
    ( ( (elem) = (&vdat(v)[(i)]) ) , OK ) );                     \
    (i--)) {

#define vec_for_each_ptr(v)                                      \
    vec_for_ptr( (v), (0), vlen(v) )

#define vec_for_each_ptr_rev(v)                                  \
    vec_for_ptr_rev(v, (vlen(v)-1), vlen(v))                         

#define END_vec_for }}




