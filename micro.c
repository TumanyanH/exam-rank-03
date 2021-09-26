#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int W;
int H;
char SYM;
char **MAP = NULL;

typedef struct s_draw
{
	char type;
	float x;
	float y;
	float width;
	float height;
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
					ret = fscanf(file, "%c %f %f %f %f %c\n", &draw.type, &draw.x, &draw.y, &draw.width, &draw.height, &draw.sym);
					if (ret == -1)
						return (error(file, 0));
					else if(ret != 6 || draw.width <= 0 || draw.height <= 0 ||(draw.type != 'r' && draw.type != 'R'))
						break;
					for (int col = 0; col < H; ++col)
					{
						for (int line = 0; line < W; ++line)
						{
							if (line - draw.x >= 0 && col - draw.y >= 0 && draw.x + draw.width - line >= 0 && draw.y + draw.height - col >= 0)
							{
								if (draw.type == 'r' && (line - draw.x < check || col - draw.y < check || draw.x + draw.width - line < check || draw.y + draw.height - col < check))
									MAP[col][line] = draw.sym;
								else if (draw.type == 'R')
									MAP[col][line] = draw.sym;
							}
						}
					}
				}
			}
		}
	}
	return (error(file, 2));
}
