#ifndef __VECTOR_2_HPP__
#define __VECTOR_2_HPP__

#include <stdint.h>
#include <assert.h>
#include "Point2.hpp"

namespace danMuGame
{
    class Vector2
    {
    private:
        float posX;
        float posY;

    public:
        constexpr Vector2() noexcept;
        constexpr Vector2(const Point2 &point) noexcept;
        constexpr Vector2(float x, float y) noexcept;

        constexpr inline float x() const noexcept;
        constexpr inline float y() const noexcept;
        constexpr inline void setX(float posX) noexcept;
        constexpr inline void setY(float posY) noexcept;

        constexpr inline float &rx() noexcept;
        constexpr inline float &ry() noexcept;

        constexpr inline Vector2 &operator+=(const Vector2 &left);
        constexpr inline Vector2 &operator-=(const Vector2 &left);
        constexpr inline Vector2 &operator*=(float c);
        constexpr inline Vector2 &operator/=(float c);

        friend constexpr inline bool operator==(const Vector2 &left, const Vector2 &right)
        {
            return (left.posX == right.posX) && (left.posY == right.posY);
        }
        friend constexpr inline bool operator!=(const Vector2 &left, const Vector2 &right)
        {
            return !(left == right);
        }
        friend constexpr inline Vector2 operator+(const Vector2 &left, const Vector2 &right)
        {
            return Vector2(left.posX + right.posX, left.posY + right.posY);
        }
        friend constexpr inline Vector2 operator-(const Vector2 &left, const Vector2 &right)
        {
            return Vector2(left.posX - right.posX, left.posY - right.posY);
        }
        friend constexpr inline Vector2 operator*(const Vector2 &left, float c)
        {
            return Vector2(left.posX * c, left.posY * c);
        }
        friend constexpr inline Vector2 operator*(float c, const Vector2 &right)
        {
            return Vector2(right.posX * c, right.posY * c);
        }
        friend constexpr inline Vector2 operator+(const Vector2 &left)
        {
            return left;
        }
        friend constexpr inline Vector2 operator-(const Vector2 &left)
        {
            return Vector2(-left.posX, -left.posY);
        }
        friend constexpr inline Vector2 operator/(const Vector2 &left, float divisor)
        {
            assert(divisor < 0 || divisor > 0);
            return Vector2(left.posX / divisor, left.posY / divisor);
        }

        inline Point2 toPoint2()
        {
            return Point2((int32_t)posX, (int32_t)posY);
        }
    };

    constexpr inline Vector2::Vector2() noexcept : posX(0), posY(0) {}

    constexpr inline Vector2::Vector2(float x, float y) noexcept : posX(x), posY(y) {}

    constexpr inline Vector2::Vector2(const Point2 &point) noexcept : posX(point.x()), posY(point.y()) {}

    constexpr inline float Vector2::x() const noexcept
    {
        return posX;
    }

    constexpr inline float Vector2::y() const noexcept
    {
        return posY;
    }

    constexpr inline void Vector2::setX(float x) noexcept
    {
        posX = x;
    }

    constexpr inline void Vector2::setY(float y) noexcept
    {
        posY = y;
    }

    constexpr inline float &Vector2::rx() noexcept
    {
        return posX;
    }

    constexpr inline float &Vector2::ry() noexcept
    {
        return posY;
    }

    constexpr inline Vector2 &Vector2::operator+=(const Vector2 &right)
    {
        posX += right.posX;
        posY += right.posY;
        return *this;
    }

    constexpr inline Vector2 &Vector2::operator-=(const Vector2 &right)
    {
        posX -= right.posX;
        posY -= right.posY;
        return *this;
    }

    constexpr inline Vector2 &Vector2::operator*=(float c)
    {
        posX *= c;
        posY *= c;
        return *this;
    }

    constexpr inline Vector2 &Vector2::operator/=(float divisor)
    {
        assert(divisor > 0 || divisor < 0);
        posX /= divisor;
        posY /= divisor;
        return *this;
    }
}

#endif
