
#include"tinyxml2.h"

#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include<regex>


using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::regex;
using namespace tinyxml2;


void test0()
{
    XMLDocument doc;
    doc.LoadFile("test.xml");
    if(doc.ErrorID())
    {
        cout << "XMLDocument load file error" << endl;
        return;
    }
    XMLElement * rssNode = doc.FirstChildElement("rss");
    XMLElement * channelNode = rssNode->FirstChildElement("channel");
    if(channelNode != nullptr)
    {
        XMLElement *TitleNode = channelNode->FirstChildElement("title");
        XMLElement *LinkNode = channelNode->FirstChildElement("link");
        XMLElement *DescriptionNode = channelNode->FirstChildElement("description"); 
        //XMLElement *itemNode = channelNode->FirstChildElement("item");
        
        const char *Title = TitleNode->GetText();
        const char *Link = LinkNode->GetText();
        const char *Description = DescriptionNode->GetText();
        cout << ">> title: " << Title << endl
             << ">> link: " << Link << endl
             << ">> description: " << Description << endl;
        XMLElement *itemNode = channelNode->FirstChildElement("item");
        while(itemNode != nullptr)
        {

            XMLElement *titleNode = itemNode->FirstChildElement("title");
            XMLElement *linkNode = itemNode->FirstChildElement("link");
            XMLElement *descriptionNode = itemNode->FirstChildElement("description");
            
            const char *title = titleNode->GetText();
            const char *link = linkNode->GetText();
            const char *description = descriptionNode->GetText();
            cout << ">> title: " << title << endl
                << ">> link: " << link << endl
                << ">> description: " << description << endl;
            itemNode = itemNode->NextSiblingElement("item"); //偏移
        } 
    } else{
        cout << "channelNode is nullptr" << endl;
    }
}

int main()
{
    test0();
    return 0;
}









