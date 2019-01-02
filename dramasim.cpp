#include "dramasim.h"

bool logAffordances = true;

void LogicRule::cleanup(){
    for (auto x : preconditions){
        delete x;
        Log("removed precondition");
    }
    for (auto x : implications)
    {
        delete x;
        Log("removed implication");
    }
    Log("clearing list");
    preconditions.clear();
};


std::string *fillInParmsEXT(std::string text, scenario scene)
{
    std::string *output = new std::string(text);
    for (auto key : scene)
    {
        //int num = LCAInteger(key.second);
        //if (num >=0) key.second.erase(0,1);
        *output = std::regex_replace(*output, std::regex("\\["+key.first+"\\]"), key.second);
    }
    return output;
}

void LogicRule::addpreconditions(std::string precond_text){
    std::vector<std::string> precs = Split(precond_text,"\\*");
    for(auto prec : precs){
        prec.erase(std::remove(std::begin(prec), std::end(prec), ' '), std::end(prec));
        addprecondition(prec);
        //Log(("   (+) Added precondition:" + prec).c_str());
    };
}
void LogicRule::addimplications(std::string impl_text)
{
    std::vector<std::string> precs = Split(impl_text, ";");
    for (auto prec : precs)
    {
        addimplication(prec);
        //Log(("   (+) Added implication:" + prec).c_str());
    };
}

bool isLogicaRule(std::string rule){
    rule.erase(std::remove(rule.begin(), rule.end(), '\n'), rule.end());
    //std::string pattern = "^(\\w+ [\"[^\"]+\"]?)[ ]?:([^->]+)[ \\n]*->[ \\n]*[\\{]+([^\\}]*)[\\}]+";
    //std::string pattern = "^(\\w+ \"?[^\"]*\"?)[ ]?:[ \\n\\t]*([^->]+)[ \\n\\t]*->[ \\n\\t]*[\\{]+([^\\}]*)[\\}]+";
    std::smatch args;
    std::regex reg(rulePattern);
    return std::regex_search(rule, args, reg);
}

bool LogicRule::addrule(std::string rule){
  //  const char *pattern = "(^([A-Za-z0-9_]+)\\s?:\\s?(.+)\\s?->\\s?(.+);)";
    //std::string pattern  = "^(\\w+) : (.+) -> ([^;]*);";
  rule.erase(std::remove(rule.begin(), rule.end(), '\n'), rule.end());
  //std::string pattern = "^(\\w+ [\"[^\"]+\"]?)[ ]?:([^->]+)[ \\n]*->[ \\n]*[\\{]+([^\\}]*)[\\}]+";
  //std::string pattern = "^(\\w+ \"?[^\"]*\"?)[ ]?:[ \\n\\t]*([^->]+)[ \\n\\t]*->[ \\n\\t]*[\\{]+([^\\}]*)[\\}]+";
  std::smatch args;
  std::regex reg(rulePattern);
  if (std::regex_search(rule, args, reg))
  {
      /*
      Log((" (+) Find on pattern:" + args.str()).c_str());
      for (int i = 0; i < args.size(); i++)
          Log(("   (+) element:" + args.str(i)).c_str());
      */
      std::string ruleprefix = args.str(1);
      std::smatch prefix_args;
      std::string pattern = "^(\\w+) \"?([^\"]*)\"?";
      std::regex reg(pattern);
      if (std::regex_search(ruleprefix, prefix_args, reg))
      {
          //Log(prefix_args.str().c_str());
          //Log(prefix_args.str(1).c_str());
          rulename = prefix_args.str(1);
          rulename = std::regex_replace(rulename.c_str(), std::regex(" "), "");
          ruledescription = prefix_args.str(2);
      } else {
          rulename = "Not identified";
      }

      addpreconditions(args.str(2));
      addimplications(args.str(3));
      return true;
    } 
    else {
        Log("Did not find a pattern");
        return false;
    };
};

