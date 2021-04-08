#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

class Graph
{
public:
    Graph( std::vector<std::pair<int, int>> V)
    {
        VertexCount = FindVertexCount(V);
        adjLists.resize(VertexCount);
        num.resize(VertexCount, -1);
        visited.resize(VertexCount, 0);
        father.resize(VertexCount, -1);
        for (auto& edge : V)
            adjLists[edge.first].push_back(edge.second);
        for (auto& e : adjLists)
            std::sort(e.begin(), e.end());
    }

    void DFS(std::vector<std::pair<int, int>>& T,
        std::vector<std::pair<int, int>>& B,
        int& i, int v)
    {
        num[v] = i++;
        for (auto u : adjLists[v])
        {
            visited[v]++;
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

    int GetVertexCount() const { return VertexCount; }
    int IsVisitedVertex(int v) { return visited[v] == adjLists[v].size(); }
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
    std::vector<int> visited;
    std::vector<int> num;
    std::vector<int> father;
};

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
        std::ifstream t("graph1.txt");
        std::string input;
        t.seekg(0, std::ios::end);
        input.reserve(t.tellg());
        t.seekg(0, std::ios::beg);
        input.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

        auto V = ParseInput(input);
        //V = { {0, 1}, {0, 6}, {1, 2}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {3, 4}, {5, 6}, {5, 7}, {5, 8}, {7, 8} };
        Graph g(V);

        std::vector<std::pair<int, int>> T;
        std::vector<std::pair<int, int>> B;
        int i = 0;

        for (int j = 0; j < g.GetVertexCount(); ++j)
            if(!g.IsVisitedVertex(j))
                g.DFS(T, B, i, j);

        bool isIncoherent = true;
        for (int j = 0; j < g.GetVertexCount(); ++j)
            if (g.IsVisitedVertex(j) != 1)
            {
                isIncoherent = false;
                break;
            }

        if(isIncoherent)
            std::cout << "incoherent\n";
        else
            std::cout << "not incoherent\n";

        if (B.size() == 0)
            std::cout << "Is tree\n";
        else
            std::cout << "Is not tree\n";

        OutputToFile("T.txt", T);
        OutputToFile("B.txt", B);
    }

    return 0;
}
