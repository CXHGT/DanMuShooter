#ifndef __POINT_2_HPP__
#define __POINT_2_HPP__

#include <stdint.h>
#include <assert.h>

namespace danMuGame
{
    class Point2
    {
    private:
        int32_t posX;
        int32_t posY;

    public:
        constexpr Point2() noexcept;
        // constexpr Point2(const QPoint &left) noexcept;
        constexpr Point2(int32_t x, int32_t y) noexcept;

        constexpr inline int32_t x() const noexcept;
        constexpr inline int32_t y() const noexcept;
        constexpr inline void setX(int32_t posX) noexcept;
        constexpr inline void setY(int32_t posY) noexcept;

        constexpr inline int32_t &rx() noexcept;
        constexpr inline int32_t &ry() noexcept;

        constexpr inline Point2 &operator+=(const Point2 &left);
        constexpr inline Point2 &operator-=(const Point2 &left);
        constexpr inline Point2 &operator*=(int32_t c);
        constexpr inline Point2 &operator/=(int32_t c);

        friend constexpr inline bool operator==(const Point2 &left, const Point2 &right)
        {
            return (left.posX == right.posX) && (left.posY == right.posY);
        }
        friend constexpr inline bool operator!=(const Point2 &left, const Point2 &right)
        {
            return !(left == right);
        }
        friend constexpr inline Point2 operator+(const Point2 &left, const Point2 &right)
        {
            return Point2(left.posX + right.posX, left.posY + right.posY);
        }
        friend constexpr inline Point2 operator-(const Point2 &left, const Point2 &right)
        {
            return Point2(left.posX - right.posX, left.posY - right.posY);
        }
        friend constexpr inline Point2 operator*(const Point2 &left, int32_t c)
        {
            return Point2(left.posX * c, left.posY * c);
        }
        friend constexpr inline Point2 operator*(int32_t c, const Point2 &right)
        {
            return Point2(right.posX * c, right.posY * c);
        }
        friend constexpr inline Point2 operator+(const Point2 &left)
        {
            return left;
        }
        friend constexpr inline Point2 operator-(const Point2 &left)
        {
            return Point2(-left.posX, -left.posY);
        }
        friend constexpr inline Point2 operator/(const Point2 &left, int32_t divisor)
        {
            assert(divisor < 0 || divisor > 0);
            return Point2(left.posX / divisor, left.posY / divisor);
        }
    };

    constexpr inline Point2::Point2() noexcept : posX(0), posY(0) {}

    constexpr inline Point2::Point2(int32_t x, int32_t y) noexcept : posX(x), posY(y) {}

    constexpr inline int32_t Point2::x() const noexcept
    {
        return posX;
    }

    constexpr inline int32_t Point2::y() const noexcept
    {
        return posY;
    }

    constexpr inline void Point2::setX(int32_t x) noexcept
    {
        posX = x;
    }

    constexpr inline void Point2::setY(int32_t y) noexcept
    {
        posY = y;
    }

    constexpr inline int32_t &Point2::rx() noexcept
    {
        return posX;
    }

    constexpr inline int32_t &Point2::ry() noexcept
    {
        return posY;
    }

    constexpr inline Point2 &Point2::operator+=(const Point2 &right)
    {
        posX += right.posX;
        posY += right.posY;
        return *this;
    }

    constexpr inline Point2 &Point2::operator-=(const Point2 &right)
    {
        posX -= right.posX;
        posY -= right.posY;
        return *this;
    }

    constexpr inline Point2 &Point2::operator*=(int32_t c)
    {
        posX *= c;
        posY *= c;
        return *this;
    }

    constexpr inline Point2 &Point2::operator/=(int32_t divisor)
    {
        assert(divisor > 0 || divisor < 0);
        posX /= divisor;
        posY /= divisor;
        return *this;
    }
}

#endif
