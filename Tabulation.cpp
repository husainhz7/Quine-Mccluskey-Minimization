#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

struct binaryNumber
{
	int binary;
	vector<int> variables;
	vector<int> decimal;
	int checked;
};

struct groupOfBinary{
	int numOf1;
	vector<binaryNumber> numbers;
};

struct Groups
{
	vector<groupOfBinary> groups;
};

int main(int argc, char const *argv[])
{
	vector<int> minterms;

	int var;
	int i = 0;
	Groups columnA;
	vector<Groups> table;
	vector<binaryNumber> unchecked;
	vector<binaryNumber> essential;

	cout << "How many variables: ";
	cin >> var;
	cout << "Enter minterms (Return to terminate): ";

	do{
		int d;
		cin >> d;
		minterms.push_back(d);
	}while(getchar() != '\n');

	//convert to binary and grouping according to number of ones
	for(int min: minterms)
	{
		int numOf1 = 0;
		int binary = 0;
		int place = 1;
		int n = min;
		while(n!=0)
		{
			int rem = n%2;
			binary += rem*place;
			place *= 10;
			n = n/2;
		}
		int flag = 0;
		n = binary;
		while(n != 0)
		{
			if(n%10 == 1)
				numOf1++;
			n = n/10;
		}
		binaryNumber a;
		a.binary = binary;
		a.variables.push_back(min);
		a.checked = 0;
		int i = 0;
		for(groupOfBinary g : columnA.groups){
			if(numOf1 == g.numOf1){
				columnA.groups[i].numbers.push_back(a);
				flag = 1;
				break;
			}
			i++;
		}
		if(flag == 0){
			groupOfBinary g;
			g.numOf1 = numOf1;
			g.numbers.push_back(a);
			columnA.groups.push_back(g);
		}
	}
	table.push_back(columnA);

	//pairing according to dashes
	int flag=0;
	int columnNumber = 0;
	while(flag == 0)
	{
		i=0;
		flag = 1;
		Groups newColumn;
		for(groupOfBinary group1:columnA.groups)
		{
			if(group1.numOf1== columnA.groups.back().numOf1)
				break;

			for(binaryNumber num1: group1.numbers)
			{
				groupOfBinary group2 = columnA.groups[i+1];
				for(binaryNumber num2: group2.numbers)
				{
					if(num1.decimal == num2.decimal)
					{
						for(int k=0;k<num1.variables.size();k++)
						{
							int min1 = num1.variables[k];
							int min2 = num2.variables[k];
							if(min2 < min1)
								continue;
							int difference = abs(min2 - min1);
							double logarithm = log10(difference)/log10(2);		//if difference is power of 2
							double intpart, fracpart;
							fracpart = modf(logarithm,&intpart);
							if(fracpart == 0.000000)
							{
								//pair
								binaryNumber a;
								a.checked = 0;
								a.decimal = num1.decimal;
								a.decimal.push_back(difference);
								sort(a.decimal.begin(),a.decimal.end());
								for(int i:num1.variables)
									a.variables.push_back(i);
								for(int i:num2.variables)
									a.variables.push_back(i);
								sort(a.variables.begin(),a.variables.end());
								if(i < newColumn.groups.size())
									newColumn.groups[i].numbers.push_back(a);
								else{
									groupOfBinary g;
									g.numOf1 = i;
									g.numbers.push_back(a);
									newColumn.groups.push_back(g);
								}
								flag = 0;


								for(int j =0; j< group1.numbers.size(); j++){
									if(group1.numbers[j].variables == num1.variables){
										table[columnNumber].groups[i].numbers[j].checked = 1;
									}
								}
								for(int j =0; j< group2.numbers.size(); j++){
									if(group2.numbers[j].variables == num2.variables){
										table[columnNumber].groups[i+1].numbers[j].checked = 1;
									}
								}

								break;
							}
							
						}
					}
				}	
			}
			i++;
		}
		if(flag != 1){
			table.push_back(newColumn);
			columnA = newColumn;
		}
		columnNumber++;
	}

	
	cout << endl;
	i=0;
	for(Groups groups: table){
		for(groupOfBinary group: groups.groups){
			flag = 1;
			for(binaryNumber num : group.numbers){
				if(num.checked == 0 && flag){
					if(i == table.size()-1){
						unchecked.push_back(num);
						flag = 0;
					}else
						unchecked.push_back(num);
				}
			}
		}
		i++;
	}



	//finding essential
	i = 0;
	vector<binaryNumber> remainingPrimeImplicants;
	vector<int> coveredMinTerms;
	for(int min: minterms){
		flag = 1;
		vector<binaryNumber> primeCoveringMinterm;
		for(binaryNumber num: unchecked){
			for(int j: num.variables){
				if(j == min)
					primeCoveringMinterm.push_back(num);
			}
		}
		if(primeCoveringMinterm.size() == 1){
			
			for(binaryNumber num:essential){		//checking if not in essential from before
				int k=0;
				for(int m: num.variables){
					if(m == primeCoveringMinterm[0].variables[k]){
						flag = 0;
					}else{
						flag = 1;
						break;
					}
					k++;
				}
				if(flag == 0)
					break;
			}
			if(flag){
				essential.push_back(primeCoveringMinterm[0]);			//essential prime implicants are put in seperate array
				for(int m: primeCoveringMinterm[0].variables)
					coveredMinTerms.push_back(m);
			}
			
		}else{
			for(binaryNumber num:primeCoveringMinterm){
				remainingPrimeImplicants.push_back(num);	
			}
		}
		i++;
	}

	//further simplifying
	vector<int> remainingMinTerms;
	for(int min: minterms){
		flag = 1;
		for(int j: coveredMinTerms)
			if(min == j)
				flag = 0;
		if(flag){
			remainingMinTerms.push_back(min);
		}
	}
	for(binaryNumber num: remainingPrimeImplicants){
		flag = 0;
		i=0;
		for(int k:num.variables){
			if(k == remainingMinTerms[i]){
				flag = 1;
			}else{
				flag = 0;
				break;
			}
			i++;
		}
		if(flag){
			int flag2 = 1;
			for(binaryNumber num2:essential){		//checking if not in essential from before
				i=0;
				for(int m: num2.variables){
					if(m == num.variables[i]){
						flag2 = 0;
					}
					else{								
						flag = 1;
						break;
					}									
					i++;
				}
				if(flag2 == 0)
					break;
			}
			if(flag2){
				essential.push_back(num);
			}
		}
	}

	// printing answer
	for(binaryNumber num: essential){
		cout <<" ";
		vector<int> binary;
		int b = 0;
		int n = num.variables[0];
		for (int i = 0; i < var; ++i)
		{
			binary.push_back(0);
		}
		int place = 1;
		while(n!=0){
			int rem = n%2;
			b += rem*place;
			place *= 10;
			n = n/2;
		}
		for (int i = 0; i < var; ++i)
		{
			binary[i] = b%10;
			b=b/10;
		}
		for (int i = 0; i < var; ++i)
		{
			for(int k: num.decimal){				//placing 3 instead of dashes
				if(pow(2,i) == k){
					binary[i] = 3;
					break;
				}
			}
		}
		int placeCounter = 0;
		for (int i = var-1; i >= 0; --i)
		{
			if(binary[i] != 3){
				char c = 65+placeCounter;			//output in ABC form
				cout << c;
				if(binary[i]==0){
					cout << '\'';
				}
			}
			placeCounter++;
		}
		cout << " +" ;
	}
	cout << "\b\b"<< endl;

	return 0;
}