void LogicRule::addimplication(std::string implication){
    implication = trim(implication);
    if (implication != "")
    {
        if (logAffordances) Log(("\tAdding implication:"+implication).c_str());
        std::string* input = new std::string(implication);
        implications.insert(implications.end(), input);
    }
};

void LogicRule::addprecondition(std::string precondition){
    precondition = trim(precondition);
    if (precondition != "")
    {
        if (logAffordances) Log(("\tAdding precondition:"+precondition).c_str());
        std::string* input = new std::string(precondition);
        preconditions.insert(preconditions.end(),input);
    }
};

bool LogicRule::combinations(Logica* worldstate, bool newScenarioList){
    scenarioList tmp;
    //scenarioList output;
    if (newScenarioList) clearscenarioList(parms); 
    //Log(("Evaluating combinations for "+rulename).c_str());
    for (auto precondition : preconditions)
    {
        //Log(precondition->c_str());
        //Log(("Evaluating combinations for " + *precondition).c_str());
        if (isExclusionLogic(*precondition) && (*precondition)[0] != '~')
        {
            if ((*precondition)[0] == '~'){
                Log(precondition->c_str());
                throw std::exception();
                }
        worldstate->parameters(*precondition, tmp);
        if (!tmp.empty())
            parms = *combineLists(parms, tmp);
        //Log(("After Combining " + *precondition).c_str());
        /*
        for (auto element : output){
            parms.insert(parms.end(),element);
        }
        */
        //pscenarioList(output);
        clearscenarioList(tmp);
        //clearscenarioList(output);
        }
    }
    //Log(("\t RULING "+rulename).c_str());
    //pscenarioList(parms);
    return parms.size()>0;
}

bool evaluateEqualities(std::string op, std::string arg1, std::string arg2) {
    //Log((" \t\t arg1=[" + arg1 + "] and arg2=[" + arg2 + "]").c_str());
    int num1 = LCAInteger(arg1);
    int num2 = LCAInteger(arg2);
    if (num1 == -1 && num2 == -1) {
        if (op == "!=") return (arg1 != arg2);
        if (op == ">>") return (arg1 > arg2);
        if (op == "<<") return (arg1 < arg2);
        if (op == "=>") return (arg1 >= arg2);
        if (op == "<=") return (arg1 <= arg2);
        if (op == "==") return (arg1 == arg2);
    }
    if (num1 >= 0 && num2 >= 0) {
        if (op == "!=") return (num1 != num2);
        if (op == ">>") return (num1 > num2);
        if (op == "<<") return (num1 < num2);
        if (op == "=>") return (num1 >= num2);
        if (op == "<=") return (num1 <= num2);
        if (op == "==") return (num1 == num2);
    }
    Log("   (ERROR) You are comparing and LCA integer to a string:");
    Log((" \t\t arg1=["+arg1+"] and arg2=["+arg2+"]").c_str());
    throw new std::exception();
    return true;
}

bool LogicRule::evaluate(Logica *worldstate, scenario scene)
{
    counter =0;
    if (logAffordances) Log(rulename.c_str());
    if (logAffordances) pscenario(scene);
    for (auto precondition : preconditions)
    {
        counter++; 
        std::string *specific = fillInParmsEXT(*precondition, scene);
//        Log(("\tEvaluating:"+ *specific).c_str());
        std::smatch prefix_args;
        std::regex reg(equalityPattern); 
        if (std::regex_search(*specific, prefix_args, reg)){
            if (!evaluateEqualities(prefix_args.str(2), prefix_args.str(1), prefix_args.str(3))){
                delete specific;
                if (logAffordances) Log(("\033[0;31m\tEvaluating:" + *precondition + "\033[0m").c_str());
                return false;
            }
        }
        else if ((*specific)[0] == '~')
        {
            specific->erase(0,1);
            if (worldstate->has(*specific))
            {
                //Log(("Do not have :" + *specific).c_str());
                delete specific;
                if (logAffordances) Log(("\033[0;31m\tEvaluating:" + *precondition + "\033[0m").c_str());
                return false;
            };
        }  else
            if (!worldstate->has(*specific))
            {
                //Log(("Do not have :" + *specific).c_str());
                delete specific;
                if (logAffordances) Log(("\033[0;31m\tEvaluating:" + *precondition + "\033[0m").c_str());
                return false;
            };
        delete specific;
        if (logAffordances) Log(("\033[0;32m\tEvaluating:" + *precondition + "\033[0m").c_str());
    }
    return true;
};

