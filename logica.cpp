#include "logica.h"

/*
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

bool isSameScenario(scenario elementA, scenario elementB, std::list<string> vars){
    for (auto varname : vars){
        if (elementA[varname] != elementB[varname]) return false;
    }
    return true;
}

scenarioList* intersection(scenarioList listA, scenarioList listB, std::list<string> vars, std::list<string> uniquevars)
{
    scenarioList *output = new scenarioList();
    if (!listA.empty())
    {
        for (auto elementA : listA)
        {
            for (auto elementB : listB)
            {
                if (isSameScenario(elementA, elementB, vars))
                {
                    scenario newElementA = elementA;
                    for (auto xtra : uniquevars)
                    {
                        newElementA[xtra] = elementB[xtra];
                    }
                    //output->push_back(newElementA);
                    output->insert(output->end(), newElementA);
                }
            }
        }
    }
    else
        {
            for (auto elementB : listB)
            {
                //output->push_back(elementB);
                output->insert(output->end(), elementB);
            }
        }
        return output;

    }

    void splitvars(scenarioList listA, scenarioList listB, std::list<string> & commonvars, std::list<string> & uniquevars)
    {
        if (!listB.empty() && !listA.empty())
        {
            auto elementB = listB.begin();
            auto elementA = listA.begin();
            for (auto key : *elementB)
            {
                if (elementA->find(key.first.c_str()) != elementA->end())
                {
                    //commonvars.push_back(key.first);
                    commonvars.insert(commonvars.end(), key.first);
                }
                else
                {
                    //uniquevars.push_back(key.first);
                    uniquevars.insert(uniquevars.end(), key.first);
                }
            }
    }
    else if (listA.empty())
    {

        //uniquevars.splice(uniquevars.end(), listB);
         
        auto elementB = listB.begin();
        for (auto key : *elementB)
        {
            //uniquevars.push_back(key.first);
            uniquevars.insert(uniquevars.end(), key.first);
        }
        
    }
}

scenarioList* combineLists(scenarioList listA, scenarioList listB)
{
    std::list<string> commonvars, uniquevars;
    splitvars(listA, listB, commonvars, uniquevars);
    scenarioList *newList = intersection(listA, listB, commonvars, uniquevars);
    //multiplyList(newListA,newListB,commonvars,uniquevars);
    return newList;
}

void clearscenarioList(scenarioList &scenes){
    for (auto scene : scenes){
        scene.clear();
        
    };
    scenes.clear();
}

void pscenario(scenario scene)
{
std:
    string tmp = "{ ";
    for (auto pair : scene)
    {
        tmp += pair.first + "=" + scene[pair.first] + " ";
    };
    tmp += "}";
    Log(tmp.c_str());
}

void pscene(scenario scene)
{
std:
    string tmp = "{ ";
    for (auto pair : scene)
    {
        tmp += pair.first + "=" + scene[pair.first] + " ";
    };
    tmp += "}";
    std::cout << tmp.c_str();
}

void pscenarioList(scenarioList scenes)
{
    for (auto qscenario : scenes) {
        pscenario(qscenario);
    };
}

void Logica::populate(Logica* logica){
    for (auto child: logica->children){ 
        //Log(child.first.c_str());
        children[child.first] = new Logica(child.first);
    }
};

Logica* Logica::add(std::string pathname){
    /*
    Logica* newchild = new Logica(childname);
    children[childname] = newchild;
    */
    LogicSearchRec childname = logicPathToString(pathname);
    if (childname.prefix == "!")
        cleanup(childname.keyword);

    if (children.find(childname.keyword) == children.end())
        children[childname.keyword] = new Logica(childname.keyword);

    if (childname.nextpath != "")
        return children[childname.keyword]->add(childname.nextpath);

    return children[childname.keyword];
}

Logica* Logica::get(std::string childname){
    if (children.find(childname) != children.end())
        return children[childname];
    return NULL;
}
void Logica::cleanup(){
    cleanup("");
}

void Logica::cleanup(std::string exclude){
    Logica* tmp = NULL;
        //auto record = children.begin();
        for (auto record : children)
    {
        Logica *child = record.second;
        if (record.first != exclude)
        {
            delete child;
            //children.erase(children.begin());
        } else {
            tmp = child;
        }
    }
    children.clear();
    if (tmp != NULL) children[exclude] = tmp;
}

void Logica::outputTree(std::string path){
    path = (path + "." + name);
    if (children.size() > 0) {
        for (auto child : children){
            children[child.first]->outputTree(path);
        }
    } 
    else{
        Log(path.c_str());
    }
}
/*
bool Logica::has(std::string path, scenario scene)
{
    if ((path.find(".") != 0) && (path.find("!") != 0))
        return has_("." + path,scene);
    else
        return has_(path,scene);
}

bool Logica::has(std::string path){
    if ((path.find(".") != 0) && (path.find("!") != 0))
        return has_("." + path);
    else 
        return has_(path); 
}
*/

