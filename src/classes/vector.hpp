#pragma once
#include <numbers>

namespace ellohim
{
	struct ScreenResolution
	{
		ScreenResolution() = default;
		ScreenResolution(int x, int y) : x(x), y(y) {}
		int x{};
		int y{};

		ScreenResolution operator/(const ScreenResolution a) const { return { x / a.x, y / a.y }; }
	};

	struct Vector2
	{
		Vector2() = default;
		Vector2(float x, float y) : x(x), y(y) {}
		float x;
		float y;

		float length_sqr() const noexcept { return x * x + y * y; }

		float length() const noexcept
		{
			const auto len_sq = length_sqr();
			if (len_sq == 0.0f) [[unlikely]]
			{
				return 0.0f;
			}

			return std::sqrtf(len_sq);
		}

		Vector2& normalize() noexcept
		{
			const auto len = length();
			if (len > 0.0f) [[likely]]
			{
				const float inv_len = 1.0f / len;
				*this *= inv_len;
			}

			return *this;
		}

		Vector2 normalized() const noexcept
		{
			auto vec = *this;
			vec.normalize();
			return vec;
		}

		void operator=(nlohmann::json const& data)
		{
			*this = data.get<Vector2>();
		}


		Vector2 operator+(const Vector2& v) const noexcept { return { x + v.x, y + v.y }; }
		Vector2 operator-(const Vector2& v) const noexcept { return { x - v.x, y - v.y }; }
		Vector2 operator*(float scalar) const noexcept { return { x * scalar, y * scalar }; }
		Vector2 operator/(float scalar) const noexcept { return { x / scalar, y / scalar }; }
		Vector2 operator-() const noexcept { return { -x, -y }; }

		Vector2& operator*=(float scalar) noexcept { x *= scalar; y *= scalar; return *this; }
		Vector2& operator/=(float scalar) noexcept { x /= scalar; y /= scalar; return *this; }
		Vector2& operator+=(const Vector2& v) noexcept { x += v.x; y += v.y; return *this; }
		Vector2& operator-=(const Vector2& v) noexcept { x -= v.x; y -= v.y; return *this; }

