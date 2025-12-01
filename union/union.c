#include <stdio.h>
#include <math.h>

typedef enum {
	SHAPE_CIRCLE,
	SHAPE_RECTANGLE
}	ShapeType;

typedef struct {
	double radius;
}	Circle;

typedef struct {
	double width;
	double height;
}	Rectangle;

typedef struct {
	ShapeType type;
	union {
		Circle circle;
		Rectangle rectangle;
	}	data;
}	Shape;

double calculate_area(const Shape *shape) {
	switch (shape->type) {
		case SHAPE_CIRCLE:
			return M_PI * shape->data.circle.radius * shape->data.circle.radius;
		case SHAPE_RECTANGLE:
			return shape->data.rectangle.width * shape->data.rectangle.height;
		default:
			return 0.0;
	}
}

int	main(void) {
	Shape circle = {
		.type = SHAPE_CIRCLE,
		.data.circle = {.radius = 5.0}
	};

	Shape rectangle = {
		.type = SHAPE_RECTANGLE,
		.data.rectangle = {.width = 4.0, .height = 6.0}
	};

	printf("circle area\t: %.2f\n", calculate_area(&circle));
	printf("rectangle area\t: %.2f\n", calculate_area(&rectangle));

	return 0;
}
