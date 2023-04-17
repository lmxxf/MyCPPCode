#include "Swap.h"

template<typename T>
Swap<T>::Swap() : m_a(0), m_b(0) {}

template<typename T>
Swap<T>::Swap(T a, T b) : m_a(a), m_b(b) {}

template<typename T>
void Swap<T>::swapValues() {
    T temp = m_a;
    m_a = m_b;
    m_b = temp;
}

template<typename T>
T Swap<T>::getA() const {
    return m_a;
}

template<typename T>
T Swap<T>::getB() const {
    return m_b;
}

template class Swap<int>;
