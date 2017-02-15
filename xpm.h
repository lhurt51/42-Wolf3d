#ifndef XPM_H
# define XPM_H

typedef struct		s_color
{
	char			*def;
	int				color;
}					t_color;

typedef struct		s_tex
{
	t_color			*color;
	char			*name;
	int				height;
	int				width;
	unsigned int	num_color;
	int				pix_w;
	int				pnts[T_SIZE][T_SIZE];
}					t_tex;

int				charhextoint(char *str, int i);

#endif
