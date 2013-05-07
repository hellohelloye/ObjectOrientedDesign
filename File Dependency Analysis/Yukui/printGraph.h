/////////////////////////////////////////////////////////////////////
//  Language:       Visual C++, ver 6.0                            //
//  Platform:       MacBook Pro, Windows7 Pro				       //
//  Application:    CSE687 project #3	                           //
//  Author:         YUKUI YE, Syracuse University	     		   //
//                  (315) 751-0156, mollysmile.ye@gmail.com        //
/////////////////////////////////////////////////////////////////////
#include <queue>
#include <string>
#include <iostream>
#include "Parser.h"
#include "SemiExpression.h"
#include "Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "classVector.h"
#include "relationVector.h"
#include "Graph.h"
#include "XmlWriter.h"
#include "XmlReader.h"

typedef GraphLib::Graph<std::string, std::string> graph;
typedef GraphLib::Vertex<std::string,std::string> vertex;
typedef GraphLib::Display<std::string,std::string> display;

class outPutGraph
{
public:
	void createGraph()
	{
		graph g;
		for(size_t i=0; i< pClassV->getClassVector().size();++i)
		{
			vertex v(pClassV->getClassVector()[i].classname);
			g.addVertex(v);
		}
		std::vector<elemRelation> Relationship=pRelationV->getRelationVector();
		for(size_t i=0;i<Relationship.size();++i)
		{
			for(size_t j=0;j<g.size();++j)
			{
				for(size_t m=0;m<g.size();++m)
				{
					if( (g[j].value()==Relationship[i].parentClass)
						&& (g[m].value()==Relationship[i].childClass))
					{
						g.addEdge(Relationship[i].relationship,g[j],g[m]);
					}
				}
			}
		}
		std::cout<<"\n\n vertex and edges exit in Graph\n ";
		display::show(g);
	}
private:
	classVector* pClassV;
	relationVector* pRelationV;
};

void graphToXmlString(graph& g)
{
	XmlWriter myxml;
	myxml.addDeclaration();
	myxml.addBody("\n\n\n\n Graph To Xml String\n\n");
	myxml.start("Dependency Analysis Between Classes\n");
    myxml.addAttribute("class","vertex");
	myxml.addAttribute("relationship","edge");
	myxml.addBody("\n");

     graph::iterator iter = g.begin();	
	 XmlWriter nodexml;
      while(iter != g.end())
      {
        vertex v = *iter;
		nodexml.addBody("\n");
        nodexml.start((v.value()).c_str());
		nodexml.addBody("\n");
		nodexml.addAttribute("className:",(v.value()).c_str());
		for(size_t i=0; i<v.size(); ++i)
        {
		  XmlWriter edgexml;
          vertex::Edge edge = v[i];
		  edgexml.start(g[edge.first].value().c_str());
		  edgexml.addAttribute("childName:",g[edge.first].value().c_str());
		  edgexml.addAttribute("relationship", (edge.second).c_str() );
		  edgexml.end();
		  edgexml.addBody("\n");
		  nodexml.addBody(edgexml.xml());
        }
		nodexml.end();
		nodexml.addBody("\n");
		++iter;
	  }		
		myxml.addBody(nodexml.xml());
		myxml.addBody("\n");
		myxml.end();

  std::cout << "\n\n  writing completed XML";
  std::cout << "\n -----------------------\n";
  std::cout << myxml.xml();
  std::ofstream out("yyetest.xml");
  if(out.good())
  {
    out << myxml.xml().c_str();
    out.close();
  }
  std::cout << std::endl; 
}