#include "MeshGraph.h"
#include "BinaryHeap.h"

// For printing
#include <fstream>
#include <iostream>
#include <sstream>


MeshGraph::MeshGraph(const std::vector<Double3>& vertexPositions,
                     const std::vector<IdPair>& edges)
{
    for (int i = 0; i < vertexPositions.size(); i++){
        Vertex vertex = {i, vertexPositions[i]};
        vertices.push_back(vertex);
    }
    adjList.resize(vertexPositions.size());
    for (int i = 0; i < edges.size(); i++){
        int u = edges[i].vertexId0;
        int v = edges[i].vertexId1;
        adjList[u].push_back(&vertices[v]);
        adjList[v].push_back(&vertices[u]);
    }
}

double MeshGraph::AverageDistanceBetweenVertices() const
{
    double result = 0;
    for (int i = 0; i < vertices.size(); i++){
        std::list<Vertex*>::const_iterator it;
        for (it = adjList[i].begin(); it != adjList[i].end(); it++){
            result += Double3::Distance(vertices[i].position3D, (*it) -> position3D);
        }
    }
    result /= 2*TotalEdgeCount();
    return result;
}

double MeshGraph::AverageEdgePerVertex() const
{
    return (double)TotalEdgeCount()/TotalVertexCount();
}

int MeshGraph::TotalVertexCount() const
{
    return (int)vertices.size();
}

int MeshGraph::TotalEdgeCount() const
{
    int result = 0;
    for (int i = 0; i < vertices.size(); i++)      
        result += VertexEdgeCount(i);

    result /= 2;
    return result;
}

int MeshGraph::VertexEdgeCount(int vertexId) const
{
    if (vertexId < 0 || vertexId >= TotalVertexCount())
        return -1;
    
    int result = 0;
    std::list<Vertex*>::const_iterator it;
    for (it = adjList[vertexId].begin(); it != adjList[vertexId].end(); it++)
        result ++;
    return result;
}

void MeshGraph::ImmediateNeighbours(std::vector<int>& outVertexIds,
                                    int vertexId) const
{
    outVertexIds.clear();
    if (vertexId < 0 || vertexId >= TotalVertexCount())
        return ;

    std::list<Vertex*>::const_iterator it;
    for (it = adjList[vertexId].begin(); it != adjList[vertexId].end(); it++)
        outVertexIds.push_back((*it) -> id);
}

void MeshGraph::PaintInBetweenVertex(std::vector<Color>& outputColorAllVertex,
                                     int vertexIdFrom, int vertexIdTo,
                                     const Color& color) const
{
    if (vertexIdFrom < 0 || vertexIdFrom >= TotalVertexCount() || vertexIdTo < 0 || vertexIdTo >= TotalVertexCount()){
        outputColorAllVertex.clear();
        return;
    }
    
    outputColorAllVertex.resize(TotalVertexCount());
    for (int i = 0; i < TotalVertexCount(); i++){
        Color black = {0, 0, 0};
        outputColorAllVertex[i] = black;
    }
    int *prev = new int[TotalVertexCount()];
    double *dist = new double[TotalVertexCount()];
    BinaryHeap bh;
    int id, neighId;
    double weight, alt;
    std::vector<int> neighbours;
    for (int i = 0; i < TotalVertexCount(); i++){
        if (i == vertexIdFrom){
            bh.Add(i, 0);
            dist[i] = 0;
        }
        else {
            bh.Add(i, INFINITY);
            dist[i] = INFINITY;
        }
    }
    prev[vertexIdFrom] = vertexIdFrom;
    while (bh.PopHeap(id, weight)){       
        if (id == vertexIdTo)
            break;
        ImmediateNeighbours(neighbours, id);
        for (int i = 0; i < neighbours.size(); i++){
            neighId = neighbours[i];
            alt = weight + Double3::Distance(vertices[id].position3D, vertices[neighId].position3D);
            if (alt < dist[neighId]){
                dist[neighId] = alt;
                prev[neighId] = id;
                bh.ChangePriority(neighId, alt);
            }
        }      
    }
    if (dist[vertexIdTo] != INFINITY){
        while (prev[id] != id){
            outputColorAllVertex[id] = color;
            id = prev[id];
        }
        outputColorAllVertex[id] = color;
    }
    delete[] prev;
    delete[] dist;
}

