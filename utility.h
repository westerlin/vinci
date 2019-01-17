#include <iostream>
#include <string>
#include <fstream>
#include <regex>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::regex_search;
using std::smatch;
using std::string;
 
struct LogicSearchRec
{
    std::string prefix;
    std::string keyword;
    std::string nextpath;
};

struct matchResult
{
    int count;
    const char* match;
    const char* snip;
};

//const char* const logicaPattern = "([\\.|\\!]{1,1})[\\[]?([A-Za-z][A-Za-z_0-9]+)[\\]]?";
const char *const logicaPattern = "([\\.|\\!]{1,1})([\\[]?[A-Za-z][A-Za-z_0-9]+[\\]]?)";

const char *const domainNamePattern = "(\\w+)";

//const char* const rulePattern = "^(\\w+ \"?[^\"]*\"?)[ ]?:[ \\n\\t]*([^-][^>])*[ \\n\\t]*->[ \\n\\t]*[\\{]+([^\\}]*)[\\}]+";
const char *const rulePattern = "^(\\w+ \"?[^\"]*\"?)[ ]?:[ \\n\\t]*([^->]+)[ \\n\\t]*->[ \\n\\t]*[\\{]+([^\\}]*)[\\}]+";
const char *const equalityPattern = "(\\w+)([!=|==|>>|<<|=>|<=]{2})(\\w+)";
const char *const calcPattern = "([a-zA-Z_0-9\\!\\.]+)\\!i([0-9]+)[ ]?([/|\\*|\\+|\\-|%]{1})[ ]?i([0-9]+)";
const char *const calcPatternOpen = "([a-zA-Z_0-9\\!\\.]+\\!\\[[a-zA-Z_0-9]+\\])[ ]?([/|\\*|\\+|\\-|%]{1})[ ]?i([0-9]+)";
const char *const ruleDomainNamePattern = "(\\w+)\\:\\:(\\w+)";
//std::string logicaPattern = "([\\.|\\!]{1,1})[\\[]?([A-Za-z_]+)[\\]]?";

std::vector<std::string> Split(const std::string &str, const std::string &regex);
//std::vector<string> Split(std::string str, std::string delimiter);

std::string trim(std::string text);
void Log(const char *message);
matchResult matcher(const char *value, const char *pattern);
int MyMatch(const char *value, const char *pattern);
int LoadFile(const char *filename);
void getSettings(int argc, char **argv);
LogicSearchRec logicPathToString(std::string path);
std::smatch* findPattern(std::string text, const char *pattern);
bool isCapital(std::string &text);
bool isDomainName(std::string token);
bool isNumber(std::string text);

void clearConsole();
void waitForAnyKey();