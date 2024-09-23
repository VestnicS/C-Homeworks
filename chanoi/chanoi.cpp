#include <iostream>

void chanoi(int count, int from, int neutral, int to)
{
    if(count == 1)
    {
        std::cout << count << " size: " << from << " -> " << to << std::endl;
        return;
    }
    chanoi(count - 1, from, to, neutral);
    std::cout << count << " size: " << from << " -> " << to << std::endl;
    chanoi(count - 1, neutral, from, to);
    return;
}

int main()
{
    int count;
    std::cout << "Enter the size of tower: ";
    std::cin >> count;

    chanoi(count, 1, 2, 3);

    return 0;
}