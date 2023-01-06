#include "../src_headers/vec_lib.h"
#include <stdio.h>

int main(void) {

    
    i64_vec v;
    vec_init(&v,4);
    
    for(int i = 0; i < 50; i++) {
        if(!(vec_push(&v,i))) {printf("notokay"); exit(EXIT_FAILURE);}
    };
    
    vec_rm(&v,45);
    i64_vec v2;
    

    if(vlen(&v2)>0) vec_for_each(&v2)
        printf("%ld ",elem);
    END_vec_for

    printf("ld= %ld\n",vlen(&v));

    //vec_free(&v2);
    vec_free(&v);   
    return 0;
}



