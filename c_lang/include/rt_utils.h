#ifndef RT_UTILS_H
# define RT_UTILS_H

double	random_double(double min, double max);
int		random_int(int min, int max);
double	clamp(double x, double min, double max);
double	pow2(double a);
int		skip_spaces(char **ptr);
int		skip_digit(char **ptr);

#endif