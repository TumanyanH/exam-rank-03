#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int W;
int H;
char SYM;
char **MAP = NULL;

typedef struct s_draw
{
	char type;
	float x;
	float y;
	float rad;
	char sym;
} t_draw;

int error(FILE *file, int err)
{
	if (err == 2 && (err = 1))
		write (1, "Error: Operation file corrupted\n", 32);
	else if (err == 1)
		write(1, "Error: argument\n", 16);
	else
	{
		for (int i = 0; i < H; ++i)
		{
			write(1, MAP[i], W);
			write(1, "\n", 1);
		}
	}
	if (file)
		fclose (file);
	if (MAP)
	{
		for(int i = 0; i < H; ++i)
			free(MAP[i]);
		free(MAP);
	}
	return (err);
}

int main(int ac, char **av)
{
	t_draw draw;
	FILE *file;
	int ret;
	float check = 1.00000000;
	float dist;

	file = NULL;
	if (ac != 2)
		return(error(file, 1));
	if ((file = fopen(av[1], "r")))
	{
		if((ret = fscanf(file, "%d %d %c\n", &W, &H, &SYM)) == 3)
		{
			if (W > 0 && W <= 300 && H > 0 && H <= 300)
			{
				MAP = malloc(sizeof(char *) * H);
				for (int i = 0; i < H; ++i)
				{
					MAP[i] = malloc(sizeof(char) * W);
					memset(MAP[i], SYM, W);
				}
				while (1)
				{
					ret = fscanf(file, "%c %f %f %f %c\n", &draw.type, &draw.x, &draw.y, &draw.rad, &draw.sym);
					if (ret == -1)
						return (error(file, 0));
					else if(ret != 5 || draw.rad <= 0 || (draw.type != 'c' && draw.type != 'C'))
						break;
					for (int col = 0; col < H; ++col)
					{
						for (int line = 0; line < W; ++line)
						{
							float dist = sqrtf(powf(draw.x - line, 2) + powf(draw.y - col, 2));
							if ((draw.rad - dist >= 0.00000000 && draw.rad - dist < check && draw.type == 'c') || (draw.rad - dist >= 0.00000000 && draw.type == 'C'))
								MAP[col][line] = draw.sym;
						}
					}
				}
			}
		}
	}
	return (error(file, 2));
}