bool LogicRule::evaluate(Logica* worldstate){
    scenario scene;
    return evaluate(worldstate,scene);
};

LogicRule* DramaSimulator::getRuleDomain(std::string ruledomainname){
    std::smatch domainname = smatch();
    std::regex reg(ruleDomainNamePattern);
    
    if (std::regex_search(ruledomainname, domainname, reg)) {
        LogicRuleSet* ruleset = getruleset(domainname.str(1));
        if (ruleset != NULL)
            if (ruleset->find(domainname.str(2)) != ruleset->end()) {
                LogicRule *rule = &(*ruleset)[domainname.str(2)];
                return rule;
            }
    }
    return NULL;
}

bool DramaSimulator::implement(Affordance affordance){
    const char *pattern = "([\\w]+)[ ]+(.+)"; 
    LogicRule rule = affordance.rule;
    scenario scene;
    if (affordance.scene != NULL)
        scene = *affordance.scene;
    else
        scene = scenario();

    //Log(("\033[0;36m (GAME - " + rule.rulename + ") " + *fillInParmsEXT(rule.ruledescription, scene) + "\033[0m").c_str());
    std::cout << "\033[1;36m  " << rule.rulename.c_str() << "\033[0;36m ";
    if (affordance.scene != NULL) pscene(*affordance.scene);
    std::cout << "\033[0m" << std::endl << std::endl;
    Log(("  "+ *fillInParmsEXT(rule.ruledescription, scene)+"\n").c_str());
    for (auto cmd : rule.implications)
    {
        //Log(cmd->c_str());
        //std::smatch* args = findPattern(*cmd,pattern);
        std::smatch args = smatch();
        std::regex reg(pattern);
        if (std::regex_search(*cmd, args, reg))
        {
            //if (args) { 
            if (args.str(1) == "add")
            {
                std::string bodytext = *fillInParmsEXT(args.str(2), scene);
                //bodytext = trim(bodytext);
                //Log(bodytext.c_str());
                std::vector<std::string> branches = Split(bodytext, "[ ]");
                if (branches.size() == 2)
                {
                    worldstate.add(branches[0])->populate(worldstate.add(branches[1]));
                    /*
                    Logica* source = worldstate->add(branches[1]);
                    Logica* destination = worldstate->add(branches[0]); //->populate(*worldstate->add(branches[1]));
                    for (auto child : source->children){
                        destination->children[child.first] = new Logica(child.first);
                    }
                    */
                } else {
                    Log("   (ERROR) Not to branches for adding:");
                    Log((" \t\t brance declarations=[" + bodytext + "]").c_str());
                    throw new std::exception();
                }
                //Log("adding OK");                //worldstate->add(bodytext);
                //worldstate->outputTree("");
            }
            else if (args.str(1) == "call")
            {
                std::string bodytext = *fillInParmsEXT(args.str(2), scene);
                bodytext = std::regex_replace(bodytext.c_str(), std::regex(" "), "");
                LogicRule *newrule = getRuleDomain(bodytext);
                if (newrule != NULL){
                    /* Her foretages en helt ny evaluering
                        Det er ikke smart - for der etableres de instances
                        Som der tidligere er valgt fra 
                   // newrule->parms = rule.parms;*/
                   clearscenarioList(newrule->parms);
                   newrule->parms.insert(newrule->parms.end(), scene);
                   newrule->combinations(&worldstate, false);
                   //pscenarioList(newrule->parms);

                    for(auto specscene : newrule->parms){
                        if (newrule->evaluate(&worldstate,specscene)){
                            Affordance newaffordance;
                            newaffordance.scene = &specscene;
                            newaffordance.rule = *newrule;
                            Log("");
                            implement(newaffordance);
                        }
                    }
                    
                } else {
                    Log("   (ERROR) Could not find called domain rule:");
                    Log((" \t\t DomainRuleName=[" + bodytext + "]").c_str());
                    throw new std::exception();
                }
            }  else if (args.str(1) == "set")
            {
                std::string bodytext = *fillInParmsEXT(args.str(2), scene);
                bodytext = std::regex_replace(bodytext.c_str(), std::regex(" "), "");
                worldstate.add(bodytext);
            } else if (args.str(1) == "pop")
            {
                std::string bodytext = *fillInParmsEXT(args.str(2), scene);
                bodytext = std::regex_replace(bodytext.c_str(), std::regex(" "), "");
                worldstate.pop(bodytext);
                /*
                    if (worldstate->pop(args->str(2)))
                    Log((" (**) Deleted branch:" + args->str(2)).c_str());
                else
                    Log((" (**) Branch" + args->str(2) + " was not delete.....").c_str());
                */
            }
            else if(args.str(1) == "flow")
            {
                std::string bodytext = *fillInParmsEXT(args.str(2), scene);
                bodytext = std::regex_replace(bodytext.c_str(), std::regex(" "), "");
                if (bodytext == "switch"){
                    if (worldstate.has(".flow!interact")){
                        worldstate.add(".flow!random");
                    }else{
                        worldstate.add(".flow!interact");}}
                else if (bodytext == "interact"){
                    worldstate.add(".flow!interact");
                }
                else if (bodytext == "random"){
                    worldstate.add(".flow!random");
                } else {
                    Log("   (ERROR) Wrong switch flow expression:");
                    Log((" \t\t expression=[" + bodytext + "]").c_str());
                    throw new std::exception();
                }
            }
            else if (args.str(1) == "print")
            {
                std::string bodytext = *fillInParmsEXT(args.str(2), scene);
                //std::string bodytext = args.str(2);
                bodytext = std::regex_replace(bodytext.c_str(), std::regex("[\"|\\)|\\(]"), "");
                bodytext = std::regex_replace(bodytext.c_str(), std::regex("[_]"), " ");
                //bodytext.erase(std::remove(std::begin(bodytext), std::end(bodytext), '('), std::end(bodytext));
                //bodytext.erase(std::remove(std::begin(bodytext), std::end(bodytext), ')'), std::end(bodytext));
                //bodytext.erase(std::remove(std::begin(bodytext), std::end(bodytext), '\"'), std::end(bodytext));
                //Log(("\n\t\t \033[1;33m" + bodytext + "\033[0m \n ").c_str());
                Log(("\t \033[1;37m" + bodytext + "\033[0m").c_str());
            } else if (args.str(1) == "calc")
                {
                    std::string bodytext = *fillInParmsEXT(args.str(2), scene);
                    bodytext = std::regex_replace(bodytext.c_str(), std::regex("[\"|\\)|\\(]"), "");
                    std::smatch calcs = smatch();
                    std::regex reg(calcPatternOpen);
                    if (std::regex_search(bodytext,calcs,reg)){
                        scenarioList valueVar;
                        //Log(calcs.str(1).c_str());
                        
                        worldstate.parameters(calcs.str(1),valueVar);
                        //pscenarioList(valueVar);
                        std::string tmp = *fillInParmsEXT(bodytext, valueVar.front());
                        bodytext = tmp;
                    }
                    calcs = smatch();
                    reg = std::regex(calcPattern);
                    if (std::regex_search(bodytext,calcs,reg)){
                        /*
                        Log(calcs.str(1).c_str());
                        Log(calcs.str(2).c_str());
                        Log(calcs.str(3).c_str());
                        Log(calcs.str(4).c_str());
                        */
                        int num1 = std::stoi(calcs.str(2));
                        int num2=std::stoi(calcs.str(4));
                        if (calcs.str(3) == "+") num1 += num2;
                        if (calcs.str(3) == "-") num1 -= num2;
                        if (calcs.str(3) == "%") num1 %= num2;
                        if (calcs.str(3) == "*") num1 *= num2;
                        if (calcs.str(3) == "/") num1 /= num2;
                        if (num1 < 0) num1 = 0;
                        std::string newPath = (calcs.str(1) + "!i" + std::to_string(num1));
                        worldstate.add(newPath);
                        //Log(newPath.c_str());
                    }
                    else
                    {
                        Log("   (ERROR) Could not recognize expression calculus:");
                        Log((" \t\t expression=[" + bodytext + "]").c_str());
                        throw new std::exception();
                    }
                }
            else {
                Log("  (WARNING) Did not recognize expression:");
                Log((" \t(--) :: " + args.str()).c_str());
                Log((" \t(--) :: " + args.str(1)).c_str());
                Log((" \t(--) :: " + args.str(2)).c_str());
                Log("  ---------------------- press enter to continue ----------------------- ");
                std::cin.get();
            };
            //delete args;
        }
        else
        {
            Log((" (ERROR) Unknown command (in rule" + rule.rulename + "):" + *cmd).c_str());
            Log( ("\t\t ["+ *cmd +"]").c_str());
            throw new std::exception();
        }
    }
    return false;
};

