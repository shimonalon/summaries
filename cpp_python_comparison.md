# C++ STL & Algorithms vs Python Comparison Guide

A comprehensive comparison of C++ Standard Template Library (STL) features with their Python equivalents, complete with practical examples.

---

## Table of Contents
- [Algorithms](#algorithms)
  - [Sorting & Searching](#sorting--searching)
  - [Binary Search Algorithms](#binary-search-algorithms)
  - [Modifying Operations](#modifying-operations)
  - [Non-Modifying Operations](#non-modifying-operations)
- [Data Structures](#data-structures)
- [Math Functions](#math-functions)
- [String Functions](#string-functions)
- [Functors](#functors)
- [List Operations](#list-operations)
- [Language-Specific Features](#language-specific-features)
  - [Python-Specific Features](#python-specific-features)
  - [C++-Specific Features](#c-specific-features)

---

## Algorithms

### Sorting & Searching

#### Sorting

**Time Complexity:** 
- C++: O(n log n) average and worst case
- Python: O(n log n) average and worst case (Timsort)

**Space Complexity:** O(log n) to O(n)

##### C++
```cpp
#include <algorithm>
#include <vector>

std::vector<int> nums = {5, 2, 8, 1, 9};
std::sort(nums.begin(), nums.end());
// nums is now {1, 2, 5, 8, 9}

// Sort in descending order
std::sort(nums.begin(), nums.end(), std::greater<int>());
```

##### Python
```python
nums = [5, 2, 8, 1, 9]
nums.sort()  # In-place sorting
# nums is now [1, 2, 5, 8, 9]

# Or create a new sorted list
sorted_nums = sorted(nums)

# Sort in descending order
nums.sort(reverse=True)
```

**Key Differences:**
- C++ requires iterators (`begin()`, `end()`), Python works directly on lists
- Python has both `sort()` (in-place) and `sorted()` (returns new list)

---

### Binary Search Algorithms

#### binary_search

**Time Complexity:** O(log n)  
**Space Complexity:** O(1)  
**Precondition:** Container must be sorted

##### C++
```cpp
#include <algorithm>
#include <vector>

std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};

// Check if element exists
bool found = std::binary_search(nums.begin(), nums.end(), 5);
// found: true

bool not_found = std::binary_search(nums.begin(), nums.end(), 10);
// not_found: false
```

##### Python
```python
import bisect

nums = [1, 2, 3, 4, 5, 6, 7, 8, 9]

# Check if element exists (manual implementation)
def binary_search(arr, target):
    left, right = 0, len(arr) - 1
    while left <= right:
        mid = (left + right) // 2
        if arr[mid] == target:
            return True
        elif arr[mid] < target:
            left = mid + 1
        else:
            right = mid - 1
    return False

found = binary_search(nums, 5)  # True
not_found = binary_search(nums, 10)  # False

# Or use bisect to check
def binary_search_bisect(arr, target):
    i = bisect.bisect_left(arr, target)
    return i != len(arr) and arr[i] == target

found = binary_search_bisect(nums, 5)  # True
```

---

#### lower_bound

**Time Complexity:** O(log n)  
**Space Complexity:** O(1)  
**Description:** Returns iterator/index to first element >= value

##### C++
```cpp
#include <algorithm>
#include <vector>

std::vector<int> nums = {1, 2, 4, 4, 4, 6, 7, 8};

// Find first element >= 4
auto it = std::lower_bound(nums.begin(), nums.end(), 4);
int index = std::distance(nums.begin(), it);
// index: 2, *it: 4

// Find first element >= 5
auto it2 = std::lower_bound(nums.begin(), nums.end(), 5);
int index2 = std::distance(nums.begin(), it2);
// index2: 5, *it2: 6

// If not found, returns end()
auto it3 = std::lower_bound(nums.begin(), nums.end(), 10);
// it3 == nums.end()
```

##### Python
```python
import bisect

nums = [1, 2, 4, 4, 4, 6, 7, 8]

# Find first element >= 4
index = bisect.bisect_left(nums, 4)
# index: 2, nums[index]: 4

# Find first element >= 5
index2 = bisect.bisect_left(nums, 5)
# index2: 5, nums[index2]: 6

# If not found, returns len(nums)
index3 = bisect.bisect_left(nums, 10)
# index3: 8 (len(nums))
```

**Key Differences:**
- C++ returns iterator, Python returns index
- C++ `lower_bound` finds first element not less than value
- Python `bisect_left` has same behavior

---

#### upper_bound

**Time Complexity:** O(log n)  
**Space Complexity:** O(1)  
**Description:** Returns iterator/index to first element > value

##### C++
```cpp
#include <algorithm>
#include <vector>

std::vector<int> nums = {1, 2, 4, 4, 4, 6, 7, 8};

// Find first element > 4
auto it = std::upper_bound(nums.begin(), nums.end(), 4);
int index = std::distance(nums.begin(), it);
// index: 5, *it: 6

// Find first element > 1
auto it2 = std::upper_bound(nums.begin(), nums.end(), 1);
int index2 = std::distance(nums.begin(), it2);
// index2: 1, *it2: 2

// Count occurrences using lower_bound and upper_bound
auto lower = std::lower_bound(nums.begin(), nums.end(), 4);
auto upper = std::upper_bound(nums.begin(), nums.end(), 4);
int count = std::distance(lower, upper);
// count: 3
```

##### Python
```python
import bisect

nums = [1, 2, 4, 4, 4, 6, 7, 8]

# Find first element > 4
index = bisect.bisect_right(nums, 4)
# Also: bisect.bisect(nums, 4)
# index: 5, nums[index]: 6

# Find first element > 1
index2 = bisect.bisect_right(nums, 1)
# index2: 1, nums[index2]: 2

# Count occurrences using bisect_left and bisect_right
lower = bisect.bisect_left(nums, 4)
upper = bisect.bisect_right(nums, 4)
count = upper - lower
# count: 3
```

**Key Differences:**
- C++ `upper_bound` = Python `bisect_right` (or `bisect`)
- Both find first element strictly greater than value
- Useful for finding ranges of equal elements

---

#### equal_range

**Time Complexity:** O(log n)  
**Space Complexity:** O(1)  
**Description:** Returns pair of iterators/indices defining range of elements equal to value

##### C++
```cpp
#include <algorithm>
#include <vector>

std::vector<int> nums = {1, 2, 4, 4, 4, 6, 7, 8};

// Find range of elements equal to 4
auto range = std::equal_range(nums.begin(), nums.end(), 4);
int start = std::distance(nums.begin(), range.first);
int end = std::distance(nums.begin(), range.second);
// start: 2, end: 5
// Elements in range [start, end): 4, 4, 4

int count = std::distance(range.first, range.second);
// count: 3
```

##### Python
```python
import bisect

nums = [1, 2, 4, 4, 4, 6, 7, 8]

# Find range of elements equal to 4
start = bisect.bisect_left(nums, 4)
end = bisect.bisect_right(nums, 4)
# start: 2, end: 5
# Elements in range [start:end]: [4, 4, 4]

count = end - start
# count: 3

# Or create a function
def equal_range(arr, value):
    return bisect.bisect_left(arr, value), bisect.bisect_right(arr, value)

start, end = equal_range(nums, 4)
```

---

### Modifying Operations

#### Copy & Move

**Time Complexity:** O(n) where n is the number of elements  
**Space Complexity:** O(1) additional space

##### C++ - Copy
```cpp
#include <algorithm>
#include <vector>

std::vector<int> source = {1, 2, 3, 4, 5};
std::vector<int> dest(5);

// Copy forward
std::copy(source.begin(), source.end(), dest.begin());

// Copy backward
std::copy_backward(source.begin(), source.end(), dest.end());
```

#### Python - Copy
```python
source = [1, 2, 3, 4, 5]

# Shallow copy
dest = source.copy()
# Or
dest = source[:]
# Or
import copy
dest = copy.copy(source)

# Deep copy (for nested structures)
dest = copy.deepcopy(source)
```

#### C++ - Move
```cpp
#include <algorithm>
#include <vector>

std::vector<int> source = {1, 2, 3, 4, 5};
std::vector<int> dest(5);

// Move elements (source elements become unspecified)
std::move(source.begin(), source.end(), dest.begin());
```

#### Python - Move
```python
# Python doesn't have explicit "move" semantics
# Assignment creates a new reference
dest = source  # Both point to same list
source = None  # Original reference removed
```

**Key Differences:**
- C++ has explicit move semantics for performance
- Python uses reference counting; "moving" is just reassigning references

---

### Swap

#### C++
```cpp
#include <algorithm>

int x = 10, y = 20;
std::swap(x, y);
// x = 20, y = 10

std::vector<int> v1 = {1, 2, 3};
std::vector<int> v2 = {4, 5, 6};
std::swap(v1, v2);
```

#### Python
```python
x, y = 10, 20
x, y = y, x  # Pythonic swap
# x = 20, y = 10

# Or for any objects
v1 = [1, 2, 3]
v2 = [4, 5, 6]
v1, v2 = v2, v1
```

---

### Merge

#### C++
```cpp
#include <algorithm>
#include <vector>

std::vector<int> v1 = {1, 3, 5};
std::vector<int> v2 = {2, 4, 6};
std::vector<int> result(6);

std::merge(v1.begin(), v1.end(), 
           v2.begin(), v2.end(), 
           result.begin());
// result: {1, 2, 3, 4, 5, 6}
```

#### Python
```python
v1 = [1, 3, 5]
v2 = [2, 4, 6]

# Using merge from heapq
import heapq
result = list(heapq.merge(v1, v2))
# result: [1, 2, 3, 4, 5, 6]

# Or simply concatenate and sort
result = sorted(v1 + v2)
```

---

### Replace

#### C++
```cpp
#include <algorithm>
#include <vector>

std::vector<int> nums = {1, 2, 3, 2, 4, 2};

// Replace all 2s with 9
std::replace(nums.begin(), nums.end(), 2, 9);
// nums: {1, 9, 3, 9, 4, 9}

// Replace if condition is met
std::replace_if(nums.begin(), nums.end(), 
                [](int x) { return x > 5; }, 0);
```

#### Python
```python
nums = [1, 2, 3, 2, 4, 2]

# List comprehension
nums = [9 if x == 2 else x for x in nums]
# nums: [1, 9, 3, 9, 4, 9]

# Replace if condition is met
nums = [0 if x > 5 else x for x in nums]
```

---

### Remove

#### C++
```cpp
#include <algorithm>
#include <vector>

std::vector<int> nums = {1, 2, 3, 2, 4, 2};

// Remove all 2s (doesn't actually resize, use erase-remove idiom)
auto new_end = std::remove(nums.begin(), nums.end(), 2);
nums.erase(new_end, nums.end());
// nums: {1, 3, 4}

// Remove if condition is met
nums.erase(std::remove_if(nums.begin(), nums.end(),
                          [](int x) { return x > 3; }),
           nums.end());
```

#### Python
```python
nums = [1, 2, 3, 2, 4, 2]

# Remove all occurrences of 2
nums = [x for x in nums if x != 2]
# nums: [1, 3, 4]

# Remove if condition is met
nums = [x for x in nums if x <= 3]

# Or use filter
nums = list(filter(lambda x: x != 2, nums))
```

---

### Transform (Map)

#### C++
```cpp
#include <algorithm>
#include <vector>

std::vector<int> nums = {1, 2, 3, 4, 5};
std::vector<int> squared(5);

// Unary operation
std::transform(nums.begin(), nums.end(), 
               squared.begin(),
               [](int x) { return x * x; });
// squared: {1, 4, 9, 16, 25}

// Binary operation
std::vector<int> v1 = {1, 2, 3};
std::vector<int> v2 = {4, 5, 6};
std::vector<int> result(3);

std::transform(v1.begin(), v1.end(), 
               v2.begin(), result.begin(),
               [](int a, int b) { return a + b; });
// result: {5, 7, 9}
```

#### Python
```python
nums = [1, 2, 3, 4, 5]

# Using map
squared = list(map(lambda x: x * x, nums))
# squared: [1, 4, 9, 16, 25]

# List comprehension (more Pythonic)
squared = [x * x for x in nums]

# Binary operation
v1 = [1, 2, 3]
v2 = [4, 5, 6]
result = [a + b for a, b in zip(v1, v2)]
# result: [5, 7, 9]

# Or using map
result = list(map(lambda a, b: a + b, v1, v2))
```

---

### Copy If (Filter)

#### C++
```cpp
#include <algorithm>
#include <vector>

std::vector<int> nums = {1, 2, 3, 4, 5, 6};
std::vector<int> evens;

std::copy_if(nums.begin(), nums.end(), 
             std::back_inserter(evens),
             [](int x) { return x % 2 == 0; });
// evens: {2, 4, 6}
```

#### Python
```python
nums = [1, 2, 3, 4, 5, 6]

# Using filter
evens = list(filter(lambda x: x % 2 == 0, nums))
# evens: [2, 4, 6]

# List comprehension (more Pythonic)
evens = [x for x in nums if x % 2 == 0]
```

---

### Accumulate (Reduce)

#### C++
```cpp
#include <numeric>
#include <vector>

std::vector<int> nums = {1, 2, 3, 4, 5};

// Sum
int sum = std::accumulate(nums.begin(), nums.end(), 0);
// sum: 15

// Product
int product = std::accumulate(nums.begin(), nums.end(), 1,
                              [](int a, int b) { return a * b; });
// product: 120

// Custom operation
int result = std::accumulate(nums.begin(), nums.end(), 0,
                             [](int a, int b) { return a + b * b; });
```

#### Python
```python
from functools import reduce

nums = [1, 2, 3, 4, 5]

# Sum (built-in)
total = sum(nums)
# total: 15

# Product
from math import prod
product = prod(nums)
# product: 120

# Or using reduce
product = reduce(lambda a, b: a * b, nums, 1)

# Custom operation
result = reduce(lambda a, b: a + b * b, nums, 0)
```

---

### Partial Sum

#### C++
```cpp
#include <numeric>
#include <vector>

std::vector<int> nums = {1, 2, 3, 4, 5};
std::vector<int> cumsum(5);

std::partial_sum(nums.begin(), nums.end(), cumsum.begin());
// cumsum: {1, 3, 6, 10, 15}

// Custom operation (cumulative product)
std::partial_sum(nums.begin(), nums.end(), cumsum.begin(),
                 [](int a, int b) { return a * b; });
// cumsum: {1, 2, 6, 24, 120}
```

#### Python
```python
from itertools import accumulate

nums = [1, 2, 3, 4, 5]

# Cumulative sum
cumsum = list(accumulate(nums))
# cumsum: [1, 3, 6, 10, 15]

# Custom operation (cumulative product)
from operator import mul
cumprod = list(accumulate(nums, mul))
# cumprod: [1, 2, 6, 24, 120]

# Or manual implementation
cumsum = []
total = 0
for num in nums:
    total += num
    cumsum.append(total)
```

---

### For Each

#### C++
```cpp
#include <algorithm>
#include <vector>
#include <iostream>

std::vector<int> nums = {1, 2, 3, 4, 5};

// Print each element
std::for_each(nums.begin(), nums.end(),
              [](int x) { std::cout << x << " "; });

// Modify each element (by reference)
std::for_each(nums.begin(), nums.end(),
              [](int& x) { x *= 2; });
// nums: {2, 4, 6, 8, 10}
```

#### Python
```python
nums = [1, 2, 3, 4, 5]

# Print each element
for num in nums:
    print(num, end=" ")

# Apply function to each (without modifying)
list(map(lambda x: print(x), nums))

# Modify each element (list comprehension)
nums = [x * 2 for x in nums]
# nums: [2, 4, 6, 8, 10]
```

---

### All Of, Any Of, None Of

#### C++
```cpp
#include <algorithm>
#include <vector>

std::vector<int> nums = {2, 4, 6, 8, 10};

// Check if all are even
bool all_even = std::all_of(nums.begin(), nums.end(),
                            [](int x) { return x % 2 == 0; });
// true

// Check if any are > 5
bool any_large = std::any_of(nums.begin(), nums.end(),
                             [](int x) { return x > 5; });
// true

// Check if none are negative
bool none_neg = std::none_of(nums.begin(), nums.end(),
                             [](int x) { return x < 0; });
// true
```

#### Python
```python
nums = [2, 4, 6, 8, 10]

# Check if all are even
all_even = all(x % 2 == 0 for x in nums)
# True

# Check if any are > 5
any_large = any(x > 5 for x in nums)
# True

# Check if none are negative
none_neg = not any(x < 0 for x in nums)
# True
# Or
none_neg = all(x >= 0 for x in nums)
```

---

### Find

#### C++
```cpp
#include <algorithm>
#include <vector>

std::vector<int> nums = {1, 2, 3, 4, 5};

// Find value
auto it = std::find(nums.begin(), nums.end(), 3);
if (it != nums.end()) {
    int index = std::distance(nums.begin(), it);
    // index: 2
}

// Find if condition is met
auto it2 = std::find_if(nums.begin(), nums.end(),
                        [](int x) { return x > 3; });
// Points to 4

// Find if condition is NOT met
auto it3 = std::find_if_not(nums.begin(), nums.end(),
                            [](int x) { return x < 3; });
// Points to 3
```

#### Python
```python
nums = [1, 2, 3, 4, 5]

# Find value
if 3 in nums:
    index = nums.index(3)
    # index: 2

# Find if condition is met
result = next((x for x in nums if x > 3), None)
# result: 4

# Or get index
try:
    index = next(i for i, x in enumerate(nums) if x > 3)
    # index: 3
except StopIteration:
    index = -1

# Find if condition is NOT met
result = next((x for x in nums if not x < 3), None)
# result: 3
```

---

### Adjacent Find

#### C++
```cpp
#include <algorithm>
#include <vector>

std::vector<int> nums = {1, 2, 3, 3, 4, 5};

// Find consecutive equal elements
auto it = std::adjacent_find(nums.begin(), nums.end());
if (it != nums.end()) {
    int index = std::distance(nums.begin(), it);
    // index: 2 (points to first 3)
}

// Custom predicate
auto it2 = std::adjacent_find(nums.begin(), nums.end(),
                              [](int a, int b) { return b - a == 1; });
```

#### Python
```python
nums = [1, 2, 3, 3, 4, 5]

# Find consecutive equal elements
index = next((i for i in range(len(nums) - 1) 
              if nums[i] == nums[i + 1]), -1)
# index: 2

# Custom predicate
index = next((i for i in range(len(nums) - 1) 
              if nums[i + 1] - nums[i] == 1), -1)

# Or using zip
for i, (a, b) in enumerate(zip(nums, nums[1:])):
    if a == b:
        print(f"Found at index {i}")
        break
```

---

### Count

#### C++
```cpp
#include <algorithm>
#include <vector>

std::vector<int> nums = {1, 2, 3, 2, 4, 2};

// Count occurrences of value
int count = std::count(nums.begin(), nums.end(), 2);
// count: 3

// Count if condition is met
int count_large = std::count_if(nums.begin(), nums.end(),
                                [](int x) { return x > 2; });
// count_large: 2
```

#### Python
```python
nums = [1, 2, 3, 2, 4, 2]

# Count occurrences of value
count = nums.count(2)
# count: 3

# Count if condition is met
count_large = sum(1 for x in nums if x > 2)
# count_large: 2

# Or
count_large = len([x for x in nums if x > 2])
```

---

## Data Structures

### Vector / List (Dynamic Array)

**C++ `std::vector`** vs **Python `list`**

#### Common Operations Complexity

| Operation | C++ vector | Python list |
|-----------|------------|-------------|
| Access by index | O(1) | O(1) |
| Append (push_back) | O(1) amortized | O(1) amortized |
| Insert at beginning | O(n) | O(n) |
| Insert at end | O(1) amortized | O(1) amortized |
| Delete at end | O(1) | O(1) |
| Delete at beginning | O(n) | O(n) |
| Search | O(n) | O(n) |

##### C++
```cpp
#include <vector>

std::vector<int> vec = {1, 2, 3};

// Access - O(1)
int first = vec[0];
int last = vec.back();

// Append - O(1) amortized
vec.push_back(4);

// Insert - O(n)
vec.insert(vec.begin(), 0);  // Insert at beginning

// Remove - O(1) for end, O(n) otherwise
vec.pop_back();  // Remove last

// Size - O(1)
size_t sz = vec.size();

// Clear - O(n)
vec.clear();
```

##### Python
```python
lst = [1, 2, 3]

# Access - O(1)
first = lst[0]
last = lst[-1]

# Append - O(1) amortized
lst.append(4)

# Insert - O(n)
lst.insert(0, 0)  # Insert at beginning

# Remove - O(1) for end, O(n) otherwise
lst.pop()  # Remove last
lst.pop(0)  # Remove first - O(n)

# Size - O(1)
sz = len(lst)

# Clear - O(n)
lst.clear()
```

---

### Set (Ordered Set)

**C++ `std::set`** vs **Python `sortedcontainers.SortedSet`** or manual approach

#### Common Operations Complexity

| Operation | C++ set | Python set | Python SortedSet |
|-----------|---------|------------|------------------|
| Insert | O(log n) | O(1) average | O(log n) |
| Delete | O(log n) | O(1) average | O(log n) |
| Search | O(log n) | O(1) average | O(log n) |
| Iteration (sorted) | O(n) | O(n log n) | O(n) |

##### C++ - std::set (ordered, based on red-black tree)
```cpp
#include <set>

std::set<int> s = {3, 1, 4, 1, 5};  // Automatically sorted, duplicates removed
// s: {1, 3, 4, 5}

// Insert - O(log n)
s.insert(2);

// Find - O(log n)
auto it = s.find(3);
if (it != s.end()) {
    // Found
}

// Contains (C++20) - O(log n)
bool exists = s.contains(3);

// Erase - O(log n)
s.erase(3);

// Size - O(1)
size_t sz = s.size();

// Lower/upper bound - O(log n)
auto lower = s.lower_bound(3);
auto upper = s.upper_bound(3);

// Iterate (in sorted order)
for (int x : s) {
    // Process x
}
```

##### Python - set (unordered, hash-based)
```python
s = {3, 1, 4, 1, 5}  # Duplicates removed, NOT sorted
# s: {1, 3, 4, 5} (order not guaranteed)

# Insert - O(1) average
s.add(2)

# Search - O(1) average
if 3 in s:
    # Found
    pass

# Remove - O(1) average
s.discard(3)  # No error if not found
s.remove(3)   # Raises KeyError if not found

# Size - O(1)
sz = len(s)

# Iterate (unordered)
for x in s:
    # Process x
    pass

# Get sorted list - O(n log n)
sorted_list = sorted(s)
```

##### Python - SortedSet (ordered, requires sortedcontainers package)
```python
from sortedcontainers import SortedSet

s = SortedSet([3, 1, 4, 1, 5])  # Automatically sorted
# s: SortedSet([1, 3, 4, 5])

# Insert - O(log n)
s.add(2)

# Search - O(log n)
if 3 in s:
    # Found
    pass

# Remove - O(log n)
s.discard(3)

# Size - O(1)
sz = len(s)

# Binary search operations - O(log n)
import bisect
index = s.bisect_left(3)

# Iterate (in sorted order)
for x in s:
    # Process x
    pass
```

---

### Unordered Set (Hash Set)

**C++ `std::unordered_set`** vs **Python `set`**

#### Common Operations Complexity

| Operation | C++ unordered_set | Python set |
|-----------|-------------------|------------|
| Insert | O(1) average, O(n) worst | O(1) average |
| Delete | O(1) average, O(n) worst | O(1) average |
| Search | O(1) average, O(n) worst | O(1) average |
| Iteration | O(n) | O(n) |

##### C++ - std::unordered_set
```cpp
#include <unordered_set>

std::unordered_set<int> us = {3, 1, 4, 1, 5};
// us: {1, 3, 4, 5} (unordered)

// Insert - O(1) average
us.insert(2);

// Find - O(1) average
auto it = us.find(3);
if (it != us.end()) {
    // Found
}

// Contains (C++20) - O(1) average
bool exists = us.contains(3);

// Erase - O(1) average
us.erase(3);

// Size - O(1)
size_t sz = us.size();

// Iterate (unordered)
for (int x : us) {
    // Process x
}
```

##### Python - set
```python
us = {3, 1, 4, 1, 5}
# us: {1, 3, 4, 5} (unordered)

# Insert - O(1) average
us.add(2)

# Search - O(1) average
if 3 in us:
    # Found
    pass

# Remove - O(1) average
us.discard(3)  # No error if not found
us.remove(3)   # Raises KeyError if not found

# Size - O(1)
sz = len(us)

# Iterate (unordered)
for x in us:
    # Process x
    pass

# Set operations - O(len(s1) + len(s2))
s1 = {1, 2, 3}
s2 = {3, 4, 5}
union = s1 | s2          # {1, 2, 3, 4, 5}
intersection = s1 & s2   # {3}
difference = s1 - s2     # {1, 2}
```

---

### Map (Ordered Map)

**C++ `std::map`** vs **Python `dict`** (Python 3.7+ maintains insertion order)

#### Common Operations Complexity

| Operation | C++ map | Python dict |
|-----------|---------|-------------|
| Insert | O(log n) | O(1) average |
| Delete | O(log n) | O(1) average |
| Search | O(log n) | O(1) average |
| Iteration (sorted) | O(n) | O(n) |

##### C++ - std::map (ordered by key)
```cpp
#include <map>

std::map<std::string, int> m;

// Insert - O(log n)
m["apple"] = 5;
m["banana"] = 3;
m.insert({"cherry", 7});

// Access - O(log n)
int val = m["apple"];  // Creates entry if not exists
int val2 = m.at("banana");  // Throws exception if not exists

// Find - O(log n)
auto it = m.find("apple");
if (it != m.end()) {
    std::string key = it->first;
    int value = it->second;
}

// Contains (C++20) - O(log n)
bool exists = m.contains("apple");

// Erase - O(log n)
m.erase("apple");

// Size - O(1)
size_t sz = m.size();

// Iterate (sorted by key)
for (const auto& [key, value] : m) {
    // Process key, value
}

// Lower/upper bound - O(log n)
auto lower = m.lower_bound("banana");
auto upper = m.upper_bound("banana");
```

##### Python - dict (insertion order maintained since 3.7)
```python
m = {}

# Insert - O(1) average
m["apple"] = 5
m["banana"] = 3
m["cherry"] = 7

# Access - O(1) average
val = m["apple"]  # Raises KeyError if not exists
val2 = m.get("banana")  # Returns None if not exists
val3 = m.get("orange", 0)  # Returns default if not exists

# Search - O(1) average
if "apple" in m:
    # Found
    pass

# Remove - O(1) average
del m["apple"]
m.pop("banana", None)  # No error if not exists

# Size - O(1)
sz = len(m)

# Iterate (insertion order)
for key, value in m.items():
    # Process key, value
    pass

# Get keys/values
keys = list(m.keys())
values = list(m.values())

# For sorted iteration - O(n log n)
for key in sorted(m.keys()):
    value = m[key]
    # Process key, value
```

---

### Unordered Map (Hash Map)

**C++ `std::unordered_map`** vs **Python `dict`**

#### Common Operations Complexity

| Operation | C++ unordered_map | Python dict |
|-----------|-------------------|-------------|
| Insert | O(1) average, O(n) worst | O(1) average |
| Delete | O(1) average, O(n) worst | O(1) average |
| Search | O(1) average, O(n) worst | O(1) average |
| Iteration | O(n) | O(n) |

##### C++ - std::unordered_map
```cpp
#include <unordered_map>

std::unordered_map<std::string, int> um;

// Insert - O(1) average
um["apple"] = 5;
um["banana"] = 3;
um.insert({"cherry", 7});

// Access - O(1) average
int val = um["apple"];
int val2 = um.at("banana");

// Find - O(1) average
auto it = um.find("apple");
if (it != um.end()) {
    std::string key = it->first;
    int value = it->second;
}

// Contains (C++20) - O(1) average
bool exists = um.contains("apple");

// Erase - O(1) average
um.erase("apple");

// Size - O(1)
size_t sz = um.size();

// Iterate (unordered)
for (const auto& [key, value] : um) {
    // Process key, value
}
```

##### Python - dict
```python
# Python dict is essentially an unordered_map
um = {}

# Insert - O(1) average
um["apple"] = 5
um["banana"] = 3
um["cherry"] = 7

# Access - O(1) average
val = um["apple"]
val2 = um.get("banana")

# Search - O(1) average
if "apple" in um:
    # Found
    pass

# Remove - O(1) average
del um["apple"]

# Size - O(1)
sz = len(um)

# Iterate (insertion order since Python 3.7)
for key, value in um.items():
    # Process key, value
    pass
```

---

### Multiset (Ordered Multiset)

**C++ `std::multiset`** vs **Python `collections.Counter`** or manual approach

#### Common Operations Complexity

| Operation | C++ multiset | Python Counter |
|-----------|--------------|----------------|
| Insert | O(log n) | O(1) average |
| Delete | O(log n) | O(1) average |
| Count | O(log n) + O(count) | O(1) |
| Iteration | O(n) | O(n) |

##### C++ - std::multiset (allows duplicates, sorted)
```cpp
#include <set>

std::multiset<int> ms = {1, 2, 2, 3, 3, 3};

// Insert - O(log n)
ms.insert(2);  // ms: {1, 2, 2, 2, 3, 3, 3}

// Count occurrences - O(log n + count)
int count = ms.count(2);  // 3

// Find - O(log n)
auto it = ms.find(2);

// Erase all occurrences - O(log n + count)
ms.erase(2);  // Removes all 2s

// Erase single occurrence - O(log n)
auto it2 = ms.find(3);
if (it2 != ms.end()) {
    ms.erase(it2);  // Remove only one 3
}

// Equal range - O(log n)
auto range = ms.equal_range(3);
int count3 = std::distance(range.first, range.second);
```

##### Python - Counter
```python
from collections import Counter

ms = Counter([1, 2, 2, 3, 3, 3])
# ms: Counter({3: 3, 2: 2, 1: 1})

# Insert - O(1) average
ms[2] += 1  # ms: Counter({3: 3, 2: 3, 1: 1})

# Count occurrences - O(1)
count = ms[2]  # 3

# Remove all occurrences - O(1)
del ms[2]

# Remove single occurrence - O(1)
ms[3] -= 1
if ms[3] == 0:
    del ms[3]

# Get elements (with repetitions)
elements = list(ms.elements())  # [1, 3, 3, 3]

# Most common - O(n log k) where k is the number requested
most_common = ms.most_common(2)  # [(3, 3), (2, 2)]
```

---

### Priority Queue (Heap)

**C++ `std::priority_queue`** vs **Python `heapq`**

#### Common Operations Complexity

| Operation | C++ priority_queue | Python heapq |
|-----------|-------------------|--------------|
| Insert | O(log n) | O(log n) |
| Get max/min | O(1) | O(1) |
| Delete max/min | O(log n) | O(log n) |
| Build heap | O(n) | O(n) |

##### C++ - std::priority_queue (max heap by default)
```cpp
#include <queue>
#include <vector>

// Max heap (default)
std::priority_queue<int> max_heap;

// Push - O(log n)
max_heap.push(3);
max_heap.push(1);
max_heap.push(4);

// Top - O(1)
int max_val = max_heap.top();  // 4

// Pop - O(log n)
max_heap.pop();  // Removes 4

// Size - O(1)
size_t sz = max_heap.size();

// Min heap
std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;
min_heap.push(3);
min_heap.push(1);
min_heap.push(4);
int min_val = min_heap.top();  // 1
```

##### Python - heapq (min heap)
```python
import heapq

# Min heap (default)
min_heap = []

# Push - O(log n)
heapq.heappush(min_heap, 3)
heapq.heappush(min_heap, 1)
heapq.heappush(min_heap, 4)

# Top - O(1)
min_val = min_heap[0]  # 1

# Pop - O(log n)
min_val = heapq.heappop(min_heap)  # 1

# Size - O(1)
sz = len(min_heap)

# Build heap from list - O(n)
nums = [3, 1, 4, 1, 5]
heapq.heapify(nums)  # Converts to min heap in-place

# Max heap (negate values)
max_heap = []
heapq.heappush(max_heap, -3)
heapq.heappush(max_heap, -1)
heapq.heappush(max_heap, -4)
max_val = -max_heap[0]  # 4
max_val = -heapq.heappop(max_heap)  # 4

# Or use tuple with negated priority
max_heap2 = []
heapq.heappush(max_heap2, (-3, "item1"))
heapq.heappush(max_heap2, (-1, "item2"))
```

---

### Deque (Double-Ended Queue)

**C++ `std::deque`** vs **Python `collections.deque`**

#### Common Operations Complexity

| Operation | C++ deque | Python deque |
|-----------|-----------|--------------|
| Access by index | O(1) | O(n) |
| Push front/back | O(1) | O(1) |
| Pop front/back | O(1) | O(1) |
| Insert middle | O(n) | O(n) |

##### C++ - std::deque
```cpp
#include <deque>

std::deque<int> dq = {1, 2, 3};

// Push - O(1)
dq.push_back(4);   // {1, 2, 3, 4}
dq.push_front(0);  // {0, 1, 2, 3, 4}

// Pop - O(1)
dq.pop_back();     // {0, 1, 2, 3}
dq.pop_front();    // {1, 2, 3}

// Access - O(1)
int first = dq.front();
int last = dq.back();
int mid = dq[1];

// Size - O(1)
size_t sz = dq.size();
```

##### Python - collections.deque
```python
from collections import deque

dq = deque([1, 2, 3])

# Push - O(1)
dq.append(4)       # deque([1, 2, 3, 4])
dq.appendleft(0)   # deque([0, 1, 2, 3, 4])

# Pop - O(1)
dq.pop()           # deque([0, 1, 2, 3])
dq.popleft()       # deque([1, 2, 3])

# Access - O(n) for middle elements!
first = dq[0]      # O(1)
last = dq[-1]      # O(1)
mid = dq[1]        # O(n)

# Size - O(1)
sz = len(dq)

# Rotate - O(k)
dq.rotate(1)       # deque([3, 1, 2])
dq.rotate(-1)      # deque([1, 2, 3])

# Max length (bounded deque)
bounded_dq = deque(maxlen=3)
bounded_dq.extend([1, 2, 3, 4])  # deque([2, 3, 4])
```

---

### Stack

**C++ `std::stack`** vs **Python `list`**

#### Common Operations Complexity

| Operation | C++ stack | Python list |
|-----------|-----------|-------------|
| Push | O(1) | O(1) amortized |
| Pop | O(1) | O(1) |
| Top/Peek | O(1) | O(1) |

##### C++ - std::stack
```cpp
#include <stack>

std::stack<int> st;

// Push - O(1)
st.push(1);
st.push(2);
st.push(3);

// Top - O(1)
int top = st.top();  // 3

// Pop - O(1)
st.pop();  // Removes 3

// Size - O(1)
size_t sz = st.size();

// Empty check - O(1)
bool empty = st.empty();
```

##### Python - list (used as stack)
```python
st = []

# Push - O(1) amortized
st.append(1)
st.append(2)
st.append(3)

# Top - O(1)
top = st[-1]  # 3

# Pop - O(1)
top = st.pop()  # 3

# Size - O(1)
sz = len(st)

# Empty check - O(1)
empty = len(st) == 0
```

---

### Queue

**C++ `std::queue`** vs **Python `collections.deque`**

#### Common Operations Complexity

| Operation | C++ queue | Python deque |
|-----------|-----------|--------------|
| Enqueue | O(1) | O(1) |
| Dequeue | O(1) | O(1) |
| Front | O(1) | O(1) |

##### C++ - std::queue
```cpp
#include <queue>

std::queue<int> q;

// Enqueue - O(1)
q.push(1);
q.push(2);
q.push(3);

// Front - O(1)
int front = q.front();  // 1

// Dequeue - O(1)
q.pop();  // Removes 1

// Size - O(1)
size_t sz = q.size();

// Empty check - O(1)
bool empty = q.empty();
```

##### Python - collections.deque (used as queue)
```python
from collections import deque

q = deque()

# Enqueue - O(1)
q.append(1)
q.append(2)
q.append(3)

# Front - O(1)
front = q[0]  # 1

# Dequeue - O(1)
front = q.popleft()  # 1

# Size - O(1)
sz = len(q)

# Empty check - O(1)
empty = len(q) == 0
```

---

## Math Functions

### Basic Math Operations

#### C++
```cpp
#include <cmath>
#include <iostream>

int main() {
    // Absolute value
    std::cout << std::abs(-5) << std::endl;        // 5
    std::cout << std::fabs(-5.5) << std::endl;     // 5.5
    
    // Power and roots
    std::cout << std::pow(2, 3) << std::endl;      // 8
    std::cout << std::sqrt(16) << std::endl;       // 4
    std::cout << std::cbrt(27) << std::endl;       // 3
    
    // Rounding
    std::cout << std::ceil(4.3) << std::endl;      // 5
    std::cout << std::floor(4.7) << std::endl;     // 4
    
    // Trigonometry
    std::cout << std::sin(M_PI / 2) << std::endl;  // 1
    std::cout << std::cos(0) << std::endl;         // 1
    std::cout << std::tan(M_PI / 4) << std::endl;  // 1
    
    // Inverse trig
    std::cout << std::asin(1) << std::endl;        // π/2
    std::cout << std::acos(0) << std::endl;        // π/2
    std::cout << std::atan(1) << std::endl;        // π/4
    
    // Hyperbolic
    std::cout << std::sinh(0) << std::endl;        // 0
    std::cout << std::cosh(0) << std::endl;        // 1
    std::cout << std::tanh(0) << std::endl;        // 0
    
    // Exponential and logarithm
    std::cout << std::exp(1) << std::endl;         // e
    std::cout << std::expm1(0) << std::endl;       // 0 (e^0 - 1)
    std::cout << std::log(M_E) << std::endl;       // 1
    
    // Min/Max
    std::cout << std::fmax(3.5, 2.1) << std::endl; // 3.5
    std::cout << std::fmin(3.5, 2.1) << std::endl; // 2.1
    
    // Modulo
    std::cout << std::fmod(5.5, 2.0) << std::endl; // 1.5
    
    // Hypotenuse
    std::cout << std::hypot(3, 4) << std::endl;    // 5
    
    // Fused multiply-add
    std::cout << std::fma(2, 3, 4) << std::endl;   // 10 (2*3+4)
    
    // Positive difference
    std::cout << std::fdim(5, 3) << std::endl;     // 2
    std::cout << std::fdim(3, 5) << std::endl;     // 0
    
    return 0;
}
```

#### Python
```python
import math

# Absolute value
print(abs(-5))              # 5
print(math.fabs(-5.5))      # 5.5

# Power and roots
print(pow(2, 3))            # 8 or 2 ** 3
print(math.sqrt(16))        # 4.0
print(math.pow(27, 1/3))    # 3.0 (cube root)

# Rounding
print(math.ceil(4.3))       # 5
print(math.floor(4.7))      # 4

# Trigonometry
print(math.sin(math.pi / 2))  # 1.0
print(math.cos(0))            # 1.0
print(math.tan(math.pi / 4))  # 1.0

# Inverse trig
print(math.asin(1))         # π/2
print(math.acos(0))         # π/2
print(math.atan(1))         # π/4

# Hyperbolic
print(math.sinh(0))         # 0.0
print(math.cosh(0))         # 1.0
print(math.tanh(0))         # 0.0

# Exponential and logarithm
print(math.exp(1))          # e
print(math.expm1(0))        # 0.0 (e^0 - 1)
print(math.log(math.e))     # 1.0

# Min/Max
print(max(3.5, 2.1))        # 3.5
print(min(3.5, 2.1))        # 2.1

# Modulo
print(math.fmod(5.5, 2.0))  # 1.5
print(5.5 % 2.0)            # 1.5

# Hypotenuse
print(math.hypot(3, 4))     # 5.0

# Fused multiply-add (Python 3.13+)
print(math.fma(2, 3, 4) if hasattr(math, 'fma') else 2*3+4)  # 10

# Positive difference (not built-in, manual)
def fdim(x, y):
    return max(x - y, 0)
print(fdim(5, 3))           # 2
print(fdim(3, 5))           # 0
```

**Key Differences:**
- C++ requires `#include <cmath>`, Python requires `import math`
- Python has `**` operator for power, C++ uses `std::pow()`
- Python's `%` works for floats, C++ has `std::fmod()`
- Many functions have similar names in both languages

---

## String Functions

### String Comparison and Length

#### C++
```cpp
#include <string>
#include <iostream>

std::string str1 = "hello";
std::string str2 = "world";

// Compare
int cmp = str1.compare(str2);  // < 0 (str1 < str2)

// Length
int len = str1.length();       // 5
int sz = str1.size();          // 5 (same as length)

// Check if empty
bool empty = str1.empty();     // false
```

#### Python
```python
str1 = "hello"
str2 = "world"

# Compare
cmp = (str1 > str2) - (str1 < str2)  # -1 (str1 < str2)
# Or simply use comparison operators
if str1 < str2:
    print("str1 is less than str2")

# Length
length = len(str1)             # 5

# Check if empty
empty = not str1               # False
# Or
empty = len(str1) == 0
```

---

### String Modification

#### C++
```cpp
#include <string>

std::string str = "hello";

// Append
str.append(" world");          // "hello world"
str += "!";                    // "hello world!"

// Insert
str.insert(5, ",");            // "hello, world!"

// Replace
str.replace(0, 5, "Hi");       // "Hi, world!"

// Erase
str.erase(2, 2);               // "Hi world!"

// Resize
str.resize(8);                 // "Hi world" (padded with null chars)
str.resize(2);                 // "Hi"

// Clear
str.clear();                   // ""

// Swap
std::string str2 = "test";
str.swap(str2);                // str = "test", str2 = ""
```

#### Python
```python
str1 = "hello"

# Append (strings are immutable, create new string)
str1 = str1 + " world"         # "hello world"
str1 += "!"                    # "hello world!"

# Insert (no direct method, use slicing)
str1 = str1[:5] + "," + str1[5:]  # "hello, world!"

# Replace
str1 = str1.replace("hello", "Hi")  # "Hi, world!"

# Remove substring (replace with empty)
str1 = str1.replace(", ", " ")      # "Hi world!"

# Truncate (slicing)
str1 = str1[:8]                     # "Hi world"
str1 = str1[:2]                     # "Hi"

# Clear
str1 = ""                           # ""

# Swap
str2 = "test"
str1, str2 = str2, str1             # str1 = "test", str2 = ""
```

---

### String Access and Substring

#### C++
```cpp
#include <string>

std::string str = "hello world";

// Access character
char ch = str[0];              // 'h'
char ch2 = str.at(1);          // 'e' (with bounds checking)

// Front and back
char first = str.front();      // 'h'
char last = str.back();        // 'd'

// Substring
std::string sub = str.substr(0, 5);  // "hello"
std::string sub2 = str.substr(6);    // "world"

// C-string conversion
const char* cstr = str.c_str();

// Get data pointer
const char* data = str.data();
```

#### Python
```python
str1 = "hello world"

# Access character
ch = str1[0]                   # 'h'
ch2 = str1[1]                  # 'e'

# First and last
first = str1[0]                # 'h'
last = str1[-1]                # 'd'

# Substring (slicing)
sub = str1[0:5]                # "hello"
sub2 = str1[6:]                # "world"

# Convert to bytes
bytes_str = str1.encode()      # b'hello world'

# Strings are already sequences of characters
data = str1                    # No special conversion needed
```

---

### String Search

#### C++
```cpp
#include <string>

std::string str = "hello world, hello universe";

// Find first occurrence
size_t pos = str.find("hello");           // 0
size_t pos2 = str.find("hello", 1);       // 13 (start from index 1)

// Find last occurrence
size_t last = str.rfind("hello");         // 13

// Find first of any character
size_t first_of = str.find_first_of("aeiou");  // 1 ('e')

// Find first NOT of
size_t first_not = str.find_first_not_of("helo ");  // 6 ('w')

// Find last of
size_t last_of = str.find_last_of("aeiou");   // 31 ('e')

// Find last NOT of
size_t last_not = str.find_last_not_of("e ");  // 30 ('s')

// Check if found
if (pos != std::string::npos) {
    // Found
}
```

#### Python
```python
str1 = "hello world, hello universe"

# Find first occurrence
pos = str1.find("hello")              # 0
pos2 = str1.find("hello", 1)          # 13 (start from index 1)

# Find last occurrence
last = str1.rfind("hello")            # 13

# Find first of any character (manual)
first_of = next((i for i, ch in enumerate(str1) 
                 if ch in "aeiou"), -1)  # 1

# Find first NOT of (manual)
first_not = next((i for i, ch in enumerate(str1) 
                  if ch not in "helo "), -1)  # 6

# Find last of (manual)
last_of = next((i for i in range(len(str1) - 1, -1, -1) 
                if str1[i] in "aeiou"), -1)  # 31

# Check if found
if pos != -1:
    # Found
    pass

# Or use 'in' operator
if "hello" in str1:
    # Found
    pass
```

---

### String Manipulation

#### C++
```cpp
#include <string>

std::string str = "hello";

// Push back (add character at end)
str.push_back('!');            // "hello!"

// Pop back (remove last character)
str.pop_back();                // "hello"

// Assign new value
str.assign("world");           // "world"

// Copy to char array
char buffer[10];
str.copy(buffer, 5, 0);        // Copy 5 chars starting at 0

// Shrink to fit
str.shrink_to_fit();           // Reduce capacity to fit size

// Reserve capacity
str.reserve(100);              // Reserve space for 100 chars

// Get max size
size_t max = str.max_size();   // Maximum possible size
```

#### Python
```python
str1 = "hello"

# Add character at end (strings are immutable)
str1 = str1 + '!'              # "hello!"

# Remove last character
str1 = str1[:-1]               # "hello"

# Assign new value
str1 = "world"                 # "world"

# Convert to list for manipulation
chars = list(str1)
# Modify chars...
str1 = ''.join(chars)

# Python strings are already optimized
# No need for shrink_to_fit or reserve

# Get max size (system dependent)
import sys
max_size = sys.maxsize         # Maximum integer size
```

---

## Functors

### Arithmetic Functors

#### C++
```cpp
#include <functional>
#include <iostream>

int main() {
    std::plus<int> add;
    std::minus<int> subtract;
    std::multiplies<int> multiply;
    std::divides<int> divide;
    std::modulus<int> mod;
    std::negate<int> neg;
    
    std::cout << add(5, 3) << std::endl;        // 8
    std::cout << subtract(5, 3) << std::endl;   // 2
    std::cout << multiply(5, 3) << std::endl;   // 15
    std::cout << divide(6, 3) << std::endl;     // 2
    std::cout << mod(5, 3) << std::endl;        // 2
    std::cout << neg(5) << std::endl;           // -5
    
    return 0;
}
```

#### Python
```python
import operator

# Arithmetic operations
print(operator.add(5, 3))       # 8
print(operator.sub(5, 3))       # 2
print(operator.mul(5, 3))       # 15
print(operator.truediv(6, 3))   # 2.0
print(operator.floordiv(6, 3))  # 2
print(operator.mod(5, 3))       # 2
print(operator.neg(5))          # -5

# Or use lambda functions
add = lambda a, b: a + b
subtract = lambda a, b: a - b
multiply = lambda a, b: a * b
```

---

### Relational Functors

#### C++
```cpp
#include <functional>
#include <iostream>

int main() {
    std::equal_to<int> eq;
    std::not_equal_to<int> neq;
    std::greater<int> gt;
    std::greater_equal<int> gte;
    std::less<int> lt;
    std::less_equal<int> lte;
    
    std::cout << eq(5, 5) << std::endl;      // 1 (true)
    std::cout << neq(5, 3) << std::endl;     // 1 (true)
    std::cout << gt(5, 3) << std::endl;      // 1 (true)
    std::cout << gte(5, 5) << std::endl;     // 1 (true)
    std::cout << lt(3, 5) << std::endl;      // 1 (true)
    std::cout << lte(5, 5) << std::endl;     // 1 (true)
    
    return 0;
}
```

#### Python
```python
import operator

# Relational operations
print(operator.eq(5, 5))        # True
print(operator.ne(5, 3))        # True
print(operator.gt(5, 3))        # True
print(operator.ge(5, 5))        # True
print(operator.lt(3, 5))        # True
print(operator.le(5, 5))        # True

# Or use comparison operators directly
print(5 == 5)                   # True
print(5 != 3)                   # True
print(5 > 3)                    # True
```

---

### Logical Functors

#### C++
```cpp
#include <functional>
#include <iostream>

int main() {
    std::logical_and<bool> and_op;
    std::logical_or<bool> or_op;
    std::logical_not<bool> not_op;
    
    std::cout << and_op(true, false) << std::endl;   // 0 (false)
    std::cout << or_op(true, false) << std::endl;    // 1 (true)
    std::cout << not_op(true) << std::endl;          // 0 (false)
    
    return 0;
}
```

#### Python
```python
import operator

# Logical operations
print(operator.and_(True, False))   # False
print(operator.or_(True, False))    # True
print(operator.not_(True))          # False

# Or use logical operators directly
print(True and False)               # False
print(True or False)                # True
print(not True)                     # False
```

---

### Bitwise Functors

#### C++
```cpp
#include <functional>
#include <iostream>

int main() {
    std::bit_and<int> and_op;
    std::bit_or<int> or_op;
    std::bit_xor<int> xor_op;
    
    std::cout << and_op(5, 3) << std::endl;   // 1 (0101 & 0011 = 0001)
    std::cout << or_op(5, 3) << std::endl;    // 7 (0101 | 0011 = 0111)
    std::cout << xor_op(5, 3) << std::endl;   // 6 (0101 ^ 0011 = 0110)
    
    return 0;
}
```

#### Python
```python
import operator

# Bitwise operations
print(operator.and_(5, 3))      # 1 (0101 & 0011 = 0001)
print(operator.or_(5, 3))       # 7 (0101 | 0011 = 0111)
print(operator.xor(5, 3))       # 6 (0101 ^ 0011 = 0110)

# Or use bitwise operators directly
print(5 & 3)                    # 1
print(5 | 3)                    # 7
print(5 ^ 3)                    # 6
```

---

## List Operations

### C++ std::list vs Python list

> **Note:** C++ `std::list` is a doubly-linked list, while Python's `list` is a dynamic array. For a closer comparison to C++ `std::list`, Python's `collections.deque` is more appropriate.

#### C++ - std::list

```cpp
#include <list>
#include <iostream>

int main() {
    std::list<int> lst = {1, 2, 3, 4, 5};
    
    // Front and back
    int first = lst.front();         // 1
    int last = lst.back();           // 5
    
    // Push back
    lst.push_back(6);                // {1, 2, 3, 4, 5, 6}
    
    // Push front
    lst.push_front(0);               // {0, 1, 2, 3, 4, 5, 6}
    
    // Pop back
    lst.pop_back();                  // {0, 1, 2, 3, 4, 5}
    
    // Pop front
    lst.pop_front();                 // {1, 2, 3, 4, 5}
    
    // Erase (by iterator)
    auto it = lst.begin();
    std::advance(it, 2);             // Move to 3rd element
    lst.erase(it);                   // {1, 2, 4, 5}
    
    // Erase range
    auto start = lst.begin();
    auto end = lst.begin();
    std::advance(end, 2);
    lst.erase(start, end);           // {4, 5}
    
    // Insert
    auto pos = lst.begin();
    lst.insert(pos, 3);              // {3, 4, 5}
    
    // Size
    size_t sz = lst.size();          // 3
    
    // Clear
    lst.clear();                     // {}
    
    return 0;
}
```

#### Python - list

```python
# Python list (dynamic array)
lst = [1, 2, 3, 4, 5]

# Front and back
first = lst[0]                   # 1
last = lst[-1]                   # 5

# Append (push back)
lst.append(6)                    # [1, 2, 3, 4, 5, 6]

# Insert at front (push front) - O(n) operation!
lst.insert(0, 0)                 # [0, 1, 2, 3, 4, 5, 6]

# Pop back
lst.pop()                        # [0, 1, 2, 3, 4, 5]

# Pop front - O(n) operation!
lst.pop(0)                       # [1, 2, 3, 4, 5]

# Remove by index
del lst[2]                       # [1, 2, 4, 5]

# Remove range
del lst[0:2]                     # [4, 5]

# Insert at position
lst.insert(0, 3)                 # [3, 4, 5]

# Length
length = len(lst)                # 3

# Clear
lst.clear()                      # []
```

#### Python - collections.deque (closer to C++ list)

```python
from collections import deque

# deque (doubly-linked list)
dq = deque([1, 2, 3, 4, 5])

# Front and back
first = dq[0]                    # 1
last = dq[-1]                    # 5

# Append (push back) - O(1)
dq.append(6)                     # deque([1, 2, 3, 4, 5, 6])

# Append left (push front) - O(1)
dq.appendleft(0)                 # deque([0, 1, 2, 3, 4, 5, 6])

# Pop (pop back) - O(1)
dq.pop()                         # deque([0, 1, 2, 3, 4, 5])

# Pop left (pop front) - O(1)
dq.popleft()                     # deque([1, 2, 3, 4, 5])

# Remove by value
dq.remove(3)                     # deque([1, 2, 4, 5])

# Insert at position - O(n)
dq.insert(0, 3)                  # deque([3, 1, 2, 4, 5])

# Length
length = len(dq)                 # 5

# Clear
dq.clear()                       # deque([])
```

**Key Differences:**
- C++ `std::list` is a doubly-linked list (O(1) insert/delete anywhere with iterator)
- Python `list` is a dynamic array (O(1) append/pop at end, O(n) at beginning)
- Python `deque` is closest to C++ `std::list` for front/back operations
- C++ uses iterators, Python uses indices
- C++ `erase()` requires iterators, Python uses `del` or `remove()`

---

## Summary Table

| Operation | C++ | Python |
|-----------|-----|--------|
| **Sort** | `std::sort(begin, end)` | `list.sort()` or `sorted(list)` |
| **Map** | `std::transform()` | `map()` or list comprehension |
| **Filter** | `std::copy_if()` | `filter()` or list comprehension |
| **Reduce** | `std::accumulate()` | `functools.reduce()` or `sum()` |
| **Find** | `std::find()` | `list.index()` or `in` operator |
| **Count** | `std::count()` | `list.count()` |
| **All/Any** | `std::all_of()/any_of()` | `all()`/`any()` |
| **Math** | `<cmath>` functions | `math` module |
| **Strings** | `std::string` methods | `str` methods |
| **List ops** | `std::list` methods | `list` or `deque` methods |

---

## Performance Considerations

### C++
- **Compiled language**: Generally faster execution
- **Manual memory management**: More control but more responsibility
- **Iterator-based**: Efficient for algorithms
- **Templates**: Zero-cost abstractions

### Python
- **Interpreted language**: Slower execution but faster development
- **Automatic memory management**: Easier to use
- **Index-based**: More intuitive for beginners
- **Dynamic typing**: More flexible but less type-safe

---

## Best Practices

### When to use C++:
- Performance-critical applications
- System programming
- Game development
- Embedded systems
- When you need fine-grained control over memory

### When to use Python:
- Rapid prototyping
- Data analysis and machine learning
- Web development
- Scripting and automation
- When development speed is more important than execution speed

---

---

## Language-Specific Features

This section covers features that are unique or particularly idiomatic to each language.

---

### Python-Specific Features

#### List Comprehensions

**Description:** Concise syntax for creating lists based on existing iterables  
**Time Complexity:** O(n) where n is the size of the input iterable

```python
# Basic list comprehension
squares = [x**2 for x in range(10)]
# [0, 1, 4, 9, 16, 25, 36, 49, 64, 81]

# With condition
evens = [x for x in range(20) if x % 2 == 0]
# [0, 2, 4, 6, 8, 10, 12, 14, 16, 18]

# Nested comprehension
matrix = [[i*j for j in range(3)] for i in range(3)]
# [[0, 0, 0], [0, 1, 2], [0, 2, 4]]

# Multiple conditions
filtered = [x for x in range(100) if x % 2 == 0 if x % 5 == 0]
# [0, 10, 20, 30, 40, 50, 60, 70, 80, 90]
```

**C++ Equivalent:** No direct equivalent, use loops or algorithms
```cpp
// C++ equivalent using loop
std::vector<int> squares;
for (int x = 0; x < 10; ++x) {
    squares.push_back(x * x);
}

// Or using std::transform
std::vector<int> range(10);
std::iota(range.begin(), range.end(), 0);
std::vector<int> squares(10);
std::transform(range.begin(), range.end(), squares.begin(),
               [](int x) { return x * x; });

// With condition using copy_if
std::vector<int> evens;
for (int x = 0; x < 20; ++x) {
    if (x % 2 == 0) evens.push_back(x);
}
```

---

#### Dictionary Comprehensions

**Description:** Create dictionaries using comprehension syntax  
**Time Complexity:** O(n)

```python
# Basic dict comprehension
squares_dict = {x: x**2 for x in range(5)}
# {0: 0, 1: 1, 2: 4, 3: 9, 4: 16}

# From two lists
keys = ['a', 'b', 'c']
values = [1, 2, 3]
d = {k: v for k, v in zip(keys, values)}
# {'a': 1, 'b': 2, 'c': 3}

# With condition
even_squares = {x: x**2 for x in range(10) if x % 2 == 0}
# {0: 0, 2: 4, 4: 16, 6: 36, 8: 64}

# Swap keys and values
original = {'a': 1, 'b': 2, 'c': 3}
swapped = {v: k for k, v in original.items()}
# {1: 'a', 2: 'b', 3: 'c'}
```

**C++ Equivalent:**
```cpp
// C++ equivalent using loop
std::map<int, int> squares_dict;
for (int x = 0; x < 5; ++x) {
    squares_dict[x] = x * x;
}

// With condition
std::map<int, int> even_squares;
for (int x = 0; x < 10; ++x) {
    if (x % 2 == 0) {
        even_squares[x] = x * x;
    }
}
```

---

#### Set Comprehensions

**Description:** Create sets using comprehension syntax  
**Time Complexity:** O(n)

```python
# Basic set comprehension
unique_lengths = {len(word) for word in ['hello', 'world', 'hi', 'bye']}
# {2, 5}

# With condition
even_set = {x for x in range(20) if x % 2 == 0}
# {0, 2, 4, 6, 8, 10, 12, 14, 16, 18}
```

---

#### Generator Expressions

**Description:** Memory-efficient iterators that generate values on-the-fly  
**Space Complexity:** O(1) - doesn't store all values in memory

```python
# Generator expression (lazy evaluation)
gen = (x**2 for x in range(1000000))  # Doesn't compute all values
first_five = [next(gen) for _ in range(5)]
# [0, 1, 4, 9, 16]

# Use in functions that accept iterables
sum_of_squares = sum(x**2 for x in range(100))
# 328350

# Generator function
def fibonacci():
    a, b = 0, 1
    while True:
        yield a
        a, b = b, a + b

fib = fibonacci()
first_ten = [next(fib) for _ in range(10)]
# [0, 1, 1, 2, 3, 5, 8, 13, 21, 34]
```

**C++ Equivalent:** C++20 ranges/views provide similar lazy evaluation
```cpp
// C++20 ranges (lazy evaluation)
#include <ranges>
#include <vector>

auto squares = std::views::iota(0, 1000000) 
             | std::views::transform([](int x) { return x * x; });

// Take first 5
std::vector<int> first_five;
for (int val : squares | std::views::take(5)) {
    first_five.push_back(val);
}
```

---

#### Decorators

**Description:** Functions that modify or enhance other functions  
**Use Cases:** Logging, timing, authentication, caching

```python
# Simple decorator
def timing_decorator(func):
    import time
    def wrapper(*args, **kwargs):
        start = time.time()
        result = func(*args, **kwargs)
        end = time.time()
        print(f"{func.__name__} took {end - start:.4f} seconds")
        return result
    return wrapper

@timing_decorator
def slow_function():
    import time
    time.sleep(1)
    return "Done"

# Using decorator
slow_function()  # Prints timing information

# Decorator with arguments
def repeat(times):
    def decorator(func):
        def wrapper(*args, **kwargs):
            for _ in range(times):
                result = func(*args, **kwargs)
            return result
        return wrapper
    return decorator

@repeat(3)
def greet(name):
    print(f"Hello, {name}!")

greet("Alice")  # Prints 3 times

# Built-in decorators
class MyClass:
    @staticmethod
    def static_method():
        return "Static"
    
    @classmethod
    def class_method(cls):
        return f"Class: {cls.__name__}"
    
    @property
    def my_property(self):
        return "Property value"
```

**C++ Equivalent:** No direct equivalent, can use function wrappers or templates
```cpp
// C++ equivalent using lambda wrapper
#include <chrono>
#include <iostream>

template<typename Func>
auto timing_wrapper(Func func) {
    return [func](auto&&... args) {
        auto start = std::chrono::high_resolution_clock::now();
        auto result = func(std::forward<decltype(args)>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << "Function took " << diff.count() << " seconds\n";
        return result;
    };
}

// Usage
auto timed_function = timing_wrapper([](int x) {
    // Some work
    return x * 2;
});
```

---

#### Multiple Assignment and Unpacking

**Description:** Assign multiple variables in one line

```python
# Multiple assignment
a, b, c = 1, 2, 3

# Swap without temporary variable
a, b = b, a

# Unpacking
first, *middle, last = [1, 2, 3, 4, 5]
# first = 1, middle = [2, 3, 4], last = 5

# Unpacking in loops
pairs = [(1, 'a'), (2, 'b'), (3, 'c')]
for num, letter in pairs:
    print(f"{num}: {letter}")

# Dictionary unpacking
point = {'x': 10, 'y': 20}
x, y = point.values()

# Extended unpacking
a, *b, c, d = [1, 2, 3, 4, 5, 6]
# a = 1, b = [2, 3, 4], c = 5, d = 6
```

**C++ Equivalent:** C++17 structured bindings (limited)
```cpp
// C++17 structured bindings
std::pair<int, std::string> get_pair() {
    return {1, "hello"};
}

auto [num, str] = get_pair();

// Tuple unpacking
std::tuple<int, double, std::string> t = {1, 2.5, "test"};
auto [a, b, c] = t;

// No equivalent for *args unpacking in C++
```

---

#### Context Managers (with statement)

**Description:** Automatic resource management

```python
# File handling
with open('file.txt', 'r') as f:
    content = f.read()
# File automatically closed

# Multiple context managers
with open('input.txt', 'r') as infile, open('output.txt', 'w') as outfile:
    outfile.write(infile.read())

# Custom context manager
class Timer:
    def __enter__(self):
        import time
        self.start = time.time()
        return self
    
    def __exit__(self, *args):
        import time
        self.end = time.time()
        print(f"Elapsed: {self.end - self.start:.4f}s")

with Timer():
    # Some code
    sum(range(1000000))

# Using contextlib
from contextlib import contextmanager

@contextmanager
def managed_resource():
    print("Acquiring resource")
    resource = "Resource"
    try:
        yield resource
    finally:
        print("Releasing resource")

with managed_resource() as r:
    print(f"Using {r}")
```

**C++ Equivalent:** RAII (Resource Acquisition Is Initialization)
```cpp
// C++ uses RAII - resources managed by constructors/destructors
#include <fstream>

{
    std::ifstream file("file.txt");
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    // File automatically closed when file goes out of scope
}

// Custom RAII class
class Timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
public:
    Timer() : start(std::chrono::high_resolution_clock::now()) {}
    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << "Elapsed: " << diff.count() << "s\n";
    }
};

{
    Timer t;
    // Some code
} // Timer automatically destroyed, prints elapsed time
```

---

#### Lambda Functions and Functional Programming

**Description:** Anonymous functions and functional tools

```python
# Lambda functions
square = lambda x: x**2
add = lambda x, y: x + y

# Map, filter, reduce
numbers = [1, 2, 3, 4, 5]
squared = list(map(lambda x: x**2, numbers))
evens = list(filter(lambda x: x % 2 == 0, numbers))

from functools import reduce
product = reduce(lambda x, y: x * y, numbers)  # 120

# Sorted with key
words = ['apple', 'pie', 'a', 'cherry']
sorted_by_length = sorted(words, key=lambda x: len(x))
# ['a', 'pie', 'apple', 'cherry']

# Partial functions
from functools import partial

def power(base, exponent):
    return base ** exponent

square = partial(power, exponent=2)
cube = partial(power, exponent=3)

print(square(5))  # 25
print(cube(5))    # 125
```

**C++ Equivalent:**
```cpp
// C++11 lambdas
auto square = [](int x) { return x * x; };
auto add = [](int x, int y) { return x + y; };

// Transform (map)
std::vector<int> numbers = {1, 2, 3, 4, 5};
std::vector<int> squared(numbers.size());
std::transform(numbers.begin(), numbers.end(), squared.begin(),
               [](int x) { return x * x; });

// Filter
std::vector<int> evens;
std::copy_if(numbers.begin(), numbers.end(), std::back_inserter(evens),
             [](int x) { return x % 2 == 0; });

// Reduce
int product = std::accumulate(numbers.begin(), numbers.end(), 1,
                               [](int a, int b) { return a * b; });

// Sort with custom comparator
std::vector<std::string> words = {"apple", "pie", "a", "cherry"};
std::sort(words.begin(), words.end(),
          [](const std::string& a, const std::string& b) {
              return a.length() < b.length();
          });
```

---

#### Slicing

**Description:** Extract portions of sequences  
**Time Complexity:** O(k) where k is the size of the slice

```python
# Basic slicing
lst = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

# [start:end:step]
print(lst[2:7])      # [2, 3, 4, 5, 6]
print(lst[:5])       # [0, 1, 2, 3, 4]
print(lst[5:])       # [5, 6, 7, 8, 9]
print(lst[::2])      # [0, 2, 4, 6, 8] - every 2nd element
print(lst[::-1])     # [9, 8, 7, 6, 5, 4, 3, 2, 1, 0] - reverse

# Negative indices
print(lst[-3:])      # [7, 8, 9]
print(lst[:-3])      # [0, 1, 2, 3, 4, 5, 6]

# String slicing
s = "Hello, World!"
print(s[7:12])       # "World"
print(s[:5])         # "Hello"
print(s[::2])        # "Hlo ol!"

# Slice assignment
lst[2:5] = [20, 30, 40]
# lst = [0, 1, 20, 30, 40, 5, 6, 7, 8, 9]
```

**C++ Equivalent:** No direct equivalent, use iterators or manual copying
```cpp
// C++ equivalent using iterators
std::vector<int> lst = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

// Extract slice [2:7]
std::vector<int> slice(lst.begin() + 2, lst.begin() + 7);

// Reverse
std::vector<int> reversed(lst.rbegin(), lst.rend());

// Every 2nd element (manual)
std::vector<int> every_second;
for (size_t i = 0; i < lst.size(); i += 2) {
    every_second.push_back(lst[i]);
}

// String slicing
std::string s = "Hello, World!";
std::string sub = s.substr(7, 5);  // "World"
```

---

#### Duck Typing and Dynamic Typing

**Description:** Type checking at runtime, "if it walks like a duck..."

```python
# Duck typing - no explicit interface needed
def process(item):
    # Works with any object that has a 'process' method
    return item.process()

class Dog:
    def process(self):
        return "Woof!"

class Cat:
    def process(self):
        return "Meow!"

# Both work without inheritance
print(process(Dog()))   # "Woof!"
print(process(Cat()))   # "Meow!"

# Dynamic typing
x = 5           # int
x = "hello"     # now str
x = [1, 2, 3]   # now list

# Type hints (Python 3.5+) - optional
def greet(name: str) -> str:
    return f"Hello, {name}"

from typing import List, Dict, Optional

def process_items(items: List[int]) -> Dict[str, int]:
    return {"count": len(items), "sum": sum(items)}
```

**C++ Equivalent:** Templates and concepts (C++20)
```cpp
// C++ templates for generic programming
template<typename T>
auto process(T item) -> decltype(item.process()) {
    return item.process();
}

class Dog {
public:
    std::string process() { return "Woof!"; }
};

class Cat {
public:
    std::string process() { return "Meow!"; }
};

// C++20 concepts for constraints
template<typename T>
concept Processable = requires(T t) {
    { t.process() } -> std::convertible_to<std::string>;
};

template<Processable T>
std::string process_with_concept(T item) {
    return item.process();
}
```

---

### C++-Specific Features

#### Templates and Generic Programming

**Description:** Compile-time polymorphism and code generation

```cpp
// Function template
template<typename T>
T max(T a, T b) {
    return (a > b) ? a : b;
}

// Usage
int i = max(3, 5);           // Instantiates max<int>
double d = max(3.5, 2.1);    // Instantiates max<double>

// Class template
template<typename T>
class Stack {
    std::vector<T> elements;
public:
    void push(const T& elem) { elements.push_back(elem); }
    T pop() {
        T elem = elements.back();
        elements.pop_back();
        return elem;
    }
    bool empty() const { return elements.empty(); }
};

// Usage
Stack<int> int_stack;
Stack<std::string> string_stack;

// Template specialization
template<typename T>
class Container {
public:
    void print() { std::cout << "Generic container\n"; }
};

// Specialization for bool
template<>
class Container<bool> {
public:
    void print() { std::cout << "Bool container\n"; }
};

// Variadic templates
template<typename... Args>
void print_all(Args... args) {
    (std::cout << ... << args) << '\n';
}

print_all(1, 2.5, "hello", 'c');  // Prints: 12.5helloc

// Template metaprogramming - compile-time factorial
template<int N>
struct Factorial {
    static const int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static const int value = 1;
};

constexpr int fact5 = Factorial<5>::value;  // Computed at compile time: 120
```

**Python Equivalent:** Generics (type hints) - runtime only
```python
from typing import TypeVar, Generic

T = TypeVar('T')

class Stack(Generic[T]):
    def __init__(self):
        self.elements: list[T] = []
    
    def push(self, elem: T) -> None:
        self.elements.append(elem)
    
    def pop(self) -> T:
        return self.elements.pop()
    
    def empty(self) -> bool:
        return len(self.elements) == 0

# Type hints don't enforce types at runtime
int_stack: Stack[int] = Stack()
string_stack: Stack[str] = Stack()
```

---

#### RAII (Resource Acquisition Is Initialization)

**Description:** Automatic resource management through object lifetime

```cpp
// RAII with smart pointers
#include <memory>

{
    std::unique_ptr<int> ptr = std::make_unique<int>(42);
    // Use ptr
} // Automatically deleted when out of scope

// Shared ownership
std::shared_ptr<int> shared1 = std::make_shared<int>(100);
std::shared_ptr<int> shared2 = shared1;  // Reference count = 2
// Deleted when last shared_ptr goes out of scope

// Custom RAII class for file handling
class FileHandle {
    FILE* file;
public:
    FileHandle(const char* filename, const char* mode) {
        file = fopen(filename, mode);
        if (!file) throw std::runtime_error("Failed to open file");
    }
    
    ~FileHandle() {
        if (file) fclose(file);
    }
    
    // Prevent copying
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
    
    FILE* get() { return file; }
};

// Usage
{
    FileHandle fh("data.txt", "r");
    // Use fh.get()
} // File automatically closed

// Lock guard for mutexes
#include <mutex>

std::mutex mtx;
{
    std::lock_guard<std::mutex> lock(mtx);
    // Critical section
} // Mutex automatically unlocked
```

**Python Equivalent:** Context managers (with statement)
```python
# Python uses context managers instead of RAII
with open('file.txt', 'r') as f:
    # Use file
    pass
# File automatically closed

# Threading lock
import threading
lock = threading.Lock()

with lock:
    # Critical section
    pass
# Lock automatically released
```

---

#### Move Semantics and Perfect Forwarding

**Description:** Efficient resource transfer without copying

```cpp
// Move constructor and move assignment
class Buffer {
    int* data;
    size_t size;
public:
    // Constructor
    Buffer(size_t s) : size(s), data(new int[s]) {}
    
    // Destructor
    ~Buffer() { delete[] data; }
    
    // Copy constructor
    Buffer(const Buffer& other) : size(other.size), data(new int[other.size]) {
        std::copy(other.data, other.data + size, data);
    }
    
    // Move constructor (C++11)
    Buffer(Buffer&& other) noexcept : size(other.size), data(other.data) {
        other.data = nullptr;
        other.size = 0;
    }
    
    // Move assignment
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
};

// Usage
Buffer b1(1000);
Buffer b2 = std::move(b1);  // Move, not copy - very efficient

// Perfect forwarding
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// std::move vs std::forward
void process(std::string&& s) {
    std::string local = std::move(s);  // Transfer ownership
}

template<typename T>
void wrapper(T&& arg) {
    process(std::forward<T>(arg));  // Perfect forwarding
}
```

**Python Equivalent:** No direct equivalent (reference semantics)
```python
# Python uses reference semantics - no explicit move
# Assignment creates new reference, not copy
b1 = [1, 2, 3, 4, 5]
b2 = b1  # Both refer to same list

# For actual copy, use copy module
import copy
b3 = copy.copy(b1)      # Shallow copy
b4 = copy.deepcopy(b1)  # Deep copy
```

---

#### Operator Overloading (More Extensive)

**Description:** Define custom behavior for operators

```cpp
class Complex {
    double real, imag;
public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}
    
    // Arithmetic operators
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }
    
    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }
    
    Complex operator*(const Complex& other) const {
        return Complex(real * other.real - imag * other.imag,
                      real * other.imag + imag * other.real);
    }
    
    // Compound assignment
    Complex& operator+=(const Complex& other) {
        real += other.real;
        imag += other.imag;
        return *this;
    }
    
    // Comparison
    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }
    
    // Stream operators
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << c.real << " + " << c.imag << "i";
        return os;
    }
    
    // Subscript operator
    double& operator[](int index) {
        return (index == 0) ? real : imag;
    }
    
    // Function call operator
    double operator()() const {
        return std::sqrt(real * real + imag * imag);
    }
    
    // Increment/decrement
    Complex& operator++() {  // Prefix
        ++real;
        return *this;
    }
    
    Complex operator++(int) {  // Postfix
        Complex temp = *this;
        ++real;
        return temp;
    }
};

// Usage
Complex c1(3, 4);
Complex c2(1, 2);
Complex c3 = c1 + c2;
std::cout << c3 << '\n';  // Uses operator<<
double magnitude = c1();   // Uses operator()
```

**Python Equivalent:** Magic methods
```python
class Complex:
    def __init__(self, real=0, imag=0):
        self.real = real
        self.imag = imag
    
    def __add__(self, other):
        return Complex(self.real + other.real, self.imag + other.imag)
    
    def __sub__(self, other):
        return Complex(self.real - other.real, self.imag - other.imag)
    
    def __mul__(self, other):
        return Complex(
            self.real * other.real - self.imag * other.imag,
            self.real * other.imag + self.imag * other.real
        )
    
    def __eq__(self, other):
        return self.real == other.real and self.imag == other.imag
    
    def __str__(self):
        return f"{self.real} + {self.imag}i"
    
    def __getitem__(self, index):
        return self.real if index == 0 else self.imag
    
    def __call__(self):
        return (self.real**2 + self.imag**2)**0.5

# Usage
c1 = Complex(3, 4)
c2 = Complex(1, 2)
c3 = c1 + c2
print(c3)
magnitude = c1()
```

---

#### Const Correctness

**Description:** Compile-time guarantees about immutability

```cpp
class Point {
    int x, y;
public:
    Point(int x, int y) : x(x), y(y) {}
    
    // Const member function - doesn't modify object
    int getX() const { return x; }
    int getY() const { return y; }
    
    // Non-const member function
    void setX(int newX) { x = newX; }
    
    // Const and non-const overloads
    int& operator[](int index) {
        return (index == 0) ? x : y;
    }
    
    const int& operator[](int index) const {
        return (index == 0) ? x : y;
    }
};

// Const parameters
void print(const Point& p) {  // Can't modify p
    std::cout << p.getX() << ", " << p.getY() << '\n';
    // p.setX(10);  // Error: can't call non-const method
}

// Const pointers
const int* ptr1;        // Pointer to const int
int* const ptr2 = &x;   // Const pointer to int
const int* const ptr3;  // Const pointer to const int

// constexpr - compile-time constants
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

constexpr int fact5 = factorial(5);  // Computed at compile time
```

**Python Equivalent:** No built-in const, use conventions
```python
# Python has no const keyword
# Use naming conventions (UPPERCASE for constants)
PI = 3.14159
MAX_SIZE = 100

# Immutable types
t = (1, 2, 3)  # Tuple is immutable
# t[0] = 5  # Error

# frozen dataclass (Python 3.7+)
from dataclasses import dataclass

@dataclass(frozen=True)
class Point:
    x: int
    y: int

p = Point(1, 2)
# p.x = 5  # Error: frozen dataclass

# Property with no setter
class Circle:
    def __init__(self, radius):
        self._radius = radius
    
    @property
    def radius(self):
        return self._radius
    
    # No setter - effectively read-only

c = Circle(5)
print(c.radius)  # OK
# c.radius = 10  # Error: can't set attribute
```

---

#### Multiple Inheritance and Virtual Functions

**Description:** Complex inheritance hierarchies with runtime polymorphism

```cpp
// Virtual functions for runtime polymorphism
class Animal {
public:
    virtual void makeSound() const {
        std::cout << "Some sound\n";
    }
    
    virtual ~Animal() = default;  // Virtual destructor
};

class Dog : public Animal {
public:
    void makeSound() const override {
        std::cout << "Woof!\n";
    }
};

// Multiple inheritance
class Flyable {
public:
    virtual void fly() const = 0;  // Pure virtual (abstract)
    virtual ~Flyable() = default;
};

class Swimmable {
public:
    virtual void swim() const = 0;
    virtual ~Swimmable() = default;
};

class Duck : public Animal, public Flyable, public Swimmable {
public:
    void makeSound() const override {
        std::cout << "Quack!\n";
    }
    
    void fly() const override {
        std::cout << "Duck flying\n";
    }
    
    void swim() const override {
        std::cout << "Duck swimming\n";
    }
};

// Usage with polymorphism
Animal* animal = new Dog();
animal->makeSound();  // "Woof!" - runtime dispatch
delete animal;

Duck duck;
duck.makeSound();
duck.fly();
duck.swim();
```

**Python Equivalent:** Multiple inheritance with MRO
```python
# Python supports multiple inheritance
class Animal:
    def make_sound(self):
        print("Some sound")

class Dog(Animal):
    def make_sound(self):
        print("Woof!")

class Flyable:
    def fly(self):
        raise NotImplementedError

class Swimmable:
    def swim(self):
        raise NotImplementedError

class Duck(Animal, Flyable, Swimmable):
    def make_sound(self):
        print("Quack!")
    
    def fly(self):
        print("Duck flying")
    
    def swim(self):
        print("Duck swimming")

# All methods are virtual in Python
animal: Animal = Dog()
animal.make_sound()  # "Woof!" - runtime dispatch

duck = Duck()
duck.make_sound()
duck.fly()
duck.swim()

# Method Resolution Order (MRO)
print(Duck.__mro__)
```

---

#### Preprocessor and Macros

**Description:** Text substitution before compilation

```cpp
// Include guards
#ifndef MYHEADER_H
#define MYHEADER_H

// Macro definitions
#define PI 3.14159
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SQUARE(x) ((x) * (x))

// Conditional compilation
#ifdef DEBUG
    #define LOG(msg) std::cout << msg << '\n'
#else
    #define LOG(msg)
#endif

// Stringification
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

// Token pasting
#define CONCAT(a, b) a##b

// Variadic macros
#define PRINT(...) printf(__VA_ARGS__)

// Usage
int area = PI * SQUARE(5);
int maximum = MAX(10, 20);

LOG("Debug message");  // Only in debug builds

const char* version = TOSTRING(VERSION);
int var_CONCAT(x, 123);  // Creates var_x123

PRINT("Value: %d\n", 42);

#endif // MYHEADER_H
```

**Python Equivalent:** No preprocessor, use runtime alternatives
```python
# Python has no preprocessor
# Use constants
PI = 3.14159

# Use functions instead of macros
def max_val(a, b):
    return a if a > b else b

def square(x):
    return x * x

# Conditional execution
import os
DEBUG = os.getenv('DEBUG', 'false').lower() == 'true'

def log(msg):
    if DEBUG:
        print(msg)

# String formatting
version = "1.0.0"
print(f"Version: {version}")

# Variable names from strings (not recommended)
var_name = "x123"
globals()[f"var_{var_name}"] = 42
```

---

## Conclusion

Both C++ STL and Python provide powerful tools for common programming tasks. C++ offers more control and performance, while Python provides simplicity and ease of use. Understanding both allows you to choose the right tool for the job and transfer knowledge between languages.

