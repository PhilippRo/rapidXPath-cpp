#include <rapidxml.hpp>
#include <rapidxpath.hpp>
#include <rapidxml_print.hpp>
#include <iostream>
#include <string>

using namespace rapidXPath;
using namespace rapidxml;
using namespace std;

string xml_string = "<languages>" 
	"<language name='C++'>high abstraction, bare metal</language>"
	"<language name='Java'> less abstraction, no bare metal </language>"
	"<fp>"
		"<language name='Haskell'> huge abstraction, really no bare metal </language>"
	"</fp>"
"</languages>";

void testSimplePathQuerry(xml_node<>* n){
	try{
		auto r = boost::any_cast<std::string>(XPath(n, "/languages/fp/language/text"));
		std::cout << "\t" << r << std::endl;
		std::cout << "[ ok   ] testSimplePathQuerry main.cpp" << endl;
	}catch(std::exception& e){
		std::cout << "[failed] testSimplePathQuerry main.cpp" << endl;
		std::cout << e.what() << std::endl;
	}
}

void testSimpleAttributeQuerry(xml_node<>* n){
	try{
		auto r = boost::any_cast<std::string>(XPath(n, "/languages/fp/language/@name"));
		std::cout << "\t" << r << std::endl;
		std::cout << "[ ok   ] testSimpleAttributeQuerry main.cpp" << endl;
	}catch(std::exception& e){
		std::cout << "[failed] testSimpleAttributeQuerry main.cpp" << endl;
		std::cout << e.what() << std::endl;
	}
}

void testIndexAdressQuerry(xml_node<>* n){
	try{
		auto r = boost::any_cast<std::string>(XPath(n, "/languages/language[1]/text"));
		std::cout << "\t" << r << std::endl;
		std::cout << "[ ok   ] testIndexAdressQuerry main.cpp" << endl;
	}catch(std::exception& e){
		std::cout << "[failed] testIndexAdressQuerry main.cpp" << endl;
		std::cout << e.what() << std::endl;
	}
}

void testIndexAttributeQuerry(xml_node<>* n){
	try{
		auto r = boost::any_cast<std::string>(XPath(n, "/languages/language[@name='C++']/text"));
		std::cout << "\t" << r << std::endl;
		std::cout << "[ ok   ] testIndexAttributeQuerry main.cpp" << endl;
	}catch(std::exception& e){
		std::cout << "[failed] testIndexAttributeQuerry main.cpp" << endl;
		std::cout << e.what() << std::endl;
	}
}

void testSimpleNodeQuerry(xml_node<>* n){
	try{
		auto r = boost::any_cast<rapidxml::xml_node<char>*>(XPath(n, "/languages/fp/language"));
		std::cout << "[ ok   ] testSimpleNodeQuerry main.cpp" << endl;
	}catch(std::exception& e){
		std::cout << "[failed] testSimpleNodeQuerry main.cpp" << endl;
		std::cout << e.what() << std::endl;
	}
}

void testSimpleNodesQuerry(xml_node<>* n){
	try{
		auto r = boost::any_cast<std::vector<rapidxml::xml_node<char>*>>(XPath(n, "/languages/language"));
		std::cout << "[ ok   ] testSimpleNodesQuerry main.cpp" << endl;
	}catch(std::exception& e){
		std::cout << "[failed] testSimpleNodesQuerry main.cpp" << endl;
		std::cout << e.what() << std::endl;
	}
}

int main(){
	xml_document <char> *d = new xml_document<>();
	d->parse<0>(const_cast<char *>(xml_string.c_str()));
	testSimplePathQuerry(d->first_node());
	testSimpleAttributeQuerry(d->first_node());
	testIndexAdressQuerry(d->first_node());
	testIndexAttributeQuerry(d->first_node());
	testSimpleNodeQuerry(d->first_node());
	testSimpleNodesQuerry(d->first_node());
}