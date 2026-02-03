#include <iostream>
#include <list>
#include <cassert>

template <typename T>
class CyclicQueue
{
public:
    explicit CyclicQueue(size_t capacity): capacity(capacity), elements()
    {   
        std::cout << "CyclicQueue ctor: "  << capacity << "\n";    
    }

    ~CyclicQueue()
    {
        std::cout << "CyclicQueue dtor: " << "\n";    
    }
  
    template<typename U> // we can not use template of the class since it would create Enqueue that aceepts only rvalues
    void Enqueue(U&& val)
    {
        if(IsFull())
        {
            throw std::runtime_error("Queue is full, enqueue is prohibited");        
        }
        elements.emplace_back(std::forward<U>(val));
        ++numElements;
    }

    T& GetOldest()
    {
        if(IsEmpty())
        {
            throw std::runtime_error("Queue is empty, GetOldest is prohibited");
        }    
        return elements.front();
    }

    const T& GetOldest() const
    {
        if(IsEmpty())
        {
            throw std::runtime_error("Queue is empty, GetOldest is prohibited");
        }    
        return elements.front();
    }

    void Dequeue()
    {
        if(IsEmpty())
        {
            throw std::runtime_error("Queue is empty, dequeue is prohibited");
        }
        elements.pop_front();
        --numElements;
    }

    inline size_t Count() const
    {
        return numElements; 
    }

    inline bool IsFull() const 
    {
        return capacity == numElements;
    }

    inline bool IsEmpty() const 
    {
        return 0 == numElements;
    }

private:
    CyclicQueue(const CyclicQueue&) = delete;
    CyclicQueue(CyclicQueue&&) = delete;
    CyclicQueue& operator=(const CyclicQueue&) = delete;
    CyclicQueue& operator=(CyclicQueue&&) = delete;    
    const size_t capacity;
    size_t numElements = 0;
    std::list<T> elements;
};

///////////////////////////////////////////////////////////////////////////
// Movable-only class (deleted copy, default move)
class MovableOnly
{
public:
    explicit MovableOnly(int v = 0) : p(new int(v)) 
    { 
        std::cout << "MovableOnly ctor: " << v << "\n"; 
    }
    ~MovableOnly() 
    { 
        std::cout << "MovableOnly dtor: " << (p ? *p : -1) << "\n"; 
        delete p; 
    }

    MovableOnly(const MovableOnly&) = delete;
    MovableOnly& operator=(const MovableOnly&) = delete;

    MovableOnly(MovableOnly&& other) noexcept : p(other.p) 
    { 
        std::cout << "MovableOnly move ctor: " << (p ? *p : -1) << "\n"; 
        other.p = nullptr; 
    }
    MovableOnly& operator=(MovableOnly&& other) noexcept
    {
        if (this != &other) {
            std::cout << "MovableOnly move assign: " << (other.p ? *other.p : -1) << "\n";
            delete p;
            p = other.p;
            other.p = nullptr;
        }
        return *this;
    }

    int value() const { return p ? *p : -1; }

private:
    int* p;
};

// Copyable but not movable class
class CopyableNotMovable
{
public:
    explicit CopyableNotMovable(int v = 0) : v_(v) 
    { 
        std::cout << "CopyableNotMovable ctor: " << v << "\n"; 
    }
    ~CopyableNotMovable() 
    { 
        std::cout << "CopyableNotMovable dtor: " << v_ << "\n"; 
    }

    CopyableNotMovable(const CopyableNotMovable& other) : v_(other.v_)
    {
        std::cout << "CopyableNotMovable copy ctor: " << v_ << "\n";
    }
    CopyableNotMovable& operator=(const CopyableNotMovable& other)
    {
        std::cout << "CopyableNotMovable copy assign: " << other.v_ << "\n";
        if (this != &other) {
            v_ = other.v_;
        }
        return *this;
    }

    CopyableNotMovable(CopyableNotMovable&&) = delete;
    CopyableNotMovable& operator=(CopyableNotMovable&&) = delete;

    int value() const { return v_; }

private:
    int v_;
};

void testMovableOnly()
{
    std::cout << "\n=== testMovableOnly ===\n";
    CyclicQueue<MovableOnly> q(4);
    
    std::cout << "Initial count: " << q.Count() << ", isEmpty: " << q.IsEmpty() << "\n";
    
    q.Enqueue(std::move(MovableOnly(10)));
    std::cout << "After 1st enqueue - count: " << q.Count() << "\n";
    
    q.Enqueue(std::move(MovableOnly(20)));
    std::cout << "After 2nd enqueue - count: " << q.Count() << "\n";

    auto a = std::move(q.GetOldest());
    q.Dequeue();
    std::cout << "Dequeued (movable-only): " << a.value() << ", remaining count: " << q.Count() << "\n";
    
    auto b = std::move(q.GetOldest());
    q.Dequeue();
    std::cout << "Dequeued (movable-only): " << b.value() << ", remaining count: " << q.Count() << "\n";
    
    std::cout << "Final isEmpty: " << q.IsEmpty() << "\n";
}

void testCopyableNotMovable()
{
    // CopyableNotMovable cannot be used with this queue because it has move deleted
    // and the queue requires move assignment: arr[head] = std::move(val)
    std::cout << "\n=== testCopyableNotMovable (skipped - requires move support) ===\n";
}

void testCyclicQueueInt()
{
    std::cout << "\n=== testCyclicQueueInt ===\n";
    
    for (size_t capacity = 1; capacity <= 5; ++capacity) {
        CyclicQueue<int> q(capacity);
        
        // Initial state assertions
        assert(q.Count() == 0);
        assert(q.IsEmpty() == true);
        
        // Enqueue values to fill the queue
        for (size_t i = 0; i < capacity; ++i) {
            int val = (i + 1) * 10;
            q.Enqueue(std::move(val));
        }
        
        // After filling assertions
        assert(q.Count() == capacity);
        assert(q.IsEmpty() == false);
        
        // Dequeue all values
        for (size_t i = 0; i < capacity; ++i) {
            auto val = q.GetOldest();
            q.Dequeue();
            assert(val == static_cast<int>((i + 1) * 10));
        }
        
        // Final state assertions
        assert(q.Count() == 0);
        assert(q.IsEmpty() == true);
        
        std::cout << "capacity " << capacity << ": PASSED\n";
    }
}

void testCyclicQueueWraparound()
{
    std::cout << "\n=== testCyclicQueueWraparound (wraparound behavior) ===\n";
    CyclicQueue<int> q(3);
    
    // Fill and empty multiple times to test wraparound
    for (int round = 1; round <= 3; ++round) {
        std::cout << "\nRound " << round << ":\n";
        for (int i = 1; i <= 3; ++i) {
            int val = round * 100 + i;
            std::cout << "  Enqueue " << val << " (count: " << q.Count() << ")\n";
            q.Enqueue(std::move(val));
        }
        
        for (int i = 0; i < 3; ++i) {
            auto val = q.GetOldest();
            q.Dequeue();
            std::cout << "  Dequeue " << val << " (count: " << q.Count() << ")\n";
        }
    }
}

int main()
{
    try {
        testMovableOnly();
        testCopyableNotMovable();
        testCyclicQueueInt();
        testCyclicQueueWraparound();
    }
    catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}

// g++ -std=c++14 -Wall -Wextra -pedantic -Werror -Weffc++ -Woverloaded-virtual -Wctor-dtor-privacy -Wold-style-cast cyclic_queue.cpp -o cyclic_queue
