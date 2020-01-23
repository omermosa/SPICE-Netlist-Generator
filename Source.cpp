//Microelectronics Project
// The American University in Cairo (AUC)
//Copyrights Abdulhakim Badawi, Omer Hassan, and Yahya Abbas 
//All rights Reserved
//16/5/2019
#include <iostream>
#include <vector>
#include <string>
using namespace std;


void PDN(string);
void PUN(string);
string Compelemnt(string original);
void invert(string, string);
void PDN2(string input);
void PUN2(string input);

struct mosfet {
	string drain;
	string source;
	string gate;
	int NetCount = 0;
	int OverCount = 0;
};
struct network {
	string ndrain;
	string nsource;
	vector<mosfet> transistors;
	int size = 0;
	int transistors_c = 0;
};

int nmosfets_count = 0;
int Pmosfets_count = 0;
bool done = false;
int mosfets_print_c = 0;
int main() {
	//read input boolean string 
	string input;
	cout << "Enter the Boolean Expression:- " << endl;
	cin >> input;
	string PUN_str = input;
	string PDN_str = input;
	
	if (input[1] == 39) {

		for (int i = 2; i < input.length(); i++) {
			if (isalpha(input[i]) && input[i + 1] == 39) { //an input needs to be complemented
				string x, y;
				x += input[i];
				x += "'";
				y+=input[i];
				invert(y, x);
			}
		}

		PDN(input); //PDN given. Process input, complement, call PUN at the end of PDN
	}
	else {
		for (int i = 2; i < input.length(); i++) {
			if (isalpha(input[i]) && input[i + 1] != 39) { //an input needs to be complemented
				 input.insert(i + 1, "'");
				 string x, y;
				 y+=input[i];
				 x += input[i]; 
				 x+="'";
				invert(y, x);
			}
			else if(input[i+1]==39) {
				input.erase( i+ 1, 1);
			}
		}
		PUN(input); //PUN given. Process input, complement, call PDN at the end of PUN
	}
	system("pause");
	return 0;
}

void PDN(string input) {

	vector<network> and_networks;
	network temp_network;
	temp_network.ndrain = input[0];//generic output
	temp_network.nsource = "0";
	mosfet temp_mosfet;

	
	for (int i = 2; i < input.length(); i++) {
		if (input[i] == '|' ) {
			and_networks.push_back(temp_network); //push the last and network to the and networks vector
			temp_network.ndrain = "";
			temp_network.nsource = "";
			temp_network.transistors.erase(temp_network.transistors.begin(), temp_network.transistors.begin() + temp_network.transistors_c);
			temp_network.transistors_c = 0;
		}
		else if (isalpha(input[i])) {
			nmosfets_count++; string t = "";
			if (input[i + 1] == 39) t = "'";
			temp_mosfet.gate = char (input[i])+t;
			if (temp_network.transistors.empty()) {
				temp_mosfet.drain = input[0];//generic output
			}
			else temp_mosfet.drain = temp_network.transistors.back().source;
			if ((i + 1) >= input.size()-1 || input[i+1]=='|'||input[i+2]=='|') temp_mosfet.source = "0";
			else temp_mosfet.source = "ON_" + to_string(and_networks.size()+1);//remove -1
			temp_network.transistors.push_back(temp_mosfet);
			temp_network.transistors_c++;
			if (i >=input.length()-2) and_networks.push_back(temp_network); //??
		}
	}
	
	for (int i = 0; i < and_networks.size(); i++) { //loop over the and networks
		for (int j = 0; j < and_networks[i].transistors_c; j++) { //loop over the transistors of each network
			cout << "M_" << ++mosfets_print_c <<" "<< and_networks[i].transistors[j].drain << " " <<
				and_networks[i].transistors[j].gate << " " << and_networks[i].transistors[j].source << " "
				<< and_networks[i].transistors[j].source << " NMOS" << endl;
		}
	}
	string comp = Compelemnt(input);
	PUN2(comp);
	
	/*if (!done)
	{
		done = true;
		string comp = Compelemnt(input);
		PUN2(comp);
	}
*/
}

void PDN2(string input) {

	vector<network> or_networks;
	network temp_network;
	temp_network.ndrain = input[0];//generic output
	temp_network.nsource = "0";
	mosfet temp_mosfet;


	for (int i = 2; i < input.length(); i++) {
		if (input[i] == '&') {
			or_networks.push_back(temp_network); //push the last and network to the and networks vector
			temp_network.ndrain = "";
			temp_network.nsource = "";
			temp_network.transistors.erase(temp_network.transistors.begin(), temp_network.transistors.begin() + temp_network.transistors_c);
			temp_network.transistors_c = 0;
		}
		else if (isalpha(input[i])) {
			nmosfets_count++; string t = "";
			if (input[i + 1] == 39) t = "'";
			temp_mosfet.gate = char(input[i]) + t;
			if (or_networks.size() == 0)
				temp_network.ndrain = input[0];
			else temp_network.ndrain = or_networks.back().nsource;
			temp_network.nsource = "ON_" + to_string(or_networks.size()+1);
			temp_mosfet.drain = temp_network.ndrain;
			temp_mosfet.source = temp_network.nsource;
			temp_network.transistors.push_back(temp_mosfet);
			temp_network.transistors_c++;
		}
		if (i+1 == input.length()) or_networks.push_back(temp_network);
	}
	//adjust the last or network's source to gnd 

	for (int i = 0; i < or_networks.back().transistors_c; i++) {
		or_networks.back().transistors[i].source = "0";

	}

	
	for (int i = 0; i < or_networks.size(); i++) { //loop over the and networks
		for (int j = 0; j < or_networks[i].transistors_c; j++) { //loop over the transistors of each network
			cout << "M_" << ++mosfets_print_c << " " << or_networks[i].transistors[j].drain << " " <<
				or_networks[i].transistors[j].gate << " " << or_networks[i].transistors[j].source << " "
				<< or_networks[i].transistors[j].source << " NMOS" << endl;
		}
	}
}

