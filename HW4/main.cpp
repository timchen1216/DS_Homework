#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "huffman.h"
#include "arcd.h"
#include <time.h>
#include <iomanip>
using namespace std;

int main()
{
	double START_huf, END_huf, START_arcd, END_arcd;

	cout <<"----------------------Huffman coding---------------------"<< endl;
	char arr[] = {'a', 'b', 'c', 'd', 'e'};
	int freq[] = {5, 10, 20, 25, 40}; // freq 0.05 0.1 0.2 0.25 0.4
	cout << "Character: a b c d e\n";
	cout << "Probability: 0.05 0.1 0.2 0.25 0.4\n";
	int size = sizeof(arr) / sizeof(arr[0]);

	START_huf = clock();
	HuffmanCodes(arr, freq, size);
	END_huf = clock();
	cout <<"HuffmanCodes :"<< setprecision(5) << (END_huf - START_huf)/ CLOCKS_PER_SEC <<"sec\n\n"<< endl;

	cout <<"--------------------Arithmetic coding---------------------"<< endl;
	int n = 5;
	double freq1[] = {0.05, 0.1, 0.2, 0.25, 0.4};
	//cout << "Enter number of characters: ";
	// cin >> n;
	unordered_map<char, node> arr1;
	vector<char> ar;
	double range_from = 0;
	//cout << "Enter probability of each character:\n";
	for (int i = 0; i < n; i++)
	{
		char ch;
		// cin >> ch;
		ch = arr[i];
		ar.push_back(ch);
		// cin >> arr[ch].prob;
		arr1[ch].prob = freq1[i];
		arr1[ch].range_from = range_from;
		arr1[ch].range_to = range_from + arr1[ch].prob;
		range_from = arr1[ch].range_to;
	}
	cout << "Symbol\tProbability\tRange_from\tRange_to\n";
	cout << "----------------------------------------------------\n";
	for (int i = 0; i < ar.size(); i++)
	{
		char ch = ar[i];
		cout << ch << "\t" << arr1[ch].prob << "\t\t" << arr1[ch].range_from << "\t\t" << arr1[ch].range_to << endl;
	}
	cout << endl;
	string s = "acdeb";
	//cout << "Enter text: ";
	//cin >> s;
	START_arcd = clock();
	double code_word = encoding(arr1, s);
	END_arcd = clock();
	cout << "Code word for " << s << " is: " << code_word << endl;
	string text = decoding(arr1, code_word, s.size());
	cout << "Text for " << code_word << " is: " << text << endl;
	cout <<"Arithmetic coding:"<< (END_arcd - START_arcd)/ CLOCKS_PER_SEC <<"sec\n\n"<< endl;
}