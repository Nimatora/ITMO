#include <iostream>
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
	float parabola(double x, double multiplier) {
		return pow(x, multiplier);
	}
	float giperbola(double x, double multiplier) {
		return 1 / pow(x, multiplier);
	}
	float linear(double x, double multiplier) {
		return x * multiplier;
	}
	float ComputeIntegral(float(*function)(float x), double lowBound, double topBound, double atomRang) {
		float result = 0;

		for (float iter = lowBound; iter <= topBound; iter+= atomRang)
			result += function(iter) * atomRang;

		return result;
	}
}
namespace tests {
	// 1
	void printEquatationSystem(float** a, float* b, size_t size) {
		for (size_t i = 0; i < size; i++) {
			for (size_t j = 0; j < size; j++) 
				cout << a[i][j] << "x" << j << " ";
			cout << "= " << b[i] << endl;
		}
	}
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
	void runIntegralReqtangleMethodTests() {
		// triangle: 1\2*h*w 
		throw "Not Implemented";
	}
	// All
	void runAllTests() {
		runGaussSeidelTest();
		runIntegralReqtangleMethodTests();
	}
}

void main() {
	// tests::runAllTests();
	char command;

	while (true) {
		cout << "What method do you want to run?" << endl;
		cout << "1. Gauss-Seidel method\n2.\n3.\n4.\n5." << endl;
		cout << "Command: ";

		cin.get(command);
		switch (command) {
		case '1': {
			float precision = 0;
			int N = 0;

			cout << "input matrix size(N*N): ";
			cin >> N; 
			float** arr = new float*[N];		// Didn't optimized for readability
			for (size_t i = 0; i < N; i++)
				arr[i] = new float[N];
			float* b = new float[N];

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
			cout << endl;

			tests::printEquatationSystem(arr, b, N);
			cout << "precision: " << precision << endl;
			cout << "Computing..." << endl << endl;
			float* result = math::GaussSeidel(arr, b, N, precision);
			
			cout << "result is: " << endl;
			for (size_t i = 0; i < N; i++)
				cout << "X[" << i << "] = " << result[i] << endl;

			for (size_t i = 0; i < N*N; i++)
				delete[] arr[i];
			delete[] arr;
			break;
		} 
		case '2': throw "NotImplementedException"; break;
		case '3': throw "NotImplementedException"; break;
		case '4': throw "NotImplementedException"; break;
		case '5': throw "NotImplementedException"; break;
		default: cout << "Wrong number. Try a number(1-5)" << endl;
		}
	}
}