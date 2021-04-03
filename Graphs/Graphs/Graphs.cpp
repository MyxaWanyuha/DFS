#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

class Graph
{
public:
    Graph(int vertices)
    {
        adjLists.resize(vertices);
        visited.resize(vertices);
    }

    void addEdge(int src, int dest)
    {
        if(!bIsConstructed)
            adjLists[src].push_back(dest);
    }

    void addEdge(std::pair<int, int>& edge)
    {
        if (!bIsConstructed)
            adjLists[edge.first].push_back(edge.second);
    }

    void FinishConstruct()
    {
        for (auto& e : adjLists)
        {
            std::sort(e.begin(), e.end());
        }
        bIsConstructed = true;
    }

    void DFS(std::vector<std::pair<int, int>>& T,
        std::vector<std::pair<int, int>>& B,
        std::vector<int>& num,
        std::vector<int>& father,
        int& i, int v);
private:
    std::vector<std::vector<int>> adjLists;
    std::vector<bool> visited;
    bool bIsConstructed = false;
};

void Graph::DFS(std::vector<std::pair<int, int>>& T,
    std::vector<std::pair<int, int>>& B, 
    std::vector<int>& num, 
    std::vector<int>& father, 
    int& i, int v)
{
    if (!bIsConstructed) return;

    num[v] = i++;
    for (auto u : adjLists[v])
    {
        if (num[u] == -1)
        {
            T.push_back(std::make_pair(u, v));
            father[u] = v;
            DFS(T, B, num, father, i, u);
        }
        else if (num[u] > num[v] && u != father[v])
        {
            B.push_back(std::make_pair(u, v));
        }
    }
}

std::vector<std::pair<int, int>> ParseInput(std::string& input)
{
    std::vector<std::pair<int, int>> res;

    //delete '[' and ']'
    input = input.substr(1);
    input.erase(input.end() - 1);

    for (int i = 0; i < input.size(); i++)
    {
        std::pair<int, int> pairRes;

        std::string digit;
        if (input[i] == '(')
        {
            i++;
            //read first digit
            while (input[i] != ',')
            {
                digit += input[i];
                i++;
            }
            pairRes.first = std::stoi(digit);

            i += 2; //skip ", "
            digit = "";
            //read second digit
            while (input[i] != ')')
            {
                digit += input[i];
                i++;
            }
            pairRes.second = std::stoi(digit);
            res.push_back(pairRes);
        }
    }
    return res;
}

int FindVertexCount(std::vector<std::pair<int, int>>& V)
{
    int res = -1;
    for (auto& e : V)
    {
        auto eMax = std::max(e.first, e.second);
        if (eMax > res)
            res = eMax;
    }

    return res + 1;
}

void OutputToFile(std::string fileName, std::vector<std::pair<int, int>>& v)
{
    std::ofstream of(fileName);
    if (!of.is_open()) return;

    of << '[';
    for (int i = 0; i < v.size(); ++i)
    {
        of << '(' << v[i].first << ", " << v[i].second << ')';
        if(i != v.size() - 1)
            of << ", ";
    }
    of << ']';
}

int main()
{
    {
        std::ifstream t("graph2.txt");
        std::string input;
        t.seekg(0, std::ios::end);
        input.reserve(t.tellg());
        t.seekg(0, std::ios::beg);
        input.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

        auto V = ParseInput(input);
        auto vertexCount = FindVertexCount(V);

        Graph g(vertexCount);
        for (auto& e : V)
        {
            g.addEdge(e);
        }
        g.FinishConstruct();

        std::vector<std::pair<int, int>> T;
        std::vector<std::pair<int, int>> B;
        std::vector<int> num(vertexCount, -1);
        std::vector<int> father(vertexCount, -1);
        int i = 0;

        for (int j = 0; j < vertexCount; ++j)
            g.DFS(T, B, num, father, i, j);

        if (B.size() == 0)
            std::cout << "Is tree\n";
        else
            std::cout << "Is not tree\n";

        OutputToFile("T.txt", T);
        OutputToFile("B.txt", B);
    }

    return 0;
}