void PUN(string input) {

	vector<network> and_networks;
	network temp_network;
	temp_network.ndrain = input[0];
	temp_network.nsource = "vdd";
	mosfet temp_mosfet;

	for (int i = 2; i < input.length(); i++) {
		if (input[i] == '|') {
			and_networks.push_back(temp_network); //push the last and network to the and networks vector
			temp_network.ndrain = "";
			temp_network.nsource = "";
			temp_network.transistors.erase(temp_network.transistors.begin(), temp_network.transistors.begin() + temp_network.transistors_c);
			temp_network.transistors_c = 0;
		}
		else if (isalpha(input[i])) {
			Pmosfets_count++;
			string t = "";
			if (input[i + 1] == 39) t = "'";
			temp_mosfet.gate = char (input[i])+t;
			if (temp_network.transistors.empty()) {
				temp_mosfet.source = "vdd";
			}
			else temp_mosfet.source = temp_network.transistors.back().drain;
			if ((i + 1) >= input.size()-1 || input[i + 1] == '|' || input[i + 2] == '|') temp_mosfet.drain = input[0];
			else temp_mosfet.drain = "OP_" + to_string(and_networks.size() + 1);
			temp_network.transistors.push_back(temp_mosfet);
			temp_network.transistors_c++;
			if (i >= input.length()-2) and_networks.push_back(temp_network);
		}
	}
	
	for (int i = 0; i < and_networks.size(); i++) { //loop over the and networks
		for (int j = 0; j < and_networks[i].transistors_c; j++) { //loop over the transistors of each network
			cout << "M_" << ++mosfets_print_c << " " << and_networks[i].transistors[j].drain << " " <<
				and_networks[i].transistors[j].gate << " " << and_networks[i].transistors[j].source << " "
				<< and_networks[i].transistors[j].source << " PMOS" << endl;
		}
	}

	string comp = Compelemnt(input);
	PDN2(comp);

	/*if (!done)
	{
		done = true;
		string comp = Compelemnt(input);
		PDN2(comp);
	}*/
}

void PUN2(string input) {
	vector<network> or_networks;
	network temp_network;
	temp_network.ndrain = input[0];//generic output
	temp_network.nsource = "vdd";
	mosfet temp_mosfet;


	for (int i = 2; i < input.length(); i++) {
		if (input[i] == '&') {
			or_networks.push_back(temp_network); //push the last and network to the and networks vector
			temp_network.ndrain = "";
			temp_network.nsource = "";
			temp_network.transistors.erase(temp_network.transistors.begin(), temp_network.transistors.begin() + temp_network.transistors_c);
			temp_network.transistors_c = 0;
		}
		else if (isalpha(input[i])) {
			Pmosfets_count++; string t = "";
			if (input[i + 1] == 39) t = "'";
			temp_mosfet.gate = char(input[i]) + t;
			
			if (or_networks.size() == 0)
				temp_network.nsource = "vdd";
			else temp_network.nsource = or_networks.back().ndrain;

			temp_network.ndrain = "OP_" + to_string(or_networks.size() + 1);
			temp_mosfet.source = temp_network.nsource;
			temp_mosfet.drain = temp_network.ndrain;
			temp_network.transistors.push_back(temp_mosfet);
			temp_network.transistors_c++;
		}
		if (i+1 == input.length()) or_networks.push_back(temp_network);
	}
	//adjust the last or network's source to gnd 

	for (int i = 0; i < or_networks.back().transistors_c; i++) {
		or_networks.back().transistors[i].drain = input[0];

	}


	for (int i = 0; i < or_networks.size(); i++) { //loop over the and networks
		for (int j = 0; j < or_networks[i].transistors_c; j++) { //loop over the transistors of each network
			cout << "M_" << ++mosfets_print_c << " " << or_networks[i].transistors[j].drain << " " <<
				or_networks[i].transistors[j].gate << " " << or_networks[i].transistors[j].source << " "
				<< or_networks[i].transistors[j].source << " PMOS" << endl;
		}
	}
}



void invert(string input, string output)
{

	cout << "M_" << input << "_1" << " " << output << " " << input << " " << "vdd" << " " << "vdd" << " " << "PMOS" << endl;

	cout << "M_" << input << "_2" << " " << output << " " << input << " " << "0" << " " << "0" << " " << "NMOS" << endl;

}
string Compelemnt(string original)
{
	string or2 = original;
	for (int i = 0; i < or2.length(); i++)
	{
		if (or2[i] == '|')or2[i] = '&';
		else if (or2[i] == '&')or2[i] = '|';

	}
	return or2;
}

