#include <stdint.h>

/**
 * \brief Generate a 32 bit random number.
 *
 * The 32 bit random number generator below was taken from Crafty by Bob Hyatt.
 * This same algorithm is in GNU Chess.
 *
 * The following comments are from Crafty:
 *
 * A 32 bit random number generator. An implementation in C of the algorithm 
 * given by Knuth, the art of computer programming, vol. 2, pp. 26-27. We use 
 * e=32, so we have to evaluate y(n) = y(n - 24) + y(n - 55) mod 2^32, which
 * is implicitly done by unsigned arithmetic.
 *
 * \return the 32 bit random number.
 */
uint32_t random32()
{
    /*
      random numbers from Mathematica 2.0.
      SeedRandom = 1;
      Table[Random[Integer, {0, 2^32 - 1}]
     */
    static unsigned long x[55] = {
        1410651636UL, 3012776752UL, 3497475623UL, 2892145026UL, 1571949714UL,
        3253082284UL, 3489895018UL, 387949491UL, 2597396737UL, 1981903553UL,
        3160251843UL, 129444464UL, 1851443344UL, 4156445905UL, 224604922UL,
        1455067070UL, 3953493484UL, 1460937157UL, 2528362617UL, 317430674UL,
        3229354360UL, 117491133UL, 832845075UL, 1961600170UL, 1321557429UL,
        747750121UL, 545747446UL, 810476036UL, 503334515UL, 4088144633UL,
        2824216555UL, 3738252341UL, 3493754131UL, 3672533954UL, 29494241UL,
        1180928407UL, 4213624418UL, 33062851UL, 3221315737UL, 1145213552UL,
        2957984897UL, 4078668503UL, 2262661702UL, 65478801UL, 2527208841UL,
        1960622036UL, 315685891UL, 1196037864UL, 804614524UL, 1421733266UL,
        2017105031UL, 3882325900UL, 810735053UL, 384606609UL, 2393861397UL };

    static int init = 1;
    static unsigned long y[55];
    static int j, k;
    unsigned long ul;

    if (init) {
        int i;

        init = 0;
        for (i = 0; i < 55; i++) {
            y[i] = x[i];
        }
        j = 24 - 1;
        k = 55 - 1;
    }

    ul = (y[k] += y[j]);
    if (--j < 0) {
        j = 55 - 1;
    }
    if (--k < 0) {
        k = 55 - 1;
    }

    return((uint32_t)ul);
}

/**
 * \brief Generate a 64 bit random number.
 *
 * \return The 64 bit random number.
 */
uint64_t random64()
{
    uint64_t r1 = random32();
    uint64_t r2 = random32();
    return ((r1<<32)|r2);
}
