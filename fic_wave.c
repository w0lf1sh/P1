#include <stdio.h>
#include "fic_wave.h"
#include <math.h>

FILE *abre_wave(const char *ficWave, float *fm)
{
    FILE *fpWave;

    if ((fpWave = fopen(ficWave, "r")) == NULL)
        return NULL;

    //Buscamos la frecuencia de muestreo en la cabecera

    unsigned char freqB[4];
    char freqHex[9];
    *fm = 0;
    fseek(fpWave, 24, SEEK_SET);                                                  //Nos colocamos en el byte 24
    fread(freqB, 1, 4, fpWave);                                                   //Leemos los 4 bytes que ocupa en memoria la frecuencia de muestreo
    sprintf(freqHex, "%02x%02x%02x%02x", freqB[3], freqB[2], freqB[1], freqB[0]); //Guardamos en hexadecimal el resultado                                                                               //de la lectura (little endian)
                                                                                  //Obtenemos los coeficientes para posterior conversión a partir de los codigos ASCII
    float freqNum[8];
    for (int i = 0; i < 8; i++)
    {
        if (freqHex[i] == 'a')
        {
            freqNum[i] = 10;
        }
        else if (freqHex[i] == 'b')
        {
            freqNum[i] = 11;
        }
        else if (freqHex[i] == 'c')
        {
            freqNum[i] = 12;
        }
        else if (freqHex[i] == 'd')
        {
            freqNum[i] = 13;
        }
        else if (freqHex[i] == 'e')
        {
            freqNum[i] = 14;
        }
        else if (freqHex[i] == 'f')
        {
            freqNum[i] = 15;
        }
        else
        {
            freqNum[i] = freqHex[i] - 48;
        }
    }
    //realizamos la conversión final, teniendo en cuenta que la data está presentada en little endian
    for (int i = 0; i < 8; i++)
    {
        *fm += freqNum[i] * pow(16, 7 - i);
    }
    printf("\nResultado final: %0.2f\n", *fm);
    rewind(fpWave);

    if (fseek(fpWave, 44, SEEK_SET) < 0)
        return NULL;

    return fpWave;
}

size_t lee_wave(void *x, size_t size, size_t nmemb, FILE *fpWave)
{
    return fread(x, size, nmemb, fpWave);
}

void cierra_wave(FILE *fpWave)
{
    fclose(fpWave);
}