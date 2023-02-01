#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <cstdio>
#include <pthread.h>
#include <chrono>

using namespace std;

// Global variables.
int Z = 0;
int gMinT = 0;
int gMaxT = 0;
int gRangeT = 0;
int gModeT = 0;
float gMedianT = 0;
int gSumT = 0;
float gArithmeticMeanT = 0;
float gHarmonicMeanT = 0;
float gSdT = 0;
float gInterquartileRangeT = 0;

double getMedianT(vector<int> arr, int size)
{
    sort(arr.begin(), arr.end());
    double med;
    if (size % 2 != 0)
    {
        med = (double)arr[size / 2];
    }
    else
    {
        med = (double)(arr[(size - 1) / 2] + arr[size / 2]) / 2.0;
    }
    return med;
}

int getMinT(vector<int> arr)
{
    int min = arr[0];
    for (int i = 0; i < Z; i++)
    {
        if (min > arr[i])
            min = arr[i];
    }
    return min;
}

int getMaxT(vector<int> arr)
{
    int max = arr[0];
    for (int i = 0; i < Z; i++)
    {
        if (max < arr[i])
            max = arr[i];
    }
    return max;
}

int getRangeT(vector<int> arr)
{
    int min = getMinT(arr);
    int max = getMaxT(arr);
    return max - min;
}

// returns the smallest when there is multiple mode.
int getModeT(vector<int> arr)
{
    int max = getMaxT(arr);

    int t = max + 1;
    int count[t];
    for (int i = 0; i < t; i++)
        count[i] = 0;

    for (int i = 0; i < Z; i++)
        count[arr[i]]++;

    int mode = 0;
    int k = count[0];
    for (int i = 1; i < t; i++)
    {
        if (count[i] > k)
        {
            k = count[i];
            mode = i;
        }
    }

    return mode;
}

double getInterquartileRangeT(vector<int> arr)
{
    sort(arr.begin(), arr.end());
    if (Z % 2 != 0)
    {
        vector<int>::const_iterator left1 = arr.begin();
        vector<int>::const_iterator right1 = arr.begin() + (Z - 1) / 2;
        vector<int> leftPart(left1, right1);
        vector<int>::const_iterator left2 = arr.begin() + (Z - 1) / 2 + 1;
        vector<int>::const_iterator right2 = arr.begin() + Z;
        vector<int> rightPart(left2, right2);
        return getMedianT(rightPart, (Z - 1) / 2) - getMedianT(leftPart, (Z - 1) / 2);
    }
    else
    {

        vector<int>::const_iterator left1 = arr.begin();
        vector<int>::const_iterator right1 = arr.begin() + Z / 2;
        vector<int> leftPart(left1, right1);
        vector<int>::const_iterator left2 = arr.begin() + Z / 2;
        vector<int>::const_iterator right2 = arr.begin() + Z;
        vector<int> rightPart(left2, right2);
        return getMedianT(rightPart, Z / 2) - getMedianT(leftPart, Z / 2);
    }
}
int getSumT(vector<int> arr)
{
    int sum = 0;

    for (int i = 0; i < Z; i++)
    {
        sum = sum + arr[i];
    }
    return sum;
}

double getArithmeticMeanT(vector<int> arr)
{
    double sum = getSumT(arr);
    return sum / Z;
}

double getSdT(vector<int> arr)
{
    double sd = 0;
    double mean = getArithmeticMeanT(arr);
    for (int i = 0; i < Z; ++i)
    {
        sd += pow(arr[i] - mean, 2);
    }
    sd = sqrt(sd / (Z - 1));
    return sd;
}

double getHarmonicMeanT(vector<int> arr)
{
    double harmonicSum = 0;
    for (int i = 0; i < Z; i++)
    {
        harmonicSum = harmonicSum + (double)1 / arr[i];
    }
    double harmonicMean = (double)Z / harmonicSum;
    return harmonicMean;
}

//thread functions, assigning

void *threadMinInterquartile(void *param)
{
    vector<int> arr = *((vector<int> *)param);
    gMinT = getMinT(arr);
    gInterquartileRangeT = getInterquartileRangeT(arr);
    pthread_exit(0);
}

void *threadRangeHarmonic(void *param)
{
    vector<int> arr = *((vector<int> *)param);
    gRangeT = getRangeT(arr);
    gHarmonicMeanT = getHarmonicMeanT(arr);
    pthread_exit(0);
}

void *threadMedianSum(void *param)
{
    vector<int> arr = *((vector<int> *)param);
    gMedianT = getMedianT(arr, Z);
    gSumT = getSumT(arr);
    pthread_exit(0);
}

void *threadArithmeticMode(void *param)
{
    vector<int> arr = *((vector<int> *)param);
    gArithmeticMeanT = getArithmeticMeanT(arr);
    gModeT = getModeT(arr);
    pthread_exit(0);
}

void *threadSdMax(void *param)
{
    vector<int> arr = *((vector<int> *)param);
    gSdT = getSdT(arr);
    gMaxT = getMaxT(arr);
    pthread_exit(0);
}

int mainFiveThread(int N)
{
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    Z = N;

    vector<int> randomNumbers(Z);

    for (int i = 0; i < Z; i++)
    {
        randomNumbers[i] = 1000 + (std::rand() % (10000 - 1000 + 1));
    }

    FILE *fp;
    fp = fopen("output2.txt", "w");

    pthread_t tid1, tid2, tid3, tid4, tid5;
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    pthread_create(&tid1, &attr, threadArithmeticMode, &randomNumbers);
    pthread_create(&tid2, &attr, threadMedianSum, &randomNumbers);
    pthread_create(&tid3, &attr, threadMinInterquartile, &randomNumbers);
    pthread_create(&tid4, &attr, threadRangeHarmonic, &randomNumbers);
    pthread_create(&tid5, &attr, threadSdMax, &randomNumbers);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    pthread_join(tid5, NULL);

    fprintf(fp, "%i\n", gMinT);
    fprintf(fp, "%i\n", gMaxT);
    fprintf(fp, "%i\n", gRangeT);
    fprintf(fp, "%i\n", gModeT);
    fprintf(fp, "%.5f\n", gMedianT);
    fprintf(fp, "%i\n", gSumT);
    fprintf(fp, "%.5f\n", gArithmeticMeanT);
    fprintf(fp, "%.5f\n", gHarmonicMeanT);
    fprintf(fp, "%.5f\n", gSdT);
    fprintf(fp, "%.5f", gInterquartileRangeT);

    fclose(fp);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Execution time = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << "[µs]" << endl;
    return 0;
}