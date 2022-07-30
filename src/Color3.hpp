#ifndef __COLOR_3_HPP__
#define __COLOR_3_HPP__

#include <stdint.h>
#include <assert.h>

namespace danMuGame
{
    class Color3
    {
    private:
        uint8_t m_r;
        uint8_t m_g;
        uint8_t m_b;

    public:
        constexpr Color3() noexcept;
        constexpr Color3(uint8_t r, uint8_t g, uint8_t b) noexcept;
        constexpr Color3(const Color3 &left) noexcept;

        constexpr inline uint8_t r() const noexcept;
        constexpr inline uint8_t g() const noexcept;
        constexpr inline uint8_t b() const noexcept;

        constexpr inline void setR(uint8_t r) noexcept;
        constexpr inline void setG(uint8_t g) noexcept;
        constexpr inline void setB(uint8_t b) noexcept;
        constexpr inline void setRGB(uint8_t r, uint8_t g, uint8_t b) noexcept;

        constexpr inline Color3 &operator+=(const Color3 &left);
        constexpr inline Color3 &operator-=(const Color3 &left);
        constexpr inline Color3 &operator*=(uint8_t c);
        constexpr inline Color3 &operator/=(uint8_t c);

        constexpr inline Color3 &operator=(const Color3 &left);

        friend constexpr inline bool operator==(const Color3 &left, const Color3 &right)
        {
            return (left.m_r == right.m_r) && (left.m_g == right.m_g) && (left.m_b == right.m_b);
        }
        friend constexpr inline bool operator!=(const Color3 &left, const Color3 &right)
        {
            return !(left == right);
        }
        friend constexpr inline Color3 operator+(const Color3 &left, const Color3 &right)
        {
            return Color3(left.m_r + right.m_r, left.m_g + right.m_g, left.m_b + right.m_b);
        }

        friend constexpr inline Color3 operator-(const Color3 &left, const Color3 &right)
        {
            return Color3(left.m_r - right.m_r, left.m_g - right.m_g, left.m_b - right.m_b);
        }
        friend constexpr inline Color3 operator*(const Color3 &left, uint8_t c)
        {
            return Color3(left.m_r * c, left.m_g * c, left.m_b * c);
        }
        friend constexpr inline Color3 operator*(uint8_t c, const Color3 &right)
        {
            return Color3(right.m_r * c, right.m_g * c, right.m_b * c);
        }
        friend constexpr inline Color3 operator+(const Color3 &left)
        {
            return left;
        }
        friend constexpr inline Color3 operator-(const Color3 &left)
        {
            return Color3((uint8_t)-left.m_r, (uint8_t)-left.m_g, (uint8_t)-left.m_b);
        }
        friend constexpr inline Color3 operator/(const Color3 &left, uint8_t divisor)
        {
            assert(divisor < 0 || divisor > 0);
            return Color3(left.m_r / divisor, left.m_g / divisor, left.m_b / divisor);
        }
    };

    constexpr inline Color3::Color3() noexcept : m_r(0), m_g(0), m_b(0) {}

    constexpr inline Color3::Color3(uint8_t r, uint8_t g, uint8_t b) noexcept : m_r(r), m_g(g), m_b(b) {}

    constexpr inline Color3::Color3(const Color3 &left) noexcept : m_r(left.m_r), m_g(left.m_g), m_b(left.m_b) {}

    constexpr inline uint8_t Color3::r() const noexcept
    {
        return m_r;
    }
    constexpr inline uint8_t Color3::g() const noexcept
    {
        return m_g;
    }
    constexpr inline uint8_t Color3::b() const noexcept
    {
        return m_b;
    }
    constexpr inline void Color3::setR(uint8_t r) noexcept
    {
        m_r = r;
    }
    constexpr inline void Color3::setG(uint8_t g) noexcept
    {
        m_g = g;
    }
    constexpr inline void Color3::setB(uint8_t b) noexcept
    {
        m_b = b;
    }
    constexpr inline void Color3::setRGB(uint8_t r, uint8_t g, uint8_t b) noexcept
    {
        m_r = r;
        m_g = g;
        m_b = b;
    }

    constexpr inline Color3 &Color3::operator+=(const Color3 &right)
    {
        m_r += right.m_r;
        m_g += right.m_g;
        m_b += right.m_b;
        return *this;
    }

