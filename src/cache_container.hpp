#ifndef CACHE_CONTAINER_
#define CACHE_CONTAINER_

#include <unordered_map>
#include <functional>
#include <deque>
#include <algorithm>

namespace game
{

template <typename Key, typename ValueType>
class cache_container
{
public:
    using map_value_type = typename std::unordered_map<Key, std::unique_ptr<ValueType>>::value_type;
    using compare_function_type = std::function<bool(ValueType *, ValueType *)>;
private:
    std::unordered_map<Key, std::unique_ptr<ValueType>> elem_;
    std::deque<ValueType *> cache_;
    compare_function_type compare_;
public:
    cache_container(compare_function_type f): compare_{f} {}

    auto insert(map_value_type && p)
    {
        ValueType* element = p.second.get();
        auto && v = elem_.insert(std::move(p));
        fast_generate_cache(element);
        return v;
    }

    auto insert(map_value_type const & p)
    {
        auto && v = elem_.insert(p);
        fast_generate_cache(p.second.get());
        return v;
    }

    auto emplace(Key const & k, std::unique_ptr<ValueType> ptr)
    {
        return insert(std::make_pair(k, std::move(ptr)));
    }

    void generate_cache()
    {
        cache_.clear();
        for (auto && pair : elem_)
            cache_.push_back(pair.second.get());
        std::sort(cache_.begin(), cache_.end(), compare_);
    }

    void fast_generate_cache(ValueType * v)
    {
        cache_.push_back(v);
        auto v_pos = std::prev(cache_.end());
        std::inplace_merge(cache_.begin(), v_pos, cache_.end(), compare_);
    }

    auto& operator[] (Key const & key) { return elem_.at(key); }
    auto begin()       { return elem_.begin(); }
    auto end()         { return elem_.end();   }
    auto cache_begin() { return cache_.begin();}
    auto cache_end()   { return cache_.end();  }
};

}

#endif // CACHE_CONTAINER_
