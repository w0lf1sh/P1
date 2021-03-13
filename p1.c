#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "pav_analysis.h"
#include "fic_wave.h"

int main(int argc, char *argv[])
{
    float durTrm = 0.010;
    float fm;
    int N;
    int trm;
    float *x;
    short *buffer;
    FILE *fpWave;

    if (argc != 2 && argc != 3)
    {
        fprintf(stderr, "Empleo: %s inputfile [outputfile]\n", argv[0]);
        return -1;
    }

    if ((fpWave = abre_wave(argv[1], &fm)) == NULL)
    {
        fprintf(stderr, "Error al abrir %s (%s)\n", argv[1], strerror(errno));
        return -1;
    }

    N = durTrm * fm;        //calcula el numero fr muestras que tenemos en 10 ms (durTrm)

    //ubica matrices para almacenar la senal que va a leer (buffer) y la que va a procesar (x)
    if ((buffer = malloc(N * sizeof(*buffer))) == 0 ||
        (x = malloc(N * sizeof(*x))) == 0)
    {
        fprintf(stderr, "Error al ubicar los vectores (%s)\n", strerror(errno)); //errno = num error sistema
        return -1;
    }

    trm = 0;
    while (lee_wave(buffer, sizeof(*buffer), N, fpWave) == N) //mientras lo que leamos sea igual a N
    {
        for (int n = 0; n < N; n++)
            x[n] = buffer[n] / (float)(1 << 15); //dividimos todas las muestras por 2^15 (valor maximo de una senal de 16 bits)
        //el margen de valores pasa de 32000:-32000 a 1:-1
        printf("%d\t%f\t%f\t%f\n", trm, compute_power(x, N),
               compute_am(x, N),
               compute_zcr(x, N, fm));
        trm += 1;
    }

    cierra_wave(fpWave);
    free(buffer);
    free(x);
    return 0;
}