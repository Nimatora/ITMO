#include <iostream>
#include <assert.h>
#include <Array>
#include <fstream>
using namespace std;

namespace math {
	// 1. Linear systems
	bool dDominance(float** arr, size_t size) {
		for (size_t i = 0; i < size; i++) {
			double sum = 0;
			for(size_t j = 0; j < size; j++)
				if (j != i)
					sum += abs(arr[i][j]);
			
			if (abs(arr[i][i]) > sum)
				return true;
		}
		return false;
	}
	bool converge(float* first, float* second, size_t size, float precision) {
		float norm = 0;

		for (int i = 0; i < size; i++) 
			norm += (first[i] - second[i]) * (first[i] - second[i]);
		
		return (sqrt(norm) < precision);
	}
	float* GaussSeidel(float** a, float* b, size_t size, float precision) {
		// aX = b

		float* X = new float[size];
		for (size_t i = 0; i < size; i++)
			X[i] = 0;
		float* pastX = new float[size];
		
		do {
			for (size_t i = 0; i < size; i++)
				pastX[i] = X[i];
			
			for (int i = 0; i < size; i++) {
				double var = 0;
				for (int j = 0; j < i; j++)
					var += a[i][j] * X[j];
				for (int j = i + 1; j < size; j++)
					var += a[i][j] * pastX[j];

				X[i] = (b[i] - var) / a[i][i];
			}
		} while (!converge(X, pastX, size, precision));
	
		delete[] pastX;
		return X;
	}

	// 2. Integrals
	double ComputeIntegral(double(*y)(double x), double lowBound, double topBound, double atomRang) {
		if (lowBound == topBound)
			return 0;
		if (lowBound > topBound)
			swap(lowBound, topBound);

		double sum = 0;
		for (double x = lowBound; x <= topBound; x += atomRang)
			sum += y(x) * atomRang;

		return sum;
	}
}
namespace tests {
	void printEquatationSystem(float** a, float* b, size_t size) {
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) 
			cout << a[i][j] << "x" << j << " ";
		cout << "= " << b[i] << endl;
	}
}
	using namespace math;
	// 1
	void runGaussSeidelTest() {
		size_t size = 3;
		float precision = 0.001;
		float** a = new float*[size] {	new float[3] {8, 4, 2}, 
										new float[3] {3, 5, 1},
										new float[3] {3, -2, 10}};
		float* b = new float[size] {10, 5, 4};
		
		printEquatationSystem(a, b, size);
		cout << "is computable? " << math::dDominance(a, size) << endl;
		float* result = math::GaussSeidel(a, b, size, precision);

		cout << "result is" << endl;
		for (size_t i = 0; i < size; i++)
			cout << "X[" << i << "]: " << result[i] << endl;
	}
	// 2
	void runComputeIntegralTests() {
		// triangle: 1\2*h*w
		double atomRang = 0.100;
		double lowBound = -3;
		double topBound = 3;
		double testResult = 0;

		// common
		testResult = ComputeIntegral([](double x) { return -abs(x) + 3; }, lowBound, topBound, atomRang);
		assert(testResult >= 9 - atomRang && testResult <= 9 + atomRang);
		
		// reversed
		testResult = ComputeIntegral([](double x) { return -abs(x) + 3; }, topBound, lowBound, atomRang);
		assert(testResult >= 9 - atomRang && testResult <= 9 + atomRang);

		// lowBound == upBound
		testResult = ComputeIntegral([](double x) { return -abs(x) + 3; }, lowBound, lowBound, atomRang);
		assert(testResult == 0);

		// y(x) = -y(x)
		testResult = ComputeIntegral([](double x) { return x; }, topBound, lowBound, atomRang);
		assert(testResult >= 0 - atomRang && testResult <= 0 + atomRang);
	}
	// All
	void runAllTests() {
		runGaussSeidelTest();
		runComputeIntegralTests();
	}
}

void main() {
	// tests::runAllTests();
	using namespace math;
	char command;

	while (true) {
		cout << "What method do you want to run?" << endl;
		cout << "1. Gauss-Seidel method\n2. Reqtangle method(integrals)\n3.\n4.\n5.\n0 - exit" << endl;
		cout << endl << "Command: ";

		cin.get(command);
		switch (command) {
		case '1': {
			cout << "Gauss-Seidel method for linear system solving" << endl;
			float precision = 0;
			int N = 0;

			// creating matrix model
			cout << "input matrix size(N*N): ";
			cin >> N; 
			float** arr = new float*[N];		// Didn't optimized for readability
			for (size_t i = 0; i < N; i++)		// That's a hard shit(code), maan.
				arr[i] = new float[N];
			float* b = new float[N];

			//  inputting all the coefficients
			for (size_t i = 0; i < N; i++) {
				for (size_t j = 0; j < N; j++) {
					cout << "input an a[" << i << "][" << j << "] coefficient: ";
					cin >> *arr[i + j]; 
				}
			}
			for (size_t i = 0; i < N; i++) {
				cout << "input b[" << i << "]: ";
				cin >> b[i];
			}
			cout << "input precition: ";
			cin >> precision;

			// printing al the statistic
			tests::printEquatationSystem(arr, b, N);
			cout << "precision: " << precision << endl;
			cout << "Computing..." << endl << endl;
			float* result = math::GaussSeidel(arr, b, N, precision);
			cout << "result is: " << endl;
			for (size_t i = 0; i < N; i++)
				cout << "X[" << i << "] = " << result[i] << endl;

			// prolog
			for (size_t i = 0; i < N*N; i++)
				delete[] arr[i];
			delete[] arr;
		} break;
		case '2': {
			cout << "Reqtangle method for computing integrals" << endl;
			float atomRang = 0;
			float lowBound = 0;
			float upBound = 0;
			double (*function)(double x) = nullptr;

			cout << "choose f(x) function: \n1. y = x^2\n2. y = 1/x^2\n3. y = 4*x\n4. y = e^x\n5. y = sin(3*x)\n\n0. - exit";
			cout << endl << "Command: ";
			while (true) {
				cin >> command;
				switch (command) {
					case '1': function = [](double x) { return pow(x, 2); }; break;
					case '2': function = [](double x) { return 1/pow(x, 2); }; break;
					case '3': function = [](double x) { return 4 * x; }; break;
					case '4': function = [](double x) { return exp(x); }; break;
					case '5': function = [](double x) { return sin(3 * x); }; break;

					case '0': break; break;
					default: cout << "Wrong number. Try a number(1-5)/n 0 to exit." << endl; break; continue;
				}
				cout << "input lower bound:";
				cin >> lowBound;
				cout << "input upper bound:";
				cin >> upBound;
				
				cout << "input positive(!) atom rang(maximum deta(x))";
				cin >> atomRang;
				if (atomRang <= 0)
					break;

				double I = ComputeIntegral(function, lowBound, upBound, atomRang);
				cout << "I = " << I << endl;
				cout << "Amount of divisions = " << abs(lowBound - upBound) / atomRang << endl;
				cout << "Error == " << abs(I - ComputeIntegral(function, lowBound, upBound, atomRang/2))/3 << endl;
			}
		} break;
		case '3': throw "NotImplementedException"; break;
		case '4': throw "NotImplementedException"; break;
		case '5': throw "NotImplementedException"; break;
		default: cout << "Wrong number. Try a number(1-5)" << endl;
		}
	}
}