#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>

using namespace std;


class XMLNode {
public:
    string tag;
    string value;
    vector<XMLNode*> children;

    XMLNode(string t) {
        tag = t;
        value = "";
    }
};
void printTree(XMLNode* root, int level = 0) {

    if (root == NULL)
        return;

    
    for (int i = 0; i < level; i++) {
        cout << "   ";
    }

    cout << "<" << root->tag << ">";

    if (!root->value.empty()) {
        cout << " : " << root->value;
    }

    cout << endl;

    
    for (XMLNode* child : root->children) {
        printTree(child, level + 1);
    }
}


string trim(string str) {

    while (!str.empty() &&
          (str.back() == '\n' || str.back() == '\r' || str.back() == ' '))
    {
        str.pop_back();
    }

    while (!str.empty() && str.front() == ' ') {
        str.erase(str.begin());
    }

    return str;
}


XMLNode* parseXML(string filename) {

    ifstream file(filename);

    if (!file) {
        cout << "Cannot open file!" << endl;
        return NULL;
    }

    string line;
    stack<XMLNode*> st;

    XMLNode* root = NULL;

    while (getline(file, line)) {

        line = trim(line);

        size_t startTag = line.find('<');
        size_t endTag = line.find('>');

        if (startTag != string::npos &&
            endTag != string::npos)
        {

            string tag = line.substr(startTag + 1,
                          endTag - startTag - 1);

            
            if (tag[0] == '/') {

                if (!st.empty())
                    st.pop();
            }

            
            else {

                XMLNode* node = new XMLNode(tag);

             
                size_t valueStart = endTag + 1;
                size_t valueEnd = line.find('<', valueStart);

                if (valueEnd != string::npos &&
                    valueEnd > valueStart)
                {
                    string value =
                        line.substr(valueStart,
                        valueEnd - valueStart);

                    node->value = trim(value);
                }

                
                if (root == NULL) {
                    root = node;
                }

                
                if (!st.empty()) {
                    st.top()->children.push_back(node);
                }

                
                st.push(node);
            }
        }
    }

    file.close();

    return root;
}

void deleteTree(XMLNode* root) {

    if (root == NULL)
        return;

    for (XMLNode* child : root->children) {
        deleteTree(child);
    }

    delete root;
}

int main() {

    /*
    SAMPLE XML FILE : sample.xml

    <college>
    <department>
    <student>Pranav</student>
    </department>
    </college>
    */

    XMLNode* root = parseXML("sample.xml");

    if (root != NULL) {

        cout << "\nXML TREE REPRESENTATION\n" << endl;

        printTree(root);

        deleteTree(root);
    }

    return 0;
}