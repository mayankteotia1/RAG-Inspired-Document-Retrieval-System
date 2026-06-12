#include <bits/stdc++.h>
#include "RateLimiter.h"
#include "LRUCache.h"
#include "SearchEngine.h"

using namespace std;

// ---------------------------------------------------------------
// main.cpp
//
// This is the entry point of the Smart Document Search Engine.
//
// Flow for every query:
//   1. Ask the rate limiter if this user is allowed to make a request.
//   2. If allowed, check the LRU cache for this exact query.
//   3. If cache hit -> return cached result immediately.
//   4. If cache miss -> run the SearchEngine to find the best document.
//   5. Store the result in the cache for future identical queries.
//   6. Print the final result to the user.
// ---------------------------------------------------------------

int main()
{
    // Rate limiter configuration:
    // capacity = 3  -> each user can burst up to 3 requests instantly
    // refillRate = 1 -> 1 token is added back every second
    RateLimiter rateLimiter(3, 1.0);

    // Cache configuration: store up to 5 most recent queries
    LRUCache cache(5);

    // Load knowledge base
    SearchEngine searchEngine;
    if (!searchEngine.loadDocuments("documents.txt"))
    {
        cout << "Error: could not open documents.txt" << endl;
        return 1;
    }

    cout << "Smart Document Search Engine" << endl;
    cout << "Loaded " << searchEngine.documentCount() << " documents." << endl;
    cout << "Type 'exit' to quit.\n" << endl;

    string userId = "user1"; // single user for simplicity

    while (true)
    {
        cout << "Enter query: ";
        string query;
        getline(cin, query);

        if (query == "exit")
        {
            break;
        }

        if (query.empty())
        {
            continue;
        }

        // Step 1: Rate limiting
        if (!rateLimiter.allowRequest(userId))
        {
            cout << "Rate limit exceeded. Please wait before sending more requests.\n" << endl;
            continue;
        }

        // Step 2: Cache lookup
        string cachedResult;
        if (cache.get(query, cachedResult))
        {
            cout << "[CACHE HIT] " << cachedResult << "\n" << endl;
            continue;
        }

        // Step 3: Search the knowledge base
        auto result = searchEngine.search(query);
        string bestDoc = result.first;
        int score = result.second;

        string finalResult;
        if (score <= 0)
        {
            finalResult = "No relevant document found.";
        }
        else
        {
            finalResult = bestDoc;
        }

        // Step 4: Store in cache for future queries
        cache.put(query, finalResult);

        cout << "[CACHE MISS] " << finalResult << "\n" << endl;
    }

    cout << "Goodbye!" << endl;
    return 0;
}
