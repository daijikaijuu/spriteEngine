//
//  seCollection.hpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 29.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#ifndef seCollection_hpp
#define seCollection_hpp

#include "seHelpers.hpp"
#include "../Debug/Debug.hpp"
#include "../Resources/seResource.hpp"
#include <stdio.h>
#include <iostream>
#include <string>
#include <map>

namespace spriteEngine {
    template<typename T>
    class seCollection {
        using seCollectionMap = std::map<const std::string, T *>;
        seCollectionMap m_collection;
    public:
        seCollection(bool autoID = false) : m_collection() {}
        virtual ~seCollection() {
            for (auto &it : m_collection) {
                LogDebug << "Item " << quoteStr(it.first) << " deleted" << eol;
                delete it.second;
            }
            m_collection.clear();
        }

        void AddItem(const std::string &name, T *item) {
            seAssert(!name.empty());
            seAssert(item);

            m_collection[name] = item;
            LogDebug << "Item " << quoteStr(name) << " added" << eol;
        }

        void AddItem(T *item) {
            seResource *resource = dynamic_cast<seResource *>(item);
            seAssert(resource);

            AddItem(resource->Name(), item);
        }

        T* GetItem(const std::string &name) const {
            seAssert(!name.empty());

            T *result = nullptr;
            typename seCollectionMap::const_iterator it = m_collection.find(name);
            if (it != m_collection.end()) {
                result = it->second;
                seAssert(result != nullptr);
            }
            return result;
        }

        seCollectionMap Items() const { return m_collection; }
        size_t size() const { return m_collection.size(); }
    };
}

#endif /* seCollection_hpp */
