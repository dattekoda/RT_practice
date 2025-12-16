#include <memory>
#include <iostream>
#include <vector>
#include <float.h>
#include <random>
#include <algorithm>

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

	class	Texture {
		public:
			virtual vec3	value(float u, float v, const vec3& p) const = 0;
	};

	class	Texture;
	typedef std::shared_ptr<Texture> TexturePtr;

	class ColorTexture : public Texture {
		public :
			ColorTexture(const vec3& c)
				: m_color(c) {}

			virtual vec3	value(float u, float v, const vec3& p) const override {
				return m_color;
			}
		private:
			vec3	m_color;
	};

	inline float	drand48() {
		return float(((double)(rand()) / (RAND_MAX)));
	}

	inline vec3	random_vector() {
		return	simd_make_float3(drand48(), drand48(), drand48());
	}

	inline vec3	random_in_unit_sphere() {
		vec3	p;

		while (1) {
			p = 2.0f * random_vector() - simd_unit(1.f);
			if (simd_length_squared(p) < 1.0f)
				break ;
		}
		return p;
	}

	inline	vec3	linear_to_gamma(const vec3& v, float gammaFactor) {
		float	recipGammaFactor = recip(gammaFactor);
		return simd_make_float3(
				powf(v.x, recipGammaFactor),
				powf(v.y, recipGammaFactor),
				powf(v.z, recipGammaFactor));
	}
	
	inline	vec3	gamma_to_linear(const vec3& v, float gammaFactor) {
		return simd_make_float3(
				powf(v.x, gammaFactor),
				powf(v.y, gammaFactor),
				powf(v.z, gammaFactor));
	}

	class	ImageFilter {
		public:
			virtual vec3	filter(const vec3& c) const = 0;
	};

	class	GammaFilter : public ImageFilter {
		public:
			GammaFilter(float factor) : m_factor(factor) {}
			virtual vec3	filter(const vec3& c) const override {
				return linear_to_gamma(c, m_factor);
			}
		private:
			float	m_factor;
	};

	class	Material;
	typedef	std::shared_ptr<Material>	MaterialPtr;

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
				m_filters.push_back(std::make_unique<GammaFilter>(GAMMA_FACTOR));
			}

			int	width() const { return m_width; }
			int	height() const { return m_height; }
			void	*pixels() const { return m_pixels.get(); }

			void	write(int x, int y, float r, float g, float b) {
				vec3	c = simd_make_float3(r, g, b);
				for (auto& f : m_filters) {
					c = f->filter(c);
				}
				int	index = m_width*y + x;
				m_pixels[index].r = static_cast<unsigned char>(c.x*255.99f);
				m_pixels[index].g = static_cast<unsigned char>(c.y*255.99f);
				m_pixels[index].b = static_cast<unsigned char>(c.z*255.99f);
			}

		private:
			int	m_width;
			int	m_height;
			std::unique_ptr<rgb[]>	m_pixels;
			std::vector< std::unique_ptr<ImageFilter> > m_filters;
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
			float	u;
			float	v;
			vec3	p;
			vec3	n;
			MaterialPtr	mat;
	};
	
	class	Shape {
		public:
			virtual bool	hit(const Ray& r, float t0, float t1, HitRec& hrec) const = 0;
	};

	class	ScatterRec {
		public:
			Ray	ray; // 散乱後の新しい光線
			vec3	albedo; // 反射率
			float	pdf_value;
	};

	class	Material {
		public:
			virtual bool	scatter(const Ray& r, const HitRec& hrec, ScatterRec& srec) const = 0; 
			virtual vec3	emitted(const Ray& r, const HitRec& hrec) const { return simd_unit(0.0f); }
			virtual float	scattering_pdf(const Ray& r, const HitRec& hrec) const { return 0; }
	};

	class	Lambertian : public Material {
		public:
			Lambertian(const vec3& c)
				: m_albedo(c) {}

			virtual bool	scatter(const Ray& r, const HitRec& hrec, ScatterRec& srec) const override {
				vec3	target = hrec.p + hrec.n + random_in_unit_sphere();
				srec.ray = Ray(hrec.p, target - hrec.p);
				srec.albedo = m_albedo;
				srec.pdf_value = simd_dot(hrec.n, srec.ray.direction()) / PI;
				return true;
			};

			virtual float	scattering_pdf(const Ray& r, const HitRec& hrec) const override {
				return std::max(simd_dot(hrec.n, simd_normalize(r.direction())), 0.0f) / PI;
			}
		private:
			vec3	m_albedo;
	};

	inline vec3	reflect(const vec3& v, const vec3& n) {
		return v - 2.f * simd_dot(v, n)*n;
	}

	class	Metal : public Material {
		public:
			Metal(const vec3& c, float fuzz)
				: m_albedo(c), m_fuzz(fuzz) {}
			virtual bool	scatter(const Ray& r, const HitRec& hrec, ScatterRec& srec) const override {
				vec3	reflected = reflect(simd_normalize(r.direction()), hrec.n);
				reflected += m_fuzz*random_in_unit_sphere();
				srec.ray = Ray(hrec.p, reflected);
				srec.albedo = m_albedo;
				// srec.pdf_value = 
				return simd_dot(srec.ray.direction(), hrec.n) > 0;
			}
		private:
			vec3	m_albedo;
			float	m_fuzz;
	};

	class DiffuseLight : public Material {
		public:
			DiffuseLight(const TexturePtr& emit)
				: m_emit(emit) {}

			virtual bool	scatter(const Ray& r, const HitRec& hrec, ScatterRec& srec) const override {
				return false;
			}

			virtual vec3	emitted(const Ray& r, const HitRec& hrec) const override {
				return m_emit->value(hrec.u, hrec.v, hrec.p);
			}
		private:
			TexturePtr	m_emit;
	};

	class Sphere : public Shape {
		public:
			Sphere() {}
			Sphere(const vec3& c, float r, const MaterialPtr& mat)
				: m_center(c), m_radius(r), m_material(mat) {}
			
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
						hrec.mat = m_material;
						return true;
					}
					temp = (-b + root) / (2.0f*a);
					if (temp < t1 && t0 < temp) {
						hrec.t = temp;
						hrec.p = r.at(hrec.t);
						hrec.n = (hrec.p - m_center) / m_radius;
						hrec.mat = m_material;
						return true;
					}
				}
				return false;
			}
		private:
			vec3	m_center;
			float	m_radius;
			MaterialPtr	m_material;
	};

	class Cylinder : public Shape {
		public:
			Cylinder() {}
			Cylinder(const vec3& c, const vec3& n, float r, float h, const MaterialPtr& mat) 
			: m_center(c), m_normal(n), m_radius(r), m_height(h), m_material(mat) {}

			virtual bool	hit(const Ray& r, float t0, float t1, HitRec& hrec) const override {
			vec3	A = simd_cross(r.direction(), m_normal);
			vec3	P = r.origin() - m_center;
			vec3	B = simd_cross(P, m_normal);
			float	a = simd_dot(A, A);
			float	b = simd_dot(A, B);
			float	c = simd_dot(B, B) - pow2(m_radius);

			float	D = pow2(b) - a*c;

			float	s = simd_dot(r.direction(), m_normal);	
			float	t = simd_dot(P, m_normal);
			bool	inside_height = false;
			float	temp_max;
			float	temp_min;
			if (s > 0) {
				temp_max = (m_height - t) / s;
				temp_min = -t / s;
			} else if (s < 0) {
				temp_max = -t / s;
				temp_min = (m_height - t) / s;
			} else {
				temp_max = FLT_MAX;
				temp_min = -FLT_MAX;
			}
			if (D > 0) {
				if (a == 0)
					return false;
				float	root = sqrtf(D);
				float	temp = (-b - root) / a;
				if (temp < t1 && t0 < temp && temp_min < temp && temp < temp_max) {
					hrec.t = temp;
					hrec.p = r.at(hrec.t);
					vec3	w = hrec.p - m_center;
					float	h = simd_dot(w, m_normal);
					hrec.n = simd_normalize(hrec.p - m_center - h * m_normal);
					hrec.mat = m_material;
					return true;
				}
				temp = (-b + root) / a;
				if (temp < t1 && t0 < temp && temp_min < temp && temp < temp_max) {
					hrec.t = temp;
					hrec.p = r.at(hrec.t);
					vec3	w = hrec.p - m_center;
					float	h = simd_dot(w, m_normal);
					hrec.n = simd_normalize(hrec.p - m_center - h * m_normal);
					hrec.mat = m_material;
					return true;
				}
			}
			return false;
		}
		private:
			vec3	m_center;
			vec3	m_normal;
			float	m_radius;
			float	m_height;
			MaterialPtr	m_material;
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
			Scene(int width, int height, int samples)
				:m_image(std::make_unique<Image>(width, height)), m_backColor(0.2f), m_samples(samples) {}
			
			void	build() {
				vec3	w = simd_make_float3(-2.0f, -1.0f, -1.0f);
				vec3	u = simd_make_float3(4.0f, 0.0f, 0.0f);
				vec3	v = simd_make_float3(0.0f, 2.0f, 0.0f);
				m_camera = std::make_unique<Camera>(u, v, w);
				ShapeList	*world = new ShapeList();
				// world->add(std::make_shared<Sphere>(simd_make_float3(-0.7, 0, -1), 0.3f, std::make_shared<Lambertian>(simd_make_float3(0.1f, 0.2f, 0.5f))));
				world->add(std::make_shared<Sphere>(simd_make_float3(0, -100.5, -1), 100, std::make_shared<Lambertian>(simd_make_float3(0.8f, 0.8f, 0.8f))));
				world->add(std::make_shared<Cylinder>(simd_make_float3(0, 0, -1), simd_normalize(simd_make_float3(0, 1, 0)), 0.3, 0.5, std::make_shared<Metal>(simd_make_float3(0.8f, 0.8f, 0.8f), 0.0f)));
				world->add(std::make_shared<Sphere>(simd_make_float3(0, -0.2, -1), 0.3, std::make_shared<DiffuseLight>(std::make_shared<ColorTexture>(simd_unit(1.0f)))));
				// world->add(std::make_shared<Sphere>(simd_make_float3(0.6, 0, -1), 0.5f, std::make_shared<Metal>(simd_make_float3(1.0f, 1.0f, 1.0f), 0.06125f)));
				m_world.reset(world);
			}

			// vec3	color(const rayt::Ray& r, const Shape *world, int depth) {
			// 	HitRec	hrec;
			//
			// 	if (depth <= 0)
			// 		return simd_unit(0.0f);
			//
			// 	if (world->hit(r, 0.001f, FLT_MAX, hrec)) {
			// 		vec3	light = simd_normalize(simd_make_float3(-3, 3, 3) - hrec.p);
			//
			// 		vec3	target = hrec.p + hrec.n + random_in_unit_sphere();
			// 		float	Brightness = std::max(simd_dot(light, target), 0.0f);
			// 		float	ambient = 0.1f;
			// 		return std::min(Brightness + ambient, 1.0f) * color(Ray(hrec.p, target - hrec.p), world, depth - 1);
			// 	}
			// 	return backgroundSky(r.direction());
			// }
			vec3	color(const rayt::Ray& r, const Shape* world, int depth) {
				HitRec	hrec;

				if (depth <= 0)
					return background(r.direction());
				if (world->hit(r, 0.001f, FLT_MAX, hrec)) {
					vec3		emitted = hrec.mat->emitted(r, hrec); // 物体が光源だった場合はその光を加える
					ScatterRec	srec;
					if (hrec.mat->scatter(r, hrec, srec) && srec.pdf_value > 0) {
						float	spdf_value = hrec.mat->scattering_pdf(srec.ray, hrec);
						vec3	albedo = srec.albedo * spdf_value; // 物体の反射色
						return emitted + albedo * color(srec.ray, world, depth - 1) / srec.pdf_value;
					} else {
						return emitted;
					}
				}
				return background(r.direction());
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
						vec3	c = simd_unit(0.0f);
						for (int s = 0; s<m_samples; s++) {
							float	u = float(i + drand48()) / float(nx);
							float	v = float(j + drand48()) / float(ny);
							Ray	r = m_camera->getRay(u, v);
							c = c + color(r, m_world.get(), 50);
							// .get()はunique_ptrで管理しているオブジェクトのアドレスを渡す所有権は渡さずデータを参照したいときに使われる
						}
						c = c / m_samples;
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
		int			m_samples;
	};

}

