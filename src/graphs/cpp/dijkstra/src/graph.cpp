#include "graph.h"
#include <algorithm>
#include <iterator>
#include <sstream>

Vertex::Vertex()
    : value(0)
    , color(Color::WHITE)
{
}

Vertex::Vertex(int _value)
    : value(_value)
    , color(Color::WHITE)
{
}

Vertex::Vertex(const Vertex& vertex)
    : value(vertex.value)
    , color(vertex.color)
{
}

bool Vertex::operator==(const Vertex& other) const
{
    return value == other.value && color == other.color;
}

bool Vertex::operator!=(const Vertex& other) const
{
    return value != other.value || color != other.color;
}

size_t Vertex::HashFunction::operator()(const Vertex& vertex) const
{
    return std::hash<int>()(vertex.value) ^ (std::hash<int>()(static_cast<int>(vertex.color)) << 1);
}

Edge::Edge(Vertex _source, Vertex _destination, int _distance)
    : source(_source)
    , destination(_destination)
    , distance(_distance)
{
}

bool Edge::operator==(const Edge& other) const
{
    return source == other.source && destination == other.destination && distance == other.distance;
}

bool Edge::operator<(const Edge& other) const
{
    return distance < other.distance;
}

Graph::Graph()
{
}

void Graph::addEdge(Vertex source, Vertex destination, int distance)
{
    if (contains(source))
        adjcDict[source].emplace_back(Edge(source, destination, distance));

    else
        adjcDict[source] = std::vector<Edge>{ Edge(source, destination, distance) };
}

std::vector<Vertex> Graph::vertices() const
{
    std::vector<Vertex> result;

    for (const auto& [key, edges] : adjcDict) {
        result.emplace_back(key);

        for (const auto& edge : edges) {
            auto vertex = edge.destination;
            if (find(result.begin(), result.end(), vertex) == result.end())
                result.emplace_back(vertex);
        }
    }

    return result;
}

std::vector<Edge> Graph::edges(const Vertex vertex) const
{
    if (!contains(vertex))
        throw std::invalid_argument("Graph doesn't contain given vertex.");

    return adjcDict.at(vertex);
}

std::vector<Edge> Graph::edges() const
{
    std::vector<Edge> result;

    for (auto [vertex, _edges] : adjcDict) {
        std::copy(_edges.begin(), _edges.end(), std::back_inserter(result));
    }

    return result;
}

bool Graph::contains(const Vertex vertex) const
{
    return adjcDict.find(vertex) != adjcDict.end();
}

unsigned int Graph::size() const
{
    return adjcDict.size();
}

bool Graph::empty() const
{
    return adjcDict.empty();
}

std::string Graph::toString() const
{
    std::string result;

    for (auto const& [vertex, edges] : adjcDict) {

        std::ostringstream oss;
        for (const auto& edge : edges) {
            oss << std::to_string(edge.destination.value) + ", ";
        }

        auto edgesStr = oss.str();

        if (edgesStr.size() > 1)
            edgesStr.erase(edgesStr.size() - 2);

        result += std::to_string(vertex.value) + " : " + edgesStr + "\n";
    }

    if (result.size() > 1)
        result.erase(result.size() - 1);

    return result;
}
