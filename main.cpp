#include <iostream>
#include<vector>
#include <string>
#include "Circuit.h"
#include "ParseError.h"

using namespace std;
using namespace vp;
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
namespace vp{
	class mynode:protected Node {
	private:
		bool ready;
		int value;
		int(*func)(vector<int> operands);
		vector<int> op;
		int new_tfall, new_trise;
	public:
		mynode(const Node& x):Node(x),ready(false),value(0),new_tfall(0),new_trise(0) {
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
				mynode &n = (x.node(x.getNodeIndex(inputNode(i).getName())));
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
	class new_circuit :protected Circuit
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

int main() {
	
	try {
		
		// Initialize Circuit
		Circuit circuit;
		circuit.parseFile("FullAdder2.v");
		circuit.parseDelaysFile("gateDelays.delay");
		vector<mynode> new_circuit;

		for (int i = 0; i < circuit.getNodesCount(); i++) {	
			/*cout << " Node " << i << endl;
			cout << "  name:      " << circuit.node(i).getName() << endl;
			cout << "  type:      " << circuit.node(i).getType() << endl;
			cout << "  is input:  " << (circuit.node(i).isInputPort() ? "Yes" : "No") << endl;
			cout << "  is output: " << (circuit.node(i).isOutputPort() ? "Yes" : "No") << endl;
			cout << "  is gate:   " << (circuit.node(i).isGate() ? "Yes" : "No") << endl;
			cout << "  trise:     " << circuit.node(i).getTRise() << endl;
			cout << "  tfall:     " << circuit.node(i).getTFall() << endl;*/
			/*cout << "  taba3y dah:" << endl;*/
			mynode x(circuit.node(i));
			new_circuit.push_back(x);
			
			if (new_circuit[i].isOutputPort())
				output_nodes.push_back(new_circuit[i]);
			else if (circuit.node(i).isInputPort())
				input_nodes.push_back(new_circuit[i]);
			printLn();
		}

		
		
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

void printLn() {
	int o = 78;
	cout << " ";
	while (o--) {
		cout << "-";
	}
	cout << endl;
}