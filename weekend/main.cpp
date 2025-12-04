#include "rtweekend.h"

#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "color.h"
#include "material.h"
#include "moving_sphere.h"

#include <iostream>

hittable_list random_scene() {
  hittable_list world;

  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

      if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;
        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
		  auto center2 = center + vec3(0, random_double(0, .5), 0);
          world.add(make_shared<moving_sphere>(
			center, center2, 0.0, 1.0, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  return world;
}

color	ray_color(const ray& r, const hittable& world, int depth) {
	hit_record	rec;

	if (depth <= 0)
		return color(0,0,0); // shadow
	if (world.hit(r, 0.001, infinity, rec)) {
		ray		scattered;
		color	attenuation;

		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return color(0,0,0);
	}

	vec3	unit_direction = unit_vector(r.direction());
	auto	t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t)*color(1.0,1.0,1.0) + t*color(0.5, 0.7, 1.0); // environment light
}

int	main(void) {
	const auto	aspect_ratio = 16.0 / 9.0;
	const int	image_width = 3840;
	const int	image_height = static_cast<int>(image_width / aspect_ratio);
	const int	samples_per_pixel = 100;
	const int	max_depth = 50;

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	auto	viewport_height = 2.0;
	auto	viewport_width = aspect_ratio * viewport_height;
	auto	focal_length = 1.0;

	auto	origin = point3(0,0,0);
	auto	horizontal = vec3(viewport_width, 0, 0);
	auto	vertical = vec3(0, viewport_height, 0);
	auto	lower_left_corner = 
		origin - horizontal/2 - vertical/2 - vec3(0,0,focal_length);

	hittable_list	world = random_scene();
	// world.add(make_shared<sphere>(point3(0,0,-1), 0.5, make_shared<lambertian>(color(0.7,0.3,0.3))));
	// world.add(make_shared<sphere>(point3(0,-100.5,-1), 100, make_shared<lambertian>(color(0.8,0.8,0.0))));
	// world.add(make_shared<sphere>(point3(0.5,0,-1),0.5,make_shared<metal>(color(1.0,1.0,1.0), 0.0)));
	// world.add(make_shared<sphere>(point3(-0.5,0,-1),-0.5,make_shared<dielectric>(1.5)));
	// world.add(make_shared<sphere>(
	// point3( 0,      0, -1),   0.5, make_shared<metal>(color(1.0,1.0,1.0), 0.0)));
	// world.add(make_shared<sphere>(
	// point3( 0, -100.5, -1),   100, make_shared<lambertian>(color(0.8, 0.8, 0.0))));
	// world.add(make_shared<sphere>(
	// point3( 1,      0, -1),   0.5, make_shared<metal>(color(0.8, 0.6, 0.2), 0.3)));
	// world.add(make_shared<sphere>(
	// point3(-1,      0, -1),   0.5, make_shared<dielectric>(1.5)));
	// world.add(make_shared<sphere>(
	// point3(-1,      0, -1), -0.45, make_shared<dielectric>(1.5)));
	// world.add(make_shared<sphere>(
	// point3(-0.5, 0.5*sqrt(3), -1), 0.5, make_shared<metal>(color(0.7,0.2,0.2), 0.5)));
	// world.add(make_shared<sphere>(
	// point3(0.5, 0.5*sqrt(3), -1), 0.5, make_shared<dielectric>(1.5)));
	// world.add(make_shared<sphere>(
	// point3(0,sqrt(3),-1), 0.5, make_shared<lambertian>(color(0.1, 0.2, 0.5))));

	point3	lookfrom(13,2,3);
	point3	lookat(0,0,0);
	vec3	vup(0,1,0);
	auto	dist_to_focus = (lookfrom-lookat).length();
	auto	aperture = 0.1;
	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
	#pragma omp parallel for schedule(dynamic, 1)
	for (int j = image_height-1; 0<=j; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			color pixel_color(0,0,0);
			for (int s = 0; s < samples_per_pixel; ++s) {
				auto u = double(i + random_double()) / (image_width-1);
				auto v = double(j + random_double()) / (image_height-1);
				ray	r = cam.get_ray(u,v);
				pixel_color += ray_color(r, world, max_depth);
			}
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";
}

// #include "rtweekend.h"
//
// #include "hittable_list.h"
// #include "sphere.h"
// #include "camera.h"
// #include "color.h"
// #include "material.h"
// #include "moving_sphere.h"
//
// #include <iostream>
// #include <vector>
// #include <thread>
// #include <atomic>
// #include <mutex>
//
// hittable_list random_scene() {
//   hittable_list world;
//
//   auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
//   world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));
//
//   for (int a = -11; a < 11; a++) {
//     for (int b = -11; b < 11; b++) {
//       auto choose_mat = random_double();
//       point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());
//
//       if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
//         shared_ptr<material> sphere_material;
//         if (choose_mat < 0.8) {
//           auto albedo = color::random() * color::random();
//           sphere_material = make_shared<lambertian>(albedo);
//           auto center2 = center + vec3(0, random_double(0, .5), 0);
//           world.add(make_shared<moving_sphere>(
//             center, center2, 0.0, 1.0, 0.2, sphere_material));
//         } else if (choose_mat < 0.95) {
//           auto albedo = color::random(0.5, 1);
//           auto fuzz = random_double(0, 0.5);
//           sphere_material = make_shared<metal>(albedo, fuzz);
//           world.add(make_shared<sphere>(center, 0.2, sphere_material));
//         } else {
//           sphere_material = make_shared<dielectric>(1.5);
//           world.add(make_shared<sphere>(center, 0.2, sphere_material));
//         }
//       }
//     }
//   }
//
//   auto material1 = make_shared<dielectric>(1.5);
//   world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
//   auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
//   world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
//   auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
//   world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
//
//   return world;
// }
//
// color ray_color(const ray& r, const hittable& world, int depth) {
//   hit_record rec;
//
//   if (depth <= 0)
//     return color(0,0,0);
//   if (world.hit(r, 0.001, infinity, rec)) {
//     ray     scattered;
//     color   attenuation;
//
//     if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
//       return attenuation * ray_color(scattered, world, depth - 1);
//     return color(0,0,0);
//   }
//
//   vec3 unit_direction = unit_vector(r.direction());
//   auto t = 0.5 * (unit_direction.y() + 1.0);
//   return (1.0 - t)*color(1.0,1.0,1.0) + t*color(0.5, 0.7, 1.0);
// }
//
// int main(void) {
//   const auto aspect_ratio = 16.0 / 9.0;
//   const int  image_width = 3840;
//   const int  image_height = static_cast<int>(image_width / aspect_ratio);
//   const int  samples_per_pixel = 100;
//   const int  max_depth = 50;
//
//   hittable_list world = random_scene();
//
//   point3 lookfrom(13,2,3);
//   point3 lookat(0,0,0);
//   vec3   vup(0,1,0);
//   auto   dist_to_focus = (lookfrom-lookat).length();
//   auto   aperture = 0.1;
//   camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
//
//   // ピクセルバッファを確保
//   std::vector<color> framebuffer(image_width * image_height);
//
//   // スレッド数（M3は8コア）
//   const int num_threads = std::thread::hardware_concurrency();
//   std::vector<std::thread> threads;
//
//   // 進捗表示用
//   std::atomic<int> lines_done(0);
//   std::mutex progress_mutex;
//
//   // 各スレッドが担当する行を計算するワーカー関数
//   auto render_rows = [&](int thread_id) {
//     for (int j = image_height - 1 - thread_id; j >= 0; j -= num_threads) {
//       for (int i = 0; i < image_width; ++i) {
//         color pixel_color(0,0,0);
//         for (int s = 0; s < samples_per_pixel; ++s) {
//           auto u = double(i + random_double()) / (image_width - 1);
//           auto v = double(j + random_double()) / (image_height - 1);
//           ray r = cam.get_ray(u, v);
//           pixel_color += ray_color(r, world, max_depth);
//         }
//         int index = (image_height - 1 - j) * image_width + i;
//         framebuffer[index] = pixel_color;
//       }
//
//       // 進捗表示
//       int done = ++lines_done;
//       if (done % 50 == 0 || done == image_height) {
//         std::lock_guard<std::mutex> lock(progress_mutex);
//         std::cerr << "\rProgress: " << done << "/" << image_height << " lines" << std::flush;
//       }
//     }
//   };
//
//   // スレッド起動
//   std::cerr << "Rendering with " << num_threads << " threads...\n";
//   for (int t = 0; t < num_threads; ++t) {
//     threads.emplace_back(render_rows, t);
//   }
//
//   // 全スレッドの完了を待つ
//   for (auto& th : threads) {
//     th.join();
//   }
//
//   std::cerr << "\nRendering complete. Writing output...\n";
//
//   // PPMヘッダ出力
//   std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
//
//   // バッファから順番に出力
//   for (int idx = 0; idx < image_width * image_height; ++idx) {
//     write_color(std::cout, framebuffer[idx], samples_per_pixel);
//   }
//
//   std::cerr << "Done.\n";
// }
