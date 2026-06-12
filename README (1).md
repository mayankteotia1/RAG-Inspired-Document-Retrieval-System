# Smart Document Search Engine

A C++17 backend project simulating the retrieval step of a RAG (Retrieval-Augmented Generation) pipeline. A user query passes through rate limiting, an LRU cache, and a keyword-based search engine over a local knowledge base.

## Structure

* `RateLimiter.h` — Token Bucket rate limiter (per-user)
* `LRUCache.h` — LRU cache built using `unordered_map` + `list`
* `SearchEngine.h` — Loads `documents.txt`, scores documents by keyword overlap
* `documents.txt` — Local knowledge base (one document per line)
* `main.cpp` — CLI entry point

## Flow

```
Query -> Rate Limiter -> Cache Check -> Search Engine -> Best Match -> Cache Result -> Response
```

## Features

* Token Bucket rate limiting with burst capacity and refill rate
* LRU cache with O(1) average get/put
* Keyword-overlap document retrieval

## Prerequisites

* C++17 compiler (g++ 9+)

## Building

```
g++ -std=c++17 -O2 -Wall -o search_engine main.cpp
./search_engine
```

## Sample Output

```
Enter query: binary search tree
[CACHE MISS] A binary search tree maintains the property that left children
are smaller and right children are larger than the parent node

Enter query: binary search tree
[CACHE HIT] A binary search tree maintains the property that left children
are smaller and right children are larger than the parent node
```

## Future Improvements

* TF-IDF based scoring
* Multi-user support with thread safety (mutex)
* Inverted index for faster search
