#include "rayt.h"

int	main(void) {
	int	nx = 800;
	int	ny = 400;
	int	ns = 50;

	std::unique_ptr<rayt::Scene> scene(std::make_unique<rayt::Scene>(nx, ny, ns));
	scene->render();
	return 0;
}