void DramaSimulator::setflow() {
    if (worldstate.has(".flow!interact")) flowController = DRAMA_FLOW_INTERACT;
    if (worldstate.has(".flow!random")) flowController = DRAMA_FLOW_RANDOM;
}

void DramaSimulator::execute(int steps){
    // run through all ruleset
    // create distribution list
    // choose one random do implementation
    LogicRuleSet* ruleset = getruleset("base");
    std::vector<Affordance> affordances; 
    std:string header;
    for (int i=0;i < steps;i++){
        header = " \033[0;32m --------------- Round " + std::to_string(i) + " --------------- \033[0m";
        //affordances.clear();
        int rulecount = 0;
        int scenariocount = 0;
        int preconditions = 0;
        int actualPrecs = 0;
        for (auto rulepair : *ruleset)
        {
            //Log((" (+) Evaluating rule:"+rule.rulename).c_str());
            LogicRule rule = (*ruleset)[rulepair.first];
            rulecount++;
            if (rule.evaluate(&worldstate)){
                Affordance newaffordance;
                newaffordance.rule = rule;
                newaffordance.scene = NULL;
                affordances.insert(affordances.end(),newaffordance);
                preconditions += rule.preconditions.size();
                // Log(("   (-) Base rule included in affordances counting " + std::to_string(affordances.size()) + " affordances.").c_str());
            } else {
                if (rule.combinations(&worldstate,true))
                {
                    auto it = rule.parms.begin();

                    while (it != rule.parms.end()){
                    //for(auto specscene : rule.parms){
                        //Log(scene["ACTORS"].c_str());
                        scenario specscene = *it;
                        scenariocount++;
                        preconditions += rule.preconditions.size();
                        if (rule.evaluate(&worldstate, specscene))
                        {
                            Affordance newaffordance;
                            newaffordance.rule = rule;
                            newaffordance.scene = new scenario(specscene);
                            //pscenario(*newaffordance.scene);
                            affordances.insert(affordances.end(), newaffordance);
                            it++;
                        } else {
                            it = rule.parms.erase(it);
                        }
                        actualPrecs += rule.counter;
                    }
                };
                // Log("   (-) Base rule was not true in worldstate.");
            }

        }

        if (affordances.size()>0){
            Log((header + "\033[0;32m  Produced " + std::to_string(affordances.size()) +
                          " affordances from "+std::to_string(rulecount) + 
                          " rules, "+std::to_string(scenariocount)+" scenarios and evaluated "+
                          std::to_string(actualPrecs) + " of " +
                          std::to_string(preconditions)+ " possible preconditions. \033[0m").c_str());
            int iSecret = 9999;
            setflow();
            switch (flowController) {
                case DRAMA_FLOW_RANDOM:  
                    iSecret = rand() % affordances.size();
                break;
                default:
                    int optionCode = 1;
                    Log("\033[0;36m  Choose from: \033[0m");
                    for (auto affordance : affordances)
                    {
                        if (affordance.scene != NULL){
                            std::cout << ("\t " + std::to_string(optionCode) + ") " + *fillInParmsEXT(affordance.rule.ruledescription, *affordance.scene)).c_str();
                            //pscene(*affordance.scene);
                            std::cout << (("(" + affordance.rule.rulename + ")").c_str()) << std::endl;
                         } else
                             Log(("/t " + std::to_string(optionCode) + ") " + affordance.rule.ruledescription + "(" + affordance.rule.rulename + ")").c_str());
                        optionCode++;                            
                    }
                    while (iSecret > affordances.size()){
                        std::cout << "\033[0;36m  >\033[0m ";
                        std::cin >> iSecret;}
                    iSecret--;
                    break;
            }
            
            //Log(("hello there:" + std::to_string(iSecret)).c_str());
            //Log(("hello there:" + std::to_string(affordances.size())).c_str());
            //Log(affordances[iSecret].rule.rulename.c_str());
            //pscenario(*affordances[iSecret].scene);
            implement(affordances[iSecret]);
        } else {
            affordances.clear();
            Log((header + "\033[0;32m No more affordances - Games Ends ... \033[0m\n").c_str());
            break;
        }
        for (auto affordance : affordances){
            if (affordance.scene != NULL) delete affordance.scene; 
        }
        affordances.clear();
        //worldstate.outputTree("");
        Log("");
        //affordances[0].rule.implement(&worldstate, scene);
        //affordances[1].rule.implement(&worldstate, scene);
    }
    Log("All ok!!");

};

