# Effective STL Summary

Summary of *Effective STL* by Scott Meyers.

## Chapter 1: Containers

### Item 1: Choose your containers with care
**Item Summary**
The STL provides a wide array of containers, each with specific strengths and trade-offs. Beyond the standard algorithmic complexity rules, you must consider memory layout (contiguous vs. node-based) and iterator invalidation rules.

**Key Points**
- **Standard Sequence Containers**: `vector`, `string`, `deque`, `list`.
- **Standard Associative Containers**: `set`, `multiset`, `map`, `multimap`.
- **Non-standard**: `slist` (singly linked list), `rope` (heavy-duty string), hashed containers (`hash_set`, etc.).
- **Contiguous-memory containers** (`vector`, `deque`, `string`): Store elements in chunks. Insertions/erasures shift elements, invalidating iterators/references.
- **Node-based containers** (`list`, associative): Store elements individually. Insertions/erasures do not move other elements; pointers remain valid (except for erased element).

**Choosing a Container**:
- Random access iterators? -> `vector`, `deque`, `string`.
- Stable pointers/iterators during insert/erase? -> Node-based (`list`, `set`, `map`).
- Insertions at ends only? -> `deque`.
- Layout compatible with C? -> `vector`.

**Example**
```cpp
// If you need to insert elements in the middle frequently:
std::list<int> myList; // Node-based, pointers stay valid
auto it = myList.begin();
// ...
myList.insert(it, 42); // Efficient, iterators to other elements valid

// If you need random access:
std::vector<int> myVec;
// myVec[5] is O(1)
```

### Item 2: Beware the illusion of container-independent code
**Item Summary**
Do not try to write code that is completely agnostic to the container type (e.g., using a generic `typedef` and expecting to swap `vector` for `list` seamlessly). Containers vary significantly in supported operations, iterator types, and invalidation rules.

**Key Points**
- **Different Capabilities**: Sequence containers have `push_back`/`push_front`, associative have `count`/`lower_bound`. `list` lacks random access (no `operator[]`). `vector` and `deque` have different invalidation guarantees.
- **Iterator Invalidation**: Code relying on `vector`'s pointers becoming invalid might break if switched to `list` (where they stay valid) or vice-versa (where `vector` invalidates everything on reallocation).
- **Encapsulation**: Use `typedef`s (e.g., `typedef vector<Widget> WidgetContainer;`) to make changing types *easier*, but don't expect it to be transparent.
- **Wrapper Classes**: To truly limit dependencies, wrap the container in a class and expose only necessary domain-specific operations (e.g., `CustomerList`).

**Example**
```cpp
// Broken attempt at container-independence
typedef std::vector<Widget> WidgetContainer;
// typedef std::list<Widget> WidgetContainer; // Switching to this might break code below!

void process(const WidgetContainer& c) {
    if (c.size() >= 5) { // Works for both
        // c[4].doSomething(); // Compiles for vector, FAILS for list!
    }
}

// Better: Encapsulate
class CustomerList {
private:
    std::vector<Customer> customers;
public:
    void add(const Customer& c) { customers.push_back(c); }
    // ... expose only needed logic
};
```
### Item 3: Make copying cheap and correct for objects in containers
**Item Summary**
STL containers copy objects frequently (on insertion, rebalancing, sorting, etc.). If copying is expensive, performance suffers. If objects are sliced (derived copied to base), correctness suffers.

**Key Points**
- **Copying Mechanism**: Uses copy constructor and copy assignment operator.
- **Slicing**: Inserting a derived object into a `vector<Base>` slices off the derived part.
- **Solution**: Use containers of pointers (preferably smart pointers like `shared_ptr` to manage lifetime) if objects are expensive to copy or polymorphic.

**Example**
```cpp
class Base { ... };
class Derived : public Base { ... };

std::vector<Base> v;
Derived d;
v.push_back(d); // Slicing! 'd' is copied as a 'Base' object. 
                // Any Derived-specific data is lost.

// Correct approach for polymorphism:
std::vector<std::shared_ptr<Base>> vPtr;
vPtr.push_back(std::make_shared<Derived>()); // Safe, polymorphism preserved
```

### Item 4: Call empty instead of checking size() against zero
**Item Summary**
Always use `c.empty()` instead of `c.size() == 0`.

**Key Points**
- **Efficiency**: `empty()` is guaranteed to be constant time (O(1)) for all standard containers.
- **List Size**: `list::size()` *can* be linear time (O(n)) in some implementations (to allow O(1) splicing). `empty()` is always safe.

**Example**
```cpp
std::list<int> myList;
// ...
if (myList.size() == 0) { // Potentially O(n) scan! bad!
    // ...
}
if (myList.empty()) {     // Guaranteed O(1). Good.
    // ...
}
```

### Item 5: Prefer range member functions to their single-element counterparts
**Item Summary**
Use range member functions (e.g., `assign`, `insert`, `erase` with iterators) instead of loops with single-element calls.

**Key Points**
- **Efficiency**: Range functions move existing elements once (vs per element in a loop), allocate memory once (avoiding multiple reallocations), and reduce function call overhead.
- **Clarity**: `v.insert(v.end(), data, data+n)` is clearer than a loop pushing back.
- **Supported Functions**:
    - **Construction**: `container(begin, end)`
    - **Insertion**: `insert(pos, begin, end)`
    - **Erasure**: `erase(begin, end)`
    - **Assignment**: `assign(begin, end)`

