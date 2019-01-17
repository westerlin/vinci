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

bool Logica::log()
{
    isLogging = true;
    visibleChildren = children.size();
    for(auto child : children) child.second->log();
    return true;
}

bool Logica::revert()
{
    isLogging = false;
    auto it = children.begin();
    //std::list<std::string> deletelist;

    while (it != children.end()) {
        std::string childname = it->first;
    //    Log(child.c_str());
        children[childname]->revert();
        switch (children[childname]->state) {
            case LOGICA_CREATED:
  //              Log(("deletes:"+child).c_str());
                // HVIS GEMMES TIL SLUT OVERSKRIVER DEN NY NAVNGIVNING
                // OMVENDT PROBLEMATISK AT VI IKKE KAN SLETTE PÅ FLOW

                //deletelist.push_back(childname);
                if (deleteChild(childname))
                    it = children.erase(it);
                else
                    it++;
                // potentially rename branches pushed aside
            break;
            case LOGICA_DELETED:
//                Log("normalising");
                    children[childname]->state = LOGICA_NORMAL;
                    
                    if (strncmp(childname.c_str(),"__" ,2) ==0)
                    {
                        //Log(childname.c_str());
                        children[childname]->name.erase(0,2);
                        //Log(children[childname]->name.c_str());
                     // HER PROBLEMATISK AT VI SÆTTER NYT IND
                     // HVAD GØR DET FOR SLETNING
                        children[children[childname]->name] = children[childname];
                        it =  children.erase(it);
                    } else it++;

                    break;
            default:
                it++;
    break;
        }
    }
    /*
    for (auto name : deletelist){
        deleteChild(name);
    }
    deletelist.clear();
*/
    return true;
}

bool Logica::deleteChild(std::string childname){
        if(children[childname]->state != LOGICA_DELETED) visibleChildren--;
        if (isLogging && children[childname]->state != LOGICA_CREATED) {
            //Log("DELETE LOGGED");
            children[childname]->state = LOGICA_DELETED;
            return false;
        } else {
            //Log(("DELETE REAL"+childname).c_str());
            children[childname]->cleanup();
            //Log(("CLEAN REAL" + childname).c_str());
            delete children[childname];
            //Log(("DELETE POINTER" + childname).c_str());
        
            //children.erase(childname);
         
            //Log(("ERASE FROM LIST" + childname).c_str());
            //outputTree("");
            //Log(("SUPRESSED FROM LIST" + childname).c_str());
            //outputTree("");
        }
        return true;
}

Logica* Logica::createChild(std::string childname){
    visibleChildren++;
    Logica *newChild = new Logica(childname);
    if (isLogging) { 
        newChild->state = LOGICA_CREATED;
        if (children.find(childname)!=children.end() && children.find("__" + childname)==children.end()){
            if(!isLogging) children[childname]->state = LOGICA_DELETED;
            children["__"+childname] = children[childname];
            children["__" + childname]->name = "__" + childname;
            children.erase(childname);
        }
        return newChild;
    } else
        return newChild;
}

bool Logica::hasChild(std::string childname){
    return (children.find(childname) != children.end() && ((!isLogging) || (children[childname]->state!=LOGICA_DELETED) ));
}

void Logica::init(){
    state = LOGICA_NORMAL;
    isLogging = false;
};

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
        children[child.first] = createChild(child.first);
    }
};

Logica* Logica::add(std::string pathname){
    /*
    Logica* newchild = new Logica(childname);
    children[childname] = newchild;
    */
    //Log(("adding:"+pathname).c_str());
    LogicSearchRec childname = logicPathToString(pathname);
    if (childname.prefix == "!")
        cleanup(childname.keyword);
    //Log("after cleanup");
    if (!hasChild(childname.keyword)){
        children[childname.keyword] = createChild(childname.keyword);
        //Log("after create");
    }
    //Log(("after haschild, nextpath;" + childname.nextpath).c_str());

    if (childname.nextpath != "")
        return children[childname.keyword]->add(childname.nextpath);
    //Log("at leaf");

    return children[childname.keyword];
}

