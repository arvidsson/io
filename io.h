// io v0.1 - c++ rapid game prototyping framework using Allegro 5
// Copyright 2017 Pär Arvidsson. All rights reserved.
// Licensed under the MIT license (https://github.com/arvidsson/io/blob/master/LICENSE).

#ifndef IO_HEADER
#define IO_HEADER

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>
#include <random>
#include <vector>
#include <array>
#include <unordered_map>
#include <string>
#include <sstream>
#include <memory>
#include <cmath>
#include <ctime>
#include <cstdarg>

namespace io
{

#define io() io::GameServices::instance()
#define IO(T) \
int main(int argc, char** argv) try\
{\
    io::init();\
    auto game = std::make_unique<T>();\
    game->run();\
}\
catch(const std::exception& e)\
{\
    al_show_native_message_box(al_get_current_display(), "Error", "Exception!", e.what(), 0, ALLEGRO_MESSAGEBOX_ERROR);\
}
#define IO_THROW(...) (io::throw_exception(__FILE__, __LINE__, __VA_ARGS__))
#define IO_CHECK(cond, msg) if (!cond) { IO_THROW(msg); }

void init();
void throw_exception(const char *file, int line, const char *format, ...);

template <typename T>
class Math
{
public:
    static const T almost_zero;
    static const T pi;
    static const T two_pi;
    static const T pi_over_two;
    static const T pi_over_four;
    static const T deg_to_rad;
    static const T rad_to_deg;
    static const T e;

    static T to_radians(const T degrees)
    {
        return degrees * deg_to_rad;
    }

    static T to_degrees(const T radians)
    {
        return radians * rad_to_deg;
    }

    static T round(const T value)
    {
        return value > 0.0 ? std::floor(value + static_cast<T>(0.5)) : std::ceil(value - static_cast<T>(0.5));
    }

    static bool is_zero(const T value, const T epsilon = almost_zero)
    {
        return std::abs(value) <= epsilon;
    }

    static bool is_positive(const T value, const T epsilon = almost_zero)
    {
        return value > epsilon;
    }

    static bool is_negative(const T value, const T epsilon = almost_zero)
    {
        return value < -epsilon;
    }

    static bool is_equal(const T x1, const T x2, const T epsilon = almost_zero)
    {
        return std::abs(x1 - x2) < epsilon;
    }

    static bool is_greater_than(const T x1, const T x2, const T epsilon = almost_zero)
    {
        return x1 > (x2 + epsilon);
    }

    static bool is_less_than(const T x1, const T x2, const T epsilon = almost_zero)
    {
        return x1 < (x2 - epsilon);
    }

    static bool is_greater_than_or_equal(const T x1, const T x2, const T epsilon = almost_zero)
    {
        return !is_less_than(x1, x2, epsilon);
    }

    static bool is_less_than_or_equal(const T x1, const T x2, const T epsilon = almost_zero)
    {
        return !is_greater_than(x1, x2, epsilon);
    }
};

template <typename T> const T Math<T>::almost_zero = static_cast<T>(0.001);
template <typename T> const T Math<T>::pi = static_cast<T>(3.141592653589793);
template <typename T> const T Math<T>::two_pi = static_cast<T>(2.0) * pi;
template <typename T> const T Math<T>::pi_over_two = pi / static_cast<T>(2.0);
template <typename T> const T Math<T>::pi_over_four = pi / static_cast<T>(4.0);
template <typename T> const T Math<T>::deg_to_rad = pi / static_cast<T>(180.0);
template <typename T> const T Math<T>::rad_to_deg = static_cast<T>(180.0) / pi;
template <typename T> const T Math<T>::e = static_cast<T>(2.718281828459045);

template <class Derived, typename T, int N>
class VectorBase;

template <class Derived, typename T>
class VectorBase<Derived, T, 2>
{
protected:
    T values[2];
    VectorBase(T v0, T v1) : values{v0, v1} {}
    
    bool operator==(const Derived& v) const 
    {
        return values[0] == v.values[0] && values[1] == v.values[1];
    }
    
