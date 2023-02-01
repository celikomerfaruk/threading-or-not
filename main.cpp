#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <cstdio>
#include <chrono>
using namespace std;
int mainFirstPart(int N);
int mainFiveThread(int Z);
int mainTenThread(int N);

//starting point of project
int main(int argc, char **argv)
{
    printf("If you want to specify thread count (1, 5 or 10) execute program with './main {Array Size} {Thread Count}'. If you want to execute program on 1 thread you can also command './main {Array Size}' \n");
    if (argc > 1)
    {
        int N = stoi(argv[1]);
        if (argc == 3)
        {
            int threadCount = stoi(argv[2]);

            if (threadCount == 5)
            {
                mainFiveThread(N);
            }
            else if (threadCount == 10)
            {
                mainTenThread(N);
            }
            else
            {
                mainFirstPart(N);
            }
        }
        else
        {
            mainFirstPart(N);
        }
    }
    return 0;
}

double getMedianF(vector<int> arr, int N)
{
    sort(arr.begin(), arr.end());
    double med;
    if (N % 2 != 0)
    {
        med = (double)arr[N / 2];
    }
    else
    {
        med = (double)(arr[(N - 1) / 2] + arr[N / 2]) / 2.0;
    }
    return med;
}

int getMin(vector<int> arr, int N)
{
    int min = arr[0];
    for (int i = 0; i < N; i++)
    {
        if (min > arr[i])
            min = arr[i];
    }
    return min;
}

int getMax(vector<int> arr, int N)
{
    int max = arr[0];
    for (int i = 0; i < N; i++)
    {
        if (max < arr[i])
            max = arr[i];
    }
    return max;
}

int getRange(vector<int> arr, int N)
{
    int min = getMin(arr, N);
    int max = getMax(arr, N);
    return max - min;
}

// returns the smallest when there is multiple mode.

int getMode(vector<int> arr, int N)
{
    int max = getMax(arr, N);

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

    return mode;
}

//divides the sorted array into two and takes the difference of the medians of the right and left parts.
double getInterquartileRange(vector<int> arr, int N)
{
    sort(arr.begin(), arr.end());

    if (N % 2 != 0)
    {
        vector<int>::const_iterator left1 = arr.begin();
        vector<int>::const_iterator right1 = arr.begin() + (N - 1) / 2;
        vector<int> leftPart(left1, right1);
        vector<int>::const_iterator left2 = arr.begin() + (N - 1) / 2 + 1;
        vector<int>::const_iterator right2 = arr.begin() + N;
        vector<int> rightPart(left2, right2);

        return getMedianF(rightPart, (N - 1) / 2) - getMedianF(leftPart, (N - 1) / 2);
    }
    else
    {
        vector<int>::const_iterator left1 = arr.begin();
        vector<int>::const_iterator right1 = arr.begin() + N / 2;
        vector<int> leftPart(left1, right1);
        vector<int>::const_iterator left2 = arr.begin() + N / 2;
        vector<int>::const_iterator right2 = arr.begin() + N;
        vector<int> rightPart(left2, right2);

        return getMedianF(rightPart, N / 2) - getMedianF(leftPart, N / 2);
    }
}
int getSum(vector<int> arr, int N)
{
    int sum = 0;

    for (int i = 0; i < N; i++)
    {
        sum = sum + arr[i];
    }
    return sum;
}

double getArithmeticMean(vector<int> arr, int N)
{
    double sum = getSum(arr, N);
    return sum / N;
}

double getSd(vector<int> arr, int N)
{
    double sd = 0;
    double mean = getArithmeticMean(arr, N);
    for (int i = 0; i < N; ++i)
    {
        sd += pow(arr[i] - mean, 2);
    }
    sd = sqrt(sd / (N - 1));
    return sd;
}

double getHarmonicMean(vector<int> arr, int N)
{
    double harmonicSum = 0;
    for (int i = 0; i < N; i++)
    {
        harmonicSum = harmonicSum + (double)1 / arr[i];
    }
    double harmonicMean = (double)N / harmonicSum;
    return harmonicMean;
}

int mainFirstPart(int N)
{
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    vector<int> randomNumbers(N);

    for (int i = 0; i < N; i++)
    {
        randomNumbers[i] = 1000 + (rand() % (10000 - 1000 + 1));
    }

    FILE *fp;
    fp = fopen("output1.txt", "w");

    fprintf(fp, "%i\n", getMin(randomNumbers, N));
    fprintf(fp, "%i\n", getMax(randomNumbers, N));
    fprintf(fp, "%i\n", getRange(randomNumbers, N));
    fprintf(fp, "%i\n", getMode(randomNumbers, N));
    fprintf(fp, "%.5f\n", getMedianF(randomNumbers, N));
    fprintf(fp, "%i\n", getSum(randomNumbers, N));
    fprintf(fp, "%.5f\n", getArithmeticMean(randomNumbers, N));
    fprintf(fp, "%.5f\n", getHarmonicMean(randomNumbers, N));
    fprintf(fp, "%.5f\n", getSd(randomNumbers, N));
    fprintf(fp, "%.5f", getInterquartileRange(randomNumbers, N));

    fclose(fp);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "Execution time = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << "[µs]" << endl;
    return 0;
}
