#include <iostream>
#include <unordered_map>
#include <list>
#include <cassert>
using namespace std;

/**
 * Task: Implement an LRU Cache with capacity 'cap'.
 * Requirement: O(1) time complexity for both get and put.
 */
class LRUCache {
    struct Node {
        int key;
        int val;
    };
    list<Node> priorityQ;
    unordered_map<int, list<Node>::iterator > keyToNode;
    const int capacity;

public:
    LRUCache(int _capacity):capacity(_capacity)
    {
    }
    
    int get(int key) 
    {
        auto kToNode = keyToNode.find(key);
        if( kToNode == keyToNode.end() ) 
        {
            return -1;
        }

        int valToReturn = kToNode->second->val;
        priorityQ.erase(kToNode->second);
        keyToNode.erase(kToNode);
        priorityQ.push_back({.key=key, .val=valToReturn});
        keyToNode.insert({key, prev(priorityQ.end())});        
        return valToReturn;
    }
    
    void put(int key, int value) 
    {      
        auto kToNode = keyToNode.find(key);
        bool keyExist = kToNode != keyToNode.end();
        
        if(capacity == keyToNode.size() and !keyExist)
        {           
            int keyToEvict = priorityQ.front().key;
            keyToNode.erase(keyToNode.find(keyToEvict));
            priorityQ.pop_front();
        }
        
        if(keyExist)
        {
            priorityQ.erase(kToNode->second);
            keyToNode.erase(kToNode);
        }
        
        priorityQ.push_back({.key=key, .val=value});
        keyToNode.insert({key, prev(priorityQ.end())});
    }
};

void test_lru() {
    LRUCache cache(2);

    cout << "Test 1: Basic Put & Get: ";
    cache.put(1, 10);
    cache.put(2, 20);
    if (cache.get(1) == 10) cout << "Pass\n";
    else cout << "FAIL (Expected 10, got " << cache.get(1) << ")\n";

    cout << "Test 2: Eviction Logic: ";
    cache.put(3, 30); // Should evict 2 (if LRU works correctly)
    if (cache.get(2) == -1) cout << "Pass\n";
    else cout << "FAIL (Key 2 should have been evicted)\n";

    cout << "Test 3: Access updates LRU order: ";
    cache.get(1);     // 1 becomes MRU
    cache.put(4, 40); // Should evict 3 now
    if (cache.get(3) == -1) cout << "Pass\n";
    else cout << "FAIL (Key 3 should have been evicted because 1 was accessed)\n";

    cout << "Test 4: Handle updates: ";
    cache.put(4, 44);
    if (cache.get(4) == 44) cout << "Pass\n";
    else cout << "FAIL (Expected updated value 44, got " << cache.get(4) << ")\n";
}

int main() {
    try {
        test_lru();
    } catch (const exception& e) {
        cerr << "Crashed with exception: " << e.what() << endl;
    } catch (...) {
        cerr << "Crashed with unknown error (likely SEGFAULT)" << endl;
    }
    return 0;
}
