#include <memory>
#include <iostream>
#include <vector>
#include <float.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

#define NUM_THREAD 8

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
inline float	pow2(float x) { return x*x; }
inline float	pow3(float x) { return x*x*x; }
inline float	pow4(float x) { return x*x*x*x; }
inline float	pow5(float x) { return x*x*x*x*x; }
inline float	clamp(float x, float a, float b) { return x < a ? a : b < x ? b : x; }
inline float	saturate(float x) { return x < 0.f ? 0.f : x > 1.f ? 1.f : x; }
inline float	recip(float x) { return 1.f / x; }
inline float	mix(float a, float b, float t) { return a*(1.0f - t) + b*t; }
inline float	step(float edge, float x) { return (x < edge) ? 0.f : 1.0f; }
inline float	smoothstep(float a, float b, float t) { if (a>=b) return 0.f; float x = saturate((t-a) / (b-a)); return x*x*(3.f - 2.f *x); }
inline float	radians(float deg) { return (deg / 180.f)*PI; }
inline float	degrees(float rad) { return (rad / PI) * 180.f; }

inline simd_float3	simd_unit(float t) {
	return simd_make_float3(t, t, t);
}

namespace rayt {

	inline float	drand48() {
		return float(((double)(rand()) / (RAND_MAX)));
	}

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
	class Ray {
		public:
			Ray() {}
			Ray(const vec3& o, const vec3& dir)
				: m_origin(o)
				  , m_direction(dir) {}
			const	vec3& origin() const { return m_origin; }
			const	vec3& direction() const { return m_direction; }
			vec3	at(float t) const { return m_origin + t*m_direction;} 
		private:
			vec3	m_origin;
			vec3	m_direction;
	};

	class	HitRec {
		public:
			float	t;
			vec3	p;
			vec3	n;
	};
	
	class	Shape {
		public:
			virtual bool	hit(const Ray& r, float t0, float t1, HitRec& hrec) const = 0;
	};
	
	class Sphere : public Shape {
		public:
			Sphere() {}
			Sphere(const vec3& c, float r)
				: m_center(c) ,m_radius(r) {}
			
			virtual bool	hit(const Ray& r, float t0, float t1, HitRec& hrec) const override {
				vec3	oc = r.origin() - m_center;
				float	a = simd_dot(r.direction(), r.direction());
				float	b = 2.0f*simd_dot(r.direction(), oc);
				float	c = simd_dot(oc, oc) - pow2(m_radius);
				float	D = pow2(b) - 4*a*c;
	
				if (D > 0) {
					float	root = sqrtf(D);
					float	temp = (-b - root) / (2.0f*a);
					if (temp < t1 && t0 < temp) {
						hrec.t = temp;
						hrec.p = r.at(hrec.t);
						hrec.n = (hrec.p - m_center) / m_radius;
						return true;
					}
					temp = (-b + root) / (2.0f*a);
					if (temp < t1 && t0 < temp) {
						hrec.t = temp;
						hrec.p = r.at(hrec.t);
						hrec.n = (hrec.p - m_center) / m_radius;
						return true;
					}
				}
				return false;
			}
		private:
			vec3	m_center;
			float	m_radius;
	};

	typedef std::shared_ptr<Shape>	ShapePtr;

	class ShapeList : public Shape {
		public:
			ShapeList() {}

			void	add(const ShapePtr& shape) {
				m_list.push_back(shape);
			}

			virtual bool	hit(const Ray& r, float t0, float t1, HitRec& hrec) const override {
				HitRec	temp_rec;
				bool	hit_anything = false;
				float	closest_so_far = t1;
				for (auto& p : m_list) {
					if (p->hit(r, t0, closest_so_far, temp_rec)) {
						hit_anything = true;
						closest_so_far = temp_rec.t;
						hrec = temp_rec;
					}
				}
				return hit_anything;
			}
				