void MeshGraph::PaintInRangeGeodesic(std::vector<Color>& outputColorAllVertex,
                                    int vertexId, const Color& color,
                                    int maxDepth, FilterType type,
                                    double alpha) const
{
    outputColorAllVertex.clear();
    if (vertexId < 0 || vertexId >= TotalVertexCount())
        return ;    

    bool *visited = new bool[TotalVertexCount()];
    double *dist = new double[TotalVertexCount()];
    int *depth = new int[TotalVertexCount()];
    outputColorAllVertex.resize(TotalVertexCount());

    for (int i = 0; i < TotalVertexCount(); i++){
        visited[i] = false;
        Color black = {0, 0, 0};
        outputColorAllVertex[i] = black;
    }

    BinaryHeap bh1, bh2;
    int id, neighId, dummy = 0;
    double weight, factor;
    std::vector<int> neighbours;
    bh1.Add(vertexId, dummy);
    dist[vertexId] = 0;   
    visited[vertexId] = true;
    depth[vertexId] = 0;
    while (bh1.PopHeap(id, weight)){
        if (depth[id] > maxDepth)
            break;

        if (type == FILTER_BOX){
            if (-alpha <= dist[id] && dist[id] <= alpha)
                factor = 1;
            else
                factor = 0;
        }   
        else if (type == FILTER_GAUSSIAN)
            factor = std::exp(-pow(dist[id], 2)/ pow(alpha, 2));
        
        outputColorAllVertex[id].r = color.r * factor;
        outputColorAllVertex[id].g = color.g * factor;
        outputColorAllVertex[id].b = color.b * factor;
        ImmediateNeighbours(neighbours, id);
        for (int i = 0; i < neighbours.size(); i++){
            neighId = neighbours[i];
            if (!visited[neighId]){
                bh2.Add(neighId, neighId);
                dist[neighId] = dist[id] + Double3::Distance(vertices[id].position3D, vertices[neighId].position3D);
                visited[neighId] = true;
                depth[neighId] = depth[id] + 1;
            }
        }
        while (bh2.PopHeap(id, weight)){
            bh1.Add(id, ++dummy);
        }
    }

    delete[] visited;
    delete[] dist;
    delete[] depth;
}

void MeshGraph::PaintInRangeEuclidian(std::vector<Color>& outputColorAllVertex,
                                      int vertexId, const Color& color,
                                      int maxDepth, FilterType type,
                                      double alpha) const
{
    outputColorAllVertex.clear();
    if (vertexId < 0 || vertexId >= TotalVertexCount())
        return ;    

    bool *visited = new bool[TotalVertexCount()];
    outputColorAllVertex.resize(TotalVertexCount());

    for (int i = 0; i < TotalVertexCount(); i++){
        visited[i] = false;
        Color black = {0, 0, 0};
        outputColorAllVertex[i] = black;
    }

    BinaryHeap bh;
    int id, neighId;
    double weight, factor, dist, depth = 0;
    std::vector<int> neighbours;
    bh.Add(vertexId, depth);  
    visited[vertexId] = true;
    while (bh.PopHeap(id, depth)){
        if (depth > maxDepth)
            break;

        dist = Double3::Distance(vertices[vertexId].position3D, vertices[id].position3D);
        if (type == FILTER_BOX){
            if (-alpha <= dist && dist <= alpha)
                factor = 1;
            else
                factor = 0;
        }   
        else if (type == FILTER_GAUSSIAN)
            factor = std::exp(-pow(dist, 2)/ pow(alpha, 2));
        
        outputColorAllVertex[id].r = color.r * factor;
        outputColorAllVertex[id].g = color.g * factor;
        outputColorAllVertex[id].b = color.b * factor;
        ImmediateNeighbours(neighbours, id);
        for (int i = 0; i < neighbours.size(); i++){
            neighId = neighbours[i];
            if (!visited[neighId]){
                bh.Add(neighId, depth+1);
                visited[neighId] = true;
            }
        }
    }
    delete[] visited;
}

void MeshGraph::WriteColorToFile(const std::vector<Color>& colors,
                                 const std::string& fileName)
{
    // IMPLEMENTED
    std::stringstream s;
    for(int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        int r = static_cast<int>(colors[i].r);
        int g = static_cast<int>(colors[i].g);
        int b = static_cast<int>(colors[i].b);

        s << r << ", " << g << ", " << b << "\n";
    }
    std::ofstream f(fileName.c_str());
    f << s.str();
}

void MeshGraph::PrintColorToStdOut(const std::vector<Color>& colors)
{
    // IMPLEMENTED
    for(int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        std::cout << static_cast<int>(colors[i].r) << ", "
                  << static_cast<int>(colors[i].g) << ", "
                  << static_cast<int>(colors[i].b) << "\n";
    }
}