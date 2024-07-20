#include "main.h"


/* pin */
titian::Pin::Pin(const String & title, PinType type, Node * node)
	: title(title), type(type), node(node)
{}

/* node */
titian::Node::Node()
{}

void titian::Node::call()
{
	if (function) {
		function();
	}
	if (flow_output && flow_output.value().node) {
		flow_output.value().node->call();
	}
}
