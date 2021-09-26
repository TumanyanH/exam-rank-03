#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

typedef struct rec
{
	int w;
	int h;
	char c;
	char **map;
}	t_rec;

typedef struct s_crl
{
	char type;
	float x;
	float y;
	float r;
	char c;
}	t_crl;

int ft_strlen(char *str)
{
	int len = 0;

	while (str[len])
		++len;
	return (len);
}

int ft_error(char *str)
{
	write(1, str, ft_strlen(str));
	return (1);
}

int ft_end(FILE *file, t_rec *rec, int fail)
{
	fclose(file);
	if (rec->map)
	{
		int i = -1;
		while (++i < rec->h)
			free(rec->map[i]);
		free(rec->map);
	}
	if (fail)
		return (ft_error("Error: Operation file corrupted\n"));
	return (0);
}

int fill(FILE *file, t_rec *rec)
{
	int a, j = -1;
	rec->map = NULL;

	if ((a = fscanf(file, "%d %d %c\n", &rec->w, &rec->h, &rec->c)) != 3)
		return (1);
	if (rec->w > 300 || rec->w <= 0 || rec->h > 300 || rec->h <= 0)
		return (1);
	rec->map = (char **)malloc((rec->h + 1) * sizeof(char *));
	while (++j < rec->h)
	{
		rec->map[j] = (char *)malloc((rec->w + 1) * sizeof(char));
		rec->map[j][rec->w] = '\0';
		memset(rec->map[j], rec->c, rec->w);
	}
	rec->map[j] = NULL;
	return (0);
}

int	in_crl(float i, float j, t_crl *crl)
{
	float check = 1.00000000;
	float dist = sqrtf(powf(crl->x - i, 2) + powf(crl->y - j, 2));

	if (crl->r - dist < 0.00000000)
		return (0);
	if (crl->r - dist >= 0.00000000 && crl->r - dist < check)
		return (2);
	return (1);
}

int	rec_fill(FILE *file, t_rec *rec)
{
	t_crl crl;
	int a, i, j, in_check;

	while ((a = fscanf(file, "%c %f %f %f %c\n", &crl.type, &crl.x, &crl.y, &crl.r, &crl.c)) == 5)
	{
		if ((crl.type != 'c' && crl.type != 'C') || crl.r <= 0.00000000)
			return (1);
		j = 0;
		while (j < rec->h)
		{
			i = 0;
			while (i < rec->w)
			{
				in_check = in_crl(i, j, &crl);
				if ((in_check == 2 && crl.type == 'c') || (in_check && crl.type == 'C'))
					rec->map[j][i] = crl.c;
				++i;
			}
			++j;
		}
	}
	if (a == -1)
		return (0);
	return (1);
}

int draw(t_rec *rec)
{
	int i, j = 0;

	while (j < rec->h)
	{
		i = 0;
		while (i < rec->w)
		{
			write(1, &rec->map[j][i], 1);
			++i;
		}
		write(1, "\n", 1);
		++j;
	}
	return (0);
}

int main(int argc, char **argv)
{
	FILE *file;
	t_rec rec;

	file = fopen(argv[1], "r");
	if (argc != 2)
		return (ft_error("Error: argument\n"));
	if (!file)
		return (ft_error("Error: Operation file corrupted\n"));
	if (fill(file, &rec))
		return (ft_end(file, &rec, 1));
	if (rec_fill(file, &rec))
		return (ft_end(file, &rec, 1));
	draw(&rec);
	return (ft_end(file, &rec, 0));
}
