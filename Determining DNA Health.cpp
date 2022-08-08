#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <fstream>

using namespace std;

string ltrim(const string&);
string rtrim(const string&);
vector<string> split(const string&);

class Node
{
public:
    Node(char c)
    {
        name = c;
    }
    long long getHealth(int first, int last)
    {
        bool found = false;
        long long result = -1;
        if (index.size() == 0 || index[0] > last || index[index.size()-1] < first)
        {
            return result;
        }
        for (int i = 0 ; i < index.size() ; i++)
        {
            if (index[i] >= first && index[i] <= last)
            {
                if (!found)
                {
                    result = 0;
                    found = true;
                }
                result += health[i];
            }
        }
        return result;
    }
    char getName()
    {
        return name;
    }
    Node* getChild(char c)
    {
        for (Node *child : children)
        {
            if (child->getName() == c)
            {
                return child;
            }
        }
        return nullptr;
    }
    void addChild(Node* child)
    {
        this->children.push_back(child);
    }
    void addHealth(int health)
    {
        this->health.push_back(health);
    }
    void addIndex(int i)
    {
        this->index.push_back(i);
    }

private:
    char name;
    vector<Node*> children;
    vector<int> index, health;
};

long long calculateHealth(Node* start, int first, int last, string dna)
{
    long long result = 0;
    int savedPosition = 0;
    long long tempResult = 0;
    bool subGeneFound = false;
    Node* child = start;

    for (int i = 0; i < dna.length(); i++)
    {
        char c = dna[i];
        child = child->getChild(c);
        if (child != nullptr)
        {
            if (subGeneFound == false)
            {
                subGeneFound = true;
                savedPosition = i;
            }

            if (child->getHealth(first, last) != -1)// substr (savedPos -> i) exists as gene (has health value for index if in range)
            {
                tempResult = child->getHealth(first, last);
                result += tempResult;
                //cout << "char[" << i << "] = " << c << endl;
                //cout << "tempResult = " << tempResult << endl;
                //cout << "result = " << result << endl;
            }
            else
            {
                tempResult = 0;
            }

            if (i == dna.length() - 1)
            {
                if (subGeneFound == true)
                {
                    i = savedPosition; // will be incremented+1
                    subGeneFound = false;
                    child = start;
                }
            }
        }
        else
        {
            tempResult = 0;
            if (subGeneFound == true)
            {
                i = savedPosition; // will be incremented+1
                subGeneFound = false;
            }
            child = start;
        }
    }
    //cout << endl;
    return result;
}

int main()
{
    ifstream file;
    file.open("test13.txt");

    string n_temp;
    getline(file, n_temp);

    int n = stoi(ltrim(rtrim(n_temp)));

    string genes_temp_temp;
    getline(file, genes_temp_temp);

    vector<string> genes_temp = split(rtrim(genes_temp_temp));

    vector<string> genes(n);

    for (int i = 0; i < n; i++) {
        string genes_item = genes_temp[i];

        genes[i] = genes_item;
    }

    string health_temp_temp;
    getline(file, health_temp_temp);

    vector<string> health_temp = split(rtrim(health_temp_temp));

    vector<int> health(n);

    for (int i = 0; i < n; i++) {
        int health_item = stoi(health_temp[i]);

        health[i] = health_item;
    }

    Node* root = new Node('\0');

    for (int i = 0; i < n; i++)
    {
        string gene = genes[i];
        Node* tempNode = root;
        for (char c : gene)
        {
            if (tempNode->getChild(c) == nullptr)
            {
                tempNode->addChild(new Node(c));
            }
            tempNode = tempNode->getChild(c);
        }
        tempNode->addIndex(i);
        tempNode->addHealth(health[i]);
    }

    string s_temp;
    getline(file, s_temp);

    long long minHealth, maxHealth, currHealth;
    maxHealth = minHealth = currHealth = 0;
    string maxString = "";
    string minString = "";

    int s = stoi(ltrim(rtrim(s_temp)));

    for (int s_itr = 0; s_itr < s; s_itr++) {
        string first_multiple_input_temp;
        getline(file, first_multiple_input_temp);

        vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

        int first = stoi(first_multiple_input[0]);

        int last = stoi(first_multiple_input[1]);

        string d = first_multiple_input[2];

        currHealth = calculateHealth(root, first, last, d);

        if (s_itr == 0)
        {
            maxHealth = minHealth = currHealth;
            maxString = minString = d;
        }
        else
        {
            if (currHealth > maxHealth) 
            {
                maxHealth = currHealth;
                maxString = d;
            }
            if (currHealth < minHealth)
            {
                minHealth = currHealth;
            }
        }
    }

    cout << minHealth << " " << maxHealth << endl;
    cout << "maxString: \"" << maxString << "\"\n";
    cout << "minString: \"" << minString << "\"\n";

    delete root;
    file.close();

    return 0;
}

string ltrim(const string& str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string& str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string& str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