    bool operator!=(const Derived& v) const 
    {
        return !(operator==(v));
    }
    
    bool is_equal(const Derived& v, T epsilon = 0.001) const 
    {
        return Math<T>::is_equal(values[0], v.values[0], epsilon) && Math<T>::is_equal(values[1], v.values[1], epsilon);
    }
    
    friend Derived operator+(const Derived& v1, const Derived& v2)
    {
        return Derived{v1.values[0] + v2.values[0], v1.values[1] + v2.values[1]};
    }

    Derived& operator+=(const Derived& v)
    {
        values[0] += v.values[0];
        values[1] += v.values[1];
        return static_cast<Derived&>(*this);
    }
    
    friend Derived operator-(const Derived& v1, const Derived& v2)
    {
        return Derived{v1.values[0] - v2.values[0], v1.values[1] - v2.values[1]};
    }

    Derived operator-() const
    {
        return Derived{-values[0], -values[1]};
    }

    Derived& operator-=(const Derived& v)
    {
        values[0] -= v.values[0];
        values[1] -= v.values[1];
        return static_cast<Derived&>(*this);
    }

    Derived operator*(T f) const
    {
        return Derived{values[0] * f, values[1] * f};
    }

    Derived& operator*=(T f)
    {
        values[0] *= f;
        values[1] *= f;
        return static_cast<Derived&>(*this);
    }

    Derived operator/(T f) const
    {
        return Derived{values[0] / f, values[1] / f};
    }

    Derived& operator/=(T f)
    {
        values[0] /= f;
        values[1] /= f;
        return static_cast<Derived&>(*this);
    }
};

template <class Derived, typename T>
class VectorBase<Derived, T, 3>
{
protected:
    T values[3];
    VectorBase(T v0, T v1, T v2) : values{v0, v1, v2} {}
    
    bool operator==(const Derived& v) const 
    {
        return values[0] == v.values[0] && values[1] == v.values[1] && values[2] == v.values[2];
    }
    
    bool operator!=(const Derived& v) const 
    {
        return !(operator==(v));
    }
    
    bool is_equal(const Derived& v, T epsilon = 0.001) const 
    {
        return Math<T>::is_equal(values[0], v.values[0], epsilon) && Math<T>::is_equal(values[1], v.values[1], epsilon && Math<T>::is_equal(values[2], v.values[2], epsilon));
    }
    
    friend Derived operator+(const Derived& v1, const Derived& v2)
    {
        return Derived{v1.values[0] + v2.values[0], v1.values[1] + v2.values[1], v1.values[2] + v2.values[2]};
    }

    Derived& operator+=(const Derived& v)
    {
        values[0] += v.values[0];
        values[1] += v.values[1];
        values[2] += v.values[2];
        return static_cast<Derived&>(*this);
    }
    
    friend Derived operator-(const Derived& v1, const Derived& v2)
    {
        return Derived{v1.values[0] - v2.values[0], v1.values[1] - v2.values[1], v1.values[2] - v2.values[2]};
    }

    Derived operator-() const
    {
        return Derived{-values[0], -values[1], -values[2]};
    }

    Derived& operator-=(const Derived& v)
    {
        values[0] -= v.values[0];
        values[1] -= v.values[1];
        values[2] -= v.values[2];
        return static_cast<Derived&>(*this);
    }

    Derived operator*(T f) const
    {
        return Derived{values[0] * f, values[1] * f, values[2] * f};
    }

    Derived& operator*=(T f)
    {
        values[0] *= f;
        values[1] *= f;
        values[2] *= f;
        return static_cast<Derived&>(*this);
    }

    Derived operator/(T f) const
    {
        return Derived{values[0] / f, values[1] / f, values[2] / f};
    }

    Derived& operator/=(T f)
    {
        values[0] /= f;
        values[1] /= f;
        values[2] /= f;
        return static_cast<Derived&>(*this);
    }
};

template <class Derived, typename T>
class VectorBase<Derived, T, 4>
{
protected:
    T values[4];
    VectorBase(T v0, T v1, T v2, T v3) : values{v0, v1, v2, v3} {}
    
