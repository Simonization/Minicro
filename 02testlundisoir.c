#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct s_data
{
	int	width;
	int	height;
	char	background;
	char	*matrix;
}	t_data;

typedef struct	s_rec
{
	float	width;
	float	height;
	float	x;
	float	y;
	char	mode;
	char	colour;
}	t_rec;

int	iscolour(float x, float y, t_rec rec)
{
	if(x < rec.x || y < rec.y || x > rec.width || y > rec.height)
		return (0);
	if(rec.mode == "R")
		return (1);
	if(x < rec.x + 1.00000000 || x > rec.x +  rec.width - 1.00000000 || y < rec.y + 1.00000000 || y > rec.y + rec.height - 1.00000000)
		return (1);
}

int	initialize(t_data *data, FILE *file)
{
	if (fscanf(file, "%d %d %c\n", &data->width, &data->height, &data->background) != 3)
		return (1);
	if (data->height < 0 || data->height > 300 || data->width < 0 || data->width > 300)
		return (1);
	data.matrix = malloc(sizeof(char * (data->height * data->width));
	if (!data->matrix)
	{
		free(data->matrix);
		fclose(file);
		return (1);
	}
	memset(data->matrix, data->background, (data->height * data->width));
	return (0);
}

int	process(t_data *data, FILE *file)
{
	int	x;
	int	y;
	t_rec	rec;

	while(fscanf(file, "%c %d %d %d %d %c", rec.mode, rec.width, rec.height, rec.x, rec.y, rec.colour) == 6)
	{
		x = -1;
		while(++x < data->width)
		{
			y = -1
			while (++y < data->height)
				if (iscolour((float)x, (float)y, rec))
					data->matrix[y * rec.width + x] = rec.colour;
		}
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;
	FILE	*file;
	int		y;

	if (ac != 2)
		return (write(1, "Error: Argument\n", 16), 1);
	if (!(file = fopen(av[1], "r")) || initialize(&data, file))
		return (write(1, "Error: Corrupted file\n", 32), 1);
	while(process(&data, file))
	{
		y = -1;
		while(++y < data.height)
		{
			write(1, y * data.matrix + data.width, data.width);
			write(1, "\n", 1);
		}
	}
	return (free(data.matrix), fclose(file), 0);
}
