#pragma once

#include <bits/stdc++.h>
using namespace std;

// LRU Cache using unordered_map + list
// front of list = most recently used, back = least recently used
// map stores key -> iterator so we can jump to that node in O(1)

class LRUCache
{
public:
    LRUCache(int capacity) : capacity(capacity)
    {
    }

    // returns true if found, fills value with cached result
    bool get(const string &key, string &value)
    {
        auto it = lookup.find(key);

        if (it == lookup.end())
            return false;

        // move to front since this was just used
        items.splice(items.begin(), items, it->second);

        value = it->second->second;
        return true;
    }

    void put(const string &key, const string &value)
    {
        auto it = lookup.find(key);

        if (it != lookup.end())
        {
            it->second->second = value;
            items.splice(items.begin(), items, it->second);
            return;
        }

        // cache full -> remove least recently used (back of list)
        if ((int)items.size() >= capacity)
        {
            auto last = items.back();
            lookup.erase(last.first);
            items.pop_back();
        }

        items.push_front({key, value});
        lookup[key] = items.begin();
    }

private:
    int capacity;
    list<pair<string, string>> items;
    unordered_map<string, list<pair<string, string>>::iterator> lookup;
};
