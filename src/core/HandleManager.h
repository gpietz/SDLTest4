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
#pragma once

#include <vector>
#include <cassert>
#include <memory>
#include "Types.h"
#include "Handle.h"

template<typename DATA, typename HANDLE>
class HandleManager {
private:
    typedef std::vector<DATA> UserVec;
    typedef std::vector<unsigned int>  MagicVec;
    typedef std::vector<unsigned int>  FreeVec;

    UserVec m_userData;         // data we're going to get to
    MagicVec m_magicNumbers;    // corresponding magic numbers
    FreeVec  m_freeSlots;       // keeps track of free slots in the db

public:
    HandleManager() {};
    ~HandleManager() {};

    // acquisition
    DATA* acquire(HANDLE& handle);
    void release(HANDLE handle);

    // dereferencing
    DATA* dereference(HANDLE handle);
    const DATA* dereference(HANDLE handle) const;

    // other query
    unsigned int getUserHandleCount() const {
        return m_magicNumbers.size() - m_freeSlots.size();
    }
    bool hasUserHandles() const {
        return !!getUserHandleCount();
    }
};

template<typename DATA, typename HANDLE>
DATA* HandleManager<DATA, HANDLE>::acquire(HANDLE& handle) {
    unsigned int index;
    if (m_freeSlots.empty()) {
        index = m_magicNumbers.size();
        handle.init(index);
        m_userData.push_back(DATA());
        m_magicNumbers.push_back(handle.getMagic());
    } else {
        index = m_freeSlots.back();
        handle.init(index);
        m_freeSlots.pop_back();
        m_magicNumbers[index] = handle.getMagic();
    }
    return (m_userData.begin().operator->() + index);
}

template<typename DATA, typename HANDLE>
void HandleManager<DATA, HANDLE>::release(HANDLE handle) {
    unsigned int index = handle.getIndex();

    // make sure it's valid
    assert(index < m_userData.size());
    assert(m_magicNumbers[index] == handle.getMagic());

    // ok remove it - tag as unused and add to free list
    m_magicNumbers[index] = 0;
    m_freeSlots.push_back(index);
}

template<typename DATA, typename HANDLE>
inline DATA* HandleManager<DATA, HANDLE>::dereference(HANDLE handle) {
    if (handle.isNull()) {
        return 0;
    }

    // check handle validity - $ this check can be removed for speed
    // if you can assume all handle references are always valid.
    unsigned int index = handle.getIndex();
    if (index >= m_userData.size() || m_magicNumbers[index] != handle.getMagic()) {
        // no good! invalid handle = client programming error
        assert(0);
        return 0;
    }

    return m_userData.begin().operator->() + index;
}

template<typename DATA, typename HANDLE>
inline const DATA* HandleManager<DATA, HANDLE>::dereference(HANDLE handle) const {
    // this lazy cast is ok - non-const version does not modify anything
    typedef HandleManager<DATA, HANDLE> ThisType;
    return const_cast<ThisType*>(this)->dereference(handle);
}
