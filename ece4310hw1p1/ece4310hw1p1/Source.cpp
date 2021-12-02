#include <iostream>
#include <stdlib.h>
#include <tchar.h>
#include <thread>
#include <future>
#include <mutex>
#include <chrono>
#include <Windows.h>

using namespace std;
int n;
int primeamount2bound(int n, int n2);
int pythag(int n, int n2);
int pythag4(int n, int n2, int thread);
mutex mu;

//void primeamount2boundthread(promise<int> prom, int n, int n2);


int _tmain(int argc, _TCHAR* argv[])
{

	


	// Enter value for prime
	cout << "(MAIN)Please enter a value for N boundary to calculate amount of primes within its scope : " << endl;
	cin >> n;

	
	// Primes calculated by main
	cout << "The total amount of prime numbers calculated by main is : " << primeamount2bound(n,n) << endl;
	// There's a bunch of different attempts at using threads specifically to return values using futures and promises below

	/*t[0] = thread(primeamount2bound, n / 2, n / 2);
	t[1] = thread(primeamount2bound, n/2, n);
	t[0].join();
	t[1].join();
	promise<int> prom;
	future<int> ftr = prom.get_future();
	thread t0(ref(primeamount2boundthread), ref(prom), n / 2, n / 2);
	int thread0out = ftr.get();
	cout << thread0out;
	auto f = prom.get_future();

	std::future<int> t1 = std::async(&primeamount2bound, n / 2, n / 2);
	std::future<int> t2 = std::async(&primeamount2bound, n, n / 2);
	int thread1out = t1.get();
	int thread2out = t2.get();*/



	// create two concurrencies, calculates primes between bounds
	auto ftr = async(primeamount2bound, n / 2, n / 2);
	auto ftr2 = async(primeamount2bound, (n/2 + 1), n);

	int thread0 = ftr.get();
	int thread1 = ftr2.get();

	cout << "The total amount of prime numbers calculated by 2 concurrencies and summed by main is " << thread0 << " + "  << thread1 << " = " << thread0 + thread1 << endl;
	// create 4 concurrencies, calculates primes between bounds
	auto ftr3 = async(primeamount2bound, n/4, n/4);
	auto ftr4 = async(primeamount2bound, (n/4 + 1), n/2);
	auto ftr5 = async(primeamount2bound, (n / 2 + 1), (3*n/4));
	auto ftr6 = async(primeamount2bound, ((3*n/4) +1), n);

	int thread2 = ftr3.get();
	int thread3 = ftr4.get();
	int thread4 = ftr5.get();
	int thread5 = ftr6.get();

	cout << "The total amount of prime numbers calculated by 4 concurrencies and summed by main is " << thread2 << "+" << thread3 << "+" << thread4 << "+" << thread5 << "=" << (thread2 + thread3 + thread4 + thread5) << endl << endl;
	
	// enter boundary for pythagoreans
	cout << "(MAIN)Please enter a valure for N boundary to calculate amount of pythagorean triplets within its scope of A,B,C <=N" << endl << endl;
	int n2;
	cin >> n2;
	// calculate by main
	cout << "(MAIN) pythag under bound " << pythag(n2, n2) << endl << endl;


	// The reason this output does not work consistently is because you cannot calculate triples with a lower bound above a certain amount
	//2 conc calculated
	auto pythftr = async(pythag, n2/2, n2/2);
	auto pythftr2 = async(pythag, ((n2 / 2) + 1), n2);

	int pyththread0 = pythftr.get();
	int pyththread1 = pythftr2.get();

	cout << "The total amount of pythag numbers calculated by 2 concurrencies and summed by main is " << pyththread0 << " + " << pyththread1 << " = " << pyththread0 + pyththread1 << endl;

	// 4 conc calculated
	auto pythftr3 = async(pythag4, (n2 / 2), (n2 / 2), 0);
	auto pythftr4 = async(pythag4, (n2 / 2), n2, 1);
	auto pythftr5 = async(pythag4, (n2 / 2), n2 , 2);
	auto pythftr6 = async(pythag4, (n2 / 2), n2 , 3);

	int pyththread2 = pythftr3.get();
	int pyththread3 = pythftr4.get();
	int pyththread4 = pythftr5.get();
	int pyththread5 = pythftr6.get();

	cout << "The total amount of pythag numbers calculated by 4 concurrencies and summed by main is (/2 to reduce duplicates) " << pyththread2 << "+" << pyththread3 << "+" << pyththread4 << "+" << pyththread5 << "=" << (pyththread2 + pyththread3 + pyththread4 + pyththread5) / 2 << endl << endl;

}
// function to retrieve primes
int primeamount2bound(int lowerbound, int upperbound) // If no lower bound, put upper bound same as lower bound
{
	int primenumbers = 0;
	if (upperbound != lowerbound) {
		for (int i = lowerbound; i <= upperbound; i++)
		{
			if (i % 2 == 0) {}
			else {
				primenumbers++;
			}

		}
	}
	else {
		
		for (int i = 1; i <= upperbound; i++)
		{
			if (i % 2 == 0) {}
			else {
				primenumbers++;
			}

		}
	}
	return primenumbers;
}
// function to retrieve triplets with standard two bound input
int pythag(int lowerbound, int upperbound)
{
	int pythagnumber = 0;
	if (lowerbound == upperbound) {
		for (int a = 1; a <= upperbound; a++)
		{
			for (int b = 1; b <= upperbound; b++)
			{
				for (int c = 1; c <= upperbound; c++)
				{
					if (((a * a) + (b * b)) == (c * c) && c <= upperbound)
					{
						cout << a << "(^2) " << "+ " << b << "(^2)" << "==" << (c) << " (^2) 111111" << endl;
						pythagnumber++;
					}
				}
			}
		}
	}
	else {
		for (int a = lowerbound; a <= upperbound; a++)
		{
			for (int b = lowerbound; b <= upperbound; b++)
			{
				for (int c = 1; c <= upperbound; c++)
				{
					if (((a * a) + (b * b)) == (c * c) && c <= n)
					{
						cout << a << "(^2) " << "+ " << b << "(^2)" << "==" << (c) << " (^2)" << endl;
						pythagnumber++;
					}
				}
			}
		}
	}
	// return half the value if more than 2 because a double was found guaranteed
	if (pythagnumber >= 2)
	{
		pythagnumber =	pythagnumber / 2;
	}

	return pythagnumber;
}
// function for triplets for 4 concurrencies
int pythag4(int lowerbound, int upperbound, int thread)
{
	
	int pythagnumber = 0;
	switch (thread)
	{
		
	case 0:
		for (int a = 1; a <= upperbound; a++)
		{
			for (int b = 1; b <= upperbound; b++)
			{
				for (int c = 1; c <= upperbound; c++)
				{
					if (((a * a) + (b * b)) == (c * c) && c <= n)
					{
						cout << a << "(^2) " << "+ " << b << "(^2)" << "==" << (c) << " (^2)" << endl;
						pythagnumber++;
					}
				}
			}
		}
		break;
	case 1:
		for (int a = 1; a <= lowerbound; a++)
		{
			for (int b = (lowerbound + 1); b <= upperbound; b++)
			{
				for (int c = 1; c <= upperbound; c++)
				{
					if (((a * a) + (b * b)) == (c * c) && c <= n)
					{
						cout << a << "(^2) " << "+ " << b << "(^2)" << "==" << (c) << " (^2)" << endl;
						pythagnumber++;
					}
				}
			}
		}
		break;
	case 2:
		for (int a = lowerbound + 1; a <= upperbound; a++)
		{
			for (int b = (1); b <= lowerbound; b++)
			{
				for (int c = 1; c <= upperbound; c++)
				{
					if (((a * a) + (b * b)) == (c * c) && c <= n)
					{
						cout << a << "(^2) " << "+ " << b << "(^2)" << "==" << (c) << " (^2)" << endl;
						pythagnumber++;
					}
				}
			}
		}
		break;
	case 3:
		for (int a = lowerbound + 1; a <= upperbound; a++)
		{
			for (int b = (lowerbound + 1); b <= upperbound; b++)
			{
				for (int c = 1; c <= upperbound; c++)
				{
					if (((a * a) + (b * b)) == (c * c) && c <= n)
					{
						cout << a << "(^2) " << "+ " << b << "(^2)" << "==" << (c) << " (^2)" << endl;
						pythagnumber++;
					}
				}
			}
		}
		break;
	default:
		break;
	}
	
	return pythagnumber;
}
//void primeamount2boundthread(promise<int> &prom, int upperbound, int lowerbound) // If no lower bound, put upper bound same as lower bound
//{
//	int primenumbers = 0;
//	if (upperbound != lowerbound) {
//		for (int i = lowerbound; i < upperbound; i++)
//		{
//			if (i % 2 == 0) {}
//			else {
//				primenumbers++;
//			}
//
//		}
//	}
//	else {
//
//		for (int i = 0; i < n; i++)
//		{
//			if (i % 2 == 0) {}
//			else {
//				primenumbers++;
//			}
//
//		}
//	}
//	prom.set_value(primenumbers);
//}
//void primeamount(int n, string thread)
//{
//	int primenumbers = 0;
//	for (int i = 0; i < n; i++)
//	{
//		if (i % 2 == 0) {}
//		else {
//			primenumbers++;
//		}
//
//	}
//	cout << "Thread #" << thread << " The amount of prime numbers is : " << primenumbers << endl;
//
//}



