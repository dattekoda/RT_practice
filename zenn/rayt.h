#include <memory>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

#include <simd/simd.h>
typedef simd_float3 vec3;
typedef simd_float3 col3;

#define PI 3.14159265359f
#define PI2 6.28318530718f
#define RECIP_PI 0.31830988618f
#define RECIP_PI2 0.15915494f
#define LOG2 1.442695f
#define EPSILON 1e-6f
#define GAMMA_FACTOR 2.2f

#include <random>
inline float	drand48() {
	return float(((double)(rand()) / (RAND_MAX)));
}

inline float	pow2(float x) { return x*x; }
inline float	pow3(float x) { return x*x*x; }
inline float	pow4(float x) { return x*x*x*x; }
inline float	pow5(float x) { return x*x*x*x*x; }
inline float	clamp(float x, float a, float b) { return x < a ? a : b < x ? b : x; }
inline float	saturate(float x) { return x < 0.f ? 0.f : x > 1.f ? 1.f : x; }
inline float	recip(float x) { return 1.f / x; }
inline float	mix(float a, float b, float t) { return a*(1.0f - t) + b*t; }
inline float	step(float edge, float x) { return (x < edge) ? 0.f : 1.0f; }
inline float	smoothstep(float a, float b, float t) { if (a>=b) return 0.f; float x = saturate((t-a) / (b-a)); return x*x*(3.f - 2.f *t); }
inline float	radians(float deg) { return (deg / 180.f)*PI; }
inline float	degrees(float rad) { return (rad / PI) * 180.f; }

namespace rayt {
	class	Image {
		public:
			struct rgb {
				unsigned char	r;
				unsigned char	g;
				unsigned char	b;
			};

			Image() : m_pixels(nullptr) { }
			Image(int w, int h) {
				m_width = w;
				m_height = h;
				m_pixels.reset(new rgb[m_width*m_height]);
			}

			int	width() const { return m_width; }
			int	height() const { return m_height; }
			void	*pixels() const { return m_pixels.get(); }

			void	write(int x, int y, float r, float g, float b) {
				int	index = m_width*y + x;
				m_pixels[index].r = static_cast<unsigned char>(r*255.99f);
				m_pixels[index].g = static_cast<unsigned char>(g*255.99f);
				m_pixels[index].b = static_cast<unsigned char>(b*255.99f);
			}

		private:
			int	m_width;
			int	m_height;
			std::unique_ptr<rgb[]>	m_pixels;
	};
}
