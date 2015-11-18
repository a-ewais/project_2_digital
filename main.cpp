#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<vector>
#include <string>
#include<sstream>
#include<map>
#include "Circuit.h"
#include "ParseError.h"
<<<<<<< HEAD
using namespace std;
using namespace vp;
string getData(string, bool, int);
string dta[100];
vector< map<string, int> > EventInput, EventOutput;
stringstream JSON;
map<string, string> out, in;
=======
#define _CRT_SECURE_NO_WARNINGS

using namespace std;
using namespace vp;

string getData(string, bool, int);
string dta[100];
vector< map<string,int> > EventInput, EventOutput;
stringstream JSON;

>>>>>>> origin/master
int xorr(vector<int> op)
{
	int x=0;
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
<<<<<<< HEAD
namespace vp {
	class new_circuit;
	class mynode;

=======
namespace vp{
	class new_circuit;
	class mynode;
	
>>>>>>> origin/master
	class mynode :public Node {
	private:
		int value;
		int(*func)(vector<int> operands);
		vector<int> op;
	public:
		mynode(const Node& x) :Node(x),value(3){
			 if (type == "xor")
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
<<<<<<< HEAD
				func=NULL;
		};
		int begin(new_circuit &x);
=======
				cout << type << endl;

		};
		void begin(new_circuit &x); 
		bool is_ready() const {
			return ready;
		}
>>>>>>> origin/master
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
		mynode& outputnode(int index)
		{
			return new_nodes[outputNodes[index]];
		}
		mynode& new_node(size_t index) {
			return new_nodes[index];
		}
		mynode& new_node(string name) {
			int index = getNodeIndex(name);
			return new_nodes[index];
		}

	};
<<<<<<< HEAD
	int mynode::begin(new_circuit &x) {
		int temp;
		for (int i = 0;i < getInputsCount();i++)
		{
			mynode &n = (x.new_node(x.getNodeIndex(inputNode(i).getName())));
			temp = n.get_value();
			if(temp<2)
				op.push_back(temp);
			else {
				value = temp;
				op.clear();
				return value;
			}
	
		}
		if (isOutputPort())
		{
			if (op.size() == 1)
				value = op[0];
			else
				value = 2;
		}
		else if(!isInputPort())
			value = func(op);
		op.clear();
		return value;
	}
=======
	void mynode::begin(new_circuit &x){
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
}
void timeline(new_circuit& x)
{
	EventOutput.resize(EventInput.size() * 2);
	for (int i = 0;i < EventInput.size();i++)
	{
		if (EventInput[i].size() != 0)
		{
			for (map<string, int>::iterator it = EventInput[i].begin();it != EventInput[i].end(); it++)
				x.new_node(it->first).set_value(it->second);
			for (int j = 0;j < x.getOutputNodesCount();j++)
			{
				x.outputnode(j).begin(x);
				//if(x.outputNode(j).)
			}

		}
	}

>>>>>>> origin/master
}
void timeline(new_circuit& x)
{
	auto eventinput(EventInput);
	EventOutput.resize(eventinput.size()+ 2);	
	for (int i = 0;i < eventinput.size();i++)
	{
		if (eventinput[i].size() != 0)
		{
			for (map<string, int>::iterator it = eventinput[i].begin();it != eventinput[i].end(); it++)
			{
				x.new_node(it->first).set_value(it->second);
				for (int j = 0;j < x.getNodesCount();j++)
				{
					
					if (x[x.getNodeIndex(it->first)][j])
					{
						eventinput[i + x.new_node(j).getTRise()][x.new_node(j).getName()] = x.new_node(j).begin(x);
						if (x.new_node(j).isOutputPort())
						{							
							EventOutput[i + x.new_node(j).getTRise()][x.new_node(j).getName()] = x.new_node(j).get_value();
						}
					}
				}
			}
		}		
	}
}	 //tested
void fill_input(new_circuit& operating)
		{
			map<string, int> x(EventInput[0]);
			for (int i = 0;i < EventInput.size();i++)
			{
				if (EventInput[i].size() != EventInput[0].size())
				{
					for (map<string, int> ::iterator it = EventInput[i].begin();it != EventInput[i].end();it++)
						x[it->first] = it->second;

<<<<<<< HEAD
					EventInput[i] = x;
				}
				else
					x = EventInput[i];
			}
			map<string, int> y;
			for (int i = 0;i < operating.getOutputNodesCount();i++)
				y[operating.outputNode(i).getName()] = 3;
			for (int i = 0;i < EventOutput.size();i++)
			{
				if (EventOutput[i].size() != 0)
					for (map<string, int> ::iterator it = EventOutput[i].begin();it != EventOutput[i].end();it++)
						y[it->first] = it->second;
				else
					EventOutput[i] = y;
			}
		}
void JSONwave();
void getSignalData(map<string, string>);
void BuildTimeline(new_circuit& x);
void convert_in();
void convert_out();
int main() {

	try {
=======
void JSONwave(map<string, string>, map<string, string>);
void getSignalData(map<string,string>);
void BuildTimeline(new_circuit& x);


int main() {

	try {
		
>>>>>>> origin/master
		// Initialize Circuit
		Circuit circuit;
		circuit.parseFile("FullAdder2.v");
		circuit.parseDelaysFile("gateDelays.delay");
<<<<<<< HEAD
		new_circuit operating(circuit);		
		BuildTimeline(operating);
		timeline(operating);
		fill_input(operating);
		convert_in();
		convert_out();
		JSONwave();
=======
		new_circuit operating(circuit);	
		BuildTimeline(operating);

>>>>>>> origin/master
		
	}
	catch (ParseError& e) {
		cout << "ParseError: " << e.what() << endl;
	}
	catch (exception& e) {
		cout << e.what() << endl;
	}
	
#ifdef _WIN32
	system("pause");
#endif
	return 0;
}

void BuildTimeline(new_circuit &x)
<<<<<<< HEAD
=======
{
	freopen("Test2.txt", "r", stdin); 
	string temp, name;
	int currentT = 0, size, Tcount= 0;
	EventInput.resize(100);
	bool flag = true;
	map<string, int> busMap;

	while(cin >> temp)
	{
		if(temp.find("#") == 0)
		{
			if(flag)
				{
					for ( int i =0; i < x.getInputNodesCount(); i++)
						if(EventInput[0].find(x.inputNode(i).getName())==EventInput[0].end())
						{
							EventInput[0][x.inputNode(i).getName()] = 3;

							if(x.inputNode(i).getName().find("[") > 0 && x.inputNode(i).getName().find("[") < x.inputNode(i).getName().size())
								busMap[x.inputNode(i).getName()] = -1;
						}

					flag = false;
				}


			temp = temp.substr(1, temp.size());
			currentT += stoi(temp);
			cout << currentT;
		}
		else
		{
			int loc = temp.find("="), count = 0;
			string tempV = temp.substr(loc+1, temp.size());
			temp = temp.substr(0, loc);
			bool Bus = false;
			
			for ( int i =0 ; i < 1e9; i++)
			{
				string tempBus = temp + "[" + to_string(i) + "]";
				map<string, int>::iterator it = busMap.find(tempBus);

				if(it != busMap.end())
					{
						int tempValue = stoi(tempV) & ( 1 << i);
						EventInput[currentT][tempBus] = tempValue >> i;
						Bus = 1;
					}
				else
					break;
			}

			if(!Bus)
				EventInput[currentT][temp] = stoi(tempV); //MAIN FUNCTIONAL
		}

	}

	EventInput;
	system("pause");
	
}

void JSONwave(map<string, string> input, map<string, string> output)
>>>>>>> origin/master
{
	freopen("Test.txt", "r", stdin);
	string temp, name;
	int currentT = 0, size, Tcount = 0;
	EventInput.resize(100);
	bool flag = true;
	map<string, int> busMap;

	for (int i = 0; i < x.getInputNodesCount(); i++)
		if (x.inputNode(i).getName().find("[") != -1)
			busMap[x.inputNode(i).getName()] = -1;

	while (getline(cin, temp))
	{
		if (temp.find("#") == 0)
		{
			if (flag)
			{
				for (int i = 0; i < x.getInputNodesCount(); i++)
					if (EventInput[0].find(x.inputNode(i).getName()) == EventInput[0].end())
						EventInput[0][x.inputNode(i).getName()] = 3;

				flag = false;
			}


			temp = temp.substr(1, temp.size());
			currentT += stoi(temp);
		}
		else
		{
			while (temp.find(" ") != -1)
			{
				int start = temp.find(" ");
				temp.erase(start, 1);
			}

			int loc = temp.find("="), count = 0;
			string tempV = temp.substr(loc + 1, temp.size());
			temp = temp.substr(0, loc);
			bool Bus = false;

			for (int i = 0; i < 1e9; i++)
			{
				string tempBus = temp + "[" + to_string(i) + "]";
				map<string, int>::iterator it = busMap.find(tempBus);

				if (it != busMap.end())
				{
					int tempValue = stoi(tempV) & (1 << i);
					EventInput[currentT][tempBus] = tempValue >> i;
					Bus = 1;
				}
				else
					break;
			}

			if (!Bus)
				EventInput[currentT][temp] = stoi(tempV); //MAIN FUNCTIONAL
		}

	}

	EventInput;
	system("pause");

}

void JSONwave()
{

	string waveFinal;

	JSON << "{\n\tconfig: {hscale:4, skin:'narrow'},\n\thead:{text:'Circuit 1', tick: 0,},\n";	//Initial Diagram configs
	JSON << "\tsignal: [\n";	//Beggining of signal code

<<<<<<< HEAD
								//Formatting input to X's and Z's
	for (auto it = in.begin(); it != in.end(); ++it)
	{
		for (int j = 0; j < it->second.size(); j++)
		{
			if (it->second[j] == '2') it->second[j] = 'z';
			else if (it->second[j] == '3') it->second[j] = 'x';
		}
	}
=======
	//Formatting input to X's and Z's
	for (auto it = input.begin(); it != input.end(); ++it)
		for ( int j =0; j < it->second.size(); j++)
			if ( it->second[j] == '2') it->second[j] = 'Z';
			else if(it->second[j] == '3') it->second[j] = 'X';
>>>>>>> origin/master

	for (auto it = out.begin(); it != out.end(); ++it)
	{
		for (int j = 0; j < it->second.size(); j++)
		{
			if (it->second[j] == '2') it->second[j] = 'z';
			else if (it->second[j] == '3') it->second[j] = 'x';
		}
	}
	//Getting input Signals
	getSignalData(in);

	JSON << "\t\t{},\n";

	//Getting output Signals
	getSignalData(out);

	JSON << "\t\t]\n}\n";			//END of signal code & wave

	waveFinal = JSON.str();	//Copying info from stream.
	freopen("Output.txt", "w", stdout);
	cout << waveFinal;	//Outputting wavedrom code
	fclose(stdout);
	system("pause");

}

//Reformats the data to wavedrom format
string getData(string data, bool type, int count)
{
	char last = data[0];	//Initializing the comparator.
	stringstream temp;


	temp << "data: [";

	//Looping and reformatting.
	if (!type)
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

			for (int i = 0; i < it->second.size(); i++)
				JSON << "=";

			JSON << "', ";

			for (int i = 1; i < map1.size() && it != map1.end(); i++)
			{
				if (++it == map1.end()) { it--; break; }

				int subset = it->first.find('[');
				string sub = it->first.substr(0, subset);
				Tcount++;

				if (sub == name && sub.size() == size)
				{
					dta[i] = it->second;
					count++;
				}
			}

			while ((Tcount--) - count) it--;

			getData(it->second, 1, count);
			JSON << "},\n";

		}
		else
		{
			JSON << "\t\t{name: '" << it->first << "', wave: '" << getData(it->second, 0, 0) << "'},\n";
		}


	}

}
<<<<<<< HEAD
void convert_in()
{	for (int i = 0; i < EventInput.size(); i++)
	{
		for (auto it = EventInput[i].begin(); it != EventInput[i].end(); ++it)
		{
			in[it->first] = in[it->first] + to_string(it->second);
		}
	}
}
void convert_out()
{
	for (int i = 0; i < EventOutput.size(); i++)
	{
		for (auto it = EventOutput[i].begin(); it != EventOutput[i].end(); ++it)
		{
			out[it->first] = out[it->first] + to_string(it->second);
		}
	}
}
=======
>>>>>>> origin/master
