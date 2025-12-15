# Summary of "More Effective C++" by Scott Meyers

## Basics

### Item 1: Distinguish between pointers and references
**Item Summary**
Pointers and references both facilitate indirect access, but they have distinct semantic differences regarding nullability and reassignability.

**Key Points**
- **Nullability**: References *cannot* be null. If you need to represent "no object", use a pointer.
- **Initialization**: References must be initialized upon creation. Pointers can be uninitialized (though risky).
- **Reassignment**: References cannot be reseated to refer to a different object. Pointers can be reassigned.
- **Usage Guide**:
    - Use **references** when the variable must always refer to an object and will not change target (e.g., `operator[]` return values).
    - Use **pointers** when `nullptr` is a valid state or you need to switch targets.

**Example**
```cpp
string s("Hello");
string& rs = s; // Must be initialized. Always refers to 's'.
string* ps = &s; // Can be initialized later. Can be null.
ps = nullptr;
```

### Item 2: Prefer C++-style casts
**Item Summary**
C-style casts `(Type)expression` are simpler to type but dangerous and hard to spot. C++ introduces four specific cast operators that convey intent and compile-time safety.

**Key Points**
- **`static_cast<T>(expr)`**: Standard type conversions (e.g., `int` to `double`, `void*` to `T*`). Replaces most C-casts. Cannot cast away `const`.
- **`const_cast<T>(expr)`**: The *only* cast that can add or remove `const`/`volatile`.
- **`dynamic_cast<T>(expr)`**: Used for safe downcasting in an inheritance hierarchy (requires virtual functions). Returns `nullptr` (for pointers) or throws (for references) on failure.
- **`reinterpret_cast<T>(expr)`**: Low-level reinterpretation of bits (e.g., `int` to `pointer`). Implementation-dependent and rarely portable.

**Example**
```cpp
// C style
// (int)x;

// C++ style
static_cast<int>(x);
dynamic_cast<Derived*>(basePtr);
const_cast<char*>(constStr);
reinterpret_cast<int*>(ptr);
```

### Item 3: Never treat arrays polymorphically
**Item Summary**
Passing an array of Derived objects to a function expecting an array of Base objects leads to undefined behavior due to pointer arithmetic errors.

**Key Points**
- **The specific mechanics**: Arrays rely on `sizeof(Element)` to calculate offsets (`array[i]` is `*(array + i)`).
- **The mismatch**: `sizeof(Derived)` is typically larger than `sizeof(Base)`. If you pass `Derived[]` as `Base*`, the compiler uses `sizeof(Base)` for arithmetic, landing in the middle of objects or totally wrong memory locations.
- **Deletion**: `delete [] basePtr` where `basePtr` points to a `Derived` array also fails (wrong destructor calls, wrong memory size calculation).

**Example**
```cpp
class Base { int x; };
class Derived : public Base { int y; };
void printArray(Base array[], int numElements) {
    for (int i=0; i<numElements; ++i) {
        // array[i] calculates address as: start + i * sizeof(Base).
        // Actual objects are sizeof(Derived). Stride is wrong!
        cout << array[i]; 
    }
}
Derived d[10];
printArray(d, 10); // Undefined behavior (Disaster)
```

### Item 4: Avoid gratuitous default constructors
**Item Summary**
Providing a default constructor when an object requires external data to be valid (e.g., an ID number) forces the object into an undefined "empty" state, complicating usage and maintenance.

