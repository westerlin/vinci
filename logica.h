#include <string>
#include <map>
#include <cassert>
#include <list>
#include "utility.h"

using scenario = std::map<std::string, std::string>;
using scenarioList = std::list<scenario>;

//const char *logicaPattern = "([/.|/!]{1,1})([A-Za-z_]+)";
//const char *logicaPattern = "([\\.|\\!]{1,1})[\\[]?([A-Za-z_]+)[\\]]?";

int LCAInteger(std::string lcainteger);
void pscenarioList(scenarioList scenes);
void pscenario(scenario scene);
void pscene(scenario scene);
void clearscenarioList(scenarioList &scenes);
scenarioList *combineLists(scenarioList listA, scenarioList listB);
void splitvars(scenarioList listA, scenarioList listB, std::list<string> &commonvars, std::list<string> &uniquevars);
scenarioList *intersection(scenarioList listA, scenarioList listB, std::list<string> vars, std::list<string> uniquevars);
bool isSameScenario(scenario elementA, scenario elementB, std::list<string> vars);
bool isExclusionLogic(std::string path);

enum LOGICA_STATES {
    LOGICA_NORMAL=0,
    LOGICA_DELETED=1,
    LOGICA_CREATED=2,
};


class Logica
{


public: 
    std::string name;
    std::map<std::string, Logica *> children;
    bool isLogging;
    LOGICA_STATES state;
    int visibleChildren = 0;
    
    /*
    for (auto const& x : iter) {
        x->first
        x->second
    }
*/

    Logica()
    {
        name = "<root>";
        init();
    };

    Logica(std::string qname)
    {
        name = qname;
        init();

    };

    ~Logica()
    {
        //Log(("I am dying .... " + name).c_str());
        cleanup();
    } 

    void init();
    Logica* add(std::string childname);
    Logica* add(std::string childname, scenario parms);
    Logica* get(std::string childname);
    Logica* get(std::string path, scenario parms);
    bool has(std::string path);
    bool has(std::string path, scenario scene);
    bool extend(std::string path);
    void cleanup(std::string exclude);
    void cleanup();
    bool pop(std::string childname);
    void outputTree(std::string path);
    //    std::map<std::string, Logica* > parameters(std::string path, std::map<std::string, Logica* > parms);
    void parameters(std::string path, std::string prefix, scenario scene, scenarioList &parms);
    void parameters(std::string path, scenarioList &parms);
    void populate(Logica* logica);
    bool log();
    bool revert();
    bool deleteChild(std::string childname);
    Logica* createChild(std::string childname);
    bool hasChild(std::string childname);
};

