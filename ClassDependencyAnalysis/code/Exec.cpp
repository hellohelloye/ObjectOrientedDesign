/////////////////////////////////////////////////////////////////////
// Exec.cpp - Support file and directory operations				   //
//  Language:       Visual C++, ver 6.0                            //
//  Platform:       MacBook Pro, Windows7 Pro				       //
//  Application:    CSE687 project #3	                           //
//  Author:         YUKUI YE, Syracuse University	     		   //
//                  (315) 751-0156, mollysmile.ye@gmail.com        //
/////////////////////////////////////////////////////////////////////

#pragma warning(disable: 4018)
#pragma warning(disable: 4129)
#pragma warning(disable: 4075)
#include <iostream>
#include <string>
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "FoldingRules.h"
#include "Graph.h"
#include "TypeAnal.h"
#include "RelationshipAnal.h"
#include "XmlWriter.h"
#include "FileManager.h"

using namespace GraphLib;
typedef Graph<std::string, std::string> graph;
typedef Vertex<std::string, std::string> vertex;
typedef Display<std::string, std::string> display;
TypeAnal * FirstParseToParser(std::vector<std::string>files);
RelationshipAnal* SecondParseToParser(TypeAnal* VectorFromFirstParse, std::vector<std::string>files);
void CreateGraph(TypeAnal* VectorFromFirstParse, RelationshipAnal* GetRelation);


///////////////////////////////////////////////////////////////////
// Main Function

int main(int argc, char* argv[])
{
	FileManager FM;
	std::string InputPath = argv[1];
	//std::cout<<"\n  Enter path:";
	//std::getline(std::cin, InputPath);
	
	std::vector<std::string>files = FM.GetFiles(InputPath);					// Getting all .h &.cpp files from input path
	TypeAnal* ClassName = FirstParseToParser(files);						// Processing First parse in parser
	RelationshipAnal* GetRelation = SecondParseToParser(ClassName, files);	// Processing Second parse in parser
	CreateGraph(ClassName,GetRelation);										// Create a graph of all vertices and their relationship	
}


//////////////////////////////////////////////////////////////////////////
//Function used to process the First parse into the files of given path

TypeAnal * FirstParseToParser(std::vector<std::string>files)
{
	TypeAnal* pType = new TypeAnal;
	for(int i=0; i<files.size(); ++i)
	{

	ConfigParseToConsole configure(pType);
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(files.at(i).c_str()))
        {
          std::cout << "\n  could not open file " << files.at(i).c_str() << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
      }
      // now that parser is built, use it

      while(pParser->next())
        pParser->parse();
		
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }

  }
	std::cout<<"  First parse : Processing all files to get Class names\n";
	std::cout<<"  -----------------------------------------------------\n";
	pType->PrintToCheck();
	return pType;
}

//////////////////////////////////////////////////////////////////////////
//Function used to process the Second parse into the files of given path

RelationshipAnal* SecondParseToParser(TypeAnal* VectorFromFirstParse, std::vector<std::string>files)
{
	std::vector<vertex> v;
	std::vector<std::string> ClassName = VectorFromFirstParse->ReturnVector();
	RelationshipAnal* pRelation = new RelationshipAnal(VectorFromFirstParse);
	for(int i=0; i<files.size(); ++i)
	{
		ConfigParseToConsole configure(VectorFromFirstParse,pRelation);
		Parser* pParser = configure.BuildForRule();
		try
		{
		  if(pParser)
		  {
			if(!configure.Attach(files.at(i).c_str()))
			{
			  std::cout << "\n  could not open file " << files.at(i).c_str() << std::endl;
			  continue;
			}
		  }
		  else
		  {
			std::cout << "\n\n  Parser not built\n\n";
		  }
		  // now that parser is built, use it

		   while(pParser->next())
			pParser->parse();
		}
		catch(std::exception& ex)
		{
		  std::cout << "\n\n    " << ex.what() << "\n\n";
		}
		std::cout << "\n\n";
	
   }
	std::cout<<"  Second parse : Findig relationship among the files\n";
	std::cout<<"  --------------------------------------------------\n";
	pRelation->PrintToCheck();
	return pRelation;
}

void mOut(const std::string& message)
{
  std::cout << "\n  " << message.c_str();
}


//////////////////////////////////////////////////////////////////////////
//Function used to write values from graph to XML file.

void GraphToXML(graph& g)
{
	XmlWriter wtr;
	mOut("\n");
	mOut("XML writer");
	mOut("----------");
	std::cout<<"\n  Write to XML...\n";
	wtr.addDeclaration();
	wtr.addBody("\n");
	wtr.start("Dependency Analysis\n");
	wtr.addAttribute("vertex", "class");	
	wtr.addAttribute("edge", "relationship");
	wtr.addBody("\n");

	graph::iterator iter = g.begin();
	XmlWriter V_xml;
      while(iter != g.end())
      {
        vertex v = *iter;
		V_xml.addBody("\n");
		V_xml.start(v.value());
		V_xml.addBody("\n");
		V_xml.addAttribute("ID", std::to_string(v.id()));
        for(size_t i=0; i<v.size(); ++i)
        {
			XmlWriter E_xml;
			vertex::Edge edge = v[i];
		    E_xml.start(g[edge.first].value().c_str());
		    E_xml.addAttribute("Relation", ((edge.second).c_str()));
			E_xml.end();
			E_xml.addBody("\n");
			V_xml.addBody(E_xml.xml());
			
        }
		 V_xml.end();
		V_xml.addBody("\n");
        ++iter;
      }

	  wtr.addBody(V_xml.xml());
	  wtr.addBody("\n");
	wtr.end();

	std::ofstream out("test.xml");
	 if(out.good())
	  {
		out << wtr.xml().c_str();
		out.close();
	  }
}

//////////////////////////////////////////////////////////////////////////
//Function used to create a graph according to the class names and 
//relations obtained from the two parses.

void CreateGraph(TypeAnal* VectorFromFirstParse, RelationshipAnal* GetRelation)
{
	graph ToGraph;
	std::vector<vertex> v;
	std::vector<std::string> ClassName = VectorFromFirstParse->ReturnVector();
	for(size_t i=0; i<ClassName.size(); ++i)
	{
		vertex v(ClassName.at(i));
		ToGraph.addVertex(v);
	}
	std::vector<elementRelationship> Relationship = GetRelation->ReturnVector(); 

	for(int i=0; i<Relationship.size();i++)
	  {
		  for(int j=0;j<ToGraph.size();j++)
		  {
			  for(int k=0;k<ToGraph.size();k++)
			  {
				  if((ToGraph[j].value()==Relationship[i].class1)&&(ToGraph[k].value()==Relationship[i].class2))
				  { 
					  ToGraph.addEdge(Relationship[i].Relation,ToGraph[j],ToGraph[k]);
				  }
			  }
		  }
	  }
	 std::cout << "\n\n";
	 std::cout << "\n  Graph of all Vertices and their relationship";
	 std::cout << "\n  --------------------------------------------";
	display::show(ToGraph);
	GraphToXML(ToGraph);
}




