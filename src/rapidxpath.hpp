#ifndef RAPID_XPATH_HPP_INCLUDED
#define RAPID_XPATH_HPP_INCLUDED

#include <string>
#include <vector>
#include <exception>
#include <iostream>
#include <algorithm>
#include <boost/any.hpp>

#include "rapidxml.hpp"
#include "rapidxml_iterators.hpp"

namespace rapidXPath{

enum class ResultType{
	node,
	many_nodes,
	string,
	many_strings,
	failure
};

typedef rapidxml::xml_node<char>* node;
typedef std::vector<rapidxml::xml_node<char>*> many_nodes;
typedef std::string string;
typedef std::vector<std::string> many_strings;


class ResultException : std::exception{
public:
	std::string s;
	ResultException(std::string str) : s(str){
	}

	const char* what ()  const throw() override{
		return s.c_str();
	}

};

typedef ResultException XPathException;

boost::any XPath(rapidxml::xml_node<char> *node, std::string querry){
	std::vector<rapidxml::xml_node<char>*> newnode;
	newnode.push_back(node);
	while(true){

		if(querry.size() == 0){
			if(newnode.size() == 1)
				return boost::any(newnode[0]);
			else if (newnode.size() == 0)
				return boost::any( int(-1));
			else
				return boost::any( std::vector<rapidxml::xml_node<>*>(newnode));
		}

		//determine sub querry
		int splitQuerry = 1;
		for(splitQuerry; splitQuerry < querry.size() && querry[splitQuerry]!='/'; splitQuerry++);
		std::string subquerry = querry.substr(1, splitQuerry-1);
		std::string namequerry = subquerry.substr(0, subquerry.find("@") == string::npos && subquerry.find("[") == string::npos ? 
														subquerry.size()
														:	std::min(subquerry.find("@"), subquerry.find("[")) == std::string::npos ?
																std::max(subquerry.find("@"), subquerry.find("["))
																: std::min(subquerry.find("@"), subquerry.find("[")));

		
		//return the values @attr text
		if(subquerry == "text" || subquerry[0] == '@'){
			std::vector<std::string> str;
			if(subquerry == "text"){
				for(int i = 0; i < newnode.size(); i++){
					str.push_back(newnode[i]->value());
				}
				switch(str.size()){
					case 0: return boost::any( int(-1));
					case 1: return boost::any( std::string(str[0]));
					default: return boost::any( std::vector<std::string>(str));
				}
			} else if(subquerry[0] == '@'){
				for(int i = 0; i < newnode.size(); i++){
					std::string attrName = subquerry.substr(1);
					rapidxml::xml_attribute<>* attr = newnode[i]->parent()->first_attribute();
					while(attr!= nullptr){
						if(attr->name() == attrName){
							str.push_back(std::string(attr->value()));
						}
						attr = attr->next_attribute();
					}
				}
			}
			switch(str.size()){
				case 0: return boost::any( int(-1)); break;
				case 1: return boost::any( string(str[0])); break;
				default: return boost::any( std::vector<std::string>(str));
			}	
		}

		//filter by name
		if(namequerry != ""){
			newnode.erase(std::remove_if(newnode.begin(),newnode.end(), [&namequerry](rapidxml::xml_node<>* n){
				return namequerry != std::string(n->name());
			}), newnode.end());
		}

		//by index
		if(std::min(subquerry.find("["), subquerry.find("]")) != string::npos){
			std::string indexString = subquerry.substr(subquerry.find("[")+1, 
														 subquerry.find("]") - subquerry.find("[") - 1);
			//by index attr
			if(indexString.find("@") != std::string::npos){
				std::string name = indexString.substr(1, indexString.find("=") -1 );
				std::string value = indexString.substr(indexString.find("'")+1, 
													indexString.size() - indexString.find("'") - 2);
				newnode.erase(std::remove_if(newnode.begin(), newnode.end(), [&name, &value](rapidxml::xml_node<>* n){
					rapidxml::xml_attribute<>* attr = n->first_attribute();
					while(attr!= nullptr){
						if((std::string(attr->name()) == name) && (std::string(attr->value()) == value)){
							return false;
						}
						attr = attr->next_attribute();
					}
					return true;
				}), newnode.end());
			}else{//index by integer
				rapidxml::xml_node<char>* n;
				int i;
				try{
					i = std::stoi(indexString);
				}catch(std::exception& e){
					return boost::any( int(-1));
				}
				if(! i<newnode.size()){
					n = newnode[i];
					newnode.clear();
					newnode.push_back(n);
				}else{
					return boost::any( int(-1));
				}
	 		}
 		}	

 		//update resources
 		querry = querry.substr(splitQuerry);	

 		std::vector <rapidxml::xml_node<char>*>newUpdatedNodeList;
		for(int i = 0; i<newnode.size();i++){
			rapidxml::xml_node<>* n = newnode[i]->first_node();		
			while (n != nullptr){
				newUpdatedNodeList.push_back(n);
				n = n->next_sibling();
			}
		}
		newnode = newUpdatedNodeList;	
	}
}

} // end namespace rapidXPath

#endif