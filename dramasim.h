#include "lcaparser.h"
#include <string>
#include <map>
#include <list>



std::string *fillInParms(std::string text, scenario scene);
bool isLogicaRule(std::string rule);
extern bool logAffordances;
//bool const logAffordances = true;

class LogicRule{
public:
  std::list<std::string* > preconditions;
  std::list<std::string* > implications;
  std::string rulename = "Hilbert";
  std::string ruledescription = "Hilbert";
  scenarioList parms;
  int counter = 0;
  bool addrule(std::string rule);
  void addimplication(std::string implication);
  void addprecondition(std::string precondition);
  bool evaluate(Logica* worldstate);
  bool evaluate(Logica *worldstate, scenario scene);
  void cleanup();
  bool combinations(Logica *worldstate, bool newScenarioList);
  void addpreconditions(std::string precond_text);
  void addimplications(std::string impl_text);
  
  ~LogicRule()
  {
     // Log(("cleaning up list:"+rulename).c_str()); 
      //cleanup();
}

LogicRule(std::string qrulename){
    rulename = qrulename;
}

LogicRule()
{
}

};

enum DramaFlowStates
{
    DRAMA_FLOW_RANDOM = 0,
    DRAMA_FLOW_INTERACT = 1
};

using LogicRuleSet = std::map<std::string, LogicRule>;
using LogicRuleBase = std::map<std::string, LogicRuleSet* >;

struct Affordance {
    LogicRule rule;
    scenario* scene;
};


class DramaSimulator
{
    LogicRuleBase rules;
public:
  DramaFlowStates flowController = DRAMA_FLOW_RANDOM;
  Logica worldstate;
  LCACommands tokenState = LCA_OPEN;
  LCARuleCommands ruleState = LCA_RULE_READ_OPEN;

  int curly = 0;
  bool commented = false;
  std::string ruleCollected;

  std::string ruleDomainName;
  std::string contextDomainName;

  void execute(int steps);
  void addContext(std::string);
  void output(std::string);
  void addrule(std::string ruleset, std::string ruledefinition);
  void printmessage(std::string);
  void loadfile(std::string filename);
  LogicRuleSet *getruleset(std::string ruleset);
  void getKeyword(std::string token);
  bool readContext(std::string token);
  bool readRules(std::string token);
  void setflow();
  bool implement(Affordance affordance);
  LogicRule* getRuleDomain(std::string ruledomainname);

  DramaSimulator()
  {

      /*
    Log("ruleset created!");
    for (auto element : rules){
        Log(element.first.c_str());
    }
    LogicRuleSet* test = getruleset("basic");
     initialize random seed: */
      std::srand(std::time(NULL));
}
~DramaSimulator()
{
    // at some point I need to run through all rulesets
        // destroy each rule in each ruleset
            // clear the ruleset list
                // finally destroy rules
                // and worldstate
  //  Log("goodbye world");
}
};