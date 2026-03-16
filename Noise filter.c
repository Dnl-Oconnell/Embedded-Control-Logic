#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>


void generateSensorData(double arr[], int len, int periods, double noise){
    for(int i = 0; i < len; i++){
        double n = ((double)rand() / RAND_MAX) * (2 * noise) - noise;
        double x = (i * periods * 2 * M_PI) / len;
        double y = sin(x) + n;
        arr[i] = y;
    }
}

void movingAverage(double input[], double output[], int len, int window){
    for(int i = 0; i < len; i++){
        double sum = 0;
        int count = 0;

        for(int j = -window; j <= window; j++){
            if((i + j) >= 0 && (i + j) < len){
                sum += input[i + j];
                count ++;
            }
        }
        output[i] = sum / count;
    }
}

int detectAnomalies(double arr[], int len, double threshold, int anomalies[]){
    int j = 0;

    for(int i = 0; i < len; i ++){
        if(arr[i] > threshold){
            anomalies[j] = i;
            j++;
        }
    }
    return j;
}

void printReport(double noisySignal[], double smoothSignal[], int len, int anomalies[], int numAnomalies){
    printf("Index %-6| Noisy %-18| Smooth %-18| Status|\n");
    printf("------------------------------\n");
    int j = 0;
    for(int i = 0; i < len; i ++){
        if(j < numAnomalies && i == anomalies[j]){
            printf("%-6d| %-10f| %-10f| ALERT\n", i, noisySignal[i], smoothSignal[i]);
            j ++;
        }
        else{
            printf("%-6d| %-10f| %-10f| \n", i, noisySignal[i], smoothSignal[i]);
        }
    }
}


int main(){


    srand(time(NULL));

    int sizeOfArray;
    printf("Enter array size: ");
    scanf("%d", &sizeOfArray);

    int periods;
    printf("\nEnter number of periods to cycle through: ");
    scanf("%d", &periods);

    double noise;
    printf("\nEnter the amount of noise to vary signal: ");
    scanf("%lf", &noise);

    int window;
    do {
        printf("\nEnter an odd number for the width of the averaging window: ");
        scanf("%d", &window);
        if(window % 2 == 0){
            printf("Window size must be odd, please try again:!\n");
        }
    } while(window % 2 == 0);

    double threshold;
    printf("\nEnter the maximum threshold: ");
    scanf("%lf", &threshold);

    double noisySignal[sizeOfArray];
    double smoothSignal[sizeOfArray];
    int anomalies[sizeOfArray];

    generateSensorData(noisySignal, sizeOfArray, periods, noise);
    movingAverage(noisySignal, smoothSignal, sizeOfArray, window);
    int numAnomalies = detectAnomalies(smoothSignal, sizeOfArray, threshold, anomalies);
    printReport(noisySignal, smoothSignal, sizeOfArray, anomalies, numAnomalies);
}
