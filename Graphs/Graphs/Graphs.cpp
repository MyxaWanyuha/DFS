#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

class Graph
{
public:
    void DFS(
        std::vector<std::pair<int, int>>& T,
        std::vector<std::pair<int, int>>& B,
        int& i, 
        int v)
    {
        num[v] = i++;
        for (int u : adjLists[v])
        {
            if (num[u] == -1)
            {
                T.push_back({ v, u });
                father[u] = v;
                DFS(T, B, i, u);
            }
            else if (num[u] > num[v] && u != father[v])
                B.push_back({ v, u });
        }
    }

    Graph(std::vector<std::pair<int, int>> V)
    {
        VertexCount = FindVertexCount(V);
        adjLists.resize(VertexCount);
        num.resize(VertexCount, -1);
        father.resize(VertexCount, -1);
        for (auto& edge : V)
            adjLists[edge.first].push_back(edge.second);
        for (auto& e : adjLists)
            std::sort(e.begin(), e.end());
    }

    int GetVertexCount() const { return VertexCount; }
private:
    int FindVertexCount(std::vector<std::pair<int, int>>& V) const
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
    int VertexCount;
    std::vector<std::vector<int>> adjLists;
    std::vector<int> num;
    std::vector<int> father;
};

std::vector<std::pair<int, int>> ParseInput(std::string& input)
{
    std::vector<std::pair<int, int>> res;
    for (int i = 1; i < input.size(); i++)
    {
        if (input[i] == '(')
        {
            std::string number;
            i++;
            //read first digit
            while (input[i] != ',')
            {
                number += input[i];
                i++;
            }
            std::pair<int, int> pairRes;
            pairRes.first = std::stoi(number);

            i += 2; //skip ", "
            number = "";
            //read second digit
            while (input[i] != ')')
            {
                number += input[i];
                i++;
            }
            pairRes.second = std::stoi(number);
            res.push_back(pairRes);
        }
    }
    return res;
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
        std::string GraphFileName = "graph2.txt";
        std::string OutputTFileName = "T" + GraphFileName.substr(5);
        std::string OutputBFileName = "B" + GraphFileName.substr(5);

        std::ifstream t(GraphFileName);
        
        std::string input;
        t.seekg(0, std::ios::end);
        input.reserve(t.tellg());
        t.seekg(0, std::ios::beg);
        input.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

        auto V = ParseInput(input);
        Graph g(V);

        std::vector<std::pair<int, int>> T;
        std::vector<std::pair<int, int>> B;
        int i = 0;

        for (int j = 0; j < g.GetVertexCount(); ++j)
           g.DFS(T, B, i, j);

        if (B.size() == 0)
            std::cout << "Is tree\n";
        else
            std::cout << "Is not tree\n";

        OutputToFile(OutputTFileName, T);
        OutputToFile(OutputBFileName, B);
    }

    return 0;
}
