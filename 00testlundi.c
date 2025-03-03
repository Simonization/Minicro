#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct		s_data
{
	int	height;
	int	width;
	char	background;
	char	*matrix;
} t_data

typedef struct	s_rectangle
{
	char	mode;
	float	x;
	float	y;
	float	width;
	float	height;
	char	colour;
} t_rectangle

int	initialize(t_data *data, FILE *file)
{
	if(fscanf(file, "%d %d %c\n", &data->height, &data->width, &data->background) != 3)
		return (1);
	if (data->height <= 0 || data->width <= 0 || data->height > 300 || data->width > 300)
		return (1);
	malloc(data->matrix) = sizeof(char) * (data->height * data->width);
	if (data->matrix == NULL)
	{
		free(data->matrix);
		close(file);
		return (1);
	}
	memset(data->matrix, data->background, data->height * data->width);
	return (0);
}

int	process(t_data *data, FILE *file)
{
	return (1);
}

int	main(int	ac, char *av)
{
	FILE	*file,
	t_data	data;
	int		i;

	if (ac != 2)
		return(write(1, "Error: argument\n", 16), 1)
	if (!initialize(&data, file) || !(file=fopen(av[1], "r")) )
		return(write(1, "Error: OPeration file corrupted\n", 32), 1)
	if(process(&data, file))
	{
		free(data.matrix);
		fclose(file);
		return(write(1, "Error: OPeration file corrupted\n", 32), 1)
	}
	i = -1
	while(++i < data.height)
	{
		write(1, , i * data.width + data.matrix, data.width);
		write(1, "\n", 1);
	}
	return(free(data.matrix), fclose(file), 0);
}