**Key Points**
- **The Cost**: Without a default constructor, you cannot verify validity at compile time. With one, every member function must check "am I valid?".
- **Array Difficulty**: Arrays of objects without default constructors are hard to initialize.
    - *Workaround*: Use an array of pointers `T* array[10]` and `new` each element.
    - *Workaround*: Use `std::vector` (though some older implementations required default constructors, modern ones don't if you provide init values).
    - *Workaround*: Allocate raw memory (`operator new[]`) and use placement `new`.
- **Recommendation**: If an object needs data to exist, require it in the constructor. Don't add a default constructor just for convenience.

**Example**
```cpp
class EquipmentPiece {
public:
    EquipmentPiece(int IDNumber) : id(IDNumber) {}
    // No default constructor
private:
    int id;
};

// EquipmentPiece bestPieces[10]; // Error! No default ctor.
EquipmentPiece* bestPieces[10]; // OK, array of pointers uninitialized.
```

## Operators

### Item 5: Be wary of user-defined conversion functions
**Item Summary**
Implicit type conversions (via `operator T()` or single-argument constructors) often lead to ambiguous code and unexpected function calls.

**Key Points**
- **Implicit Conversion Operators**: Functions like `operator double()` allow the compiler to silently convert your object to a `double` in mixed arithmetic or printing, often when you didn't intend it.
    - *Solution*: Replace with explicit named functions (e.g., `asDouble()`).
- **Single-Argument Constructors**: Can turn an `int` into an `Array<int>` silently.
    - *Solution*: Use the `explicit` keyword on constructors.
    - *Workaround*: If `explicit` isn't available, use "proxy classes" (e.g., `Array` takes `ArraySize` object, `ArraySize` takes `int`).

**Example**
```cpp
class Rational {
public:
    Rational(int n, int d = 1);
    operator double() const; // Implicit conversion
};
Rational r(1, 2);
// cout << r; 
// Might match 'operator<<(double)' via implicit conversion!
```

### Item 6: Distinguish between prefix and postfix forms of increment and decrement operators
**Item Summary**
Prefix (`++i`) and postfix (`i++`) have different signatures and semantics. Postfix is generally less efficient.

**Key Points**
- **Signatures**:
    - Prefix: `UPInt& operator++();` (Returns reference to *new* value).
    - Postfix: `const UPInt operator++(int);` (Returns generic const object with *old* value). Takes a dummy `int` parameter.
- **Efficiency**: Postfix must create and return a temporary object holding the old value. Sentinel loops should preserve efficiency by using prefix.
- **Implementation**: Implement postfix *in terms of* prefix to ensure consistent behavior.
    ```cpp
    const UPInt operator++(int) {
        UPInt oldValue = *this;
        ++(*this); // call prefix
        return oldValue;
    }
    ```
- **Const Return**: Postfix returns `const` to prevent nonsensical chains like `i++++`.

**Example**
```cpp
// Prefix
UPInt& operator++() { *this += 1; return *this; }

// Postfix
const UPInt operator++(int) { 
    UPInt oldValue = *this; 
    ++(*this); // Call prefix
    return oldValue; 
}
```

### Item 7: Never overload `&&`, `||`, or `,`
**Item Summary**
Built-in logical operators guarantee **short-circuit evaluation** and definition of evaluation order. Overloaded versions are just function calls, which evaluate *all* arguments in unspecified order.

**Key Points**
- **The specific mechanics**: `if (a && b)` stops if `a` is false. `operator&&(a, b)` evaluates both.
- **Comma Operator**: Built-in `,` evaluates left, discard, then right. Overloaded `,` is a function call with unspecified order.
- **Rule**: Do not overload these. You cannot replicate the built-in semantics.

**Example**
```cpp
if (expression1 && expression2) ...
// Standard: if expression1 is false, expression2 is NOT executed.

// Overloaded: operator&&(expression1, expression2)
// Function call semantics: Evaluates BOTH arguments!
```

### Item 8: Understand the different meanings of `new` and `delete`
**Item Summary**
Clarifies the distinction between the language keywords and the memory allocation functions.

**Key Points**
- **`new` operator**: The built-in keyword. Does two things: (1) Allocates memory (calls `operator new`), (2) Calls constructor. You cannot overload this behavior.
- **`operator new`**: Function that just allocates raw memory. You *can* overload this.
    - `void* operator new(size_t size);`
- **Placement `new`**: A standard version of `operator new` that takes an extra pointer argument and does nothing but return it. Used to construct objects in explicit memory locations: `new (buffer) Widget(size);`.
- **`delete` operator**: Calls destructor, then deallocates memory (calls `operator delete`).
- **`operator delete`**: Function that frees raw memory.

**Example**
```cpp
// new operator (Expression)
Widget* w = new Widget(); 
// 1. Calls operator new(sizeof(Widget)) to alloc memory
// 2. Calls Widget() ctor

// Placement new
void* buffer = malloc(sizeof(Widget));
Widget* w2 = new (buffer) Widget(); // Constructs at buffer
```

## Exceptions

### Item 9: Use destructors to prevent resource leaks
**Item Summary**
Exceptions can cause control to bypass explicit `delete` statements. To prevent leaks, resources should be managed by objects (RAII) so that destructors automatically release them during stack unwinding.

**Key Points**
- **Problem**: `func() { P* p = new P; ... throw; ... delete p; }` leaks `p`.
- **Solution**: Use `std::auto_ptr` (deprecated in modern C++ in favor of `std::unique_ptr`) or other smart pointers to wrap resources.
- **RAII**: Resource Acquisition Is Initialization. Acquire resource in constructor, release in destructor.

**Example**
```cpp
void f() {
    Window* w = new Window();
    // ...
    if (error) throw exception(); // w is leaked!
    delete w;
}

// Fix
void f() {
    // Destructor called automatically on stack unwind
    std::unique_ptr<Window> w(new Window()); 
    // ...
}
```

### Item 10: Prevent resource leaks in constructors
**Item Summary**
C++ only destroys *fully constructed* objects. If a constructor throws an exception, the object's destructor is *not* called, potentially leaking resources allocated earlier in the constructor.

**Key Points**
- **The Issue**: `Class::Class() { ptr1 = new T; ptr2 = new T; }`. If `ptr2` throws, `ptr1` is leaked because `~Class()` is never called.
- **Fix 1**: Use `try...catch` inside the constructor to clean up (`delete ptr1`) before rethrowing.
- **Fix 2 (Better)**: Use smart pointers (`auto_ptr` members). They are fully constructed objects themselves, so their destructors *will* be called if the outer constructor throws.

**Example**
```cpp
class BookEntry {
public:
    BookEntry() {
        theImage = new Image();
        theAudioClip = new AudioClip(); // If this throws exception...
    }
    ~BookEntry() {
        // ... this destructor is NOT called! 'theImage' is leaked.
        delete theImage; 
        delete theAudioClip;
    }
private:
    Image *theImage;
    AudioClip *theAudioClip;
};
```

### Item 11: Prevent exceptions from leaving destructors
**Item Summary**
Destructors are called during exception propagation (stack unwinding). If a destructor throws another exception, the program terminates immediately (`std::terminate`).

**Key Points**
- **Scenario**: Exception A is thrown -> stack unwinds -> Object B's destructor is called -> Destructor throws Exception B -> Crash.
- **Practice**: All destructors should handle exceptions internally (swallow them or log them). Never let an exception escape a destructor.

### Item 12: Understand how throwing an exception differs from passing a parameter or calling a virtual function
**Item Summary**
Although syntactically similar to function calls, passing exceptions involves mandatory copying (by static type) and uses different type-matching rules.

**Key Points**
- **Copying**: Thrown objects are *always* copied, even if caught by reference (the handler gets a reference to the temporary copy). Consider the cost of copy construction.
- **Slicing**: If you throw `Derived` but catch `Base` (by value), the object is sliced. Catching by reference avoids this.
- **Type Matching**: No implicit conversions (like `int` to `double`) are performed to match handlers.
- **Order of Evaluation**: `catch` blocks are matched in order of appearance ("First Fit"), whereas virtual functions pick the "Best Fit". Put more specific handlers first.

### Item 13: Catch exceptions by reference
**Item Summary**
Catching by pointer is risky (ownership issues); catching by value slices objects and incurs copying costs. Catching by reference is the correct default.

**Key Points**
- **By Pointer**: Who is responsible for deleting the pointer? What if it points to a local stack object? Too many pitfalls.
- **By Value**: Causes object slicing (loss of polymorphism) and duplicate copying.
- **By Reference**: Efficient (no extra copy), safe (no slicing, polymorphism works), and works with standard exceptions (`bad_alloc`, etc.).

### Item 14: Use exception specifications judiciously
**Item Summary**
(Note: Dynamic exception specifications e.g., `throw(int)`, were deprecated in C++11 and removed in C++17, except for `throw()`/`noexcept`).
Original advice: They document code but if violated, the program calls `unexpected()` and often terminates. They are hard to maintain, especially with templates.

**Key Points**
- **Violation**: If a function throws an exception not in its list, `std::unexpected` is called (default triggers `terminate`).
- **Templates**: Impossible to know what a template parameter operation might throw, making specifications on templates impractical.
- **Advice**: Avoid them unless you are certain (e.g., `noexcept` for move operations in modern C++).

### Item 15: Understand the costs of exception handling
**Item Summary**
Enabling exceptions incurs a cost in compilation size and runtime overhead (for stack unwinding tables), even if no exception is thrown. Actual throwing is expensive.

**Key Points**
- **Static Cost**: Code size increases (tables for tracking destructors).
- **Dynamic Cost**: Try blocks and exception specifications add overhead. Throwing an exception is orders of magnitude slower than a normal return.
- **Rule**: Use exceptions for *exceptional* conditions, not for normal control flow (like exiting a loop).

## Efficiency

### Item 16: Remember the 80-20 rule
**Item Summary**
80% of a program's resources are consumed by 20% of the code. Optimizing the other 80% is a waste of time.

**Key Points**
- **Don't Guess**: Programmers are notoriously bad at guessing performance bottlenecks.
- **Measure**: Use a profiler to identify the critical 20% of code that actually impacts performance.
- **Focus**: Direct optimization efforts only to the "hot" paths identified by profiling.

### Item 17: Consider using lazy evaluation
**Item Summary**
Defer computation until the result is actually needed. This avoids the cost of computations that turn out to be unnecessary.

**Key Points**
- **Reference Counting**: Share data between objects (e.g., `String` copies) until one tries to write, then copy.
- **Lazy Fetching**: Don't load all database fields for an object until they are requested. Use `mutable` for lazy members accessed in `const` functions.
- **Expression Evaluation**: `Matrix m3 = m1 + m2` can store a "promise" to add. If `m3` is never read, the addition never happens.

### Item 18: Amortize the cost of expected computations
**Item Summary**
"Over-eager evaluation." If a computation is expected to be done often, do it in advance or cache it to lower the average cost per call.

**Key Points**
- **Caching**: Store results of expensive lookups (e.g., in a map) so subsequent requests are instant.
- **Prefetching**: Read more data than requested (e.g., disk blocks, instruction cache) assuming locality of reference.
- **examples**: `std::vector` or dynamic arrays allocating extra capacity to avoid frequent reallocations.

### Item 19: Understand the origin of temporary objects
**Item Summary**
Temporary objects are invisible but expensive. They arise from implicit type conversions and function return values.

**Key Points**
- **Implicit Conversions**: Passing a `char[]` to a function taking `const string&` creates a temporary `string`.
    - *Fix*: Overload function for `char*` or avoid conversion.
- **Return Values**: Functions returning objects by value create temporaries.
- **Cost**: Constructor and destructor calls for every temporary.

### Item 20: Facilitate the return value optimization
**Item Summary**
To minimize the cost of returning objects by value, construct the return object essentially in place, allowing the compiler to elide the temporary (RVO).

**Key Points**
- **Bad ideas**: Returning pointers (leak risk) or references (dangling reference risk) to avoid copying.
- **RVO (Return Value Optimization)**: Write functions to return constructor calls: `return MyObject(args);`.
- **Compiler**: Compilers are allowed to optimize this by constructing the object directly in the memory of the caller's variable.

### Item 21: Overload to avoid implicit type conversions
**Item Summary**
 Implicit type conversions create expensive temporary objects. Overloading functions for common argument types eliminates the need for these conversions.

**Key Points**
- **Scenario**: `void func(const string&);` called with `"text"` creates a temporary `string`.
- **Fix**: Overload `void func(const char*);` to handle the raw type directly without conversion.
- **Arithmetic**: Instead of relying on `operator+(const Msg&, const Msg&)` and implicit conversion for `msg + 5`, provide `operator+(const Msg&, int)` and `operator+(int, const Msg&)`.

### Item 22: Consider using `operator op=` instead of stand-alone `operator op`
**Item Summary**
Assignment operators (`+=`, `-=`) are inherently more efficient than stand-alone operators (`+`, `-`) because they modify the object in place and avoid creating a temporary return value.

**Key Points**
- **Implementation**: Implement `operator+` in terms of `operator+=`.
    ```cpp
    const T operator+(const T& lhs, const T& rhs) {
        return T(lhs) += rhs; // Create copy, modify, return
    }
    ```
- **Trade-off**: By providing both, you let users choose between convenience (`a = b + c + d`) and efficiency (`a += b; a += c;`).

### Item 23: Consider alternative libraries
**Item Summary**
Different libraries offering similar functionality (e.g., `stdio` vs `iostream`) have different performance characteristics. Choose the one that suits your bottlenecks.

**Key Points**
- **Example**: `stdio` (`printf`) is often faster and produces smaller executables than `iostream` (though less type-safe and extensible).
- **Strategy**: If profiling identifies a library as a bottleneck, consider a faster alternative, even if it's less "C++-ish" or convenient.

### Item 24: Understand the costs of virtual functions, multiple inheritance, virtual base classes, and RTTI
**Item Summary**
These features add hidden costs in terms of object size (hidden pointers) and execution time (indirection, inhibition of optimizations).

**Key Points**
- **Virtual Functions**:
    - *Space*: Each class has a `vtbl` (static). Each object has a `vptr` (dynamic, increases instance size).
    - *Time*: Call overhead is small, but they **prevent inlining**, which is a significant optimization loss.
- **Multiple Inheritance**: Adds execution overhead (this-pointer offsets) and space overhead (multiple `vptr`s).
- **Virtual Base Classes**: Adds pointers to base class parts, increasing object size.
- **RTTI**: Stores `type_info` (usually accessible via `vtbl`). Minimal cost unless used frequentely.

## Techniques

### Item 25: Virtualizing constructors and non-member functions
**Item Summary**
"Virtual constructors" create objects whose type depends on runtime information (like input data). "Virtual non-member functions" allow non-member operators to behave polymorphically.

**Key Points**
- **Virtual Constructor**: Use a `static` method (e.g., `readComponent(istream&)`) that reads input and decides which derived class to instantiate (`new TextBlock` vs `new Graphic`).
- **Virtual Copy Constructor**: A `virtual clone()` function returns a copy of the object (`return new Derived(*this);`).
- **Virtual Non-Member**: To make `operator<<` polymorphic, define it as a non-member inline function that calls a virtual `print()` member function.

### Item 26: Limiting the number of objects of a class
**Item Summary**
Techniques to restrict class instantiation to zero, one, or a fixed number of objects.

**Key Points**
- **Zero Instances**: Declare constructors private.
- **Singleton (One Instance)**: Encapsulate a static object inside a function (`thePrinter()`) or class. Better than a global static variable due to initialization order guarantees.
- **N Instances**: Use a static counter (`numObjects`). Throw an exception in the constructor if the limit is exceeded.
- **Counting Base Class**: A `Counted<T>` template using the CRTP (Curiously Recurring Template Pattern) can automate instance counting.

### Item 27: Requiring or prohibiting heap-based objects
**Item Summary**
Enforcing where an object resides (heap vs. stack) involves manipulating access to constructors, destructors, and memory allocation operators.

**Key Points**
- **Requiring Heap**: Make the **destructor** private. Stack objects cannot be destroyed (compiler error), forcing usage of `new` and a specialized `destroy()` method.
    - *Tracking*: To know if a pointer is safe to `delete`, use a `HeapTracked` mixin that records addresses returned by `operator new`.
- **Prohibiting Heap**: Declare `operator new` and `operator delete` as **private**. This prevents `new MyClass`.

### Item 28: Smart pointers
**Item Summary**
Smart pointers are objects designed to act like pointers (using `->` and `*`) but with added features like potential resource management or automatic cleanup.

**Key Points**
- **Construction**: Usually initialize with a raw pointer (default `0`).
- **Destruction**: Often `delete` the pointed-to object.
- **Copying**:
    - **Deep Copy**: Safe but expensive.
    - **Transfer Ownership** (`auto_ptr`): Source becomes null. Pass-by-value is dangerous.
    - **Reference Counting**: Best for sharing values (see Item 29).
- **Dereferencing**: `operator->` returns the raw pointer. `operator*` returns a reference to the object.
- **Null Checks**: Providing `operator void*` (or `bool`) allows `if (ptr)` but permits unsafe mixed-type comparisons. `operator!` is safer.
- **Inheritance**: `SmartPtr<Derived>` is *not* a subclass of `SmartPtr<Base>`. Use member function templates to allow conversion.

### Item 29: Reference counting
**Item Summary**
Reference counting (RC) allows multiple objects to share a single value representation, saving memory and speeding up copying. It also simplifies memory management by tracking ownership.

**Key Points**
- **Structure**: The main class (e.g., `String`) holds a pointer to a shared value class (`StringValue`), which contains the data and a `refCount`.
- **Copy-on-Write (COW)**: When modifying a shared value (e.g., via non-const `operator[]`), check if `refCount > 1`. If so, create a unique copy before modifying.
- **Base Class**: An `RCObject` base class can encapsulate `addReference` and `removeReference` logic.
- **Smart Pointer**: An `RCPtr<T>` template can automate calling reference counting methods.
- **Existing Classes**: To add RC to a class you can't modify (like `Widget`), use a `CountHolder` (inherits `RCObject`, holds `Widget*`) and an `RCIPtr` (indirect smart pointer).

### Item 30: Proxy classes
**Item Summary**
Proxy classes are objects that stand in for other objects. They are useful for implementing multidimensional arrays, distinguishing read/write operations, and suppressing implicit conversions.

**Key Points**
- **2D Arrays**: Implement `operator[]` in `Array2D` to return an `Array1D` proxy, which in turn implements `operator[]` to return the element. Allows `data[x][y]`.
- **Read vs Write**: `operator[]` cannot distinguish lvalue (write) from rvalue (read). Return a `CharProxy` instead of `char&`.
    - `CharProxy::operator=(char)` handles the write (performing COW if needed).
    - `CharProxy::operator char()` handles the read.
- **Limitations**: Proxies cause issues with `operator&` (taking address), binding to non-const references, and implicit type conversions.

### Item 31: Making functions virtual with respect to more than one object
**Item Summary**
"Double dispatch" is needed when an operation (like `collide`) depends on the types of *two* objects (e.g., `SpaceShip` vs `Asteroid`). C++ virtual functions only dispatch on the first object (`this`).

**Key Points**
- **Virtual Function Calls**: `obj1.collide(obj2)`. Inside `SpaceShip::collide`, call `obj2.collide(*this)`. This resolves to `obj2.collide(SpaceShip&)`. Requires modifying the base `GameObject` for every new subclass (recompilation bottleneck).
- **RTTI**: Use `typeid` checks. Hard to maintain.
- **Emulated Virtual Tables**: Use a lookup map: `map< pair<type1, type2>, HitFunctionPtr >`.
    - Functions like `shipAsteroid(GameObject&, GameObject&)` use `dynamic_cast` internally.
    - Can allow dynamic registration of collision functions at runtime.

### Item 32: Program in the future tense
**Item Summary**
Design code to accommodate future change. Accept that classes will be derived from, functions will be overloaded, and libraries will evolve. Avoid "demand-paged" design where features (like virtual destructors) are only added when immediately needed.

**Key Points**
- **Enforce Constraints in Code**: Use C++ features (`private`, `const`, `explicit`) to enforce design decisions rather than just comments.
- **Virtual Functions**: Declare functions virtual if they represent a concept that *could* be specialized, even if no one specializes it yet.
- **Copying**: Explicitly handle (or prohibit via `private`) copy constructors and assignment operators to prevent incorrect compiler-generated ones later.
- **No Present-Tense Assumptions**: e.g., "This class isn't used as a base class *now*, so I won't make the destructor virtual." This breaks future code that *does* try to derive from it.
- **Generalize**: If a function can easily be made generic (templated), do so to increase reusability.

### Item 33: Make non-leaf classes abstract
**Item Summary**
Concrete base classes often lead to "partial assignment" problems where assigning a derived object to a base object (via pointers/references) slices off the derived parts.

**Key Points**
- **Partial Assignment**: `Base *pb = &derived; *pb = *otherDerived;`. If `Base` is concrete, `Base::operator=` is called, enabling partial assignment.
- **Virtual Assignment**: Making `operator=` virtual allows mixed-type assignments (e.g., assigning a `Chicken` to a `Lizard`), which is often undesirable and requires runtime type checks (`dynamic_cast`).
- **Solution**: Make the base class abstract (e.g., `AbstractAnimal`).
    - Concrete classes (`Lizard`, `Chicken`) inherit from `AbstractAnimal`.
    - You cannot instantiate `AbstractAnimal`, so you cannot assign to it directly.
    - Prevents partial assignment and forces separation of interface (abstract base) from implementation (concrete derived).

### Item 34: Understand how to combine C++ and C in the same program
**Item Summary**
Mixing C and C++ requires handling name mangling, static initialization, memory allocation, and data structure layout compatibility.

**Key Points**
- **Name Mangling**: C++ compilers mangle function names (to support overloading). C compilers do not. Use `extern "C"` to suppress mangling for C functions.
- **Initialization**: C++ performs static initialization (before `main`) and destruction (after `main`). If `main` is in C, these might not run. Ideally, write `main` in C++.
- **Memory**: Never mix `new`/`delete` with `malloc`/`free`. Be careful with functions like `strdup` (usually uses `malloc`).
- **Data Compatibility**: Structs with identical layouts can be passed between C and C++. Adding virtual functions (vptr) or inheritance usually breaks binary compatibility with C structs.

### Item 35: Familiarize yourself with the language standard
**Item Summary**
The C++ standard (ISO/ANSI) introduced significant features and the Standard Template Library (STL) which were not present in older references like the ARM.

**Key Points**
- **New Features**: RTTI, namespaces, `bool`, `mutable`, `explicit`, member templates, new cast operators (`static_cast`, `dynamic_cast`, etc.).
- **Standard Library**:
    - **STL**: Containers (`vector`, `list`, `map`, etc.), Iterators, and Algorithms (`find`, `sort`, `copy`, etc.).
    - **Strings**: `std::string` is a typedef for `basic_string<char>`.
    - **Namespaces**: Almost everything is in `namespace std`.
    - **Headers**: use `<iostream>` instead of `<iostream.h>`, etc.







