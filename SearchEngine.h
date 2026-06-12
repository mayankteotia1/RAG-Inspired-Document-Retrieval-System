#pragma once

#include <bits/stdc++.h>
using namespace std;

// ---------------------------------------------------------------
// SearchEngine
//
// Loads a small "knowledge base" from documents.txt and answers
// queries by finding the document whose content has the highest
// keyword overlap with the query.
//
// This mimics the "retrieval" step of a RAG (Retrieval-Augmented
// Generation) pipeline -> given a query, find the most relevant
// chunk of text from a knowledge base.
//
// Format of documents.txt:
//   Each line is ONE document.
//   Example:
//     Binary search works on sorted arrays and runs in O(log n)
//     A linked list allows O(1) insertion at the head
//     ...
//
// Scoring:
//   - Split both the query and each document into lowercase words.
//   - Score(doc) = number of query words that also appear in doc.
//   - The document with the highest score wins.
//   - Ties are broken by picking the first document with that score.
// ---------------------------------------------------------------

class SearchEngine
{
public:
    // Loads documents from the given file path.
    // Each non-empty line becomes one document.
    bool loadDocuments(const string &filePath)
    {
        ifstream file(filePath);

        if (!file.is_open())
        {
            return false;
        }

        string line;
        while (getline(file, line))
        {
            if (!line.empty())
            {
                documents.push_back(line);
            }
        }

        file.close();
        return true;
    }

    // Returns the best matching document for the given query,
    // along with its score. If no documents match at all
    // (score 0 for everything), it still returns the first document
    // but the caller can check `score`.
    pair<string, int> search(const string &query)
    {
        unordered_set<string> queryWords = tokenize(query);

        string bestDoc = "";
        int bestScore = -1;

        for (const string &doc : documents)
        {
            int score = scoreDocument(doc, queryWords);

            if (score > bestScore)
            {
                bestScore = score;
                bestDoc = doc;
            }
        }

        return {bestDoc, bestScore};
    }

    int documentCount() const
    {
        return (int)documents.size();
    }

private:
    vector<string> documents;

    // Converts a string into a set of lowercase words.
    // Time complexity: O(L) where L = length of the string.
    unordered_set<string> tokenize(const string &text)
    {
        unordered_set<string> words;
        string current;

        for (char c : text)
        {
            if (isalnum((unsigned char)c))
            {
                current += (char)tolower((unsigned char)c);
            }
            else
            {
                if (!current.empty())
                {
                    words.insert(current);
                    current.clear();
                }
            }
        }

        if (!current.empty())
        {
            words.insert(current);
        }

        return words;
    }

    // Counts how many words from `queryWords` appear in `doc`.
    // Time complexity: O(D) where D = number of words in the document
    // (each lookup in queryWords is O(1) average since it's a hash set).
    int scoreDocument(const string &doc, const unordered_set<string> &queryWords)
    {
        unordered_set<string> docWords = tokenize(doc);

        int score = 0;
        for (const string &word : docWords)
        {
            if (queryWords.count(word))
            {
                score++;
            }
        }

        return score;
    }
};
