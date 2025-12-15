# Effective Modern C++ Summary

### Item 1: Understand template type deduction
**Item Summary**
Template type deduction is the foundation for `auto`. It generally works as expected (ignoring reference-ness for by-value parameters), but has specific rules for direct references, universal references, and by-value passing.

**Key Points**
- **Case 1: Pointer/Reference (non-universal)**: `template<typename T> void f(T& param);`
    - Reference-ness of argument is ignored.
    - `const` of argument is preserved.
- **Case 2: Universal Reference**: `template<typename T> void f(T&& param);`
    - If arg is lvalue: `T` and `ParamType` deduced as lvalue reference (`T&`).
    - If arg is rvalue: Normal deduction rules apply (`T` is non-ref).
- **Case 3: By-Value**: `template<typename T> void f(T param);`
    - Reference-ness, `const`, and `volatile` of argument are ignored (since it's a copy).
- **Decay**: Arrays and functions decay to pointers unless passed by reference (where they are deduced as reference-to-array/function).

**Example**
```cpp
template<typename T> void f(T& param);
int x = 27; const int cx = x; const int& rx = x;

f(x);  // T=int, param=int&
f(cx); // T=const int, param=const int&
f(rx); // T=const int, param=const int& // constness preserved
```

### Item 2: Understand auto type deduction
**Item Summary**
`auto` type deduction is practically identical to template type deduction, with one key exception regarding braced initializers.

**Key Points**
- **Correspondence**: `auto` plays the role of `T` and the type specifier (e.g., `const auto&`) plays the role of `ParamType` in template deduction.
- **The Exception**: Braced initializers (`auto x = {27};`) are deduced as `std::initializer_list`, whereas template deduction would fail for `{27}`.
- **C++14**: `auto` in function return types or lambda parameters uses *template* deduction rules, not `auto` rules (so `{}` will fail).

**Example**
```cpp
auto x = 27; // int
const auto cx = x; // const int
auto& rx = x; // int&

// The Exception:
auto x1 = {27}; // std::initializer_list<int>
auto x2 = {1, 2}; // std::initializer_list<int>
// auto x3 = {1, 2.0}; // Error! Types don't match
```

### Item 3: Understand decltype
**Item Summary**
`decltype` returns the exact declared type of a name or expression, including top-level `const` and references, unlike `auto`/templates which often strip them.

**Key Points**
- **Names**: `decltype(var)` gives the exact type of `var`.
- **Expressions**: `decltype(expr)` returns `T&` if `expr` is an lvalue expression (that isn't just a name) and `T` if rvalue.
    - Caution: `int x = 0;` `decltype(x)` is `int`, but `decltype((x))` is `int&`.
- **decltype(auto)** (C++14): Enables type deduction (like `auto`) but uses `decltype` rules (preserving exact ref/const qualifiers). Useful for return types that must forward the exact result (e.g., `return c[i];`).

**Example**
```cpp
const int i = 0;
// decltype(i) -> const int

bool f(const Widget& w); 
// decltype(f) -> bool(const Widget&)
// decltype(f(w)) -> bool (return type)

std::vector<int> v = {1};
// decltype(v[0]) -> int& (reference to element)
```

### Item 4: Know how to view deduced types
**Item Summary**
Tools can help you verify what types are being deduced, though they vary in reliability.

**Key Points**
- **IDE**: Hovering often shows types, but may simplify them (omitting `const`, reference).
- **Compiler Diagnostics**: Intentionally cause an error (e.g., try to instantiate an undefined template `TD<decltype(x)>`) to see the deduced type in the error message.
- **Runtime**: `typeid(x).name()` discards reference/const/volatile. `Boost.TypeIndex` is more accurate for full type dumping.

**Example**
```cpp
template<typename T>
void f(const T& param) {
    // std::cout << typeid(T).name() << endl; // Often unreliable (e.g. just "int")
    
    // Using Boost TypeIndex (if available)
    // using boost::typeindex::type_id_with_cvr;
    // cout << type_id_with_cvr<T>().pretty_name() << endl;
}
```

## Chapter 2: auto

### Item 5: Prefer auto to explicit type declarations
**Item Summary**
Using `auto` ensures variables are initialized, avoids verbose types, and prevents implicit conversions that can lead to bugs or performance issues.

**Key Points**
- **Initialization**: `auto x;` is an error; forces initialization.
- **Type Mismatches**: Explicit types might not match the right-hand side exactly (e.g., `unsigned` vs `std::vector::size_type` (64-bit)). `auto` guarantees a match.
- **Performance**: In loops like `for (const std::pair<std::string,int>& p : map)`, explicit types might cause temporary copies if the key type is `const std::string` but declared as `std::string`. `auto` avoids this.
- **Refactoring**: Changing return types propagates automatically to `auto` variables.

**Example**
```cpp
std::map<std::string, int> m;
// ...
// Loop copies keys because std::pair key is const!
// std::pair<std::string, int> vs std::pair<const std::string, int>
for (const std::pair<std::string, int>& p : m) { ... } // Implicit Cast + Copy!

// Auto avoids this
for (const auto& p : m) { ... } // p deduced as std::pair<const std::string, int>&
```

### Item 6: Use the explicitly typed initializer idiom when auto deduces undesired types
**Item Summary**
`auto` can deduce "proxy types" (e.g., `std::vector<bool>::reference` or expression templates) instead of the value type you expect, potentially leading to dangling pointers or obscure errors.

**Key Points**
- **Proxy Types**: Helper classes like `std::vector<bool>::reference` behave like the value but are temporary objects. Reference-to-proxy is dangerous if the proxy dies.
- **Solution**: Use `static_cast` to force the desired type when using `auto`.
    - `auto highPriority = static_cast<bool>(features(w)[5]);`
    - Creates a clear intention ("I am casting this to bool") and fixes the deduction.
**Item Summary**
`auto` can deduce "proxy types" (e.g., `std::vector<bool>::reference` or expression templates) instead of the value type you expect, potentially leading to dangling pointers or obscure errors.

**Key Points**
- **Proxy Types**: Helper classes like `std::vector<bool>::reference` behave like the value but are temporary objects. Reference-to-proxy is dangerous if the proxy dies.
- **Solution**: Use `static_cast` to force the desired type when using `auto`.
    - `auto highPriority = static_cast<bool>(features(w)[5]);`
    - Creates a clear intention ("I am casting this to bool") and fixes the deduction.

**Example**
```cpp
std::vector<bool> features(10);
// auto highPriority = features[5]; // Deduction: std::vector<bool>::reference (Proxy!)
// process(highPriority); // Undefined behavior if 'features' changes/dies?

// Fix
auto highPriority = static_cast<bool>(features[5]); // bool
```

## Chapter 3: Moving to Modern C++

### Item 7: Distinguish between () and {} when creating objects
**Item Summary**
Braced initialization (uniform initialization) is the most widely usable syntax and prevents narrowing conversions, but it strongly prefers `std::initializer_list` constructors, which can lead to surprising behavior.

**Key Points**
- **Uniformity**: Braces `{}` can be used everywhere, including default initialization of class members and initializing containers with specific values (`std::vector<int> v{1, 2, 3};`).
- **Narrowing**: Braced initialization forbids narrowing conversions (e.g., `int x{3.14}` is an error), whereas `()` and `=` allow them.
- **Vexing Parse**: `{}` avoids C++'s most vexing parse (where `Widget w();` declares a function). `Widget w{};` calls the default constructor.
- **The Caveat**: If a class has a constructor taking `std::initializer_list`, braced initialization *strongly* prefers it, even performing hidden conversions to match it.
    - `auto` deduction: `auto x = {1};` deduces `std::initializer_list<int>`, which might not be what you want.

**Example**
```cpp
std::vector<int> v1(10, 20); // 10 elements with value 20
std::vector<int> v2{10, 20}; // 2 elements, values 10 and 20

// Widget w(); // Function declaration!
// Widget w{}; // Default ctor
```

### Item 8: Prefer nullptr to 0 and NULL
**Item Summary**
`nullptr` has a dedicated type `std::nullptr_t` that implicitly converts to all pointer types but not to integers, avoiding overload resolution ambiguities and increasing clarity.

**Key Points**
- **Ambiguity**: `0` and `NULL` are integers. Passing them to functions overloaded on `int` and `void*` will call the `int` version (or be ambiguous), never the pointer version. `nullptr` always calls the pointer version.
- **Clarity**: `auto result = findRecord(); if (result == nullptr)` makes it obvious `result` is a pointer. `if (result == 0)` is ambiguous.
- **Templates**: `nullptr` works correctly in template type deduction where `0` is deduced as `int` (causing type errors when forwarded to a pointer-expecting function).

**Example**
```cpp
void f(int);
void f(bool);
void f(void*);

f(0);       // calls f(int)
f(NULL);    // implementation defined, likely f(int)
f(nullptr); // calls f(void*)
```

### Item 9: Prefer alias declarations to typedefs
**Item Summary**
Alias declarations (`using`) support templates (alias templates) whereas `typedef` does not, often avoiding the need for `typename` and `::type` in template metaprogramming.

**Key Points**
- **Sytax**: `using UPtrMapSS = std::unique_ptr<...>;` is often cleaner than `typedef`.
- **Templates**:
    - `template<typename T> using MyList = std::list<T, MyAlloc<T>>;` works directly.
    - With `typedef`, you must wrap it in a struct: `struct MyList { typedef ... type; };`. Using it requires `typename MyList<T>::type`.
- **Type Traits**: C++14 provides alias templates (`std::remove_const_t<T>`) which are easier to use than C++11's struct-based traits (`typename std::remove_const<T>::type`).

**Example**
```cpp
// Typedef (C++98) - Verbose
typedef std::unique_ptr<std::unordered_map<std::string, std::string>> UPtrMapSS;

// Alias Declaration (C++11) - Clearer
using UPtrMapSS = std::unique_ptr<std::unordered_map<std::string, std::string>>;

// Templates
template <typename T>
using MyLst = std::list<T, MyAlloc<T>>; // Clean
```

### Item 10: Prefer scoped enums to unscoped enums
**Item Summary**
Scoped enums (`enum class`) prevent name pollution and implicit conversions to integral types, making code safer and cleaner.

**Key Points**
- **Scoping**: Enumerators are inside the enum's scope (`Color::Red`), not the enclosing scope. Prevents name clashes.
- **Strong Typing**: No implicit conversion to `int`. `if (color < 14.5)` is an error with scoped enums, but valid (and likely buggy) with unscoped enums.
- **Forward Declaration**: Scoped enums can always be forward declared. Unscoped enums can only be forward declared if intrinsic type is specified (C++11).

**Example**
```cpp
enum class Color { Red, Green, Blue };
// int color = Color::Red; // Error: no implicit conversion
int color = static_cast<int>(Color::Red); // OK
Color c = Color::Red; // Safe
```

### Item 11: Prefer deleted functions to private undefined ones
**Item Summary**
Use `= delete` to disable functions (like copy constructors) instead of the C++98 hack of declaring them `private` and undefined. Deleted functions result in better error messages and can be used to disable non-member functions and template instantiations.

**Key Points**
- **Better Errors**: Accessing a deleted function is a compile-time error. Accessing a private undefined function is a link-time error (if done by a friend/member) or a "private" error.
- **Any Function**: You can delete non-member functions to prevent implicit conversions (e.g., `bool isLucky(int); bool isLucky(char) = delete;` prevents `isLucky('a')`).
- **Templates**: Delete specific template specializations to forbid them (e.g., `void process(void*) = delete;`). This serves as a compile-time constraint usage.

**Example**
```cpp
bool isLucky(int number);
// Prevent char->int promotion
bool isLucky(char) = delete; 
// isLucky('a'); // Error!

// Prevent copying
class Widget {
public:
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;
};
```

### Item 12: Declare overriding functions override
**Item Summary**
Marking derived class virtual function overrides with `override` allows the compiler to catch mismatch errors (e.g., mismatched types, constness, or reference qualifiers) that would otherwise silently fail to override.

**Key Points**
- **Safety**: `void func() const override;` checks that there is a base class virtual function with the exact same signature.
- **Reference Qualifiers**: C++11 allows member functions to be overloaded for lvalues (`void f() &`) and rvalues (`void f() &&`). `override` ensures these match the base class too.
- **Refactoring**: If you change the base class signature, `override` ensures all derived classes break at compile time, prompting necessary updates.

**Example**
```cpp
class Base {
    virtual void mf1() const;
    virtual void mf2(int x);
};

class Derived : public Base {
    virtual void mf1();           // Warning! Hides Base::mf1, doesn't override (missing const)
    virtual void mf1() const override; // OK: Overrides

    // virtual void mf2(double x) override; // Error! Signature mismatch
};
```

### Item 13: Prefer const_iterators to iterators
**Item Summary**
Using `const_iterator` (pointer-to-const equivalent) is now practical and easy in C++11/14 thanks to `cbegin()`/`cend()` and non-member support, promoting const-correctness.

**Key Points**
- **C++98 Pain**: It was hard to get a `const_iterator` from a non-const container, and `insert`/`erase` didn't accept them.
- **C++11 Fix**: Container functions `insert`/`erase` now accept `const_iterator`. `cbegin()`/`cend()` provide easy access.
- **Generic Code**: In generic code, prefer non-member `std::cbegin(c)` to support arrays and containers alike. (Note: C++11 missed non-member `cbegin`, added in C++14, but easy to polyfill).

**Example**
```cpp
std::vector<int> v = {1, 2, 3};
// C++14 generic way to get const_iterator
auto it = std::find(std::cbegin(v), std::cend(v), 2);
```

### Item 14: Declare functions noexcept if they won't emit exceptions
**Item Summary**
Declaring non-throwing functions `noexcept` allows better compiler optimizations (smaller binary, faster code) and is critical for move operations to be used by strong-exception-safety containers like `std::vector`.

**Key Points**
- **Optimization**: Compilers don't need to maintain unwindable stack states for `noexcept` functions.
- **Move Semantics**: `std::vector::push_back` uses move operations only if they are `noexcept`; otherwise, it falls back to copy to satisfy strong exception guarantees.
- **Design**: `noexcept` is part of the interface. Use it for leaf functions, move operations, and swaps. Destructors are implicitly `noexcept`.
- **Caution**: Only use it if you are sure. Violating `noexcept` terminates the program immediately (calling `std::terminate`).

**Example**
```cpp
void f(int) noexcept; // Optimizable: compiler knows it won't throw

class Widget {
public:
    // Move operations should be noexcept for best vector performance
    Widget(Widget&&) noexcept;
};
```

### Item 15: Use constexpr whenever possible
**Item Summary**
`constexpr` is stricter than `const`: it indicates a value that is not only constant but known during compilation, allowing it to be used in array sizes, template arguments, etc.

**Key Points**
- **Objects**: `constexpr int x = 10;` ensures `x` is a compile-time constant. `const int x = val` might be runtime-initialized.
- **Functions**: `constexpr` functions produce compile-time constants *if* called with compile-time constants. Otherwise, they act as normal runtime functions.
    - C++11 restrictions (one return statement) were loosened in C++14 (loops, multiple statements allowed).
- **User Types**: Classes can have `constexpr` constructors and getters, allowing user-defined types to be created and manipulated at compile time (e.g., `constexpr Point p(1.0, 2.0);`).

**Example**
```cpp
constexpr int pow(int base, int exp) {
    return (exp == 0) ? 1 : base * pow(base, exp - 1);
}
constexpr int res = pow(2, 10); // Computed at compile time
std::array<int, res> data; // Valid, res is a constant
```

### Item 16: Make const member functions thread safe
**Item Summary**
A `const` member function implies read-only access, which clients expect to be safe to call concurrently. If a `const` function modifies internal state (via `mutable`), it must handle synchronization itself.

**Key Points**
- **Logical vs Bitwise Const**: C++ compilers enforce bitwise const, but `mutable` allows logical constness (e.g., caching a calculation).
- **Thread Safety**: If multiple threads call `root()` (const) on the same object, and `root()` updates a `mutable` cache, you have a data race.
- **Solution**: Use `std::mutex` (or `std::atomic` for simple counters) within the `const` function to protect mutable state. Note that `std::mutex` is move-only, making the class move-only.

**Example**
```cpp
class Widget {
public:
    void doWork() const {
        std::lock_guard<std::mutex> g(m); // Mutex makes it safe
        // ... modify mutable state ...
    }
private:
    mutable std::mutex m;
};
```

### Item 17: Understand special member function generation
**Item Summary**
C++11 adds move operations to the "special member functions" (default ctor, dtor, copy ctor, copy assign). The rules for when they are automatically generated are specific and interact with each other.

**Key Points**
- **Move Operations**: Generated *only* if the class has NO user-declared copy operations, move operations, or destructor.
- **Copy Operations**:
    - Default generation is deprecated if a destructor or copy-assign is present (Rule of Three).
    - Deleted if a move operation is declared.
- **Rule of Five**: If you declare one (dtor, copy/move ctor/assign), you should probably declare all five.
- **Recommendations**: Use `= default` to explicitly opt-in to default behavior if you wrote a destructor but still want default move/copy behavior.

**Example**
```cpp
class Widget {
public:
    ~Widget() { ... } // User-declared dtor suppresses move ops
    
    // Restore move ops
    Widget(Widget&&) = default;
    Widget& operator=(Widget&&) = default;
    
    // Restore copy ops (if desired)
    Widget(const Widget&) = default;
    Widget& operator=(const Widget&) = default;
};
```

## Chapter 4: Smart Pointers

### Item 18: Use std::unique_ptr for exclusive-ownership resource management
**Item Summary**
`std::unique_ptr` is the default smart pointer to use. It has practically zero overhead (same size as a raw pointer), supports move-only semantics (exclusive ownership), and assumes ownership of a raw pointer to automatic delete it when the `unique_ptr` goes out of scope.

**Key Points**
- **Migration**: Easily converts to `std::shared_ptr`.
- **Custom Deleters**: Can specify a custom deleter type. `std::unique_ptr<T, DeleterType>`. Note that stateful deleters (like lambdas with captures) increase the size of the `unique_ptr`.
- **Factory Functions**: Ideal for factory functions because the caller can decide whether to keep it unique or convert to shared.

**Example**
```cpp
std::unique_ptr<Widget> makeWidget() {
    return std::make_unique<Widget>();
}
auto up = makeWidget(); // unique_ptr
std::shared_ptr<Widget> sp = makeWidget(); // Converts to shared_ptr automatically
```

### Item 19: Use std::shared_ptr for shared-ownership resource management
**Item Summary**
`std::shared_ptr` uses reference counting to manage resources. The resource is destroyed when the last `shared_ptr` pointing to it is destroyed.

**Key Points**
- **Overhead**: Twice the size of a raw pointer (one for object, one for control block). Control block contains ref count, weak count, custom deleter, allocator, etc.

**Example**
```cpp
auto sp1 = std::make_shared<Widget>(); // Ref count 1
auto sp2 = sp1; // Ref count 2
// sp1 and sp2 share ownership. Widget deleted only when both die.
```
- **Control Block**: Created when a `shared_ptr` is created from a raw pointer or via `make_shared`. Avoid creating multiple `shared_ptr`s from the same raw pointer (results in double free); use `make_shared` or creating from `unique_ptr` instead.
- **`enable_shared_from_this`**: Use this base class if an object needs to create a `shared_ptr` to itself (e.g., adding `this` to a list of listeners). Call `shared_from_this()`.

### Item 20: Use std::weak_ptr for std::shared_ptr-like pointers that can dangle
**Item Summary**
`std::weak_ptr` observes an object managed by `std::shared_ptr` without keeping it alive (doesn't increment ref count). It can detect if the object has been destroyed (dangles).

**Key Points**
- **Use Cases**: Caching, Observer pattern (subject holds weak pointers to observers), Breaking reference cycles (A->B, B->A).
- **Access**: Cannot dereference directly. Must convert to `shared_ptr` via `lock()` (returns null if expired) or constructor (throws if expired).
- **Control Block**: Weak pointers also keep the control block alive (but not the object), so memory for the control block isn't freed until all weak pointers are gone too.

### Item 21: Prefer std::make_unique and std::make_shared to direct use of new
**Item Summary**
`std::make_unique` (C++14) and `std::make_shared` (C++11) should be preferred over `new`. They eliminate code duplication (repeating type name), improve exception safety, and (for `make_shared`) improve performance.

**Key Points**
- **Exception Safety**: `process(std::unique_ptr<T>(new T), priority())` can leak `T` if `priority()` throws after `new T` but before `unique_ptr` constructor. `make_unique` fixes this.
- **Performance**: `std::make_shared` allocates the object and control block in a single heap allocation. Direct `new` requires two.
- **Limitations**: Cannot use `make` functions if you need a custom deleter or if you need to pass a braced initializer list (use `auto list = {1, 2}; make_shared<Vec>(list)` workaround).

**Example**
```cpp
// Bad:
process(std::shared_ptr<Widget>(new Widget), priority());
// If priority() throws, "new Widget" leak!

// Good:
process(std::make_shared<Widget>(), priority()); // Safe
```

### Item 22: When using the Pimpl Idiom, define special member functions in the implementation file
**Item Summary**
When using the Pimpl (Pointer to Implementation) idiom with `std::unique_ptr`, you must define the destructor (and move operations) in the implementation file where the implementation class is fully defined.

**Key Points**
- **Incomplete Type**: `std::unique_ptr` checks (via `static_assert` in `default_delete`) that the pointed-to type is complete when the deleter is instantiated.
- **Destructor**: If you don't declare a destructor, the compiler generates one in the header file. If `Impl` is incomplete there, compilation fails.
- **Fix**: Declare `~Widget();` in the header, and define `Widget::~Widget() = default;` in the `.cpp` file (after `struct Impl` is defined). Same applies to move constructor and move assignment.

**Example**
```cpp
// Header
class Widget {
public:
    Widget();
    ~Widget(); // Declaration only
private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};

// Implementation file
struct Widget::Impl { ... };
Widget::~Widget() = default; // Definition here
```

## Chapter 5: Rvalue References, Move Semantics, and Perfect Forwarding

### Item 23: Understand std::move and std::forward
**Item Summary**
`std::move` and `std::forward` do nothing at runtime; they are just casts. `std::move` unconditionally casts to an rvalue (indicating "eligible for move"), while `std::forward` casts to an rvalue only if the argument was initialized with an rvalue.

**Key Points**
- **std::move**: Calculates `remove_reference<T>::type&&`. Doesn't actually move anything; just enables the compiler to choose move overloads.
- **Const**: Don't `std::move` a `const` object if you intend to move it. `const T&&` binds to the copy constructor (`const T&`), not the move constructor (`T&&`), so it will silently copy.
- **std::forward**: Used for perfect forwarding. Keeps the value category (lvalue vs rvalue) of the original argument.

**Example**
```cpp
// std::move casts to rvalue
std::string s;
std::move(s); // cast string& to string&&

// std::forward conditional
template<typename T>
void f(T&& p) {
    process(std::forward<T>(p)); // Casts to rvalue only if p was bound to rvalue
}
```

### Item 24: Distinguish universal references from rvalue references
**Item Summary**
`T&&` is not always an rvalue reference. If type deduction is involved (e.g., function template parameter `T&&` or `auto&&`), it is a "Universal Reference" (now officially called Forwarding Reference) that can bind to lvalues or rvalues.

**Key Points**
- **Synthesis**:
    - `Widget&& param`: Rvalue reference (no deduction).
    - `template<typename T> void f(T&& param)`: Universal reference (deduction).
    - `template<typename T> void f(std::vector<T>&& param)`: Rvalue reference (pattern is not exactly `T&&`).
- **Binding**: Universal references bind as lvalue references if initialized with an lvalue, and as rvalue references if initialized with an rvalue.

**Example**
```cpp
void f(Widget&& param);       // Rvalue Ref (No deduction)

template<typename T>
void f(T&& param);            // Universal Ref (T&& with deduction)

auto&& var = param;           // Universal Ref
```

### Item 25: Use std::move on rvalue references, std::forward on universal references
**Item Summary**
When passing rvalue references to other functions, use `std::move`. When passing universal references, use `std::forward`. This ensures proper value category propagation.

**Key Points**
- **Last Use**: Only apply `move`/`forward` on the *last* use of the variable in the function.
- **Return Value Optimization (RVO)**: Do **not** use `std::move` when returning a local object by value. It prevents RVO (copy elision). Compilers will automatically treat the local object as an rvalue if RVO isn't performed.

**Example**
```cpp
template<typename T>
void f(T&& w) {
    // Universal ref: use forward
    process(std::forward<T>(w));
}

void f(Widget&& w) {
    // Rvalue ref: use move
    process(std::move(w));
}
```

### Item 26: Avoid overloading on universal references
**Item Summary**
Universal references (`T&&` in templates) allow functions to accept any type of argument effectively. However, overloading a function that takes a universal reference is dangerous because the universal reference overload is extremely greedy: it often matches arguments better than specific overloads (e.g., `short` matches `T&&` better than `int`).

**Key Points**
- **Greediness**: `template<typename T> void f(T&&)` matches almost everything exactly, often beating implicit conversions needed for other overloads.
- **Perfect Forwarding Constructors**: A constructor `template<typename T> Widget(T&&)` is problematic. It can hijack the copy constructor (`Widget(const Widget&)`) for non-const lvalues, because `T&&` (deducing `T=Widget&`) is a better match than `const Widget&`.
- **Inheritance**: Derived class copy/move operations call the base class constructor. If the base has a forwarding constructor, it might intercept these calls instead of the base copy/move constructor, causing errors.

**Example**
```cpp
template<typename T> void logAndAdd(T&& name) { ... }
std::string name("Bart");
logAndAdd(name); // T=string&. OK.

// Problem:
short idx = 22;
logAndAdd(idx);  
// T=short&. Exact match!
// If logAndAdd expects string, it tries to construct string from short (char). 
// "Bart" constructor preferred over int->string?
// Actually int->string ctor is explicit?
// In any case, T&& matches 'idx' better than 'int' or 'string'.
```

### Item 27: Familiarize yourself with alternatives to overloading on universal references
**Item Summary**
Since overloading on universal references is problematic (Item 26), consider alternatives: distinct function names, passing by `const T&`, passing by value, Tag Dispatch, or explicitly constraining templates (SFINAE).

**Key Points**
- **Tag Dispatch**: Use `std::is_integral` (or other traits) to dispatch to specific implementation functions. `void log(T&& name) { logImpl(forward<T>(name), is_integral<remove_ref_t<T>>()); }`.
- **`std::enable_if`**: Disable the template if the condition isn't met. For constructors, disable if `T` is the class type (to avoid hijacking copy/move ctors).
- **Trade-offs**: "Perfect forwarding" approaches (Tag Dispatch, `enable_if`) are efficient but give poor error messages. Pass-by-value is simple but potentially less efficient.

**Example**
```cpp
// Tag dispatch
template<typename T>
void logAndAdd(T&& name) {
    logAndAddImpl(std::forward<T>(name), 
                  std::is_integral<typename std::remove_reference<T>::type>());
}
```

### Item 28: Understand reference collapsing
**Item Summary**
Reference collapsing is the mechanism that allows `T&&` to work as a universal reference. References to references are illegal in user code but allowed in compiler-generated contexts (templates, `auto`, `decltype`, `typedef`).

**Key Points**
- **Rule**: If either reference is an lvalue reference (`&`), the result is an lvalue reference. Only if both are rvalue references (`&&`) is the result an rvalue reference.
- **Deduction**:
    - Pass lvalue `w`: `T` deduced as `Widget&`. `T&&` -> `Widget& &&` -> `Widget&`.
    - Pass rvalue `move(w)`: `T` deduced as `Widget`. `T&&` -> `Widget &&`.
- **`std::forward`**: Relies on this to cast back to `T&&` (which collapses to `&` for lvalues and `&&` for rvalues).

**Example**
```cpp
// T& && -> T&
// T&& && -> T&&
template<typename T> void f(T&& param);
int x;
f(x); // T is int&. Param is int& && -> int&.
```

### Item 29: Assume that move operations are not present, not cheap, and not used
**Item Summary**
Move semantics are great, but don't overestimate them. Many types don't support them (legacy C++98), some moves are expensive (linear time for `std::array`), and some operations refuse to use them if they aren't `noexcept` (e.g., `vector` resizing).

**Key Points**
- **Cheapness**: `std::string` move is constant time (pointer swap), but `std::array` move is linear (moves all elements). SSO (Small String Optimization) can also make string moves behave like copies for small strings.
- **Exception Safety**: `std::vector::push_back` will copy instead of move if the move constructor isn't `noexcept`, to preserve Strong Exception Guarantee.
- **Generic Code**: In templates, assume copy cost unless you know the type supports cheap moves.

**Example**
```cpp
std::array<int, 1000> a1, a2;
a2 = std::move(a1); // Compiles, but is O(N)! 
// std::array move performs element-wise move (int moves are copies).
// Unlike std::vector which is O(1) (pointer swap).
```

### Item 30: Familiarize yourself with perfect forwarding failure cases
**Item Summary**
Perfect forwarding (Item 24) is powerful but not truly perfect. It fails when the template types cannot be deduced or when the deduced types behave differently than expected.

**Key Points**
- **Braced Initializers**: `fwd({1, 2, 3})` fails because types cannot be deduced for `{...}` unless the parameter is explicitly `std::initializer_list`.
- **0 or NULL**: Deduced as `int`, not pointer types. Fails if forwarded to a pointer parameter.
- **Static Const Members**: Passing declaration-only static const members by reference (universal ref) requires a definition (ODR-use), otherwise linking fails.
- **Overloaded Functions**: `fwd(funcName)` fails because the compiler doesn't know which overload to pick for deduction.
- **Bitfields**: Cannot bind a non-const reference to a bitfield.

**Example**
```cpp
void f(const std::vector<int>& v);
// f({1, 2, 3}); // OK, implicit conversion to vector

template<typename T> void fwd(T&& param) { f(std::forward<T>(param)); }
// fwd({1, 2, 3}); // Error! Template deduction fails for braced init list.
```

## Chapter 6: Lambda Expressions

### Item 31: Avoid default capture modes
**Item Summary**
Default capture modes (`[=]` and `[&]`) are dangerous. `[&]` can capture references to local variables that act as dangling references if the lambda outlives the scope. `[=]` can misleadingly appear self-contained but actually captures `this` pointer implicitly if used in a member function, causing use-after-free if the object dies.

**Key Points**
- **Dangling References**: `[&]` captures variables by reference. If the lambda is returned or stored, the references will dangle.
- **The `this` Trap**: `[=]` inside a member function captures `this` pointer, not member variables. Accessing members inside the lambda dereferences `this`, crashing if the object is destroyed.
- **Static Data**: Default captures do *not* capture static duration variables (they can be used without capture), leading to misconceptions about closure self-containment.

**Example**
```cpp
using FilterContainer = std::vector<std::function<bool(int)>>;
FilterContainer filters;

void addDivisorFilter() {
    int divisor = 5;
    // Captures divisor by reference. 'divisor' dies when function returns.
    filters.emplace_back([&](int value) { return value % divisor == 0; });
    // DANGER! Lambda holds dangling reference.
}
```

### Item 32: Use init capture to move objects into closures
**Item Summary**
C++14 introduces "init capture" (generalized lambda capture), allowing you to define new data members in the closure initialized by arbitrary expressions. This solves the problem of moving move-only types (like `unique_ptr`) into a lambda.

**Key Points**
- **C++14 Syntax**: `[pw = std::move(ptr)] { ... }`. Creates a member `pw` in the closure, initialized via move.
- **C++11 Workaround**: Use `std::bind`. `auto func = std::bind([](const unique_ptr<W>& pw) { ... }, std::move(ptr));`. `bind` can move arguments into its state object.
- **Naming**: Scopes are different on LHS vs RHS of `=`. `[x = x]` means closure member `x` initialized by outer scope `x`.

**Example**
```cpp
auto ptr = std::make_unique<Widget>();
// Move unique_ptr into lambda closure
auto func = [p = std::move(ptr)]() { p->doWork(); }; 
// ptr is now null. func owns the widget.
```

### Item 33: Use `decltype` on `auto&&` parameters to `std::forward` them
**Item Summary**
Generic lambdas (C++14) use `auto` parameters. To perfect-forward them, use `decltype` in `std::forward`.

**Key Points**
- **Pattern**: `auto f = [](auto&& param) { return func(std::forward<decltype(param)>(param)); };`
- **Why it works**: `decltype(param)` yields `T&` for lvalues and `T&&` for rvalues. `std::forward<T&&>` is valid and behaves correctly (returning rvalue reference), just like `std::forward<T>`.

**Example**
```cpp
auto f = [](auto&& x) {
    return normalize(std::forward<decltype(x)>(x));
};
```

### Item 34: Prefer lambdas to `std::bind`
**Item Summary**
`std::bind` (from C++11/TR1) is largely obsolete in modern C++ (C++14 and beyond). Lambdas are more readable, expressive, and often more efficient (better inlining).

**Key Points**
- **Readability**: Lambdas show the code to be executed clearly. `bind` requires understanding placeholders (`_1`, etc.) and implicit behavior.
- **Evaluation**: `bind` evaluates arguments immediately unless nested `bind` is used. Lambdas evaluate at call time naturally.
- **Overloading**: `bind` struggles with overloaded functions (requires casting). Lambdas handle them via normal overload resolution inside the body.
- **Exceptions**: `bind` is still useful in C++11 for move capture emulation (Item 32) or polymorphic function objects (template operator()), but C++14 solves these with init capture and generic lambdas.

**Example**
```cpp
// Bind:
auto bound = std::bind(f, _1, std::bind(plus<int>, _1, 10)); // Hard to read

// Lambda
auto lambda = [](int x) { return f(x + 10); }; // Clear
```

## Chapter 7: The Concurrency API

### Item 35: Prefer task-based programming to thread-based
**Item Summary**
Task-based programming (`std::async`) is generally superior to thread-based programming (`std::thread`). `std::async` handles thread management (creation, exhaustion, load balancing) and provides a direct mechanism (`std::future`) to retrieve return values or exceptions.

**Key Points**
- **Return Values**: `std::thread` doesn't provide a direct way to get results; `std::async` returns a future.
- **Exceptions**: Exceptions in `std::async` tasks are propagated to the caller via `future::get()`. Uncaught exceptions in `std::thread` terminate the program (`std::terminate`).
- **Management**: `std::async` delegates thread management to the Standard Library, reducing the risk of oversubscription (too many threads) or exhaustion.

**Example**
```cpp
// Thread-based
std::thread t(doWork);
t.join(); // Must manually join/detach. No return value easy access.

// Task-based
auto future = std::async(doWork); // Easy.
auto result = future.get(); // Get result + exceptions propogated.
```

### Item 36: Specify `std::launch::async` if asynchronicity is essential
**Item Summary**
The default launch policy for `std::async` is `std::launch::async | std::launch::deferred`. This means the runtime *might* run it on a new thread, or it *might* defer execution until `get()` is called (running synchronously on the caller's thread). This unpredictability can break assumptions.

**Key Points**
- **Issues with Default**:
    - Thread-local storage (`thread_local`) access is unpredictable (could be caller's thread or new thread).
    - `wait_for` loops can be infinite: if the task is deferred, `wait_for` returns `future_status::deferred` and never `ready`.
- **Solution**: Explicitly use `std::launch::async` if you need guaranteed concurrency.

**Example**
```cpp
auto f = std::async(doWork); // Default policy: might be deferred!
// if (f.wait_for(0s) == std::future_status::deferred) ... // Stuck?

// Force async
auto f2 = std::async(std::launch::async, doWork);
```

### Item 37: Make `std::threads` unjoinable on all paths
**Item Summary**
A `std::thread` must be either joined or detached before it is destroyed. If a joinable `std::thread`'s destructor is called, the program terminates (calls `std::terminate`). This is to prevent difficult-to-debug bugs (like data races with stack variables).

**Key Points**
- **RAII Wrapper**: Use an RAII class (like `scope_guard` or a custom `ThreadRAII`) to ensure `join()` (or `detach()`) is called on all exit paths (including exceptions).
- **Destructor Choice**: `join()` in destructor can lead to hangs; `detach()` can lead to undefined behavior (using destroyed stack variables). Pick the poison that fits the design (usually `join` is safer).

**Example**
```cpp
void f() {
    std::thread t([]{ ... });
    if (condition) return; 
    // t is destroyed while joinable! std::terminate call!
}

// Fix: Use RAII wrapper that joins on destruction
```

### Item 38: Be aware of varying thread handle destructor behavior
**Item Summary**
Futures (`std::future`) act as handles to asynchronous results. Normally, a future's destructor just destroys the future. **However**, the last future referring to a shared state created by `std::async` with `launch::async` will **block** in its destructor until the task completes (implicit join).

**Key Points**
- **Normal Protocol**: Destructor implies detach (doesn't wait). Used by `std::packaged_task` futures.
- **Async Exception**: `std::async` futures block (wait) to ensure the task finishes. This is a compromise to avoid the program termination rule of `std::thread`.
- **Consequence**: `std::async(std::launch::async, []{...})` will block at the end of the line (temporary future destroyed), effectively making it synchronous! Capture the future to run asynchronously.

**Example**
```cpp
{
    auto f = std::async(std::launch::async, []{}); 
    // f's destructor waits for task to finish
} 
```

### Item 39: Consider void futures for one-shot event communication
**Item Summary**
To notify a task that an event has occurred, simple Condition Variables or Atomic Flags have interaction issues (mutex usage without data, polling costs). `std::promise<void>` and `std::future<void>` provide a clean, race-free mechanism for one-shot notifications.

**Key Points**
- **Mechanism**: Setting the promise (`set_value()`) wakes up the thread waiting on the future (`wait()`).
- **Pros**: No mutex needed for the signal, no spurious wakeups, true blocking (no polling).
- **Cons**: Heap allocation for shared state. One-shot only (cannot be reset).
- **Use Case**: Starting/Suspended threads, signal-to-start.

**Example**
```cpp
std::promise<void> p;
auto future = p.get_future();
std::thread t([&p]{ 
    // ... work ...
    p.set_value(); // Signal completion
});
future.wait(); // true wait (no polling)
t.join();
```

### Item 40: Use `std::atomic` for concurrency, `volatile` for special memory
**Item Summary**
`std::atomic` provides atomic operations and memory ordering guarantees suitable for concurrent programming. `volatile` (in C++) only tells the compiler not to optimize memory access (redundant loads/stores), suitable for memory-mapped I/O, but provides NO thread safety.

**Key Points**
- **`std::atomic`**: Use for thread synchronization. Guarantees sequential consistency (by default) and prevents data races.
- **`volatile`**: Use for interacting with hardware (special memory). Does NOT imply atomicity or memory barriers.
- **Confusion**: Different from Java/C# `volatile`. Do not use `volatile` for concurrency in C++.

**Example**
```cpp
std::atomic<bool> flag(false);
// Thread 1
flag = true; // Atomic store
// Thread 2
if (flag) ... // Guaranteed to see it eventually (sequential consistency)
```

## Chapter 8: Tweaks

### Item 41: Consider pass by value for copyable parameters that are cheap to move and always copied
**Item Summary**
For efficient handling of both lvalues and rvalues, the standard approach is overloading (`const T&` and `T&&`) or using universal references (`T&&`). However, passing by value (PBV) is a simpler alternative: it handles both cases with a single function, at the cost of one extra move.

**Key Points**
- **The Protocol**: Pass by value (`void f(T param)`), then `std::move(param)` into the destination.
- **Cost**:
    - *Lvalue arg*: 1 Copy (construction of param) + 1 Move (into dest).
    - *Rvalue arg*: 1 Move (construction of param) + 1 Move (into dest).
    - *Overhead*: Compared to overloading, PBV costs exactly **one extra move**.
- **Conditions**: Only use if:
    1.  Type is **cheap to move**.
    2.  Parameter is **always copied** (if function returns early, you paid for unused construction/destruction).
    3.  Parameter is **copyable** (move-only types don't need it).
    4.  No **slicing** (base class parameters).
    5.  Copying via **construction**, not assignment (assignment-based copies might forfeit capacity reuse).

**Example**
```cpp
class Widget {
public:
    // Pass By Value
    void setName(std::string n) { name = std::move(n); }
    // Lvalue arg: 1 copy (param), 1 move (member)
    // Rvalue arg: 1 move (param), 1 move (member)
private:
    std::string name;
};
```

### Item 42: Consider emplacement instead of insertion
**Item Summary**
Insertion functions (`push_back`, `insert`) take an object of the container's type. If you pass constructor arguments, a temporary object is created, moved/copied, and destroyed. Emplacement functions (`emplace_back`, `emplace`) perfect-forward arguments to construct the object directly in the container, avoiding temporaries.

**Key Points**
- **Efficiency**: Faster when (1) arguments differ from container type (temporary needed otherwise), (2) adding via construction (not assignment), and (3) no duplicate rejection (associative containers).
- **Explicit Constructors**: Emplacement uses *direct initialization*, allowing explicit constructors (e.g., `vector<regex> v; v.emplace_back(nullptr)` compiles but crashes; `push_back(nullptr)` fails to compile). Be careful.
- **Resource Limits**: `v.emplace_back(new Widget, deleter)` is unsafe if the vector reallocates/throws (pointer leaks). Create `shared_ptr` separately, then pass (`push_back(sp)` or `emplace_back(sp)`).

**Example**
```cpp
std::vector<std::string> v;
// Push back
v.push_back("xyzzy"); 
// 1. "xyzzy" (char*) -> temp std::string
// 2. temp moved into vector. 3. temp destroyed

// Emplace
v.emplace_back("xyzzy");
// 1. "xyzzy" passed to std::string constructor inside vector node.
```