		bool operator==(const Vector2& v) const noexcept { return x == v.x && y == v.y; }
		bool operator!=(const Vector2& v) const noexcept { return !(*this == v); }

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vector2, x, y)
	};

	struct Vector3
	{
		Vector3() = default;
		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
		float x{};
		float y{};
		float z{};

		float dot(Vector3 v) const noexcept { return x * v.x + y * v.y + z * v.z; }

		float distance(const Vector3& v) const noexcept { return (*this - v).length(); }
		float distance_sqr(const Vector3& v) const noexcept { return (*this - v).length_sqr(); }

		Vector3& normalize() noexcept
		{
			const auto len = this->length();
			if (len > 0.0f) [[likely]]
			{
				const float inv_len = 1.0f / len;
				*this *= inv_len;
			}

			return *this;
		}

		Vector3 normalized() const noexcept
		{
			auto vec = *this;
			vec.normalize();
			return vec;
		}

		float length_sqr() const noexcept { return x * x + y * y + z * z; }

		float length() const noexcept
		{
			const auto len_sq = this->length_sqr();

			if (len_sq == 0.0f) [[unlikely]]
			{
				return 0.0f;
			}

			return std::sqrtf(len_sq);
		}

		float length_2d() const noexcept
		{
			const auto len_sq_2d = x * x + y * y;
			if (len_sq_2d == 0.0f) [[unlikely]]
			{
				return 0.0f;
			}

			return std::sqrtf(len_sq_2d);
		}

		Vector3 cross(const Vector3& v) const noexcept { return { y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x }; }

		Vector3 operator+(const Vector3& v) const noexcept { return { x + v.x, y + v.y, z + v.z }; }
		Vector3 operator-(const Vector3& v) const noexcept { return { x - v.x, y - v.y, z - v.z }; }
		Vector3 operator*(float scalar) const noexcept { return { x * scalar, y * scalar, z * scalar }; }
		Vector3 operator/(float scalar) const noexcept { return { x / scalar, y / scalar, z / scalar }; }
		Vector3 operator-() const noexcept { return { -x, -y, -z }; }

		Vector3& operator*=(float scalar) noexcept { x *= scalar; y *= scalar; z *= scalar; return *this; }
		Vector3& operator/=(float scalar) noexcept { x /= scalar; y /= scalar; z /= scalar; return *this; }
		Vector3& operator+=(const Vector3& v) noexcept { x += v.x; y += v.y; z += v.z; return *this; }
		Vector3& operator-=(const Vector3& v) noexcept { x -= v.x; y -= v.y; z -= v.z; return *this; }

		bool operator==(const Vector3& v) const noexcept { return x == v.x && y == v.y && z == v.z; }
		bool operator!=(const Vector3& v) const noexcept { return !(*this == v); }

		void operator=(nlohmann::json const& data)
		{
			*this = data.get<Vector3>();
		}

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vector3, x, y, z)
	};

	struct Rotator
	{
		Rotator() = default;
		Rotator(float pitch, float yaw, float roll) : pitch(pitch), yaw(yaw), roll(roll) {}
		float pitch{};
		float yaw{};
		float roll{};

		Rotator operator-(const Rotator vec3) const { return { vec3.pitch - pitch, vec3.yaw - yaw, vec3.roll - roll }; }
		Rotator operator*(const Rotator& a) const { return { pitch * a.pitch, yaw * a.yaw, roll * a.roll }; }
		Rotator operator+(const Rotator& vec3) const { return { pitch + vec3.pitch, yaw * vec3.yaw, roll * vec3.roll }; }
		Rotator operator/(const Rotator& vec3) const { return { vec3.pitch / pitch, vec3.yaw / yaw, vec3.roll / roll }; }
		bool operator==(const Rotator a) const { return pitch == a.pitch && yaw == a.yaw && roll == a.roll; }
	};

	struct Vector4
	{
		Vector4() = default;
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
		float x{};
		float y{};
		float z{};
		float w{};

		float dot(Vector4 vec4)
		{
			return (x * vec4.x) + (y * vec4.y) + (z * vec4.z) + (w * vec4.w);
		}

		float bitwise_and_first(int v)
		{
			return float((int)x & v);
		}

		[[nodiscard]] Vector3 rotate_vector(const Vector3& v) const noexcept
		{
			const auto q_vec = Vector3(this->x, this->y, this->z);
			auto uv = q_vec.cross(v);
			auto uuv = q_vec.cross(uv);

			uv *= (2.0f * this->w);
			uuv *= 2.0f;

			return v + uv + uuv;
		}

		Vector4 operator&(const Vector4& v) { return { float((int)v.x & (int)x), float((int)v.y & (int)y), float((int)v.z & (int)z), float((int)v.w & (int)w) }; };
	};

	class Matrix3x4
	{
	public:
		float mat[3][4];

		[[nodiscard]] const float* operator[](int i) const noexcept { return mat[i]; }
		[[nodiscard]] float* operator[](int i) noexcept { return mat[i]; }
	};

	class Matrix4x4
	{
	public:
		float mat[4][4];

		[[nodiscard]] const float* operator[](int index) const noexcept { return mat[index]; }
		[[nodiscard]] float* operator[](int index) noexcept { return mat[index]; }
	};

	class calculations
	{
	public:
		void angle_vectors(const Vector3& angles, Vector3& forward, Vector3& right, Vector3& up) noexcept
		{
			const auto pitch = angles.x * (std::numbers::pi_v<float> / 180.0f);
			const auto yaw = angles.y * (std::numbers::pi_v<float> / 180.0f);
			const auto roll = angles.z * (std::numbers::pi_v<float> / 180.0f);

			const auto sin_pitch = std::sinf(pitch);
			const auto cos_pitch = std::cosf(pitch);
			const auto sin_yaw = std::sinf(yaw);
			const auto cos_yaw = std::cosf(yaw);
			const auto sin_roll = std::sinf(roll);
			const auto cos_roll = std::cosf(roll);

			forward.x = cos_pitch * cos_yaw;
			forward.y = cos_pitch * sin_yaw;
			forward.z = -sin_pitch;

			right.x = -sin_roll * sin_pitch * cos_yaw + cos_roll * sin_yaw;
			right.y = -sin_roll * sin_pitch * sin_yaw - cos_roll * cos_yaw;
			right.z = -sin_roll * cos_pitch;

			up.x = cos_roll * sin_pitch * cos_yaw + sin_roll * sin_yaw;
			up.y = cos_roll * sin_pitch * sin_yaw - sin_roll * cos_yaw;
			up.z = cos_roll * cos_pitch;
		}

		void normalize_angles(Vector3& angles) noexcept
		{
			while (angles.y > 180.0f) { angles.y -= 360.0f; }
			while (angles.y < -180.0f) { angles.y += 360.0f; }
			while (angles.x > 89.0f) { angles.x -= 180.0f; }
			while (angles.x < -89.0f) { angles.x += 180.0f; }

			angles.z = 0.0f;
		}

		Vector3 vector_to_angle(const Vector3& forward) noexcept
		{
			auto pitch = rad_to_deg(std::atan2(-forward.z, forward.length_2d()));
			auto yaw = rad_to_deg(std::atan2(forward.y, forward.x));
			return { pitch, yaw, 0.f };
		}

		Vector3 calculate_angle(const Vector3& src, const Vector3& dst) noexcept
		{
			const auto delta = dst - src;
			const auto length = delta.length_2d();

			Vector3 angles;
			angles.x = rad_to_deg(std::atan2f(-delta.z, length));
			angles.y = rad_to_deg(std::atan2f(delta.y, delta.x));
			angles.z = 0.0f;

			return angles;
		}

		float calculate_fov(const Vector3& view_angles, const Vector3& src, const Vector3& dst) noexcept
		{
			const auto angle = calculate_angle(src, dst);
			const auto delta = Vector2(view_angles.x - angle.x, normalize_yaw(view_angles.y - angle.y));

			return delta.length();
		}

		float deg_to_rad(float degrees) noexcept
		{
			return degrees * (std::numbers::pi_v<float> / 180.0f);
		}

		float rad_to_deg(float radians) noexcept
		{
			return radians * (180.0f / std::numbers::pi_v<float>);
		}

		float normalize_yaw(float yaw) noexcept
		{
			while (yaw > 180.0f) { yaw -= 360.0f; }
			while (yaw < -180.0f) { yaw += 360.0f; }
			return yaw;
		}

		Vector3 rotate_by_quat(const Vector4& q, const Vector3& v)
		{
			const auto qx = q.x, qy = q.y, qz = q.z, qw = q.w;
			const auto tx = 2.0f * (qy * v.z - qz * v.y);
			const auto ty = 2.0f * (qz * v.x - qx * v.z);
			const auto tz = 2.0f * (qx * v.y - qy * v.x);

			return
			{
				v.x + qw * tx + (qy * tz - qz * ty),
				v.y + qw * ty + (qz * tx - qx * tz),
				v.z + qw * tz + (qx * ty - qy * tx)
			};
		}
	};

	using FVector = Vector3;
	using FRotator = Rotator;
	using FVector2D = Vector2;
}