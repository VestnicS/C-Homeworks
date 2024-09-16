#include <iostream>
using namespace std;

void out_triangle(int count, const int size, bool middle = false)
{
    if(count < 1)
        return;
    
    for (int i = 0; i < count; i++)
    {
        cout << "*";
    }
    cout << endl;

    if(count == size)
        out_triangle(count - 1, size, true);
    else if(middle)
        out_triangle(count - 1, size, middle);
    else 
        out_triangle(count + 1, size, middle);

    return;
}

int main()
{
    int n;
    cout << "Enter the size of triangle: ";
    cin >> n;
    out_triangle(1, n);

    return 0;
}