Logica* Logica::get(std::string childname){
    if (hasChild(childname))
        return children[childname];
    return NULL;
}
void Logica::cleanup(){
    cleanup("");
}

void Logica::cleanup(std::string exclude){
    auto record = children.begin();
    //Log("---------begin ------");
    //outputTree("");
    //Log("---------begin ------");
    //std::list<std::string> deletelist;
    while (record != children.end())
    {
        std::string childname = record->first;
        Logica *child = record->second;
         
        if (childname != exclude) {
            if (deleteChild(childname)) 
                record = children.erase(record);
            else
                record++;    
            //deletelist.push_back(childname);
        } else
        record++;

    }
    /*
    for (auto name : deletelist){
        deleteChild(name);
    }
    deletelist.clear();
    */
    //Log("---------finish ------");
    //outputTree("");
    //Log("---------finish ------");
}

void Logica::outputTree(std::string path){
    switch (state) {
        case LOGICA_CREATED:
            path = (path + ".\033[0;32m" + name+ "\033[0m");
        break;
        case LOGICA_DELETED:
            path = (path + ".\033[0;31m" + name+ "\033[0m");
        break;
        default:
            path = (path + "." + name);
        break;
    }
    if (children.size() > 0) {
        for (auto child : children){
            children[child.first]->outputTree(path);
        }
    } 
    else {
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
    if (isLogging){
        int tmp = visibleChildren;
        visibleChildren = 0;
        for (auto child : children) if (child.second->state != LOGICA_DELETED) visibleChildren++;
        if (tmp != visibleChildren) {
            Log(("ERROR VisibleChildren count:"+std::to_string(tmp)+", actual count:"+std::to_string(visibleChildren)).c_str());
            throw std::exception();
        }
    } else
        visibleChildren = children.size();

    std::smatch matches;
    //    const char *pattern = "([/.|/!]{1,1})([A-Za-z_]+)";
    std::regex reg(logicaPattern);
    if (std::regex_search(path, matches, reg)) {
        std::string nextpath = matches.suffix().str();
        std::string branch = matches.str(2);
        if (isCapital(branch)) {
            //parms[matches.str(2)] = this;
            //Log(matches.str(2).c_str());
            if ((matches.str(1) != "!") || (visibleChildren == 1))
                for (auto child : children) {
                    if (!isLogging || child.second->state != LOGICA_DELETED){
                        scene[branch] = child.first;
                        std::string tmp = prefix + ":" + branch + "=" + child.first;
                        children[child.first]->parameters(nextpath, tmp, scene, parms);
                    }
                    //children[child.first]->parameters(nextpath, prefix, parms);
                }
        }
        else if (hasChild(branch)) {
            //prefix += ":" + matches.str(2);
            if ((matches.str(1) != "!") || (visibleChildren == 1))
                children[branch]->parameters(nextpath, prefix, scene, parms);
            //return parms;
        }
    } else {
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

bool Logica::has(std::string path, scenario parms) {
    return (get(path,parms)!=NULL);
};

Logica* Logica::get(std::string path, scenario parms)
{
    std::smatch matches;
    std::regex reg(logicaPattern);

    if (isLogging){
        int tmp = visibleChildren;
        visibleChildren = 0;
        for (auto child : children) if (child.second->state != LOGICA_DELETED) visibleChildren++;
        if (tmp != visibleChildren) {
            Log(("ERROR VisibleChildren count:"+std::to_string(tmp)+", actual count:"+std::to_string(visibleChildren)).c_str());
            throw std::exception();
        }
    } else
        visibleChildren = children.size();
    if (std::regex_search(path, matches, reg))
    {
        std::string keyname = matches.str(2);
        if (isCapital(keyname))
            if (parms.find(keyname) != parms.end()) {
                keyname = parms[keyname];
            }
        if (hasChild(keyname)) {
            if (matches.str(1) == "!" && visibleChildren != 1) {
                    // not unique
                    return NULL;
            }
            if (matches.suffix().str() != "") {
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

    if (hasChild(childname.keyword)){
        if (childname.nextpath != "")
            return children[childname.keyword]->pop(childname.nextpath);
        if (deleteChild(childname.keyword))
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