    bool operator==(const Derived& v) const 
    {
        return values[0] == v.values[0] && values[1] == v.values[1] && values[2] == v.values[2] && values[3] == v.values[3];
    }
    
    bool operator!=(const Derived& v) const 
    {
        return !(operator==(v));
    }
    
    bool is_equal(const Derived& v, T epsilon = 0.001) const 
    {
        return Math<T>::is_equal(values[0], v.values[0], epsilon) && Math<T>::is_equal(values[1], v.values[1], epsilon && Math<T>::is_equal(values[2], v.values[2], epsilon) && Math<T>::is_equal(values[3], v.values[3], epsilon));
    }
    
    friend Derived operator+(const Derived& v1, const Derived& v2)
    {
        return Derived{v1.values[0] + v2.values[0], v1.values[1] + v2.values[1], v1.values[2] + v2.values[2], v1.values[3] + v2.values[3]};
    }

    Derived& operator+=(const Derived& v)
    {
        values[0] += v.values[0];
        values[1] += v.values[1];
        values[2] += v.values[2];
        values[3] += v.values[3];
        return static_cast<Derived&>(*this);
    }
    
    friend Derived operator-(const Derived& v1, const Derived& v2)
    {
        return Derived{v1.values[0] - v2.values[0], v1.values[1] - v2.values[1], v1.values[2] - v2.values[2], v1.values[3] - v2.values[3]};
    }

    Derived operator-() const
    {
        return Derived{-values[0], -values[1], -values[2], -values[3]};
    }

    Derived& operator-=(const Derived& v)
    {
        values[0] -= v.values[0];
        values[1] -= v.values[1];
        values[2] -= v.values[2];
        values[3] -= v.values[3];
        return static_cast<Derived&>(*this);
    }

    Derived operator*(T f) const
    {
        return Derived{values[0] * f, values[1] * f, values[2] * f, values[3] * f};
    }

    Derived& operator*=(T f)
    {
        values[0] *= f;
        values[1] *= f;
        values[2] *= f;
        values[3] *= f;
        return static_cast<Derived&>(*this);
    }

    Derived operator/(T f) const
    {
        return Derived{values[0] / f, values[1] / f, values[2] / f, values[3] / f};
    }

    Derived& operator/=(T f)
    {
        values[0] /= f;
        values[1] /= f;
        values[2] /= f;
        values[3] /= f;
        return static_cast<Derived&>(*this);
    }
};

} // namespace io

#ifdef IO_IMPLEMENTATION

namespace io
{

void init()
{
    IO_CHECK(al_init(), "Failed to initialize allegro");
    IO_CHECK(al_install_keyboard(), "Failed to install keyboard");
    IO_CHECK(al_install_mouse(), "Failed to install mouse");
    
    ALLEGRO_PATH* resource_path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_change_directory(al_path_cstr(resource_path, ALLEGRO_NATIVE_PATH_SEP));
    al_destroy_path(resource_path);
    
    IO_CHECK(al_init_primitives_addon(), "Failed to initialize primitives addon");
    IO_CHECK(al_init_image_addon(), "Failed to initialize image addon");
    IO_CHECK(al_init_font_addon(), "Failed to initialize font addon");
    IO_CHECK(al_init_ttf_addon(), "Failed to initialize ttf addon");
    IO_CHECK(al_install_audio(), "Failed to install audio addon");
    IO_CHECK(al_init_acodec_addon(), "Failed to initialize acodec addon");
    IO_CHECK(al_reserve_samples(32), "Failed to reserve samples");
}

void throw_exception(const char *file, int line, const char *format, ...)
{
    char buffer[4096];
    va_list ap;
    va_start(ap, format);
    vsprintf_s(buffer, format, ap);
    va_end(ap);
    std::stringstream ss;
    ss << buffer << " (" << file << ":" << line << ")";
    throw std::runtime_error(ss.str());
}

} // namespace io

#endif // IO_IMPLEMENTATION

#endif // IO_HEADER