**Example**
```cpp
std::vector<int> v1 = {1, 2, 3};
std::vector<int> v2;

// Bad: Loop insertion
for (auto i : v1) {
    v2.push_back(i); // repeated calls, potential repeated failure/realloc
}

// Good: Range insertion
v2.insert(v2.end(), v1.begin(), v1.end()); // Optimized, single alloc likely
```

### Item 6: Be alert for C++'s most vexing parse
**Item Summary**
Be careful when declaring a container with a range constructor using temporary `istream_iterator`s. The compiler might interpret it as a function declaration.

**Key Points**
- **The Problem**: `list<int> data(istream_iterator<int>(f), istream_iterator<int>());` declares a function named `data` taking an iterator and a function pointer, returning a list.
- **The Fix**: Wrap arguments in parentheses: `list<int> data((istream_iterator<int>(f)), istream_iterator<int>());` OR use named variables for iterators.

**Example**
```cpp
std::ifstream dataFile("ints.dat");

// This declares a function 'data' that returns a list<int>
// and takes two parameters! It is NOT a variable definition.
std::list<int> data(std::istream_iterator<int>(dataFile), 
                    std::istream_iterator<int>()); 

// Fix 1: Extra parentheses (C++98)
std::list<int> data((std::istream_iterator<int>(dataFile)), 
                    std::istream_iterator<int>());

// Fix 2: Uniform initialization (C++11)
std::list<int> data{ std::istream_iterator<int>(dataFile), 
                     std::istream_iterator<int>() };
```

### Item 7: When using containers of newed pointers, remember to delete the pointers before the container is destroyed
**Item Summary**
Standard containers manage memory for the objects they hold, but not for objects pointed to by pointers they hold. If you store pointers to heap-allocated objects, you must manually delete them to avoid leaks.

**Key Points**
- **The Problem**: A container's destructor destroys the pointers it holds (which is a no-op), not the objects they point to.
- **Solution 1**: Manually delete pointers in a loop or using a helper functor (like `DeleteObject`) passed to `for_each`.
- **Solution 2 (Preferred)**: Use smart pointers like `shared_ptr` (aka `boost::shared_ptr` at the time of writing, now `std::shared_ptr`) as the container element type.

**Example**
```cpp
void doSomething() {
    vector<Widget*> vwp;
    // ... fill vwp with new Widget pointers ...
    
    // Manual cleanup (prone to exception safety issues if not careful)
    for (vector<Widget*>::iterator i = vwp.begin(); i != vwp.end(); ++i) {
        delete *i; 
    }
} // Better: Use vector<shared_ptr<Widget>>
```

### Item 8: Never create containers of auto_ptrs
**Item Summary**
`auto_ptr` has destructive copy semantics (copying transfers ownership and sets source to NULL). This violates requirements for standard container elements, leading to undefined or erratic behavior.

**Key Points**
- **Copy Semantics**: Copying an `auto_ptr` modifies the source. Algorithms like `sort` often copy elements (e.g., to a pivot), which would silently wipe out elements in the container.
- **Standard Compliance**: The C++ Standard explicitly forbids containers of `auto_ptr`.
- **Alternative**: Use `unique_ptr` (Modern C++) or `shared_ptr`.

**Example**
```cpp
std::vector<std::auto_ptr<Widget>> v; // Do not do this!
// ... fill v ...

// Simple sort might copy elements to a temporary pivot.
// Copying auto_ptr transfers ownership, setting the original to NULL.
std::sort(v.begin(), v.end()); 
// Now some elements in 'v' are likely NULL!
```

### Item 9: Choose carefully among erasing options
**Item Summary**
The method to erase elements depends on the container type and the erasing criteria (specific value vs. predicate).

**Key Points**
- **Sequence Containers (vector, deque, string)**:
    - To remove specific value: Use the **erase-remove idiom**: `c.erase(remove(c.begin(), c.end(), val), c.end());`
    - To remove with predicate: Use `c.erase(remove_if(c.begin(), c.end(), pred), c.end());`
- **List**:
    - Use member functions `remove(val)` or `remove_if(pred)`. They are more efficient and don't require the erase-remove dance.
- **Associative Containers (set, map)**:
    - Use member function `erase(val)` (O(log n)).
    - To remove with predicate: Iterate and erase, ensuring to post-increment the iterator to avoid invalidation: `c.erase(i++);`

**Example**
```cpp
std::vector<int> v = {1, 2, 3, 4, 5, 6};

// Erase-remove idiom for vector
v.erase(std::remove(v.begin(), v.end(), 3), 
        v.end()); // Removes 3

// Predicate erase
v.erase(std::remove_if(v.begin(), v.end(), [](int i){ return i % 2 == 0; }), 
        v.end()); // Removes evens

// Associative container (e.g., set)
std::set<int> s = {1, 2, 3, 4};
for (auto it = s.begin(); it != s.end(); ) {
    if (*it % 2 == 0) {
        s.erase(it++); // Valid: pass current, then increment
    } else {
        ++it;
    }
}
```

### Item 10: Be aware of allocator conventions and restrictions
**Item Summary**
Allocators were originally for memory models (unused now) and custom memory management. The Standard imposes strict equivalence requirements.

**Key Points**
- **Equivalence**: Allocators of the same type must be equivalent (compare equal). This implies portable allocators cannot have state.
- **Rebind**: Allocators must provide the `rebind` template to allow containers to allocate internal nodes (like `list` nodes) instead of just the element type `T`.
- **Interface**: Allocator interface (`allocate`, `deallocate`) is different from `operator new`/`delete`.

### Item 11: Understand the legitimate uses of custom allocators
**Item Summary**
Custom allocators are useful for improving performance or using special memory types (e.g., shared memory).