    constexpr inline Color3 &Color3::operator-=(const Color3 &right)
    {
        m_r -= right.m_r;
        m_g -= right.m_g;
        m_b -= right.m_b;
        return *this;
    }

    constexpr inline Color3 &Color3::operator*=(uint8_t c)
    {
        m_r *= c;
        m_g *= c;
        m_b *= c;
        return *this;
    }

    constexpr inline Color3 &Color3::operator/=(uint8_t divisor)
    {
        assert(divisor > 0 || divisor < 0);
        m_r /= divisor;
        m_g /= divisor;
        m_b /= divisor;
        return *this;
    }

    constexpr inline Color3 &Color3::operator=(const Color3 &left)
    {
        m_r = left.m_r;
        m_g = left.m_g;
        m_b = left.m_b;
        return *this;
    }

    namespace FastColor3
    {
        constexpr inline Color3 aliceblue{0xf0, 0xf8, 0xff};
        constexpr inline Color3 antiquewhite{0xfa, 0xeb, 0xd7};
        constexpr inline Color3 aqua{0x00, 0xff, 0xff};
        constexpr inline Color3 aquamarine{0x7f, 0xff, 0xd4};
        constexpr inline Color3 azure{0xf0, 0xff, 0xff};
        constexpr inline Color3 beige{0xf5, 0xf5, 0xdc};
        constexpr inline Color3 bisque{0xff, 0xe4, 0xc4};
        constexpr inline Color3 black{0x00, 0x00, 0x00};
        constexpr inline Color3 blanchedalmond{0xff, 0xeb, 0xcd};
        constexpr inline Color3 blue{0x00, 0x00, 0xff};
        constexpr inline Color3 blueviolet{0x8a, 0x2b, 0xe2};
        constexpr inline Color3 brown{0xa5, 0x2a, 0x2a};
        constexpr inline Color3 burlywood{0xde, 0xb8, 0x87};
        constexpr inline Color3 cadetblue{0x5f, 0x9e, 0xa0};
        constexpr inline Color3 chartreuse{0x7f, 0xff, 0x00};
        constexpr inline Color3 chocolate{0xd2, 0x69, 0x1e};
        constexpr inline Color3 coral{0xff, 0x7f, 0x50};
        constexpr inline Color3 cornflowerblue{0x64, 0x95, 0xed};
        constexpr inline Color3 cornsilk{0xff, 0xf8, 0xdc};
        constexpr inline Color3 crimson{0xdc, 0x14, 0x3c};
        constexpr inline Color3 cyan{0x00, 0xff, 0xff};
        constexpr inline Color3 darkblue{0x00, 0x00, 0x8b};
        constexpr inline Color3 darkcyan{0x00, 0x8b, 0x8b};
        constexpr inline Color3 darkgoldenrod{0xb8, 0x86, 0x0b};
        constexpr inline Color3 darkgray{0xa9, 0xa9, 0xa9};
        constexpr inline Color3 darkgreen{0x00, 0x64, 0x00};
        constexpr inline Color3 darkgrey{0xa9, 0xa9, 0xa9};
        constexpr inline Color3 darkkhaki{0xbd, 0xb7, 0x6b};
        constexpr inline Color3 darkmagenta{0x8b, 0x00, 0x8b};
        constexpr inline Color3 darkolivegreen{0x55, 0x6b, 0x2f};
        constexpr inline Color3 darkorange{0xff, 0x8c, 0x00};
        constexpr inline Color3 darkorchid{0x99, 0x32, 0xcc};
        constexpr inline Color3 darkred{0x8b, 0x00, 0x00};
        constexpr inline Color3 darksalmon{0xe9, 0x96, 0x7a};
        constexpr inline Color3 darkseagreen{0x8f, 0xbc, 0x8f};
        constexpr inline Color3 darkslateblue{0x48, 0x3d, 0x8b};
        constexpr inline Color3 darkslategray{0x2f, 0x4f, 0x4f};
        constexpr inline Color3 darkslategrey{0x2f, 0x4f, 0x4f};
        constexpr inline Color3 darkturquoise{0x00, 0xce, 0xd1};
        constexpr inline Color3 darkviolet{0x94, 0x00, 0xd3};
        constexpr inline Color3 deeppink{0xff, 0x14, 0x93};
        constexpr inline Color3 deepskyblue{0x00, 0xbf, 0xff};
        constexpr inline Color3 dimgray{0x69, 0x69, 0x69};
        constexpr inline Color3 dimgrey{0x69, 0x69, 0x69};
        constexpr inline Color3 dodgerblue{0x1e, 0x90, 0xff};
        constexpr inline Color3 firebrick{0xb2, 0x22, 0x22};
        constexpr inline Color3 floralwhite{0xff, 0xfa, 0xf0};
        constexpr inline Color3 forestgreen{0x22, 0x8b, 0x22};
        constexpr inline Color3 fuchsia{0xff, 0x00, 0xff};
        constexpr inline Color3 gainsboro{0xdc, 0xdc, 0xdc};
        constexpr inline Color3 ghostwhite{0xf8, 0xf8, 0xff};
        constexpr inline Color3 gold{0xff, 0xd7, 0x00};
        constexpr inline Color3 goldenrod{0xda, 0xa5, 0x20};
        constexpr inline Color3 gray{0x80, 0x80, 0x80};
        constexpr inline Color3 green{0x00, 0x80, 0x00};
        constexpr inline Color3 greenyellow{0xad, 0xff, 0x2f};
        constexpr inline Color3 grey{0x80, 0x80, 0x80};
        constexpr inline Color3 honeydew{0xf0, 0xff, 0xf0};
        constexpr inline Color3 hotpink{0xff, 0x69, 0xb4};
        constexpr inline Color3 indianred{0xcd, 0x5c, 0x5c};
        constexpr inline Color3 indigo{0x4b, 0x00, 0x82};
        constexpr inline Color3 ivory{0xff, 0xff, 0xf0};
        constexpr inline Color3 khaki{0xf0, 0xe6, 0x8c};
        constexpr inline Color3 lavender{0xe6, 0xe6, 0xfa};
        constexpr inline Color3 lavenderblush{0xff, 0xf0, 0xf5};
        constexpr inline Color3 lawngreen{0x7c, 0xfc, 0x00};
        constexpr inline Color3 lemonchiffon{0xff, 0xfa, 0xcd};
        constexpr inline Color3 lightblue{0xad, 0xd8, 0xe6};
        constexpr inline Color3 lightcoral{0xf0, 0x80, 0x80};
        constexpr inline Color3 lightcyan{0xe0, 0xff, 0xff};
        constexpr inline Color3 lightgoldenrodyellow{0xfa, 0xfa, 0xd2};
        constexpr inline Color3 lightgray{0xd3, 0xd3, 0xd3};
        constexpr inline Color3 lightgreen{0x90, 0xee, 0x90};
        constexpr inline Color3 lightgrey{0xd3, 0xd3, 0xd3};
        constexpr inline Color3 lightpink{0xff, 0xb6, 0xc1};
        constexpr inline Color3 lightsalmon{0xff, 0xa0, 0x7a};
        constexpr inline Color3 lightseagreen{0x20, 0xb2, 0xaa};
        constexpr inline Color3 lightskyblue{0x87, 0xce, 0xfa};
        constexpr inline Color3 lightslategray{0x77, 0x88, 0x99};
        constexpr inline Color3 lightslategrey{0x77, 0x88, 0x99};
        constexpr inline Color3 lightsteelblue{0xb0, 0xc4, 0xde};
        constexpr inline Color3 lightyellow{0xff, 0xff, 0xe0};
        constexpr inline Color3 lime{0x00, 0xff, 0x00};
        constexpr inline Color3 limegreen{0x32, 0xcd, 0x32};
        constexpr inline Color3 linen{0xfa, 0xf0, 0xe6};
        constexpr inline Color3 magenta{0xff, 0x00, 0xff};
        constexpr inline Color3 maroon{0x80, 0x00, 0x00};
        constexpr inline Color3 mediumaquamarine{0x66, 0xcd, 0xaa};
        constexpr inline Color3 mediumblue{0x00, 0x00, 0xcd};
        constexpr inline Color3 mediumorchid{0xba, 0x55, 0xd3};
        constexpr inline Color3 mediumpurple{0x93, 0x70, 0xdb};
        constexpr inline Color3 mediumseagreen{0x3c, 0xb3, 0x71};
        constexpr inline Color3 mediumslateblue{0x7b, 0x68, 0xee};
        constexpr inline Color3 mediumspringgreen{0x00, 0xfa, 0x9a};
        constexpr inline Color3 mediumturquoise{0x48, 0xd1, 0xcc};
        constexpr inline Color3 mediumvioletred{0xc7, 0x15, 0x85};
        constexpr inline Color3 midnightblue{0x19, 0x19, 0x70};
        constexpr inline Color3 mintcream{0xf5, 0xff, 0xfa};
        constexpr inline Color3 mistyrose{0xff, 0xe4, 0xe1};
        constexpr inline Color3 moccasin{0xff, 0xe4, 0xb5};
        constexpr inline Color3 navajowhite{0xff, 0xde, 0xad};
        constexpr inline Color3 navy{0x00, 0x00, 0x80};
        constexpr inline Color3 oldlace{0xfd, 0xf5, 0xe6};
        constexpr inline Color3 olive{0x80, 0x80, 0x00};
        constexpr inline Color3 olivedrab{0x6b, 0x8e, 0x23};
        constexpr inline Color3 orange{0xff, 0xa5, 0x00};
        constexpr inline Color3 orangered{0xff, 0x45, 0x00};
        constexpr inline Color3 orchid{0xda, 0x70, 0xd6};
        constexpr inline Color3 palegoldenrod{0xee, 0xe8, 0xaa};
        constexpr inline Color3 palegreen{0x98, 0xfb, 0x98};
        constexpr inline Color3 paleturquoise{0xaf, 0xee, 0xee};
        constexpr inline Color3 palevioletred{0xdb, 0x70, 0x93};
        constexpr inline Color3 papayawhip{0xff, 0xef, 0xd5};
        constexpr inline Color3 peachpuff{0xff, 0xda, 0xb9};
        constexpr inline Color3 peru{0xcd, 0x85, 0x3f};
        constexpr inline Color3 pink{0xff, 0xc0, 0xcb};
        constexpr inline Color3 plum{0xdd, 0xa0, 0xdd};
        constexpr inline Color3 powderblue{0xb0, 0xe0, 0xe6};
        constexpr inline Color3 purple{0x80, 0x00, 0x80};
        constexpr inline Color3 red{0xff, 0x00, 0x00};
        constexpr inline Color3 rosybrown{0xbc, 0x8f, 0x8f};
        constexpr inline Color3 royalblue{0x41, 0x69, 0xe1};
        constexpr inline Color3 saddlebrown{0x8b, 0x45, 0x13};
        constexpr inline Color3 salmon{0xfa, 0x80, 0x72};
        constexpr inline Color3 sandybrown{0xf4, 0xa4, 0x60};
        constexpr inline Color3 seagreen{0x2e, 0x8b, 0x57};
        constexpr inline Color3 seashell{0xff, 0xf5, 0xee};
        constexpr inline Color3 sienna{0xa0, 0x52, 0x2d};
        constexpr inline Color3 silver{0xc0, 0xc0, 0xc0};
        constexpr inline Color3 skyblue{0x87, 0xce, 0xeb};
        constexpr inline Color3 slateblue{0x6a, 0x5a, 0xcd};
        constexpr inline Color3 slategray{0x70, 0x80, 0x90};
        constexpr inline Color3 slategrey{0x70, 0x80, 0x90};
        constexpr inline Color3 snow{0xff, 0xfa, 0xfa};
        constexpr inline Color3 springgreen{0x00, 0xff, 0x7f};
        constexpr inline Color3 steelblue{0x46, 0x82, 0xb4};
        constexpr inline Color3 tan{0xd2, 0xb4, 0x8c};
        constexpr inline Color3 teal{0x00, 0x80, 0x80};
        constexpr inline Color3 thistle{0xd8, 0xbf, 0xd8};
        constexpr inline Color3 tomato{0xff, 0x63, 0x47};
        constexpr inline Color3 turquoise{0x40, 0xe0, 0xd0};
        constexpr inline Color3 violet{0xee, 0x82, 0xee};
        constexpr inline Color3 wheat{0xf5, 0xde, 0xb3};
        constexpr inline Color3 white{0xff, 0xff, 0xff};
        constexpr inline Color3 whitesmoke{0xf5, 0xf5, 0xf5};
        constexpr inline Color3 yellow{0xff, 0xff, 0x00};
        constexpr inline Color3 yellowgreen{0x9a, 0xcd, 0x32};
    }
}

#endif