// определитель

#include <iostream>

double determinant(double* arr, int n)
{
    for (int r = 0; r < n; ++r)
    {
        for(int j = r + 1; j < n; ++j)
        {
            if (!arr[r * n + r])
                return 0.0;
            if (!arr[j * n + r])
                continue;
            double k = arr[r * n + r] / arr[j * n + r];
            for(int i = 0; i < n; ++i)
            {
                arr[j * n + i] -= arr[r * n + i] / k;
            }
        }
    }
    double result = 1.0;
    for(int i = 0; i < n; ++i)
    {
        result *= arr[i * n + i];
    }
    return result;
}

int main()
{
    int n;
    std::cout << "Enter the matrix size: ";
    std::cin >> n;

    double* arr = new double[n * n];
    std::cout << "Enter the matrix:\n";
    for(int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; j++)
        {
            std::cin >> arr[i * n + j];
        }
    }
    std::cout << determinant(arr, n);
    delete[] arr;
    return 0;
}