**Key Points**
- **Shared Memory**: Put container elements in shared memory. (Note: The container object itself is usually on the stack).
- **Heap Segregation**: Keep elements of different containers in different heaps to prevent fragmentation or improve locality.
- **Usage**: Pass the custom allocator as a template argument: `vector<int, MyAllocator<int>> v;`.

**Example**
```cpp
// Shared memory allocator usage
typedef std::vector<double, SharedMemAllocator<double>> SharedVec;
SharedVec v; // Elements allocated in shared memory
```

### Item 12: Have realistic expectations about the thread safety of STL containers
**Item Summary**
The STL provides limited thread safety guarantees. You must handle synchronization yourself.

**Key Points**
- **Guarantees**:
    - Multiple readers (const access) are safe.
    - Multiple writers to *different* containers are safe.
- **Manual Locking**: You must lock the container when multiple threads access it and at least one is a writer.
- **Atomicity**: Even single statements like `v.push_back(x)` are not atomic in the context of a sequence of operations (e.g., check size then push). Lock the entire logical operation.

**Example**
```cpp
std::vector<int> v;
std::mutex m;

// Bad: Race condition between !empty() and front()
if (!v.empty()) {
    // Another thread could allow pop_back() here!
    std::cout << v.front(); 
}

// Good: Lock the scope
{
    std::lock_guard<std::mutex> lock(m);
    if (!v.empty()) {
        std::cout << v.front();
    }
}
```

### Item 13: Prefer vector and string to dynamically allocated arrays
**Item Summary**
`vector` and `string` manage memory automatically, reducing leaks and errors associated with `new T[]` and `delete[]`.

**Key Points**
- **Safety**: automatic destruction prevents leaks.
- **Convenience**: `size()`, `begin()`, `end()`, algorithms support.
- **Legacy Compatibility**: Vectors and strings can be passed to C APIs (see Item 16).
- **Exception Safety**: Easier to write exception-safe code with RAII containers.

**Example**
```cpp
void process() {
    // Risky: Manual memory management
    int* arr = new int[100];
    // ... if exception throws here, memory leak!
    delete[] arr;

    // Safe: RAII
    std::vector<int> v(100);
    // ... automatic cleanup
}
```
### Item 14: Use reserve to avoid unnecessary reallocations
**Item Summary**
`vector` and `string` grow by reallocating memory, copying elements, and destroying old ones. Use `reserve` to prevent this expensive cycle when you know the approximate number of elements.

**Key Points**
- **Reallocation Steps**: Allocate new block, copy elements, destroy old elements, deallocate old block. Invalidates all iterators/pointers.
- **reserve(n)**: Forces capacity to be at least `n`. Does not change `size()`.
- **Strategy**: Call `reserve` immediately after construction if the count is known, or before a large loop of insertions.

**Example**
```cpp
std::vector<int> v;
v.reserve(1000); // Allocate once
for (int i = 0; i < 1000; ++i) {
    v.push_back(i); // No reallocation
}
```

### Item 15: Be aware of variations in string implementations
**Item Summary**
`string` implementations vary significantly in size, memory allocation policy, and support for reference counting.

**Key Points**
- **Size**: `sizeof(string)` can range from 1x to 7x the size of a pointer.
- **Reference Counting**: Many implementations (at the time) were reference counted. This helps with copying but hurts in multithreaded environments due to locking overhead.
- **Small String Optimization**: Some implementations store short strings directly in the object to avoid dynamic allocation.

### Item 16: Know how to pass vector and string data to legacy APIs
**Item Summary**
C++ standard guarantees `vector` elements are contiguous (like arrays). C APIs often expect pointers to arrays.

