#ifndef __SIZE_2_HPP__
#define __SIZE_2_HPP__

#include <stdint.h>
#include <assert.h>

namespace danMuGame
{
    class Size2
    {
    private:
        int32_t m_width;
        int32_t m_height;

    public:
        constexpr Size2() noexcept;
        // constexpr Size2(const QPoint &left) noexcept;
        constexpr Size2(int32_t width, int32_t height) noexcept;

        constexpr inline int32_t width() const noexcept;
        constexpr inline int32_t height() const noexcept;
        constexpr inline void setWidth(int32_t m_width) noexcept;
        constexpr inline void setHeight(int32_t m_height) noexcept;

        constexpr inline int32_t &rwidth() noexcept;
        constexpr inline int32_t &rheight() noexcept;

        constexpr inline Size2 &operator+=(const Size2 &left);
        constexpr inline Size2 &operator-=(const Size2 &left);
        constexpr inline Size2 &operator*=(int32_t c);
        constexpr inline Size2 &operator/=(int32_t c);

        friend constexpr inline bool operator==(const Size2 &left, const Size2 &right)
        {
            return (left.m_width == right.m_width) && (left.m_height == right.m_height);
        }
        friend constexpr inline bool operator!=(const Size2 &left, const Size2 &right)
        {
            return !(left == right);
        }
        friend constexpr inline Size2 operator+(const Size2 &left, const Size2 &right)
        {
            return Size2(left.m_width + right.m_width, left.m_height + right.m_height);
        }
        friend constexpr inline Size2 operator-(const Size2 &left, const Size2 &right)
        {
            return Size2(left.m_width - right.m_width, left.m_height - right.m_height);
        }
        friend constexpr inline Size2 operator*(const Size2 &left, int32_t c)
        {
            return Size2(left.m_width * c, left.m_height * c);
        }
        friend constexpr inline Size2 operator*(int32_t c, const Size2 &right)
        {
            return Size2(right.m_width * c, right.m_height * c);
        }
        friend constexpr inline Size2 operator+(const Size2 &left)
        {
            return left;
        }
        friend constexpr inline Size2 operator-(const Size2 &left)
        {
            return Size2(-left.m_width, -left.m_height);
        }
        friend constexpr inline Size2 operator/(const Size2 &left, int32_t divisor)
        {
            assert(divisor < 0 || divisor > 0);
            return Size2(left.m_width / divisor, left.m_height / divisor);
        }
    };

    constexpr inline Size2::Size2() noexcept : m_width(0), m_height(0) {}

    constexpr inline Size2::Size2(int32_t width, int32_t height) noexcept : m_width(width), m_height(height) {}

    constexpr inline int32_t Size2::width() const noexcept
    {
        return m_width;
    }

    constexpr inline int32_t Size2::height() const noexcept
    {
        return m_height;
    }

    constexpr inline void Size2::setWidth(int32_t width) noexcept
    {
        m_width = width;
    }

    constexpr inline void Size2::setHeight(int32_t height) noexcept
    {
        m_height = height;
    }

    constexpr inline int32_t &Size2::rwidth() noexcept
    {
        return m_width;
    }

    constexpr inline int32_t &Size2::rheight() noexcept
    {
        return m_height;
    }

    constexpr inline Size2 &Size2::operator+=(const Size2 &right)
    {
        m_width += right.m_width;
        m_height += right.m_height;
        return *this;
    }

    constexpr inline Size2 &Size2::operator-=(const Size2 &right)
    {
        m_width -= right.m_width;
        m_height -= right.m_height;
        return *this;
    }

    constexpr inline Size2 &Size2::operator*=(int32_t c)
    {
        m_width *= c;
        m_height *= c;
        return *this;
    }

    constexpr inline Size2 &Size2::operator/=(int32_t divisor)
    {
        assert(divisor > 0 || divisor < 0);
        m_width /= divisor;
        m_height /= divisor;
        return *this;
    }
}

#endif