void DramaSimulator::addContext(std::string context){
    worldstate.add(context);
};

void DramaSimulator::output(std::string){

};

void DramaSimulator::addrule(std::string rulesetname, std::string ruledefinition){
    // split preconditions and implications
    // split preconditions and add to rule class
    // split implications and add to rule class
    // add rule class to list of specific ruleset
    LogicRule newRule;
    if (newRule.addrule(ruledefinition))
    {
        LogicRuleSet *ruleset = getruleset(rulesetname);
        //ruleset->insert(ruleset->end(), newRule);
        (*ruleset)[newRule.rulename]=newRule;
        Log(("\033[1;33m   (-) Rule was added: " + newRule.rulename + "\033[0m").c_str());
    } else {
        Log("\031[1;33m   (-) The rule could not be included. Wrong format:");
        Log(("       (-)  " + ruledefinition + "\033[0m").c_str());
        throw new std::exception();
    }
};

void DramaSimulator::printmessage(std::string){

};

bool DramaSimulator::readContext(std::string token){
    if (token != ",") {
        if (isExclusionLogic(token)){
            if (contextDomainName == "") contextDomainName = "base";
            worldstate.add(token);
            return true;
        } else {
            Log ("  (ERROR) Unrecognized Exclusion Logic while reading context:");
            Log ((" \t\t "+token).c_str());
            return false;
        }
    }
    return true;
}

