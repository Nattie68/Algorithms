#include <vector>


template<typename T>
class SegmentTree {
public:
     SegmentTree(const std::vector<T>& vec, 
                T (*combiner)(T, T))
        : combine(combiner)
    {
        n = 1;
        while (n < vec.size()) {
            n *= 2;
        }
        segment_tree.resize(2 * n - 1, T());
        
        for (size_t i = 0; i < vec.size(); ++i) {
            segment_tree[n - 1 + i] = vec[i];
        }
        if (n > 1) {
            for (int i = n - 2; i >= 0; --i) {
                segment_tree[i] = combine(segment_tree[2 * i + 1], 
                                          segment_tree[2 * i + 2]);
            }
        }
    }

    T Request(size_t left, size_t right) const
    {
        size_t Left = GetNewInd(left);
        size_t Right = GetNewInd(right);
        T ans = T();
        
        while(Left < Right)
        {
            if(Left % 2 == 0)
            {
                ans = combine(ans, segment_tree[Left]);
                Left++;
            }
            if(Right % 2 == 1)
            {
                ans = combine(ans, segment_tree[Right]);
                if (Right > 0) Right--;
                else break;
            }
            Left = Parent(Left);
            Right = Parent(Right);
        }
        
        if (Left == Right) {
            ans = combine(ans, segment_tree[Left]);
        }
        
        return ans;
    }

private:
    std::vector<T> segment_tree;
    size_t n;
    T (*combine)(T, T);

    size_t Parent(size_t i) const
    {
        return (i - 1) / 2;
    }
    
    size_t GetNewInd(size_t i) const
    {
        return n - 1 + i;
    }
};