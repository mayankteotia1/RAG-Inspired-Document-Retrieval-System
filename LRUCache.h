#pragma once

#include <bits/stdc++.h>
using namespace std;

// ---------------------------------------------------------------
// LRUCache
//
// Stores results of recent queries so repeated searches don't
// require scanning documents.txt again.
//
// Design:
//   - We keep a doubly linked list (std::list) of {key, value} pairs.
//     The FRONT of the list = most recently used.
//     The BACK of the list  = least recently used.
//
//   - We keep an unordered_map<key, list-iterator> so we can jump
//     directly to any node in the list in O(1).
//
// Operations:
//   - get(key):  O(1) average
//       If key exists, move it to the front (mark as recently used)
//       and return its value.
//
//   - put(key, value): O(1) average
//       If key exists, update value and move to front.
//       If key doesn't exist:
//           - If cache is full, remove the back element (least recently used).
//           - Insert new element at the front.
//
// Why unordered_map + list?
//   - list gives O(1) insertion/removal from both ends and from the middle
//     (given an iterator).
//   - unordered_map gives O(1) average lookup of "where is this key in the list".
//   - Together they give us O(1) get/put, which is the classic LRU trick.
// ---------------------------------------------------------------

class LRUCache
{
public:
    LRUCache(int capacity) : capacity(capacity)
    {
    }

    // Returns true if found, and writes the cached value into `value`.
    bool get(const string &key, string &value)
    {
        auto it = lookup.find(key);

        if (it == lookup.end())
        {
            return false; // cache miss
        }

        // Move the accessed node to the front (most recently used).
        items.splice(items.begin(), items, it->second);

        value = it->second->second;
        return true; // cache hit
    }

    // Inserts or updates a key-value pair.
    void put(const string &key, const string &value)
    {
        auto it = lookup.find(key);

        if (it != lookup.end())
        {
            // Key already exists -> update value and move to front.
            it->second->second = value;
            items.splice(items.begin(), items, it->second);
            return;
        }

        // If the cache is full, evict the least recently used item (back of list).
        if ((int)items.size() >= capacity)
        {
            auto last = items.back();
            lookup.erase(last.first);
            items.pop_back();
        }

        // Insert the new item at the front.
        items.push_front({key, value});
        lookup[key] = items.begin();
    }

private:
    int capacity;

    // front = most recently used, back = least recently used
    list<pair<string, string>> items;

    // maps key -> iterator pointing to its node in `items`
    unordered_map<string, list<pair<string, string>>::iterator> lookup;
};