bool DramaSimulator::readRules(std::string token){
    if(ruleState==LCA_RULE_READ_COLLECT) {
        ruleCollected += " "+token;
    }
    if(ruleState==LCA_RULE_READ_OPEN && curly == 1) {
        ruleState=LCA_RULE_READ_COLLECT;
        ruleCollected = token;
    }
    if (ruleState==LCA_RULE_READ_COLLECT && token == "}" && curly == 1){
        ruleState=LCA_RULE_READ_OPEN;
        if (isLogicaRule(ruleCollected)){
            if (ruleDomainName == "") ruleDomainName = "base";
            addrule(ruleDomainName,ruleCollected);
            return true;
        } else {
            Log("  (ERROR) Unrecognized Logical Rule:");
            Log((" \t\t " + ruleCollected).c_str());
            Log((" \t\t problems with:[" + token + "]").c_str());
            return false;
        }
    }
    return true;
}

void DramaSimulator::getKeyword(std::string token){
    //std::cout << std::to_string(ruleState).c_str() << "::";
    //std::cout << std::to_string(curly).c_str() << "::";
    //Log(token.c_str());
    if (token == "//") {
        commented = !commented;
    } else
    if (!commented){
        if (token == "{") curly++;
        if (token == "}") curly--;
        if (curly == 0 && (tokenState == LCA_RULE_READ || tokenState == LCA_CONTEXT_READ)) tokenState = LCA_OPEN;
        if (tokenState == LCA_RULE_READ) {
            readRules(token);
        }
        
        if (tokenState == LCA_CONTEXT_READ) {
            readContext(token); 
        } 
        if (isDomainName(token) && tokenState == LCA_RULE_BEGIN) ruleDomainName = trim(token);
        if (isDomainName(token) && tokenState == LCA_CONTEXT_BEGIN) contextDomainName = trim(token);
        if (token == "context" && tokenState == LCA_OPEN) {
            tokenState = LCA_CONTEXT_BEGIN;
            contextDomainName = "";
        }
        if (token == "stage" && tokenState==LCA_OPEN) {
             tokenState = LCA_RULE_BEGIN;
             ruleDomainName = "";
        }
        if (token == "{" && tokenState == LCA_CONTEXT_BEGIN) tokenState = LCA_CONTEXT_READ;
        if (token == "{" && tokenState == LCA_RULE_BEGIN) tokenState = LCA_RULE_READ;
    }
}

