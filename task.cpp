#include <vector>
#include <stack>
#include <iostream>
#include "graph.h"


using graph::Graph;
using graph::Vertex;


enum class Colors {
  White,
  Gray,
  Black
};

Graph GetTransposedGraph(const Graph& g)
{
    Graph transposed(g.size());
    for (Vertex v = 0; v <g.size(); ++v)
    {
        for (Vertex neighbor : g.GetAdjacents(v))
        {
            transposed.AddEdge(neighbor, v);
        }
    }
    return transposed;
}

void GoDFS(Vertex v, std::vector<Colors>& colors, const Graph& gr)
{
    switch (colors[v])
    {
        case Colors::White :
            colors[v] = Colors::Gray;
            std::cout << "I'm seing " << v << std::endl;
            for(Vertex w : gr.GetAdjacents(v))
                GoDFS(w, colors, gr);
            colors[v] = Colors::Black;
            break;
        case Colors::Gray :
            std::cout << "I'm seeing incoming cycle at " << v << std::endl;
            break;
        case Colors::Black :
            std::cout << "I'm seeing completed point " << v << std::endl;
            break;
    }

}

void FillOrderDFS(Vertex v, std::vector<Colors>& colors, const Graph& gr, std::stack<Vertex>& stack)
{
        switch (colors[v])
    {
        case Colors::White :
            colors[v] = Colors::Gray;
            for(Vertex w : gr.GetAdjacents(v))
                FillOrderDFS(w, colors, gr, stack);
            colors[v] = Colors::Black;
            stack.push(v);
            break; 
        case Colors::Gray :
            break;
        case Colors::Black :
            break;
    }
}

void FirstDFS(const Graph& g, std::stack<Vertex>& stack)
{
    std::vector<Colors> colors(g.size(), Colors::White);
    
    for (Vertex i = 0; i < g.size(); ++i) {
        if (colors[i] == Colors::White) {
            FillOrderDFS(i, colors, g, stack);
        }
    }
}

int FindConnectedComponents(const Graph& g)
{
    if (g.empty()) return 0;

    std::stack<Vertex> stack;
    int count = 0;

    FirstDFS(g, stack);
    Graph transposed = GetTransposedGraph(g);
    
    std::vector<Colors> colors(transposed.size(), Colors::White);
    while(!stack.empty())
    {
        Vertex v = stack.top();
        stack.pop();

        if (colors[v] == Colors::White)
        {
            GoDFS(v, colors, transposed);
            count++;
        }
    }
    return count;
}
