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

## Conclusion

Both C++ STL and Python provide powerful tools for common programming tasks. C++ offers more control and performance, while Python provides simplicity and ease of use. Understanding both allows you to choose the right tool for the job and transfer knowledge between languages.
