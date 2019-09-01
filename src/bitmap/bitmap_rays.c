#include <prophet/bitmap.h>
#include <prophet/position/square.h>

#include <assert.h>
#include <string.h>

static uint64_t bb_rays[64][8];


/**
 * \brief Return a bitmap representing a ray.
 *
 * Constructs a bitmap representing the sequence of squares starting from
 * \p sq, in the direction of \p dir, to the edge of the board.
 *
 * \param sq            the starting square
 * \param dir           the direction to travel in
 *
 * \return a bitmap representation of the ray
 */
uint64_t ray(square_t sq, dir_t dir)
{
	assert(sq >= 0);
	assert(sq < 64);

	return bb_rays[sq][dir];
}

/**
 * \brief Initialize the bitmap ray function.
 */
void init_bitmap_ray()
{
	memset(bb_rays, 0, sizeof(uint64_t) * 64 * 8);

	for (int i=0; i<64; i++)
	{
		file_t file_i = get_file(i);
		rank_t rank_i = get_rank(i);

		for (int j=0; j<64; j++)
		{
			file_t file_j = get_file(j);
			rank_t rank_j = get_rank(j);
			uint64_t candidate_sq = square_to_bitmap(j);

			/* figure out if j is north or south of i */
			if (file_i == file_j)
			{
				if (j < i)
				{
					bb_rays[i][NORTH] |= candidate_sq;
				}
				else if (j > i)
				{
					bb_rays[i][SOUTH] |= candidate_sq;
				}
			}

			/* figure out if j is east or west of i */
			if (rank_i == rank_j)
			{
				if (j < i)
				{
					bb_rays[i][WEST] |= candidate_sq;
				}
				else if (j > i)
				{
					bb_rays[i][EAST] |= candidate_sq;
				}

			}

			if (is_diagonal(i, j))
			{
				if (rank_j < rank_i)
				{
					if (file_j < file_i)
					{
						bb_rays[i][NORTHWEST] |= candidate_sq;
					}
					else
					{
						bb_rays[i][NORTHEAST] |= candidate_sq;
					}
				}
				else if (rank_j > rank_i)
				{
					if (file_j < file_i)
					{
						bb_rays[i][SOUTHWEST] |= candidate_sq;
					}
					else
					{
						bb_rays[i][SOUTHEAST] |= candidate_sq;
					}
				}
			}
		}
	}
}
