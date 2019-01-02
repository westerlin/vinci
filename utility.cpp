#include "utility.h"


/*

https://stackoverflow.com/questions/3578083/what-is-the-best-way-to-use-a-hashmap-in-c

#include <map>
#include <iostream>
#include <cassert>

int main(int argc, char **argv)
{
  std::map<std::string, int> m;
  m["hello"] = 23;
  // check if key is present
  if (m.find("world") != m.end())
    std::cout << "map contains key world!\n";
  // retrieve
  std::cout << m["hello"] << '\n';
  std::map<std::string, int>::iterator i = m.find("hello");
  assert(i != m.end());
  std::cout << "Key: " << i->first << " Value: " << i->second << '\n';
  return 0;
}
*/

bool isDomainName(std::string token){
    std::smatch matches;
    std::regex reg(domainNamePattern);
    return (std::regex_search(token, matches, reg));
}

std::string trim(std::string text){
    while (text[0] == ' ') text.erase(0,1);
    return text;
}

std::vector<std::string> Split(const std::string &str, const std::string &regex)
{ 
    return {std::sregex_token_iterator(str.begin(), str.end(), std::regex(regex), -1), std::sregex_token_iterator()};
}

void Log(const char *message)
{
    std::cout << message << std::endl;
}

void getSettings(int argc, char **argv)
{
    string temp;
    if (argc >= 2)
    {
        temp = argv[1];
        if (temp.compare("-file") == 0)
        {
            string filename = argv[2];
            if (LoadFile(filename.c_str()) != 0)
            {
                Log(("Error loading file: " + filename).c_str());
            }
            Log("You inputted an parameter");
        }
    }
    else
    {
        Log("Not enough parameters: \n\t Usage logica -file <filename> ");
    }
}

LogicSearchRec logicPathToString (std::string path){
    std::smatch matches;
    std::regex reg(logicaPattern);
    if (std::regex_search(path, matches, reg))
    {
        std::string nextpath = matches.suffix().str();
        return {matches.str(1), matches.str(2), nextpath};
    } else {
        return {"",path, ""};
    }
}

std::smatch* findPattern(std::string text, const char *pattern){
    std::smatch* matches = new smatch();
    std::regex reg(pattern);
    if (std::regex_search(text, *matches, reg)) return matches;
    return NULL;
}


bool isCapital(std::string &text){
    //std::smatch matches;
    //std::regex reg("([A-Za_z][A-Za_z_0-9]+)");
    //Log(text.c_str());
    if (text.front() == '[' && text.back() == ']') {
        text.erase(0,1); text.pop_back();
        //Log(text.c_str());
        return true;
    }
    return false;
    //return std::regex_search(text, matches, reg);
    /*
    for (int i = 0; i < text.size(); i++)
    {
        if (!std::isupper(text[i])) return false;
    }
    return true;
    */
}

matchResult matcher(const char *value, const char *pattern){
    std::string tmp = value;
    std::smatch matches;
    std::regex reg(pattern);
    if(std::regex_search(tmp, matches, reg)) {
        return {static_cast<int>(matches.size()), matches.str(1).c_str(), matches.suffix().str().c_str()};
    }
    return {0,NULL,NULL};
}

int MyMatch(const char *value, const char *pattern)
{
    std::string tmp = value;
    std::smatch matches;
    std::regex reg(pattern);
    int result = -1;
    while (std::regex_search(tmp, matches, reg))
    {
        Log(matches.str(1).c_str());
        tmp = matches.suffix().str();
        Log(tmp.c_str());
        if (result < 0)
            result = matches.size();
    }
    return result;
}

int LoadFile(const char *filename)
{
    std::ifstream myReadFile;
    myReadFile.open(filename);
    if (myReadFile)
    {
        /*
        char output[100];
        if (myReadFile.is_open()){
            while (!myReadFile.eof()){
                myReadFile >> output;
                std::cout << output;
            }
            myReadFile.close();
        }
        */
        std::string line;
        //const char[2] *break_off = ";";
         while (std::getline(myReadFile, line, ';'))
        {
            //Log(line.c_str());

        }
        myReadFile.close();
    }
    else
    {
        return -1;
    }
    return 0;
}