**Key Points**
- **Vector**: Use `&v[0]` to get a pointer to the data. Valid even for C APIs that write to the array (respecting `v.size()`).
- **String**: Use `s.c_str()` for read-only C APIs. Returns a null-terminated string.
- **Safety**: Do not use `v.begin()` as a pointer (it's an iterator). If `v` is empty, `&v[0]` is undefined; check `!v.empty()` first.

**Example**
```cpp
extern "C" void fillArray(int* arr, size_t n);

std::vector<int> v(100);
if (!v.empty()) {
    fillArray(&v[0], v.size()); // Valid
}
// Note: v.begin() is NOT guaranteed to be a pointer
```

### Item 17: Use “the swap trick” to trim excess capacity
**Item Summary**
Containers like `vector` and `string` do not automatically shrink their capacity when elements are erased. Use the swap trick to force a "shrink to fit".

**Key Points**
- **The Trick**: `vector<T>(v).swap(v);`
    - Creates a temporary vector (copy of `v`) which allocates just enough memory for `v.size()`.
    - Swaps the temporary's data with `v`. `v` now has minimal capacity.
    - Temporary is destroyed, reclaiming the bloated memory.
- **Clearing**: `vector<T>().swap(v);` clears the vector *and* minimizes capacity.

**Example**
```cpp
std::vector<int> v;
v.reserve(10000);
// ... fill only 10 elements ...
// v.capacity() is still 10000.

std::vector<int>(v).swap(v);
// 1. Create temporary copy of v (capacity ~10).
// 2. Swap contents. v now has capacity ~10.
// 3. Temporary (with capacity 10000) is destroyed.
```

### Item 18: Avoid using vector<bool>
**Item Summary**
`vector<bool>` is not a standard container and does not store `bool`s. It stores bits (packed) to save space.

**Key Points**
- **Not a Container**: It does not satisfy container requirements because `operator[]` returns a proxy object, not a `bool&`. You cannot take the address of an element.
- **Alternatives**:
    - `deque<bool>`: A true container, stores one `bool` per byte (usually).
    - `bitset`: If size is fixed at compile time.

**Example**
```cpp
std::vector<bool> vb;
vb.push_back(true);
// bool* p = &vb[0]; // Error! &vb[0] returns a proxy object, not address
// auto x = vb[0];   // type of x is std::vector<bool>::reference
```

## Chapter 3: Associative Containers

### Item 19: Understand the difference between equality and equivalence
**Item Summary**
Standard associative containers are sorted and use *equivalence* (based on `operator<` or a custom predicate) to determine if two elements are the "same", not *equality* (based on `operator==`).

**Key Points**
- **Equality**: `x == y`. Used by unordered algorithms like `find`.
- **Equivalence**: `!(x < y) && !(y < x)`. Used by associative containers (e.g., `set::insert`, `set::find`).
- **Implication**: Two objects can be equivalent but not equal (e.g., case-insensitive strings "Abc" and "abc").
- **Consistency**: Mixing equality-based algorithms (`std::find`) with equivalence-based containers (`set`) can yield inconsistent results.

**Example**
```cpp
struct CiStringCompare { 
    bool operator()(const string& a, const string& b) const { /* case insensitive < */ } 
};
std::set<string, CiStringCompare> s;
s.insert("Apple");
// s.find("apple") != s.end() -> True (Equivalence)
// std::find(s.begin(), s.end(), "apple") == s.end() -> True (Equality fails)
```

### Item 20: Specify comparison types for associative containers of pointers
**Item Summary**
If you store pointers in a `set` or `map`, the default comparison sorts by pointer address, which is rarely what you want. You must provide a custom comparator that dereferences the pointers.

**Key Points**
- **Default Behavior**: `set<string*>` sorts by the address of the string pointers.
- **Solution**: Define a functor (e.g., `StringPtrLess`) that takes two pointers, dereferences them, and compares the objects.
- **Functor Template**: A generic `DereferenceLess` functor acts as a reusable solution for containers of pointers.

**Example**
```cpp
std::set<string*> s;
s.insert(new string("Zoo"));
s.insert(new string("Ant"));
// Iterating 's' yields "Zoo", "Ant" (likely) because address of Zoo < Ant on heap.

struct DereferenceLess {
    template <typename PtrType>
    bool operator()(PtrType p1, PtrType p2) const { return *p1 < *p2; }
};
std::set<string*, DereferenceLess> s2; // Sorts by string content: "Ant", "Zoo"
```

### Item 21: Always have comparison functions return false for equal values
**Item Summary**
Comparison functions for associative containers must define a "strict weak ordering". A critical requirement is that for two equal values, the comparison must return `false`.

**Key Points**
- **The Rule**: `comp(x, x)` must be false.
- **The Danger**: If `comp(x, x)` returns true (e.g., using `<=` instead of `<`), the container assumes `x` is not equivalent to itself!
- **Consequence**: You might end up with duplicate entries in a `set` or corrupt internal structures.

**Example**
```cpp
bool badLess(int a, int b) { return a <= b; } // Error: badLess(10, 10) is true
std::set<int, decltype(&badLess)> s(&badLess);
s.insert(10);
s.insert(10); // Might insert duplicate! Equivalency check logic breaks.
```

### Item 22: Avoid in-place key modification in set and multiset
**Item Summary**
Modifying a key element inside a `set` or `multiset` can break the sorted order, corrupting the container.

**Key Points**
- **Const Keys**: `map` and `multimap` enforce this by making keys `const`. `set` and `multiset` elements are not strictly `const` (in C++98, though later standards made them const), but you should treat them as such.
- **Safe Modification**: To change a key element safely:
    1. Locate the element.
    2. Copy it.
    3. Modify the copy.
    4. Erase the old element.
    5. Insert the new element (using the iterator as a hint).

**Example**
```cpp
std::set<int> s = {1, 2, 3};
auto it = s.find(2);
if (it != s.end()) {
    // *it = 4; // Error: Read-only
    // const_cast<int&>(*it) = 4; // UNDEFINED BEHAVIOR: corrupts order if 4 is misplace
    
    // Correct:
    int newVal = 4;
    s.erase(it);
    s.insert(newVal); 
}
```

### Item 23: Consider replacing associative containers with sorted vectors
**Item Summary**
For data structures that are initialized once and then read frequently, a sorted `vector` can outperform a `set` or `map`.

**Key Points**
- **Memory**: Vectors have no per-element overhead (pointers), whereas tree-based containers add 3 pointers per node.
- **Locality**: Vectors store elements contiguously, improving cache locality and reducing page faults during binary search.
- **Workflow**:
    1. **Setup**: Insert into vector, then `sort`.
    2. **Lookup**: Use `binary_search`, `lower_bound`, etc.
    3. **Reorganize**: Use this strategy only if lookups, insertions, and deletions are not intermixed frequently.

**Example**
```cpp
std::vector<string> dictionary;
// ... load dictionary ...
std::sort(dictionary.begin(), dictionary.end());

// Check if word exists
if (std::binary_search(dictionary.begin(), dictionary.end(), "word")) {
    // ...
}
// Faster / minimal memory compared to std::set<string>
```

### Item 24: Choose carefully between map::operator[] and map::insert when efficiency is important
**Item Summary**
`map::operator[]` provides "add or update" functionality but can be inefficient for adding new elements due to default construction.

**Key Points**
- **operator[]**:
    1. Checks if key exists.
    2. If not, *default constructs* a value, returns reference.
    3. You then *assign* to that value.
    - Cost: Default Construction + Assignment.
- **insert**:
    - `m.insert(make_pair(k, v))` constructs the pair directly.
    - Cost: Construction (associated with pair).
- **Guideline**: Use `insert` when adding new elements (avoids default construction + assignment). Use `operator[]` when updating existing elements (more readable).

**Example**
```cpp
std::map<int, Widget> m;
// Efficient Insert:
m.insert(std::make_pair(1, w)); // Copy constructs 'w' directly

// Inefficient Insert (via operator[]):
m[1] = w; 
// 1. m.find(1) fails.
// 2. new pair(1, Widget()) created (Default Ctor).
// 3. pair.second = w (Assignment).
```

### Item 25: Familiarize yourself with the nonstandard hashed containers
**Item Summary**
Standard C++ (C++98) did not include hash tables. However, non-standard implementations (`hash_set`, `hash_map`) were widely available (and are now standard `unordered_` containers in C++11).

**Key Points**
- **Performance**: Offer amortized O(1) lookup vs O(log n) for tree-based containers.
- **Interface**: Generally similar to standard associative containers but require a Hash Function and Equality Function.
- **Ordering**: Elements are unsorted.

## Chapter 4: Iterators

### Item 26: Prefer iterator to const_iterator, reverse_iterator, and const_reverse_iterator
**Item Summary**
Standard containers are biased towards `iterator`. Other iterator types often require jumps through hoops to function with container methods like `insert` and `erase`.

**Key Points**
- **Privilege**: Only `iterator` is accepted by all `insert` and `erase` member functions (in C++98).
- **Conversions**:
    - `iterator` converts to `const_iterator`.
    - `iterator` converts to `reverse_iterator`.
    - `reverse_iterator` converts to `iterator` (via `base()`).
    - *No* implicit conversion from `const_iterator` to `iterator`.
- **Comparisons**: Comparing `iterator` and `const_iterator` was not guaranteed to work on all compilers, though the standard required it.
- **Guideline**: Use `iterator` unless you have a specific reason not to.

**Example**
```cpp
std::vector<int> v;
// ...
std::vector<int>::const_iterator ci = v.begin();
// v.erase(ci); // Error in C++98: erase took iterator, not const_iterator.
// C++11 fixed this, but legacy code/compilers might struggle.
```

### Item 27: Use distance and advance to convert a container’s const_iterators to iterators
**Item Summary**
Since there is no direct conversion from `const_iterator` to `iterator`, you must manually calculate the position and advance a new iterator.

**Key Points**
- **The Problem**: `const_cast`ing a `const_iterator` to `iterator` is undefined behavior (and often doesn't compile).
- **The Solution**:
    1. Create a new `iterator` at `begin()`.
    2. Calculate `distance` from `begin()` to the `const_iterator`.
    3. `advance` the new iterator by that distance.
    ```cpp
    Iter i(d.begin());
    advance(i, distance<ConstIter>(i, ci));
    ```
- **Efficiency**: this is O(1) for random access iterators but O(n) for others.

**Example**
```cpp
std::deque<int> d = {1, 2, 3, 4, 5};
std::deque<int>::const_iterator ci = d.begin() + 3; // points to 4

// How to convert 'ci' to iterator to use with modifyable operations?
std::deque<int>::iterator i(d.begin());
std::advance(i, std::distance<std::deque<int>::const_iterator>(i, ci));
// i now points to 4
```

### Item 28: Understand how to use a reverse_iterator’s base iterator
**Item Summary**
`reverse_iterator::base()` returns an `iterator` pointing to the element *after* the one the `reverse_iterator` logically points to (due to the asymmetric ranges `[begin, end)`).

**Key Points**
- **Relationship**: `ri.base()` points next to `ri` in physical order.
    - If `ri` points to element `3`, `ri.base()` points to element `4`.
- **Insert**: `insert(ri.base(), val)` inserts *at* `ri`'s logical position. Safe and equivalent.
- **Erase**: `erase(ri.base())` erases the element *after* `ri`. To erase `ri` itself, you must erase the element *preceding* base: `v.erase((++ri).base())`.

**Example**
```cpp
std::vector<int> v = {1, 2, 3, 4, 5};
auto ri = std::find(v.rbegin(), v.rend(), 3); // ri -> 3
auto i = ri.base(); // i -> 4! (The element after 3)

// Insert works as expected (inserts before logical element 3, physically at 4)
v.insert(i, 99); // {1, 2, 99, 3, 4, 5}
```

### Item 29: Consider istreambuf_iterators for character-by-character input
**Item Summary**
`istream_iterator` uses formatted input (`operator>>`), which skips whitespace and has significant overhead (sentry objects, error checking). `istreambuf_iterator` bypasses this for raw character access.

**Key Points**
- **istream_iterator**: Skips whitespace by default. Slower for bulk character reading.
- **istreambuf_iterator**: Reads unformatted characters directly from the buffer. Does not skip whitespace. Much faster (up to 40%).
- **Usage**:
    ```cpp
    string data((istreambuf_iterator<char>(inputFile)),
                 istreambuf_iterator<char>());
    ```

## Chapter 5: Algorithms

### Item 30: Make sure destination ranges are big enough
**Item Summary**
Algorithms like `transform` write to a destination iterator but do not allocate memory. If the destination container isn't large enough, you will overwrite memory (undefined behavior).

**Key Points**
- **The Problem**: `transform(v.begin(), v.end(), results.end(), func);` is wrong if `results` is empty. It tries to write to non-existent elements.
- **Solution 1 (Reserve + Inserter)**: Use `back_inserter` (calls `push_back`), `front_inserter` (calls `push_front`), or `inserter` (calls `insert`).
    ```cpp
    transform(v.begin(), v.end(), back_inserter(results), func);
    ```
- **Solution 2 (Resize)**: Resize the destination container beforehand and use `begin()` as the destination.
    ```cpp
    results.resize(v.size());
    transform(v.begin(), v.end(), results.begin(), func);
    ```

**Example**
```cpp
std::vector<int> src = {1, 2, 3};
std::vector<int> dest;
// std::copy(src.begin(), src.end(), dest.begin()); // CRASH! dest too small.

// Fix:
std::copy(src.begin(), src.end(), std::back_inserter(dest));
```

### Item 31: Know your sorting options
**Item Summary**
`sort` is not the only way to order elements. If you only need partial ordering (e.g., top N items), other algorithms are faster and express intent better.

**Key Points**
- **`sort`**: Full sort. O(N log N).
- **`stable_sort`**: Preserves relative order of equivalent elements.
- **`partial_sort`**: Sorts the top N elements (puts them at the beginning). The rest are undefined.
- **`nth_element`**: Puts the element that *would* be at position N (in a sorted sequence) into that position. Elements before it are <=, elements after are >=. Great for finding medians or top-k sets without sorting them.
- **`partition` / `stable_partition`**: Reorders so elements satisfying a predicate come first. O(N).
- **Cost**: `partition` < `nth_element` < `partial_sort` < `sort` < `stable_sort`.

**Example**
```cpp
std::vector<int> v = {10, 5, 8, 1, 3};
// Sort top 2:
std::partial_sort(v.begin(), v.begin() + 2, v.end());
// v[0], v[1] are smallest two {1, 3}. Rest undefined order {10, 8, 5}.

// partition: items < 5 come first
std::partition(v.begin(), v.end(), [](int i){ return i < 5; });
```

### Item 32: Follow remove-like algorithms by erase if you really want to remove something
**Item Summary**
`remove` (and `remove_if`, `unique`) cannot change the size of a container because it only works with iterators. It moves elements to be kept to the front and returns a "logical end" iterator. Use container member `erase` to actually delete them.

**Key Points**
- **The Idiom**: `v.erase(remove(v.begin(), v.end(), 99), v.end());`
- **How `remove` works**: It shuffles "kept" elements forward to overwrite "removed" ones.
- **List Exception**: `list::remove` and `list::unique` *do* actually remove elements and are more efficient.

**Example**
```cpp
std::vector<int> v = {1, 2, 3, 99, 4};
// v.size() is 5
auto newEnd = std::remove(v.begin(), v.end(), 99); 
// v.size() is still 5! v might be {1, 2, 3, 4, 4}
// newEnd points to the second 4.

v.erase(newEnd, v.end()); 
// v.size() is now 4. {1, 2, 3, 4}
```

### Item 33: Be wary of remove-like algorithms on containers of pointers
**Item Summary**
Because `remove` overwrites elements, if you use it on a container of pointers, you lose the pointers to the "removed" objects (they get overwritten) before you can `delete` them, causing leaks.

**Key Points**
- **Problem**: `remove` doesn't `delete` what pointers point to. It just overwrites the pointer values.
- **Solution 1**: Delete the objects and set pointers to `nullptr` *before* calling `remove`. Then remove `nullptr`s.
- **Solution 2**: Use smart pointers (e.g., `shared_ptr`) which handle destruction automatically when overwritten/erased.

**Example**
```cpp
std::vector<Widget*> v; 
// ...
// std::remove(v.begin(), v.end(), nullptr); 
// Pointers are overwritten. Objects they pointed to are LEAKED.

// Fix:
// 1. Partition
auto it = std::stable_partition(v.begin(), v.end(), [](Widget* w){ return w != nullptr; });
// 2. Delete garbage
for (auto i = it; i != v.end(); ++i) delete *i;
// 3. Erase
v.erase(it, v.end());
```

### Item 34: Note which algorithms expect sorted ranges
**Item Summary**
Many algorithms (e.g., `binary_search`, `set_union`) require sorted ranges to function correctly (or efficiently). Passing unsorted ranges leads to undefined behavior.

**Key Points**
- **Algorithms**: `binary_search`, `lower_bound`, `upper_bound`, `equal_range`, `merge`, `set_...` (union/intersection/etc.), `includes`.
- **Constraint**: The range must be sorted using the *same* predicate passed to the algorithm.
- **Performance**: These usually offer O(log N) or O(N) performance, which is only possible with sorted data.

**Example**
```cpp
std::vector<int> v = {1, 3, 2}; // Unsorted!
// bool found = std::binary_search(v.begin(), v.end(), 3); // Undefined Behavior!

std::sort(v.begin(), v.end());
bool found = std::binary_search(v.begin(), v.end(), 3); // OK
```

### Item 35: Implement simple case-insensitive string comparisons via mismatch or lexicographical_compare
**Item Summary**
Standard `string` comparisons are case-sensitive. For case-insensitivity without complex locales, you can use STL algorithms with a custom character comparator.

**Key Points**
- **Comparator**: Write a `ciCharLess` function that calls `tolower` on both chars.
- **Comparison**: Use `lexicographical_compare` with `ciCharLess` to emulate `operator<`.
- **Alternative**: For `strcmp`-style behavior (returning -1, 0, 1), use `mismatch` to find the first differing pair, then compare them.

**Example**
```cpp
bool ciCharLess(char c1, char c2) {
    return std::tolower(static_cast<unsigned char>(c1)) < 
           std::tolower(static_cast<unsigned char>(c2));
}

std::string s1 = "Apple", s2 = "apple";
// Compare using custom comparator
bool less = std::lexicographical_compare(s1.begin(), s1.end(), 
                                         s2.begin(), s2.end(), ciCharLess);
```

### Item 36: Understand the proper implementation of copy_if
**Item Summary**
`copy_if` was missing from C++98 (added in C++11). Implementing it correctly is trivial but often done wrong.

**Key Points**
- **Implementation**:
    ```cpp
    template <typename InputIterator, typename OutputIterator, typename Predicate>
    OutputIterator copy_if(InputIterator begin, InputIterator end, OutputIterator dest, Predicate p) {
        while (begin != end) {
            if (p(*begin)) *dest++ = *begin;
            ++begin;
        }
        return dest;
    }
    ```
- **Note**: `remove_copy_if` copies elements where predicate is *false*. A double-negative (`not1`) can simulate `copy_if` but requires adaptable functors. The explicit loop above is better.

### Item 37: Use accumulate or for_each to summarize ranges
**Item Summary**
To boil a range down to a single value (sum, product, average), use `accumulate` (from `<numeric>`). For more complex summaries requiring side effects, `for_each` is permissible.

**Key Points**
- **accumulate**: `T result = accumulate(begin, end, initial_value, binary_op);`
    - Make sure `initial_value` has the correct type (e.g., `0.0` for double, not `0`).
    - Standard forbids side effects in the binary operation (though it often works).
- **for_each**: Returns the function object after applying it to the range.
    - Can accumulate state in the functor's members.
    - Explicitly allowed to have side effects.

**Example**
```cpp
std::vector<int> v = {1, 2, 3};
// Accumulate (Fold)
int sum = std::accumulate(v.begin(), v.end(), 0); // 6

// For_each (Side effects)
std::for_each(v.begin(), v.end(), [](int& i){ i *= 2; }); 
// v is now {2, 4, 6}
```

## Chapter 6: Functors, Functor Classes, Functions, etc.

### Item 38: Design functor classes for pass-by-value
**Item Summary**
In the STL, function objects are passed by value (copied). This mirrors how function pointers are passed in C.

**Key Points**
- **Implication**: Functors should be small and valid to copy.
- **Polymorphism**: Do *not* pass polymorphic functors by value to algorithms like `for_each`, as this causes **slicing**. The derived parts are stripped off.
- **Pattern**: If you need a large or polymorphic functor, use the **Bridge Pattern** (Pimpl idiom). The functor holds a pointer (e.g., `shared_ptr`) to the implementation class. The functor itself remains small and copyable, merely forwarding calls.

**Example**
```cpp
// Problem: Heavy functor
struct BigFunctor {
    int data[1000]; 
    void operator()(int i) const { /* ... */ }
};

// Solution: Bridge
struct BridgeFunctor {
    std::shared_ptr<BigFunctor> pImpl; // Cheap to copy
    void operator()(int i) const { (*pImpl)(i); }
};
```

### Item 39: Make predicates pure functions
**Item Summary**
A **predicate** is a function returning `bool`. It must be a **pure function**, meaning its result depends *only* on its arguments. It should not modify internal state that affects the result.

**Key Points**
- **The Risk**: Algorithms may accept predicates by value and copy them.
    - Example: `remove_if` may internally copy the predicate to `find_if` and then use another copy for `remove_copy_if`.
    - If your predicate relies on internal state (e.g., "return true the 3rd time called"), the copies track state independently, leading to incorrect behavior (removing the 3rd element twice).
- **Rule**: Declare `operator()` as `const` in predicate classes. Ensure no mutable state affects the return value.

**Example**
```cpp
struct BadPredicate {
    int count = 0;
    // Removing the 3rd element... but if copied, multiple elements might be the "3rd"
    bool operator()(int i) { return ++count == 3; } 
};
// Algorithms might make copies of BadPredicate internally!
```

### Item 40: Make functor classes adaptable
**Item Summary**
Function adapters like `not1`, `bind2nd`, etc., require certain typedefs (`argument_type`, `result_type`, etc.) to be defined in the functor class.

**Key Points**
- **Adaptable**: A functor that defines these typedefs.
- **How**: Inherit from `std::unary_function<Arg, Result>` or `std::binary_function<Arg1, Arg2, Result>`.
    - Example: `struct IsOdd : public std::unary_function<int, bool> { ... };`
- **Benefit**: Allows users to write `find_if(..., not1(IsOdd()));`. Without inheritance, `not1` will fail to compile.

**Example**
```cpp
// C++98 style adaptation
struct IsOdd : public std::unary_function<int, bool> { 
    bool operator()(int i) const { return i % 2 != 0; }
};

std::vector<int> v;
// std::not1 requires argument_type typedefs provided by inheritance
auto it = std::find_if(v.begin(), v.end(), std::not1(IsOdd()));
```

### Item 41: Understand the reasons for ptr_fun, mem_fun, and mem_fun_ref
**Item Summary**
These are adapters that harmonize calling syntax so algorithms can use them uniformly.

**Key Points**
- **Syntax Variations**:
    1. Function: `f(x)`
    2. Member Function (Object): `x.f()`
    3. Member Function (Pointer): `p->f()`
- **STL Expectation**: Algorithms hardcode syntax #1 (`f(x)`).
- **Adapters**:
    - `ptr_fun`: Adds typedefs to standard function pointers (makes them adaptable). Use if you need to use `not1` on a function pointer.
    - `mem_fun`: Adapts syntax #3 to #1. Takes a `T*`, calls `p->f()`.
    - `mem_fun_ref`: Adapts syntax #2 to #1. Takes a `T&`, calls `x.f()`.

### Item 42: Make sure less<T> means operator<
**Item Summary**
`std::less<T>` is the default comparator for many containers. Programmers expect it to do what `operator<` does. Specializing it to do something else violates the principle of least astonishment.

**Key Points**
- **Don't**: Do not specialize `std::less<Widget>` to sort by "MaxSpeed" while `operator<` sorts by "Weight".
- **Do**: Create a separate functor `MaxSpeedCompare` and use that for the container type: `set<Widget, MaxSpeedCompare>`.
- Exception: You *can* specialize `std::less` for user-defined types (like smart pointers) to make them behave *more* like the default `operator<` (e.g., sorting by content instead of pointer address), but be careful.

**Example**
```cpp
// Don't do this!
// User expects less<Widget> to mean w1 < w2
template<> struct std::less<Widget> {
    bool operator()(const Widget& a, const Widget& b) const {
        return a.maxSpeed < b.maxSpeed; // Surprising!
    }
};

// Do this:
struct MaxSpeedCompare {
    bool operator()(const Widget& a, const Widget& b) const { ... }
};
```

## Chapter 7: Programming with the STL

### Item 43: Prefer algorithm calls to hand-written loops
**Item Summary**
Algorithms are generally superior to hand-written loops in terms of efficiency, correctness, and maintainability.

**Key Points**
- **Efficiency**: Library implementers can optimize algorithms (e.g., using knowledge of internal data structures). Algorithms often avoid repeated re-evaluation of `end()` checks.
- **Correctness**: Hand-written loops are prone to iterator invalidation errors (e.g., `insert` or `erase` inside a loop). Algorithms handle this correctly.
- **Clarity**: Names like `transform`, `find`, or `partition` convey intent immediately. A raw `for` loop requires parsing the body to understand what it does.

### Item 44: Prefer member functions to algorithms with the same names
**Item Summary**
If a container implies a member function with the same name as a standard algorithm (e.g., `set::find` vs `std::find`), use the member function.

**Key Points**
- **Associative Containers**: `set::find` is O(log N). `std::find` is O(N).
    - Also, member functions use **equivalence** (which is correct for these containers), while `std::find` uses **equality**.
- **Lists**: `list::remove`, `list::unique`, `list::sort`, etc. operate on pointers without copying data and are much more efficient. `std::sort` typically doesn't even work on `list` iterators (as they are not random access).

### Item 45: Distinguish among count, find, binary_search, lower_bound, upper_bound, and equal_range
**Item Summary**
Choose the search algorithm that best fits your question and data structure.

**Key Points**
- **Unsorted Ranges**: Use `count` (how many?) or `find` (where is the first?). O(N).
- **Sorted Ranges**: Use O(log N) algorithms.
    - `binary_search`: Returns `bool` (is it there?).
    - `lower_bound`: Returns iterator to first element `>=` value. Useful for finding insertion points.
    - `upper_bound`: Returns iterator to first element `>` value.
    - `equal_range`: Returns pair `{lower_bound, upper_bound}`. Best for "where are all/any of them?"
- **Associative Containers**: Use member functions (`count`, `find`, `lower_bound`, etc.) for performance and correctness.

### Item 46: Consider function objects instead of functions as algorithm parameters
**Item Summary**
Passing function objects (functors) is often faster than passing raw function pointers due to inlining.

**Key Points**
- **Inlining**: Comparison functors like `std::greater<double>` are easily inlined by the compiler. Function pointers passed to `std::sort` usually result in indirect function calls that are not inlined.
- **Benchmarks**: `std::sort` with a functor can be significantly faster than C's `qsort` or `std::sort` with a function pointer.
- **Robustness**: Functors help avoid subtle compilation issues involving templates and linkage.

### Item 47: Avoid producing write-only code
**Item Summary**
The STL encourages combining algorithms and adapters, but code can become unreadable ("write-only") if heavily nested.

**Key Points**
- **Complexity**: `v.erase(remove_if(find_if(base(...)), ...))` is powerful but hard to read.
- **Advice**: Break complex statements into smaller, named parts (e.g., defining limits or range iterators separately) or use comments liberally.
- **Maintainability**: Code is read more often than written. Favor clarity over clever one-liners.

### Item 48: Always #include the proper headers
**Item Summary**
Standard headers do not necessarily include other standard headers. Relying on transitive includes is non-portable.

**Key Points**
- **Rule**: If you use a component, `#include` its header.
- **Mapping**:
    - `<vector>`, `<list>`, etc. for containers.
    - `<algorithm>` for most algorithms.
    - `<numeric>` for `accumulate`, `inner_product`.
    - `<functional>` for functors (`less`, `greater`) and adapters (`not1`, `bind2nd`).
    - `<iterator>` for `istream_iterator`, `back_inserter`.

### Item 49: Learn to decipher STL-related compiler diagnostics
**Item Summary**
STL templates can generate massive, cryptic error messages. Learning to read them is a necessary skill.

**Key Points**
- **Strategy**:
    - Replace `std::basic_string<char, ...>` with `string` in your head (or use a script).
    - Ignore internal implementation templates (like `_Tree`, `__rb_tree`).
    - Look for the core type mismatch (e.g., passing a `const_iterator` to a function expecting an `iterator`).
    - Remember that `string` is a typedef, not a class, which often confuses diagnostic output.

### Item 50: Familiarize yourself with STL-related web sites
**Item Summary**
References and documentation are vital.

**Key Points**
- (Note: This item refers to specific websites from the early 2000s, but the principle holds: use updated references like cppreference.com).
- **Resources**: SGI STL docs (historic), Boost (for extensions), and modern C++ documentation sites.
