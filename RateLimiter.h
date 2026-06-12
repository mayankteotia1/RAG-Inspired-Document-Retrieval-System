#pragma once

#include <bits/stdc++.h>
using namespace std;

// ---------------------------------------------------------------
// RateLimiter
//
// Implements the Token Bucket algorithm, per user.
//
// Idea:
//   - Every user has a "bucket" that can hold up to `capacity` tokens.
//   - Tokens refill over time at a fixed rate (`refillRate` tokens/sec).
//   - Every request costs 1 token.
//   - If the bucket has at least 1 token -> request allowed, consume a token.
//   - If the bucket is empty -> request rejected (rate limited).
//
// Why Token Bucket?
//   - Allows short bursts of traffic (up to `capacity` requests at once)
//   - But smooths out traffic over time, since tokens regenerate slowly.
//   - Easy to reason about and implement using just a timestamp + a counter.
// ---------------------------------------------------------------

class RateLimiter
{
public:
    RateLimiter(int capacity, double refillRate)
        : capacity(capacity), refillRate(refillRate)
    {
    }

    // Checks whether the given user is allowed to make a request right now.
    // Returns true if allowed (and consumes a token), false if rate limited.
    bool allowRequest(const string &userId)
    {
        refill(userId);

        auto &bucket = buckets[userId];

        if (bucket.tokens >= 1.0)
        {
            bucket.tokens -= 1.0;
            return true;
        }

        return false;
    }

private:
    struct Bucket
    {
        double tokens;
        chrono::steady_clock::time_point lastRefillTime;
    };

    int capacity;        // max tokens a bucket can hold
    double refillRate;   // tokens added per second

    unordered_map<string, Bucket> buckets;

    // Refills tokens for a user based on how much time has passed
    // since their last request.
    void refill(const string &userId)
    {
        auto now = chrono::steady_clock::now();

        // If this is the first time we see this user, give them a full bucket.
        if (buckets.find(userId) == buckets.end())
        {
            buckets[userId] = {(double)capacity, now};
            return;
        }

        auto &bucket = buckets[userId];

        double elapsedSeconds = chrono::duration<double>(now - bucket.lastRefillTime).count();

        double tokensToAdd = elapsedSeconds * refillRate;

        bucket.tokens = min((double)capacity, bucket.tokens + tokensToAdd);
        bucket.lastRefillTime = now;
    }
};
