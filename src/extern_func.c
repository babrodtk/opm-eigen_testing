/*
 * extern_func.c
 *
 *  Created on: Aug 24, 2015
 *      Author: babrodtk
 */




int getValue(void* ptr) {
    unsigned char* char_ptr = (unsigned char*) ptr;

    int retval;

    if (char_ptr[0]<128) {
        retval = 1;
    }
    else {
        retval = (int) char_ptr[0];
    }
    return retval;
}
