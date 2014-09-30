//
//  AssetManager.h
//
//  Created by Jingshu Huang on 12/22/12.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _AssetManager_h
#define _AssetManager_h

#include "Asset.h"
#include "platformDef.h"


template <class K, class T>
class AssetManager {
public:
	T* getAsset(const K&) const;
    void setAsset(const K&, T*);
    T* load(const K&);

protected:
    typedef unordered_map<K, T*> AssetMap;
    typedef typename AssetMap::const_iterator AssetMapIterator;
	AssetMap assets;
};


template <class K, class T>
T* AssetManager<K, T>::getAsset(const K& key) const {
    AssetMapIterator iter = assets.find(key);
    if (iter != assets.end()) {
        return iter->second;
    }
    return NULL;
}

template <class K, class T>
void AssetManager<K, T>::setAsset(const K& key, T* t) {
    AssetMapIterator iter = assets.find(key);
    if (iter != assets.end() && iter->second) {
        delete iter->second;
    }
    assets[key] = t;
}

template <class K, class T>
T* AssetManager<K, T>::load(const K& key) {
    T* t = NULL;
    AssetMapIterator iter = assets.find(key);
    if (iter != assets.end()) {
        return iter->second;
    }
    
    t = new T(key);
    t->load();  // Assume the asset has a load() method.
    assets[key] = t;
    return t;
}

#endif

