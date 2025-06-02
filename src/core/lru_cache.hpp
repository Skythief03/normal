//
// Created by HP on 2025/6/2.
//

#ifndef LRU_CACHE_HPP
#define LRU_CACHE_HPP

#include <list>
#include <memory>
#include <unordered_map>

template<typename K, typename V>
class lru_cache {
public:
    explicit lru_cache(const size_t cap, const size_t fre): capacity(cap), min_freq(fre) {
    }

private:

    struct Node {
        K key;
        std::shared_ptr<V> val;
        size_t freq;
    };

    size_t capacity;
    size_t min_freq;

    std::unordered_map<K, typename std::list<Node>::iterator> keyNode_;
    std::unordered_map<size_t, std::list<Node> > freqList_;



    void touch_node(typename std::list<Node>::iterator) {

    }
};


#endif //LRU_CACHE_HPP
