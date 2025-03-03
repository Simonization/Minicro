#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct s_data
{
	int				width;
	int				height;
	char			background;
	char			*matrix;
}	t_data;

typedef struct s_rectangle
{
	float		width;
	float		height;
	float	x;
	float	y;
	char	mode;
	char	colour;
}	t_rectangle;

int	iscolour(float x, float y, t_rectangle rectangle)
{
	if (x < rectangle.x || x > rectangle.x + rectangle.width || y < rectangle.y || y > rectangle.y + rectangle.height)
		return (0);
	if(rectangle.mode == 'R')
		return(1);
	if (x < rectangle.x + 1.00000000 || x > rectangle.x + rectangle.width - 1.00000000 || y < rectangle.y + 1.00000000 || y > rectangle.y + rectangle.width -1.00000000)
		return(1);
	return(0);
}

int	initialize(t_data *data, FILE *file)
{
	if (fscanf(file, "%d %d %c \n", &data->width, &data->height, &data->background) != 3)
		return(1);
	if (data->width <= 0 || data->width > 300 || data->height <= 0 || data->height > 300)
		return(1);
	data->matrix = malloc(sizeof(char) * (data->height *data->width));
	if(!data->matrix)
		return(1);
	memset(data->matrix, data->background, data->height * data->width);
	return(0);
}

int	process(t_data	*data, FILE *file)
{
	int	x;
	int	y;
	t_rectangle	rectangle;

	while(fscanf(file, "%c %f %f %f %f %c", &rectangle.mode, &rectangle.x, &rectangle.y,
		&rectangle.width, &rectangle.height, &rectangle.colour) == 6)
	{
		if((rectangle.mode != 'r' && rectangle.mode != 'R') || rectangle.width <= 0.00000000 || rectangle.height <= 0.00000000)
			return (1);
		x = -1;
		while(++x < data->width)
		{
			{
				y = -1;
				while(++y < data->height)
					if (iscolour((float)x, (float)y, rectangle))
						data->matrix[y * data->width + x] = rectangle.colour; 
			}
		}
	}
	return (0);
}

int	main(int ac, char **av)
{
	int		y;
	FILE	*file;
	t_data	data;

	if(ac != 2)
		write(1, "Error: argument\n", 16);
	if  (!(file = fopen(av[1], "r")) || initialize(&data, file) )
		return(write(1, "Error: Operation file Corrupted\n" , 32), 1);
	if(process(&data, file))
	{
		free(data.matrix);
		fclose(file);
		return ((write(1, "Error: Operation File Corrupted\n", 32)),1);
	}
	y = -1;
	while (++y < data.height)
	{
		write(1, y * data.width + data.matrix, data.width);
		write(1, "\n", 1);
	}
	return(free(data.matrix), fclose(file), 0);
}

/*


int	iscolour(float x, float y, t_rectangle *rectangle)
{
	if (x < rectangle.x || x > rectangle.x + rectangle.width || y < rectangle.y || y > rectangle.y + rectangle.height)
		return(0);
	if (rectangle.mode == "R");
		return(1);
	if (x < rectangle.x + 1.00000000 || x > rectangle.x + rectangle.width -1.00000000 || y < rectangle.y + 1.00000000 || y > rectangle.y + rectangle.height -1.00000000)
		return(1);
	return (0);
}

int	iscolour(float x, float y, t_circle circle)
{
	float	distance;
	distance = sqrtf((x - circle.x) * (x - circle.x) + (y - circle.y) * (y - circle.y));
	if (distance > circle.radius)
		return (0);
	if (circle.mode == "R")
		return (1);
	if (distance > circle.radius - - 1.00000000)
	return (0);
}

*/
