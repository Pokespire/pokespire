#include <os.h>
#include "n2DLib/n2DLib.h"
#include "map.h"

#if INTERFACE
typedef struct
{
	unsigned w;
	unsigned h;
	unsigned *layer0;
	unsigned *layer1;
} Map;
#endif

void map_draw(unsigned x, unsigned y, const Map map)
{
	x += 20;
	y += 12;

	unsigned offset_x = x % 24 * -1;
	unsigned offset_y = y % 24 * -1;

	unsigned i, j, tile_offset;
	Rect sprite;
	sprite.y = 0;
	sprite.w = 24;
	sprite.h = 24;

	tile_offset = x / 24 - 7 + (y / 24 - 5) * map.w;

	for (j = 0; j < 11; j++)
	{
		for (i = 0; i < 15; i++)
		{
			sprite.x = map.layer0[tile_offset] * 24;
			drawSpritePart(tiles, offset_x, offset_y, &sprite);

			tile_offset++;
			offset_x += 24;
		}

		tile_offset += map.w - 15;
		offset_x -= 24 * 15;
		offset_y += 24;
	}
}

unsigned map_collide(unsigned x, unsigned y, const Map map)
{
	return 0;
}

static void map_walk_speed_load(unsigned time)
{
	timer_load(0, time);
	while (timer_read(0) != time);
}

static unsigned map_walk_speed_read(unsigned time, unsigned div)
{
	return (time - timer_read(0)) / div;
}

void map_walk(unsigned x, unsigned y, Map map)
{
	unsigned i, walk_time, walk_div;

	while (!isKeyPressed(KEY_NSPIRE_ESC))
	{
		if (isKeyPressed(KEY_NSPIRE_VAR))
		{
			walk_time = 6554;
			walk_div = 273;
		}
		else
		{
			walk_time = 13107;
			walk_div = 546;
		}

		if (isKeyPressed(KEY_NSPIRE_5) && !map_collide(x, y + 1, map))
		{
			map_walk_speed_load(walk_time);

			for (i = 1; i < 24; i++)
			{
				if (i >= map_walk_speed_read(walk_time, walk_div))
				{
					while (i > map_walk_speed_read(walk_time, walk_div));
					map_draw(x * 24, y * 24 + i, map);
					bufferSwap();
				}
			}

			y++;
		}

		else if (isKeyPressed(KEY_NSPIRE_8) && !map_collide(x, y - 1, map))
		{
			map_walk_speed_load(walk_time);

			for (i = 1; i < 24; i++)
			{
				if (i >= map_walk_speed_read(walk_time, walk_div))
				{
					while (i > map_walk_speed_read(walk_time, walk_div));
					map_draw(x * 24, y * 24 - i, map);
					bufferSwap();
				}
			}

			y--;
		}

		else if (isKeyPressed(KEY_NSPIRE_4) && !map_collide(x - 1, y, map))
		{
			map_walk_speed_load(walk_time);

			for (i = 1; i < 24; i++)
			{
				if (i >= map_walk_speed_read(walk_time, walk_div))
				{
					while (i > map_walk_speed_read(walk_time, walk_div));
					map_draw(x * 24 - i, y * 24, map);
					bufferSwap();
				}
			}

			x--;
		}

		else if (isKeyPressed(KEY_NSPIRE_6) && !map_collide(x + 1, y, map))
		{
			map_walk_speed_load(walk_time);

			for (i = 1; i < 24; i++)
			{
				if (i >= map_walk_speed_read(walk_time, walk_div))
				{
					while (i > map_walk_speed_read(walk_time, walk_div));
					map_draw(x * 24 + i, y * 24, map);
					bufferSwap();
				}
			}

			x++;
		}

		map_draw(x * 24, y * 24, map);
		bufferSwap();
	}
}

