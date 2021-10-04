#pragma once

#include "Types.h"
#include <cassert>

/*
 * Copyright (C) Scott Bilas, 2000.
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Scott Bilas, 2000"
 */
#define HANDLE_DECL(classname) \
    struct tag##classname { };   \
    typedef Handle<tag##classname> H##classname;

template <typename TAG>
class Handle
{
    union {
        enum ST {
            // sizes to use for bit fields
            MAX_BITS_INDEX = 16,
            MAX_BITS_MAGIC = 16,

            // sizes to compare against for asserting derefences
            MAX_INDEX = (1 << MAX_BITS_INDEX) - 1,
            MAX_MAGIC = (1 << MAX_BITS_MAGIC) - 1
        };

        struct {
            unsigned int m_index : MAX_BITS_INDEX;  // index into resource array
            unsigned int m_magic : MAX_BITS_MAGIC;  // magic number to check
        };

        unsigned int m_handle;
    };

public:
    Handle() : m_handle(0) {}

    void init(unsigned int index);
    unsigned int getIndex() const  { return m_index; }
    unsigned int getMagic() const  { return m_magic; }
    unsigned int getHandle() const { return m_handle; }
    bool isNull() const   { return !m_handle; }

    operator unsigned int() const { return m_handle; }
};

template<typename TAG>
void Handle<TAG>::init(unsigned int index) {
    assert(isNull());               // prevent reassignment
    assert(index <= MAX_INDEX);     // verify index range

    static unsigned int s_autoMagic = 0;
    if (++s_autoMagic > MAX_MAGIC) {
        s_autoMagic = 1;            // 0 is used for "null handle"
    }

    m_index = index;
    m_magic = s_autoMagic;
}

template<typename TAG>
inline bool operator != (Handle<TAG> left, Handle<TAG> right) {
    return left.getHandle() != right.getHandle();
}

template<typename TAG>
inline bool operator == (Handle<TAG> left, Handle<TAG> right) {
    return left.getHandle() == right.getHandle();
}