int LCAInteger(std::string lcainteger){
    std::smatch matches;
    std::regex reg("i([0-9]+)");
    if( std::regex_search(lcainteger, matches, reg)){
        return std::stoi(matches.str(1));
    } return -1;
}

bool isExclusionLogic(std::string path){
    std::smatch matches; 
    std::regex reg(logicaPattern);
    return std::regex_search(path, matches, reg);
}

void Logica::parameters(std::string path, std::string prefix, scenario scene, scenarioList &parms)
{
    //prefix += ":" + name;
    std::smatch matches;
//    const char *pattern = "([/.|/!]{1,1})([A-Za-z_]+)";
    std::regex reg(logicaPattern);
    if (std::regex_search(path, matches, reg))
    {

        std::string nextpath = matches.suffix().str();
        std::string branch = matches.str(2);
        if (isCapital(branch))
        {
            //parms[matches.str(2)] = this;
            //Log(matches.str(2).c_str());
            if ((matches.str(1) != "!") || (children.size() == 1))
                for (auto child : children)
                {
                    scene[branch] = child.first;
                    std::string tmp = prefix + ":" + branch + "=" + child.first;
                    children[child.first]->parameters(nextpath, tmp, scene, parms);
                    //children[child.first]->parameters(nextpath, prefix, parms);
                }
        }
        else if (children.find(branch) != children.end())
        {
            //prefix += ":" + matches.str(2);
            if ((matches.str(1) != "!") || (children.size() == 1))
                children[branch]->parameters(nextpath, prefix, scene, parms);
            //return parms;
        }
    }
    else
    {
        /*
                Log ("--------------------");
                for (auto pair : scene){
                    Log(("\t" + pair.first + "=" + scene[pair.first]).c_str());
                }
                Log("--------------------");
                */
        //Log(("\t" + prefix).c_str());
        if (!scene.empty()){
            scenario newscene;
            newscene = scene;
            parms.push_back(newscene);
            //parms.insert(parms.end(), newscene);
        }
    }
    //return parms;
}

void Logica::parameters(std::string path, scenarioList &parms){
    scenario scene;
    parameters(path,"",scene, parms);
}

bool Logica::has(std::string path){
    scenario parms;
    return has(path,parms);
}

bool Logica::has(std::string path, scenario parms)
{
    //std::list<std::string> listOfChildren;

    std::smatch matches;
    //    const char *pattern2 = "([/.|/!])";
    //    const char* pattern = "([A-Za-z_]+)[/.|/!]{0,1}";
    //const char *pattern = "([/.|/!]{1,1})([A-Za-z_]+)";
    std::regex reg(logicaPattern);

    if (std::regex_search(path, matches, reg))
    {
        std::string keyname = matches.str(2);
        if (isCapital(keyname))
            if (parms.find(keyname) != parms.end())
            {
                keyname = parms[keyname];
            }
        /*
        Log(("My name is:"+name).c_str());
        for (auto x : children)
        {
            Log(("\t"+x.first).c_str());
        }

        Log(("prefix:["+matches.prefix().str()+"]").c_str());
        Log(("match:[" + matches.str(1)+"]").c_str());
        Log(("match:[" + matches.str(2) + "]").c_str());
        Log(("suffix:[" + matches.suffix().str() + "]").c_str());
        Log("found regex");
        */
        if (children.find(keyname) != children.end())
        {
            //Log("found children");
            if ((matches.str(1) == "!") && (children.size() != 1))
            {
                // not unique
                return false;
            }
            if (matches.suffix().str() != "")
            {
                std::string nextpath = matches.suffix().str();
                int result = children[keyname]->has(nextpath, parms);
                //Log(("returns:"+std::to_string(result)).c_str());
                return result;
            }
            //Log("here tooo...");
            return true;
        }
    }
    return false;
};

Logica* Logica::get(std::string path, scenario parms)
{
    std::smatch matches;
    std::regex reg(logicaPattern);

    if (std::regex_search(path, matches, reg))
    {
        std::string keyname = matches.str(2);
        if (isCapital(keyname))
            if (parms.find(keyname) != parms.end())
            {
                keyname = parms[keyname];
            }
        if (children.find(keyname) != children.end())
        {
            if ((matches.str(1) == "!") && (children.size() != 1))
            {
                return NULL;
            }
            if (matches.suffix().str() != "")
            {
                std::string nextpath = matches.suffix().str();
                return children[keyname]->get(nextpath, parms);
            }
            return this;
        }
    }
    return NULL;
};

bool Logica::pop(std::string pathname){

    LogicSearchRec childname = logicPathToString(pathname);

    if (children.find(childname.keyword) != children.end()){
        if (childname.nextpath != "")
            return children[childname.keyword]->pop(childname.nextpath);
        children[childname.keyword]->cleanup();
        delete children[childname.keyword];
        children.erase(childname.keyword);
        //Log((" (++) DelETEING : "+childname.keyword).c_str());
        //outputTree("");
        return true;
    }
    return false;
};

bool Logica::extend(std::string path){
    return 0;
};
