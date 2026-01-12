#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
using namespace std;
/**
 * Task: Given a stream of integers, calculate the median at any time.
 * Requirements: 
 *   - addNum(int num): O(log N)
 *   - findMedian(): O(1)
 */
class MedianFinder {
    priority_queue<int> heapMax; // holds the smallers
    priority_queue<int ,vector<int> , greater<int> > heapMin; // hold the largers
public:
    void addNum(int num) 
    {
        if(!heapMax.empty() and heapMax.top() >= num)
        {
            heapMax.push(num);            
        }
        else
        {
            heapMin.push(num);
        }

        if(heapMax.size() > heapMin.size())
        {
            if(heapMax.size() - heapMin.size() > 1)
            {
                int toMove = heapMax.top();
                heapMax.pop();
                heapMin.push(toMove);
            }
        }
        else if(heapMax.size() < heapMin.size())
        {
            if(heapMin.size() - heapMax.size() > 1)
            {
                int toMove = heapMin.top();
                heapMin.pop();
                heapMax.push(toMove);
            }
        }
    }
    
    double findMedian() 
    {
        if(heapMax.size() > heapMin.size())
        {
            return heapMax.top();
        }
        else if(heapMax.size() < heapMin.size())
        {
            return heapMin.top();
        }

        return (heapMax.top() +  heapMin.top()) / 2.0;
    }
};

void test_median() {
    MedianFinder mf;
    
    mf.addNum(1);
    mf.addNum(2);
    assert(mf.findMedian() == 1.5);
    
    mf.addNum(3);
    assert(mf.findMedian() == 2.0);
    
    std::cout << "MedianFinder basic tests passed!\n";
}

int main() {
    test_median();
    return 0;
}
