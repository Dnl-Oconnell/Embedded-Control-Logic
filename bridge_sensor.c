/*
Bridge Sensor Data Filter & Anomaly Detector
-----------------------------------------------
Simulates noisy sensor readings from a bridge strain gauge,
applies a moving average filter, and detects anomalies.
 
Functions:
  generateSensorData() - generates noisy sine wave signal
  movingAverage()      - smooths signal using moving average
  detectAnomalies()    - flags readings above threshold
  printReport()        - prints formatted results table
 
Usage: run and follow input prompts for array size,
    noise level, periods, window size and threshold
 */


#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

// Generating a noisy signal
void generateSensorData(double arr[], int len, int periods, double noise){
    for(int i = 0; i < len; i++){

        // randomising an offset to add to the sin wave
        double n = ((double)rand() / RAND_MAX) * (2 * noise) - noise;
        // to not overflow the array, the amount of periods will be contained within the bounds of 
        // the length of the array, multiplied by 2 PI
        double x = (i * periods * 2 * M_PI) / len;
        double y = sin(x) + n;
        arr[i] = y;
    }
}

// Using a "window" of values to find the average of a number of values to smooth out the noise 
void movingAverage(double input[], double output[], int len, int window){
    for(int i = 0; i < len; i++){
        double sum = 0;
        int count = 0;

        // need to account for any numbers the program may want to count outside of the bounds of
        // the array
        for(int j = -window; j <= window; j++){
            if((i + j) >= 0 && (i + j) < len){
                sum += input[i + j];
                // keeps track of the number of values added together for average calculation
                count ++;
            }
        }
        output[i] = sum / count;
    }
}

// finds if any of the values in the smoothed signal are above a certain threshold. If so, their position
// fill be stored and the amount of anomalies will be returned
int detectAnomalies(double arr[], int len, double threshold, int anomalies[]){
    int j = 0;

    for(int i = 0; i < len; i ++){
        // if value in arr[] is larger than the threshold, store the location (value of i)
        if(arr[i] > threshold){
            anomalies[j] = i;
            j++;
        }
    }
    return j;
}

// printing the results in a neat table with an alert in case the values exceeded the threshold
void printReport(double noisySignal[], double smoothSignal[], int len, int anomalies[], int numAnomalies){
    printf("%-6s| %-9s | %-9s | %s\n", "Index", "Noisy", "Smooth", "Status");
    printf("------|-----------|-----------|---------\n");
    int j = 0;
    for(int i = 0; i < len; i ++){
        // checks if the current value of i is also a value above the error from detectAnomalies
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
    // seeding randomiser with time
    srand(time(NULL));

    // asks user for each variable input
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
