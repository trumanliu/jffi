/* 
 * Copyright (C) 2007, 2008 Wayne Meissner
 * 
 * This file is part of jffi.
 *
 * This code is free software: you can redistribute it and/or modify it under 
 * the terms of the GNU Lesser General Public License version 3 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License 
 * version 3 for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * version 3 along with this work.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef jffi_jffi_h
#define jffi_jffi_h

#include <sys/param.h>
#include <stdint.h>
#include <string.h>
#include <jni.h>

#ifndef roundup
#  define roundup(x, y)   ((((x)+((y)-1))/(y))*(y))
#endif
typedef struct StackAllocator {
    size_t used;
    char data[256];
} StackAllocator;

static inline void 
initStackAllocator(StackAllocator* alloc) 
{
    alloc->used = 0;
}

static inline void*
allocStack(StackAllocator* alloc, size_t size) 
{
    if ((alloc->used + size + 7) < sizeof(alloc->data)) {
        uintptr_t data = roundup((uintptr_t) &alloc->data[alloc->used], 8);
        alloc->used = (data + size) - (uintptr_t) &alloc->data[0];
        return (void *) data;
    }
    return NULL;
}

/**
 * Convert a C pointer into a java long
 */
static inline jlong 
p2j(void *p) 
{ 
    return (jlong)(uintptr_t) p; 
}

/**
 * Convert a java long into a C pointer
 */
static inline void* 
j2p(jlong j) 
{
    return (void *)(uintptr_t) j;
}

static inline
jboolean loadClass(JNIEnv* env, const char *name, jclass *classp)
{
    jclass tmp = (*env)->FindClass(env, name);
    if (tmp == NULL) {
        return JNI_FALSE;
    }
    *classp = (jclass)(*env)->NewGlobalRef(env, tmp);
    return JNI_TRUE;
}
static inline jint
getInt(void* address) 
{
    jint tmp;
    memcpy(&tmp, address, sizeof(tmp));
    return tmp;
}
static inline jlong
getLong(void* address) 
{
    jlong tmp;
    memcpy(&tmp, address, sizeof(tmp));
    return tmp;
}



#endif /* jffi_jffi_h */

