#ifndef __COLOR_4_HPP__
#define __COLOR_4_HPP__

#include <stdint.h>
#include <assert.h>

namespace danMuGame
{
    class Color4
    {
    private:
        uint8_t m_a;
        uint8_t m_r;
        uint8_t m_g;
        uint8_t m_b;

    public:
        constexpr Color4() noexcept;
        constexpr Color4(uint8_t a, uint8_t r, uint8_t g, uint8_t b);

        constexpr inline uint8_t a() const noexcept;
        constexpr inline uint8_t r() const noexcept;
        constexpr inline uint8_t g() const noexcept;
        constexpr inline uint8_t b() const noexcept;

        constexpr inline void setA(uint8_t a) noexcept;
        constexpr inline void setR(uint8_t r) noexcept;
        constexpr inline void setG(uint8_t g) noexcept;
        constexpr inline void setB(uint8_t b) noexcept;
        constexpr inline void setRGB(uint8_t r, uint8_t g, uint8_t b) noexcept;
        constexpr inline void setARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b) noexcept;

        constexpr inline Color4 &operator+=(const Color4 &left);
        constexpr inline Color4 &operator-=(const Color4 &left);
        constexpr inline Color4 &operator*=(uint8_t c);
        constexpr inline Color4 &operator/=(uint8_t c);

        friend constexpr inline bool operator==(const Color4 &left, const Color4 &right)
        {
            return (left.m_a == right.m_a) && (left.m_r == right.m_r) && (left.m_g == right.m_g) && (left.m_b == right.m_b);
        }
        friend constexpr inline bool operator!=(const Color4 &left, const Color4 &right)
        {
            return !(left == right);
        }
        friend constexpr inline Color4 operator+(const Color4 &left, const Color4 &right)
        {
            return Color4(left.m_a + right.m_a, left.m_r + right.m_r, left.m_g + right.m_g, left.m_b + right.m_b);
        }

        friend constexpr inline Color4 operator-(const Color4 &left, const Color4 &right)
        {
            return Color4(left.m_a - right.m_a, left.m_r - right.m_r, left.m_g - right.m_g, left.m_b - right.m_b);
        }
        friend constexpr inline Color4 operator*(const Color4 &left, uint8_t c)
        {
            return Color4(left.m_a * c, left.m_r * c, left.m_g * c, left.m_b * c);
        }
        friend constexpr inline Color4 operator*(uint8_t c, const Color4 &right)
        {
            return Color4(right.m_a * c, right.m_r * c, right.m_g * c, right.m_b * c);
        }
        friend constexpr inline Color4 operator+(const Color4 &left)
        {
            return left;
        }
        friend constexpr inline Color4 operator-(const Color4 &left)
        {
            return Color4((uint8_t)-left.m_a, (uint8_t)-left.m_r, (uint8_t)-left.m_g, (uint8_t)-left.m_b);
        }
        friend constexpr inline Color4 operator/(const Color4 &left, uint8_t divisor)
        {
            assert(divisor < 0 || divisor > 0);
            return Color4(left.m_a / divisor, left.m_r / divisor, left.m_g / divisor, left.m_b / divisor);
        }
    };

    constexpr inline Color4::Color4() noexcept : m_a(0), m_r(0), m_g(0), m_b(0) {}

    constexpr inline Color4::Color4(uint8_t a, uint8_t r, uint8_t g, uint8_t b) noexcept : m_a(a), m_r(r), m_g(g), m_b(b) {}

    constexpr inline uint8_t Color4::a() const noexcept
    {
        return m_a;
    }
    constexpr inline uint8_t Color4::r() const noexcept
    {
        return m_r;
    }
    constexpr inline uint8_t Color4::g() const noexcept
    {
        return m_g;
    }
    constexpr inline uint8_t Color4::b() const noexcept
    {
        return m_b;
    }

    constexpr inline void Color4::setA(uint8_t a) noexcept
    {
        m_a = a;
    }
    constexpr inline void Color4::setR(uint8_t r) noexcept
    {
        m_r = r;
    }
    constexpr inline void Color4::setG(uint8_t g) noexcept
    {
        m_g = g;
    }
    constexpr inline void Color4::setB(uint8_t b) noexcept
    {
        m_b = b;
    }
    constexpr inline void Color4::setRGB(uint8_t r, uint8_t g, uint8_t b) noexcept
    {
        m_r = r;
        m_g = g;
        m_b = b;
    }
    constexpr inline void Color4::setARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b) noexcept
    {
        m_a = a;
        m_r = r;
        m_g = g;
        m_b = b;
    }

    constexpr inline Color4 &Color4::operator+=(const Color4 &right)
    {
        m_a += right.m_a;
        m_r += right.m_r;
        m_g += right.m_g;
        m_b += right.m_b;
        return *this;
    }

    constexpr inline Color4 &Color4::operator-=(const Color4 &right)
    {
        m_a -= right.m_a;
        m_r -= right.m_r;
        m_g -= right.m_g;
        m_b -= right.m_b;
        return *this;
    }

    constexpr inline Color4 &Color4::operator*=(uint8_t c)
    {
        m_a *= c;
        m_r *= c;
        m_g *= c;
        m_b *= c;
        return *this;
    }

    constexpr inline Color4 &Color4::operator/=(uint8_t divisor)
    {
        assert(divisor > 0 || divisor < 0);
        m_a /= divisor;
        m_r /= divisor;
        m_g /= divisor;
        m_b /= divisor;
        return *this;
    }
}

#endif