		private:
			std::vector<ShapePtr>	m_list;
	};
	class Camera {
		public:
			Camera() {}
			Camera(const vec3& u, const vec3& v, const vec3& w) {
				m_origin = vec3(0);
				m_uvw[0] = u;
				m_uvw[1] = v;
				m_uvw[2] = w;
			}
			Camera(const vec3& lookfrom, const vec3& lookat, const vec3& vup, float vfov, float aspect) {
				vec3	u, v, w;
				float	halfH = tanf(radians(vfov)/2.f);
				float	halfW = aspect * halfH;
	
				m_origin = lookfrom;
				w = simd_normalize(lookfrom - lookat);
				u = simd_normalize(simd_cross(vup, w));
				v = simd_cross(w, u);
				m_uvw[2] = m_origin - halfW*u - halfH*v - w; //スクリーン左下位置ベクトル
				m_uvw[0] = 2.0f * halfW * u; //横幅ベクトル
				m_uvw[1] = 2.0f * halfH * v; //高さベクトル
			}
			Ray	getRay(float u, float v) const {
				return Ray(m_origin, m_uvw[2] + m_uvw[0]*u + m_uvw[1]*v - m_origin);
			}
		private:
			vec3	m_origin; // 位置
			vec3	m_uvw[3]; // 直交基底ベクトル
	};
	class Scene {
		public:
			Scene(int width, int height)
				:m_image(std::make_unique<Image>(width, height)), m_backColor(0.2f) {}
			
			void	build() {
				vec3	w = simd_make_float3(-2.0f, -1.0f, -1.0f);
				vec3	u = simd_make_float3(4.0f, 0.0f, 0.0f);
				vec3	v = simd_make_float3(0.0f, 2.0f, 0.0f);
				m_camera = std::make_unique<Camera>(u, v, w);
				ShapeList	*world = new ShapeList();
				world->add(std::make_shared<Sphere>(simd_make_float3(0, 0, -1), 0.5f));
				world->add(std::make_shared<Sphere>(simd_make_float3(0, -100.5, -1), 100));
				m_world.reset(world);
			}

			// vec3	color(const rayt::Ray& r) {
			// 	vec3	c = simd_make_float3(0, 0, -1);
			// 	float	t = hit_sphere(c, 0.5f, r);
			//
			// 	if (0.0f < t) {
			// 		vec3	light = simd_normalize(simd_make_float3(-3, 3, 3) - c);
			// 		vec3	N = simd_normalize(r.at(t) - c);
			// 		float	Brightness = std::max(simd_dot(light, N), 0.0f);
			// 		float	ambient = 0.1f;
			// 		vec3	base_color = simd_make_float3(1.0f, 0.0f, 0.0f);
			// 		return base_color * std::min(Brightness + ambient, 1.0f);
			// 	}
			// 	return backgroundSky(r.direction());
			// }
			vec3 color(const rayt::Ray& r, const Shape* world) {
				HitRec	hrec;

				if (world->hit(r, 0, FLT_MAX, hrec))
					return 0.5f*(hrec.n + simd_unit(1.0f));
				return backgroundSky(r.direction());
			}

			vec3	background(const vec3& d) const {
				return m_backColor;
			}

			vec3	backgroundSky(const vec3& d) const {
				vec3	v = simd_normalize(d);
				float	t = 0.5f * (v.y + 1.0f);
				return simd_mix(simd_unit(1.0f), simd_make_float3(0.5f, 0.7f, 1.0f), t);
			}

			void	render() {
				build();

				int	nx = m_image->width();
				int	ny = m_image->height();
		#pragma	omp parallel for schedule(dynamic, 1) num_threads(NUM_THREAD)
				for (int j = 0; j<ny; j++) {
					// std::cerr << "Rendering (y = " << j << ") " << (100.0 * j / (ny - 1)) << "%" << std::endl;
					for (int i = 0; i<nx; i++) {
						float	u = float(i + drand48()) / float(nx);
						float	v = float(j + drand48()) / float(ny);
						Ray	r = m_camera->getRay(u, v);
						vec3	c = color(r, m_world.get());
						// .get()はunique_ptrで管理しているオブジェクトのアドレスを渡す所有権は渡さずデータを参照したいときに使われる
						m_image->write(i, (ny - j - 1), c.x, c.y, c.z);
					}
				}
				stbi_write_bmp("render.bmp", nx, ny, sizeof(Image::rgb), m_image->pixels());
			}
	private:
		std::unique_ptr<Camera>	m_camera;
		std::unique_ptr<Image>	m_image;
		std::unique_ptr<Shape>	m_world;
		vec3			m_backColor;
	};

}

