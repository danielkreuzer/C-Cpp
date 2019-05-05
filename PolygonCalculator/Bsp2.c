#include <stdio.h>
#include <math.h>

double calcDis1(int x1, int xn,int y1,int yn){
    return sqrt(pow(x1 - xn, 2.0) + pow(y1 - yn, 2.0));
}

double calcDis2(int xi1, int xi, int yi1, int yi)
{
    return sqrt(pow(xi1 - xi, 2.0) + pow(yi1 - yi, 2.0));
}

int main(int argc, char * argv[]){
    if (argc == 1){
        printf("-- %s --\n", argv[0]);
        printf("Dieses Programm berechnet den Umfang eines Polygons\n");
        printf("Bitte geben sie mindestens 3 ganzzahlige Koordinatenpaare ein!");
        return -1;
    } else if (argc < 7 || !(argc % 2)){
        printf("Falsche Anzahl an Koordinatenpaaren!");
    } else {
        printf("    Polygon-Umfangsberechnung   \n");
        int count = 0;
        for (int i = 1; i < argc - 1; i = i + 2)
        {
            count++;
            int inputx = 0;
            int inputy = 0;
            if (sscanf(argv[i], "%d", &inputx) != 1){
                printf("Parameterfehler bei Parameter x%d!\n", count);
                return -2;
            } else {
                printf("x%d: %d \n", count, inputx);
            }

            if (sscanf(argv[i+1], "%d", &inputy) != 1)
            {
                printf("Parameterfehler bei Parameter y%d!\n", count);
                return -2;
            }
            else
            {
                printf("y%d: %d \n", count, inputy);
            }
        }
        printf("\n");
        
        double result = 0;
        int x1, xn, y1, yn = 0;
        sscanf(argv[1], "%d", &x1);
        sscanf(argv[argc - 2], "%d", &xn);
        sscanf(argv[2], "%d", &y1);
        sscanf(argv[argc - 1], "%d", &yn);
        result = calcDis1(x1, xn, y1, yn);
        for(int i = 1; i < argc-2; i = i + 2){
            int xi1, xi, yi1, yi = 0;
            sscanf(argv[i+2], "%d", &xi1);
            sscanf(argv[i], "%d", &xi);
            sscanf(argv[i+3], "%d", &yi1);
            sscanf(argv[i+1], "%d", &yi);
            result = result + calcDis2(xi1, xi, yi1, yi);
        }
        printf("Umfang des Polygon: %lf\n", result);
    }

    return 0;
}