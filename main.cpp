void begin(new_circuit &x); {
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