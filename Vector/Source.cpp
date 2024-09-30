#include "Vector.cpp"

int main()
{
	/*Vector<int> v(5);
	for (int i = 0; i < 5; i++)
	{
		std::cin >> v[i];
	}
	for (int i = 0; i < v._size(); i++)
	{
		std::cout << v[i] << " ";
	}
	std::cout << std::endl;

	for (int i = 0; i < v._size(); i++)
	{
		v[i] *= -1;
	}
	for (int i = 0; i < v._size(); i++)
	{
		std::cout << v[i] << " ";
	}
	std::cout << std::endl;

	v.pop_back();
	for (int i = 0; i < v._size(); i++)
	{
		std::cout << v[i] << " ";
	}
	std::cout << std::endl;

	v.push_back(9);
	for (int i = 0; i < v._size(); i++)
	{
		std::cout << v[i] << " ";
	}
	std::cout << std::endl;

	Vector<int> a;
	a = v;

	for (int i = 0; i < a._size(); i++)
	{
		std::cout << a[i] << " ";
	}*/


	int n;
	std::cin >> n;

	Vector<Vector<int>> v(n, Vector<int>(n, 3));

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			std::cout << v[i][j] << " ";
		}
		std::cout << std::endl;
	}

	Vector<int> vv(n, 5);

	vv.resize(n * 3);

	for (int i = 0; i < vv._size(); ++i)
	{
		std::cout << vv[i] << " ";
	}
	std::cout << '\n';

	/*try
	{
		std::cout << vv[16];
	}
	catch (const std::exception&)
	{
		std::cout << "Out of range";
	}*/

	/*Vector<int> v(1);
	v[0] = 1;
	v.push_back(2);
	v.push_back(4);
	for (int i = 0; i < v._size(); i++)
	{
		std::cout << v[i] << " ";
	}
	std::cout << std::endl;

	std::cout << v._capacity();*/
}