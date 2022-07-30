#ifndef __GAME_STACK_HPP__
#define __GAME_STACK_HPP__

#include <vector>

template <typename T>
class GameStack
{
private:
    std::vector<T> m_memoryVec;
    int m_index = 0;

public:
    inline void setSize(int size)
    {
        m_memoryVec.resize(size);
    }

    inline int32_t getSize()
    {
        return (int32_t)m_memoryVec.size();
    }

    inline void clear()
    {
        m_index = 0;
    }
    inline bool empty()
    {
        return 0 == m_index;
    }
    inline void push(const T &t)
    {
        if (m_index < m_memoryVec.size())
        {
            m_memoryVec[m_index] = t;
        }
        else
        {
            m_memoryVec.push_back(t);
        }
        ++m_index;
    }
    inline void pop()
    {
        if (m_index > 0)
        {
            --m_index;
        }
    }
    T &top() { return m_memoryVec[m_index - 1]; }
    const T &top() const { return m_memoryVec[m_index - 1]; }

    inline std::vector<T>& getSource()
    {
        return this->m_memoryVec;
    }
};

#endif