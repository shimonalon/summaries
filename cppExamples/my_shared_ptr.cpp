#include <iostream>
#include <cassert>

// Implement SharedPtr here:
template<typename T>
class SharedPtr 
{
public:    
    explicit SharedPtr(T* p)
    {
        rawPointer = p;        
        counter = new size_t(1);
    }

    SharedPtr(SharedPtr&& other) // move ctor
    {
        rawPointer = other.rawPointer;
        other.rawPointer = nullptr;
        counter = other.counter;
        other.counter = nullptr;
    }

    SharedPtr(const SharedPtr& other) // copy ctor
    {
        rawPointer = other.rawPointer;
        counter = other.counter;
        ++(*counter);
    }
    
    SharedPtr& operator=(SharedPtr&& other) // move assignment operator
    {
        if(this != &other)
        {
            --(*(this->counter));
            if(*(this->counter) == 0)
            {
                delete this->rawPointer;
                delete this->counter;
            }            
            rawPointer = other.rawPointer;
            counter = other.counter;
            other.rawPointer = nullptr;
            other.counter = nullptr;            
        }
        return *this;
    }

    SharedPtr& operator=(const SharedPtr& other) // copy assignment operator
    {
        if(this != &other)
        {
            --(*(this->counter));
            if(*(this->counter) == 0)
            {
                delete this->rawPointer;
                delete this->counter;
            }

            rawPointer = other.rawPointer;
            counter = other.counter;
            ++(*counter);                        

        }
        return *this;
    }

    T* operator->() // dereference operator
    {
        return rawPointer;
    }

    const T& operator*()
    {
        return *rawPointer;
    }

    ~SharedPtr()
    {   
        if( counter and *counter > 0)
        {
            --(*counter);
            if(*counter == 0)
            {
                delete rawPointer;
                rawPointer = nullptr;
                delete counter;
                counter = nullptr;
            }
        }
    }


    size_t use_count() const
    {
        return counter ? *counter : 0;
    } 

private:
    size_t* counter;
    T* rawPointer;
};


struct TestObj {
    int value;
    TestObj(int v) : value(v) { std::cout << "TestObj(" << value << ") created\n"; }
    ~TestObj() { std::cout << "TestObj(" << value << ") destroyed\n"; }
};

int main() {
    std::cout << "--- Test 1: Construction ---\n";
    {
        SharedPtr<TestObj> sp1(new TestObj(10));
        assert(sp1.use_count() == 1);
        assert(sp1->value == 10);
    } // Should destroy 10 here

    std::cout << "\n--- Test 2: Copying ---\n";
    {
        SharedPtr<TestObj> sp1(new TestObj(20));
        {
            SharedPtr<TestObj> sp2 = sp1; // Copy construct
            assert(sp1.use_count() == 2);
            assert(sp2.use_count() == 2);
            assert(sp2->value == 20);
        } // sp2 dies, count -> 1
        assert(sp1.use_count() == 1);
    } // sp1 dies, count -> 0, destroy 20

    std::cout << "\n--- Test 3: Assignment ---\n";
    {
        SharedPtr<TestObj> sp1(new TestObj(30));
        SharedPtr<TestObj> sp2(new TestObj(40));
        
        sp2 = sp1; // sp2 drops 40 (destroy), takes 30
        
        assert(sp1.use_count() == 2);
        assert(sp2.use_count() == 2);
        assert((*sp2).value == 30);
    }

    std::cout << "\n--- Test 4: Move Construction ---\n";
    {
        SharedPtr<TestObj> sp1(new TestObj(50));
        SharedPtr<TestObj> sp2 = std::move(sp1);
        
        // After move, use_count should still be 1 (sp2 took over)
        // sp1 should ideally be empty/null (depending on your implementation)
        assert(sp2.use_count() == 1);
        assert(sp2->value == 50);
    }

    std::cout << "\n--- Test 5: Move Assignment ---\n";
    {
        SharedPtr<TestObj> sp1(new TestObj(60));
        SharedPtr<TestObj> sp2(new TestObj(70));
        
        sp2 = std::move(sp1);
        
        assert(sp2.use_count() == 1);
        assert(sp2->value == 60);
    }

    std::cout << "\nAll Tests Passed (if implemented correctly)!\n";
    return 0;
}
