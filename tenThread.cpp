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
int N = 0;
int gMin = 0;
int gMax = 0;
int gRange = 0;
int gMode = 0;
float gMedian = 0;
int gSum = 0;
float gArithmeticMean = 0;
float gHarmonicMean = 0;
float gSd = 0;
float gInterquartileRange = 0;

double getMedian(vector<int> arr, int size)
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
int getMin(vector<int> arr)
{
    int min = arr[0];
    for (int i = 0; i < N; i++)
    {
        if (min > arr[i])
            min = arr[i];
    }
    return min;
}

int getMax(vector<int> arr)
{
    int max = arr[0];
    for (int i = 0; i < N; i++)
    {
        if (max < arr[i])
            max = arr[i];
    }
    return max;
}

void *getRange(void *param)
{
    vector<int> arr = *((vector<int> *)param);
    int min = getMin(arr);
    int max = getMax(arr);

    gRange = max - min;
    pthread_exit(0);
}

// returns the smallest when there is multiple mode.

void *getMode(void *param)
{
    vector<int> arr = *((vector<int> *)param);

    int max = getMax(arr);

    int t = max + 1;
    int count[t];
    for (int i = 0; i < t; i++)
        count[i] = 0;

    for (int i = 0; i < N; i++)
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

    gMode = mode;
    pthread_exit(0);
}

void *getInterquartileRange(void *param)
{
    vector<int> arr = *((vector<int> *)param);
    sort(arr.begin(), arr.end());
    if (N % 2 != 0)
    {
        vector<int>::const_iterator left1 = arr.begin();
        vector<int>::const_iterator right1 = arr.begin() + (N - 1) / 2;
        vector<int> leftPart(left1, right1);
        vector<int>::const_iterator left2 = arr.begin() + (N - 1) / 2 + 1;
        vector<int>::const_iterator right2 = arr.begin() + N;
        vector<int> rightPart(left2, right2);

        gInterquartileRange = getMedian(rightPart, (N - 1) / 2) - getMedian(leftPart, (N - 1) / 2);
    }
    else
    {
        vector<int>::const_iterator left1 = arr.begin();
        vector<int>::const_iterator right1 = arr.begin() + N / 2;
        vector<int> leftPart(left1, right1);
        vector<int>::const_iterator left2 = arr.begin() + N / 2;
        vector<int>::const_iterator right2 = arr.begin() + N;
        vector<int> rightPart(left2, right2);

        gInterquartileRange = getMedian(rightPart, N / 2) - getMedian(leftPart, N / 2);
    }
    pthread_exit(0);
}
int getSum(vector<int> arr)
{
    int sum = 0;

    for (int i = 0; i < N; i++)
    {
        sum = sum + arr[i];
    }
    return sum;
}

double getArithmeticMean(vector<int> arr)
{
    double sum = getSum(arr);
    return sum / N;
}

void *getSd(void *param)
{
    vector<int> arr = *((vector<int> *)param);
    double sd = 0;
    double mean = getArithmeticMean(arr);
    for (int i = 0; i < N; ++i)
    {
        sd += pow(arr[i] - mean, 2);
    }
    gSd = sqrt(sd / (N - 1));
    pthread_exit(0);
}

void *getHarmonicMean(void *param)
{
    vector<int> arr = *((vector<int> *)param);
    double harmonicSum = 0;
    for (int i = 0; i < N; i++)
    {
        harmonicSum = harmonicSum + (double)1 / arr[i];
    }
    double harmonicMean = (double)N / harmonicSum;
    gHarmonicMean = harmonicMean;
    pthread_exit(0);
}

void *threadSum(void *param)
{
    vector<int> arr = *((vector<int> *)param);
    gSum = getSum(arr);
    pthread_exit(0);
}

void *threadMin(void *param)
{
    vector<int> arr = *((vector<int> *)param);
    gMin = getMin(arr);
    pthread_exit(0);
}

void *threadMax(void *param)
{
    vector<int> arr = *((vector<int> *)param);
    gMax = getMax(arr);
    pthread_exit(0);
}

void *threadMedian(void *param)
{
    vector<int> arr = *((vector<int> *)param);
    gMedian = getMedian(arr, N);
    pthread_exit(0);
}

void *threadArithmeticMean(void *param)
{
    vector<int> arr = *((vector<int> *)param);
    gArithmeticMean = getArithmeticMean(arr);
    pthread_exit(0);
}

int mainTenThread(int Z)
{
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    N = Z;

    vector<int> randomNumbers(N);

    for (int i = 0; i < N; i++)
    {
        randomNumbers[i] = 1000 + (rand() % (10000 - 1000 + 1));
    }

    FILE *fp;
    fp = fopen("output2.txt", "w");

    pthread_t tid1, tid2, tid3, tid4, tid5, tid6, tid7, tid8, tid9, tid10;
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    pthread_create(&tid1, &attr, threadMin, &randomNumbers);
    pthread_create(&tid2, &attr, threadMax, &randomNumbers);
    pthread_create(&tid3, &attr, getRange, &randomNumbers);
    pthread_create(&tid4, &attr, getMode, &randomNumbers);
    pthread_create(&tid5, &attr, threadMedian, &randomNumbers);
    pthread_create(&tid6, &attr, threadSum, &randomNumbers);
    pthread_create(&tid7, &attr, threadArithmeticMean, &randomNumbers);
    pthread_create(&tid8, &attr, getHarmonicMean, &randomNumbers);
    pthread_create(&tid9, &attr, getSd, &randomNumbers);
    pthread_create(&tid10, &attr, getInterquartileRange, &randomNumbers);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    pthread_join(tid5, NULL);
    pthread_join(tid6, NULL);
    pthread_join(tid7, NULL);
    pthread_join(tid8, NULL);
    pthread_join(tid9, NULL);
    pthread_join(tid10, NULL);

    fprintf(fp, "%i\n", gMin);
    fprintf(fp, "%i\n", gMax);
    fprintf(fp, "%i\n", gRange);
    fprintf(fp, "%i\n", gMode);
    fprintf(fp, "%.5f\n", gMedian);
    fprintf(fp, "%i\n", gSum);
    fprintf(fp, "%.5f\n", gArithmeticMean);
    fprintf(fp, "%.5f\n", gHarmonicMean);
    fprintf(fp, "%.5f\n", gSd);
    fprintf(fp, "%.5f", gInterquartileRange);

    fclose(fp);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Execution time = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << "[µs]" << endl;

    return 0;
}