/////////////////////////////////////////////////////////////////////
// Graph.h - Graph Library      Analysing StrongComponents         //
//            And Display StrongComponent , test StrongComponents  //
//                by given hardcoded graph                         //
// Ver 1.1                                                         //
// Language:    Visual C++ 2012                                    //
// Platform:    MacPro     virtual Machine Windows 8               //
// Application: Help for CSE687 - OOD, Pr#1, Spring 2013           //
// Author:      YUKUI YE,            Syracuse University           //
//              (315) 751-0156, yye@syr.edu                        //
/////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include "Graph.h"
using namespace GraphLib;

typedef Graph<std::string, std::string> graph;
typedef Vertex<std::string, std::string> vertex;
typedef Display<std::string, std::string> display;
typedef Utility<std::string, std::string> utility;

void showVert(Vertex<std::string, std::string>& v)
{
  std::cout << "\n  " << v.id();
}

template<typename V, typename E>
void TshowVert(Vertex<V,E>& v)
{
  std::cout << "\n  " << v.id();
}

  //----< convert integer to string >--------------------------
  std::string ToString(int i)
  {
    std::ostringstream in;
    in << i;
    return in.str();
  }
  //----< convert string to integer >--------------------------
  int ToInt(const std::string& str)
  {
    std::istringstream in(str);
    int retn;
    in >> retn;
    return retn;
  }


#ifdef TEST_GRAPH

int main()
{
  std::cout << "\n  Testing Graph Library";
  std::cout << "\n =======================\n";
  try
  {
    std::cout << "\n  Constructing Graph instance";
    std::cout << "\n -----------------------------";
    graph g;
	vertex v("");
    vertex v1("v1");
    vertex v2("v2");
    vertex v3("v3");
    vertex v4("v4");
    vertex v5("v5");
    vertex v6("v6");
    vertex v7("v7");
    vertex v8("v8");
    vertex v9("v9");
    g.addVertex(v1);
    g.addVertex(v2);
    g.addVertex(v3);
    g.addVertex(v4);
    g.addVertex(v5);
    g.addVertex(v6);
    g.addVertex(v7);
    g.addVertex(v8);
    g.addVertex(v9);
    g.addEdge("e1",v1,v2);
    g.addEdge("e2",v1,v3);
    g.addEdge("e3",v1,v4);
    g.addEdge("e4",v1,v5);
    g.addEdge("e5",v1,v6);
	g.addEdge("e6",v3,v1);
    g.addEdge("e7",v3,v8);
    g.addEdge("e8",v6,v4);
	g.addEdge("e9",v7,v1);
	g.addEdge("e10",v7,v3);
	g.addEdge("e11",v7,v4);
	g.addEdge("e12",v8,v7);
	g.addEdge("e13",v9,v1);
	g.addEdge("e14",v9,v7);
	g.addEdge("e15",v9,v6);
	display::show(g);
	std::cout<<std::endl;
	utility::displayStrongConnectedComponents(g);
	system("pause");
    /*
    display::show(g);
    std::cout << "\n";

    std::cout << "\n  Making copy of instance";
    std::cout << "\n -------------------------";
    graph gcopy = g;
    display::show(gcopy);
    std::cout << "\n";

    std::cout << "\n  Modifying copy's values";
    std::cout << "\n -------------------------";
    for(auto& v : gcopy)
      v.value() += "copy";

    display::show(gcopy);
    std::cout << "\n";

    std::cout << "\n  Assigning original instance to copy";
    std::cout << "\n -------------------------------------";
    gcopy = g;
    display::show(gcopy);
    std::cout << "\n";

    std::cout << "\n  Vertices with no Parents:";
    std::cout << "\n ---------------------------";

    std::vector< vertex > verts = display::vertsWithNoParents(g);
    std::cout << "\n  ";
    for(size_t i=0; i<verts.size(); ++i)
      std::cout << verts[i].value().c_str() << " ";
    std::cout << "\n";

    std::cout << "\n  Testing Depth First Search with function pointer";
    std::cout << "\n --------------------------------------------------";
    for(auto& vert : g)
    {
      std::cout << "\n  starting at id " << vert.id();
      g.dfs(vert, TshowVert<std::string, std::string>);
      // this works too: 
      // g.dfs(vert, showVert);
    }
    std::cout << "\n";

    std::cout << "\n  Testing Depth First Search with Functor";
    std::cout << "\n -----------------------------------------";

    class showFunctor
    {
    public:
      void operator()(Vertex<std::string, std::string>& vert)
      {
        std::cout << "\n  From functor: vertix id = " << vert.id();
        std::cout << ", number of edges = " << vert.size();
      }
    };

    g.dfs(g[0], showFunctor());
    std::cout << "\n";

    std::cout << "\n  Testing Serialization to XML";
    std::cout << "\n ------------------------------";
    std::string str = GraphToXmlString(g);
    std::cout << str << "\n";
    std::ofstream out("testGraph.xml");
    out << str << "\n";

    std::cout << "\n  Testing Graph construction from XML";
    std::cout << "\n -------------------------------------";
    graph gtest;
    GraphFromXmlString(gtest, str);
    display::show(gtest);
    std::cout << "\n\n";*/
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
  }
  std::cout << "\n\n";
  return 0;
}

#endif

