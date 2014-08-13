#ifndef LIB_INC_H
#define LIB_INC_H

#include <string>
#include <cmath>
#include <dav/inc.h>

#define MXT_BGCOLOR 0x00627d
#define MXT_WHITE   0xffffff
#define MXT_BLACK   0x000000

struct vec
{
	constexpr vec(float a = 0, float b = 0, float c = 0) : dx(a), dy(b), dz(c) { }
	float dx, dy, dz;
};

inline constexpr float mag_sqr(const vec& v) { return v.dx * v.dx + v.dy * v.dy + v.dz * v.dz; }
inline constexpr float mag(const vec& v) { return sqrt(mag_sqr(v)); }
inline constexpr vec operator*(float s, const vec& v) { return vec(s * v.dx, s * v.dy, s * v.dz); }
inline constexpr vec operator*(const vec& v, float s) { return vec(s * v.dx, s * v.dy, s * v.dz); }
inline constexpr vec operator/(const vec& v, float s) { return vec(v.dx / s, v.dy / s, v.dz / s); }
inline constexpr vec operator+(const vec& v1, const vec& v2) { return vec(v1.dx + v2.dx, v1.dy + v2.dy, v1.dz + v2.dz); }
inline constexpr vec operator-(const vec& v1, const vec& v2) { return vec(v1.dx - v2.dx, v1.dy - v2.dy, v1.dz - v2.dz); }
inline constexpr vec operator-(const vec& v) { return vec(-v.dx, -v.dy, -v.dz); }
inline constexpr float operator*(const vec& v1, const vec& v2) { return v1.dx * v2.dx + v1.dy * v2.dy + v1.dz * v2.dz; }
inline constexpr vec normalize(const vec& v) { return vec(v.dx / mag(v), v.dy / mag(v), v.dz / mag(v)); }
inline constexpr float angle(const vec& v1, const vec& v2) { return 180.0 * acos((v1.dx * v2.dx + v1.dy * v2.dy + v1.dz * v2.dz) / (mag(v1) * mag(v2))) / 3.14159265; }

inline constexpr vec operator^(const vec& v1, const vec& v2)
{
	return vec(v1.dy * v2.dz - v1.dz * v2.dy,
			   v1.dz * v2.dx - v1.dx * v2.dz,
			   v1.dx * v2.dy - v1.dy * v2.dx);
}

struct plane
{
	plane( ) { }
	plane(const vec& v, const vec& l) : normal(v), p(l), D(-(v * l)) { }
	plane(const vec& p1, const vec& p2, const vec& p3) : plane(normalize((p2 - p1) * (p2 - p3)), p2) { }
	vec normal;
	vec p;
	float D;
};

inline float operator-(const plane& p, const vec& v) { return p.normal * v + p.D; }

#endif