void DramaSimulator::loadfile(std::string filename)
{
    std::ifstream myReadFile;
    myReadFile.open(filename);
    LCAParser parser = LCAParser(); 
    std::string* token;
    bool comment = false;
    if (myReadFile)
    {
        char output[100];
        int pos;
        if (myReadFile.is_open()){
            while (myReadFile.get(output,100,0)){
                //myReadFile >> output;
                //if (std::string(output) == "//") comment = !comment;
                //if (!comment) std::cout << output;
                /*
                pos =0;
                while (pos <= strlen(output))
                    std::cout << output[pos++];
                //Log("\n\t----------------------------------");
                */
                token = parser.tokenize(output); 
                while (token != NULL){
                    getKeyword(*token);
                    //std::cout << std::to_string(parser.loading_state).c_str() << "::";
                    //Log(("New Token:"+*token).c_str());
                    token = parser.nextToken();
                }
                 
            }
            token = parser.token;
            getKeyword(*token);
            //Log(("New Token:" + *token).c_str());
            delete token;
            myReadFile.close();
        }
    }
    else
    {
        Log("  (DRAMA): File could not be loaded");
    }
    Log("  (DRAMA): File was loaded");
}

LogicRuleSet* DramaSimulator::getruleset(std::string ruleset)
{
    if (rules.find(ruleset) != rules.end()){
        return rules[ruleset]; 
    } else{
        LogicRuleSet *base = new LogicRuleSet();
        rules[ruleset] = base;
        Log(("(-) Ruleset created: " + ruleset).c_str());
        return base;
        //return NULL;
    }
};
