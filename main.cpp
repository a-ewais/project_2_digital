#include <iostream>
#include<vector>
#include <string>
#include<sstream>
#include<map>
#include "Circuit.h"
#include "ParseError.h"

using namespace std;
using namespace vp;

void JSONwave(map<string, string>, map<string, string>);
void getSignalData(map<string,string>);
string getData(string, bool, int);
string dta[100];
stringstream JSON;
vector< map<string,int> > EventInput;

int xorr(vector<int> op)
{
	int x = 0;
	for (int i = 0;i < op.size();i++)
		x ^= op[i];
	return x;
}
int andd(vector<int> op)
{
	int x = 1;
	for (int i = 0;i < op.size();i++)
		x &= op[i];
	return x;
}
int orr(vector<int> op)
{
	int x = 0;
	for (int i = 0;i < op.size();i++)
		x |= op[i];
	return x;
}
int inv(vector<int> op)
{
	return !op[0];
}
int xnorr(vector<int> op)
{
	return !xorr(op);
}
int nandd(vector<int> op)
{
	return !andd(op);
}
int norr(vector<int> op)
{
	return !orr(op);
}
int buff(vector<int> op)
{
	return op[0];
}
namespace vp {
	class mynode :protected Node {
	private:
		bool ready;
		int value;
		int(*func)(vector<int> operands);
		vector<int> op;
		int new_tfall, new_trise;
	public:
		mynode(const Node& x) :Node(x), ready(false), value(0), new_tfall(0), new_trise(0) {
			if (this->isInputPort())
				ready = true;
			else if (type == "xor")
				func = xorr;
			else if (type == "or")
				func = orr;
			else if (type == "and")
				func = andd;
			else if (type == "xnor")
				func = xnorr;
			else if (type == "nand")
				func = nandd;
			else if (type == "nor")
				func = norr;
			else if (type == "inv")
				func = inv;
			else if (type == "buf")
				func = buff;
			else
				throw exception("not supported gate");

		};
		void begin(new_circuit &x) {
			for (int i = 0;i < this->getInputsCount();i++)
			{
				mynode &n = (x.new_node(x.getNodeIndex(inputNode(i).getName())));
				if (!n.is_ready())
					n.begin(x);
				op.push_back(n.get_value());
				new_tfall = max(new_tfall, n.new_tfall);
				new_trise = max(new_trise, n.new_trise);
			}
			new_tfall += tFall;
			new_trise += tRise;
			value = func(op);
			ready = true;
		}
		bool is_ready() const {
			return ready;
		}
		int get_value() { return value; }
		bool set_value(int x) {
			if (isInputPort())
			{
				value = x;
				return true;
			}
			else
				return false;
		}
	};
}
namespace vp {
	class new_circuit :public Circuit
	{
		vector<mynode> new_nodes;

	public:
		new_circuit(Circuit& x) :Circuit(x) {
			for (int i = 0;i < getNodesCount();i++)
			{
				mynode n(x.node(i));
				new_nodes.push_back(n);
			}
		}
		mynode& new_node(size_t index) {
			return new_nodes[index];
		}

	};
}


void printLn();
void BuildTimeline(new_circuit& x);

int main() {

	try {

		// Initialize Circuit
		Circuit circuit;
		circuit.parseFile("FullAdder2.v");
		circuit.parseDelaysFile("gateDelays.delay");
		new_circuit operating(circuit);

	}
	catch (ParseError& e) {
		cout << "ParseError: " << e.what() << endl;
	}
	catch (exception& e) {
		cout << e.what() << endl;
	}

	cout << endl << endl;

#ifdef _WIN32
	system("pause");
#endif
	return 0;
}


void BuildTimeline(new_circuit &x)
{
	freopen("Test2.txt", "r", stdin); 
	string temp;
	int currentT = 0;
	EventInput.resize(100);
	bool flag = true;

	while(cin >> temp)
	{
		if(temp.find("#") == 0)
		{
			if(flag)
				{
					for ( int i =0; i < x.getInputNodesCount(); i++)
						if(EventInput[0].find(x.inputNode(i).getName())==EventInput[0].end())
							EventInput[0][x.inputNode(i).getName()] = 3;

					flag = false;
				}


			temp = temp.substr(1, temp.size());
			currentT += stoi(temp);
			cout << currentT;
		}
		else
		{
			int loc = temp.find("=");
			string tempV = temp.substr(loc+1, temp.size());
			temp = temp.substr(0, loc);
			EventInput[currentT][temp] = stoi(tempV);
		}

	}

	EventInput;
	system("pause");
	
}


void JSONwave(map<string, string> input, map<string, string> output)
{
	string waveFinal;

	JSON << "{\n\tconfig: {hscale:4, skin:'narrow'},\n\thead:{text:'Circuit 1', tick: 0,},\n";	//Initial Diagram configs
	JSON << "\tsignal: [\n";	//Beggining of signal code

	//Formatting input to X's and Z's
	for (auto it = input.begin(); it != input.end(); ++it)
		for ( int j =0; j < it->second.size(); j++)
			if ( it->second[j] == '2') it->second[j] = 'Z';
			else if(it->second[j] == '3') it->second[j] = 'X';

	//Getting input Signals
	getSignalData(input);
	
	JSON << "\t\t{},\n";

	//Getting output Signals
	getSignalData(output);

	JSON << "\t\t]\n}\n";			//END of signal code & wave

	waveFinal = JSON.str();	//Copying info from stream.
	cout << waveFinal;	//Outputting wavedrom code
	system("pause");
	
}

//Reformats the data to wavedrom format
string getData(string data, bool type, int count)
{
	char last = data[0];	//Initializing the comparator.
	stringstream temp;


	temp << "data: [";

	//Looping and reformatting.
	if(!type)
		for (int i = 1; i < data.size(); i++)
		{
			if (data[i] == last) data[i] = '.';
			else last = data[i];
		}
	else
	{
		
		for (int i = 0; i < dta[0].size(); i++)
		{
			string out;
			for (int j = count; j >= 0; j--)
				out += dta[j][i];

			temp << "'" << out << "',";
		}

		temp << "]";
		JSON << temp.str();

	}

	return data;
}

void getSignalData(map<string, string> map1)
{
	for (auto it = map1.begin(); it != map1.end(); ++it)
	{
		if (it->first.find('[') > 0 && it->first.find('[') < it->first.size())
		{
			int size = it->first.find('[');	//Getting Variable name size
			int Tcount = 0; //Total variables searches count.
			string name = it->first.substr(0, size);	//Getting Variable name
			dta[0] = it->second; //Initializing first value.
			int count = 0; //Total variables of same bus count.

			JSON << "\t\t{name: '" << name << "', wave: '";

			for ( int i =0; i < it->second.size(); i++)
				JSON << "=";

			JSON << "', ";

			for (int i = 1; i < map1.size() && it != map1.end(); i++)
			{
				it++;
				int subset = it->first.find('[');
				string sub = it->first.substr(0, subset);
				Tcount++;

				if (sub == name && sub.size() == size)
				{
					dta[i] = it->second;
					count++;
				}
			}

			while((Tcount--)-count) it--;

			getData(it->second, 1, count);
			JSON << "},\n";
			
		}
		else
		{
			 JSON << "\t\t{name: '" << it->first << "', wave: '" << getData(it->second, 0, 0) << "'},\n";
		}

		
	}

}
