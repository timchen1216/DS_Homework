#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<unordered_map>
#include<vector>
#include "huffman.h"
#include "arcd.h"
using namespace std;

int main(){
	int n;
	cout<<"Enter number of characters: ";
	cin>>n;
	unordered_map<char, node> arr;
	vector<char> ar;
	double range_from= 0;
	cout<<"Enter probability of each character:\n";
	for(int i=0; i<n; i++){
		char ch;
		cin>>ch;
		ar.push_back(ch);
		cin>>arr[ch].prob;
		arr[ch].range_from= range_from;
		arr[ch].range_to= range_from+ arr[ch].prob;
		range_from= arr[ch].range_to;
	}
	cout<<"Symbol\tProbability\tRange_from\tRange_to\n";
	cout<<"----------------------------------------------------\n";
	for(int i=0; i<ar.size(); i++){
		char ch= ar[i];
		cout<<ch<<"\t"<<arr[ch].prob<<"\t\t"<<arr[ch].range_from<<"\t\t"<<arr[ch].range_to<<endl;
	}
	cout<<endl;
	string s;
	cout<<"Enter text: ";
	cin>>s;
	double code_word= encoding(arr, s);
	cout<<"Code word for "<<s<<" is: "<<code_word<<endl;
	string text= decoding(arr, code_word, s.size());
	cout<<"Text for "<<code_word<<" is: "<<text<<endl;

}