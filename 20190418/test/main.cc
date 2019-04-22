#include"tinyxml2.h"
#include<iostream>
#include<regex>
#include<string>

using std::cout;
using std::endl;
using std::regex;
using std::string;
using namespace tinyxml2;

void test0()
{
    XMLDocument doc;
    doc.LoadFile("coolshell.xml");
    if(doc.Error())
    {
        cout << "XMLDocument load file error" << endl;
        return;
    }
    XMLElement *rssNode = doc.FirstChildElement("rss");
    XMLElement *itemNode = rssNode->FirstChildElement("channel")->FirstChildElement("item");
    if(itemNode != nullptr)
    {
        XMLElement *titleNode = itemNode->FirstChildElement("title");
        XMLElement *linkNode = itemNode->FirstChildElement("link");
        XMLElement *descriptionNode = itemNode->FirstChildElement("description");
        XMLElement *contentNode = itemNode->FirstChildElement("content:encoded");
        
        const char *title = titleNode->GetText();
        const char *link = linkNode->GetText();
        const char *description = descriptionNode->GetText();
        const char *content = contentNode->GetText();

        cout << ">> title: " << title << endl << endl 
             << ">> link: " << link << endl << endl;
        
        //cout << ">> description: " << description << endl << endl
         //    << ">> content: " << content << endl;
        string desc(description);
        string cont(content);
        regex re("<[^>]*>");
        regex re2("<.*?>");
        cout << ">>description: " << std::regex_replace(desc, re2, string("")) << endl << endl;
        cout << ">>content: " << std::regex_replace(cont, re2, string("")) << endl;
    }else{
        cout << "itemNode is nullptr" << endl;
    }
}

int main()
{
    test0();
    return 0;
}
