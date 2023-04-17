#ifndef SWAP_H
#define SWAP_H

template<typename T>
class Swap {
public:
    Swap();
    Swap(T a, T b);
    void swapValues();
    T getA() const;
    T getB() const;

private:
    T m_a;
    T m_b;
};

#endif // SWAP_H
