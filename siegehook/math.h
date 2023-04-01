#pragma once

#include <string.h>
#include <string>
#include <iostream>
#include <imgui.h>

class vec3 {
public:
	float x, y, z;

	inline vec3(float x, float y, float z) : x(x), y(y), z(z) {};

	inline vec3() : x(0), y(0), z(0) {};

	inline vec3 operator+(const vec3& other) {
		return vec3(x + other.x, y + other.y, z + other.z);
	}

	inline vec3 operator-(const vec3& other) {
		return vec3(x - other.x, y - other.y, z - other.z);
	}

	inline vec3 operator*(const vec3& other) {
		return vec3(x * other.x, y * other.y, z * other.z);
	}

	inline vec3 operator/(const vec3& other) {
		return vec3(x / other.x, y / other.y, z / other.z);
	}

	inline vec3 operator*(const float& other) {
		return vec3(x * other, y * other, z * other);
	}

	inline vec3 operator/(const float& other) {
		return vec3(x / other, y / other, z / other);
	}

	inline float magnitude() {
		return sqrt(x * x + y * y + z * z);
	}

	inline std::string to_string() {
		return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
	}

	inline static float dot(const vec3& a, const vec3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	inline vec3 normalize() {
		float mag = magnitude();
		return vec3(x / mag, y / mag, z / mag);
	}

	inline float distance(const vec3& other) {
		return sqrtf(powf(x - other.x, 2) + powf(y - other.y, 2) + powf(z - other.z, 2));
	}

	inline static float distance(const vec3& a, const vec3& b) {
		return sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2) + powf(a.z - b.z, 2));
	}

};

class mat4 {
public:
	float m[16];

	inline mat4() {
		memset(m, 0, sizeof(m));
	}

	inline float& operator()(int i, int j) {
		return m[i * 4 + j];
	}

	inline vec3 position() {
		int i = 3;
		return vec3(m[i * 4 + 0], m[i * 4 + 1], m[i * 4 + 2]);
	}

	inline mat4 operator*(const mat4& other) {
		mat4 result = mat4();
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					result.m[i * 4 + j] += m[i * 4 + k] * other.m[k * 4 + j];
				}
			}
		}
		return result;
	}

	inline std::string to_string() {
		std::string result = "";
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result += std::to_string(m[i * 4 + j]) + " ";
			}
			result += "\n";
		}
		return result;
	}
};

class vec2 {
public:
	float x, y;

	inline vec2(float x, float y) : x(x), y(y) {};

	inline vec2() : x(0), y(0) {};

	inline vec2 operator+(const vec2& other) {
		return vec2(x + other.x, y + other.y);
	}

	inline vec2 operator-(const vec2& other) {
		return vec2(x - other.x, y - other.y);
	}

	inline vec2 operator*(const vec2& other) {
		return vec2(x * other.x, y * other.y);
	}

	inline vec2 operator/(const vec2& other) {
		return vec2(x / other.x, y / other.y);
	}

	inline vec2 operator*(const float& other) {
		return vec2(x * other, y * other);
	}

	inline vec2 operator/(const float& other) {
		return vec2(x / other, y / other);
	}

	inline float magnitude() {
		return sqrt(x * x + y * y);
	}

	// Convert to string
	inline std::string to_string() {
		return std::to_string(x) + ", " + std::to_string(y);
	}

	inline ImVec2 to_imgui() {
		return ImVec2(x, y);
	}

	inline float distance(const vec2& other) {
		return sqrtf(powf(x - other.x, 2) + powf(y - other.y, 2));
	}
};

class vec4 {
public:
	float x, y, z, w;

	inline vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};

	inline vec4() : x(0), y(0), z(0), w(0) {};

	inline vec4 operator+(const vec4& other) {
		return vec4(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	inline vec4 operator-(const vec4& other) {
		return vec4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	inline vec4 operator*(const vec4& other) {
		return vec4(x * other.x, y * other.y, z * other.z, w * other.w);
	}

	inline vec4 operator/(const vec4& other) {
		return vec4(x / other.x, y / other.y, z / other.z, w / other.w);
	}

	inline vec4 operator*(const float& other) {
		return vec4(x * other, y * other, z * other, w * other);
	}

	inline vec4 operator/(const float& other) {
		return vec4(x / other, y / other, z / other, w / other);
	}

	inline vec3 xyz() {
		return vec3(x, y, z);
	}

	inline vec2 xy() {
		return vec2(x, y);
	}

	inline vec2 zw() {
		return vec2(z, w);
	}

	inline float magnitude() {
		return sqrt(x * x + y * y + z * z + w * w);
	}

	inline vec4 transform(mat4& matrix) {
		vec4 result;
		result.x = x * matrix(0, 0) + y * matrix(1, 0) + z * matrix(2, 0) + w * matrix(3, 0);
		result.y = x * matrix(0, 1) + y * matrix(1, 1) + z * matrix(2, 1) + w * matrix(3, 1);
		result.z = x * matrix(0, 2) + y * matrix(1, 2) + z * matrix(2, 2) + w * matrix(3, 2);
		result.w = x * matrix(0, 3) + y * matrix(1, 3) + z * matrix(2, 3) + w * matrix(3, 3);
		return result;
	};
};

namespace math
{
	inline bool world_to_screen(vec3 world, vec2 viewport, mat4 matrix, vec2& screen) {
		vec2 center(viewport.x / 2, viewport.y / 2);
		vec4 screen_space = vec4(world.x, world.y, world.z, 1.0f).transform(matrix);

		screen.x = center.x + (center.x * screen_space.x / screen_space.w);
		screen.y = center.y - (center.y * screen_space.y / screen_space.w);

		if (screen_space.w < 0.01f)
			return false;

		return true;
	}
};