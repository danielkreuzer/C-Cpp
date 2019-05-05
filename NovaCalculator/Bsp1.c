#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int main(int argc, char * argv[]){
    char *fuelType = "";
    double fuelcon = 0;
    double nova = 0;

    if (argc != 3){
        printf("Falsche Anzahl an Parameter!");
        return -1;
    } else {
        printf("    NOVA-Rechner    \n");

        if (sscanf(argv[1], "%lf", &fuelcon) != 1){
            printf("Falscher Verbrauchsparameter!\n");
            return -2;
        } else {

            if (strcmp("1", argv[2]) == 0){
                nova = (fuelcon - 2) * 2 * 100;
                fuelType = "Diesel";
            }
            else if (strcmp("2", argv[2]) == 0)
            {
                nova = (fuelcon - 3) * 2 * 100;
                fuelType = "Benzin";
            } else {
                printf("Falscher Kraftstoffparameter!\n");
                return -2;
            }
        
            double result = roundf(nova * 100) / 100;
            printf("Nova fuer einen Verbrauch von %.2fl bei dem Kraftstoff %s: %.2f Euro\n", fuelcon, fuelType, result);

            return 0;
        }
    }
}