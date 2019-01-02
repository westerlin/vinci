#include "dramasim.h"

const char *pattern = "([A-Za-z_]+)[/.|/!]{0,1}";

int main(int argc, char **argv)
{
    Log("Logica 2018 running ... ");
    getSettings(argc, argv);
    Logica* logica = new Logica();
    Log("ok");
    logica->add("actors");
    logica->add("locations");
    Logica* locations = logica->get("locations");
    locations->add("farm");
    locations->add("fields");
    Log(locations->get("fields")->name.c_str());
    //Log(logica.name.c_str());
    auto mymatch = matcher("worldstate.actors.charles", pattern);
    while (mymatch.snip){
        Log(mymatch.match);
        mymatch = matcher(mymatch.snip, pattern);
    }
    for( auto x : logica->children){
        Log(x.first.c_str());
    }
    for (auto x : locations->children)
    {
        Log(("\t"+x.first).c_str());
    }
    if (logica->has(".locations.farm"))
        Log ("Found...");
    else
        Log("NOT Found...");
    locations->pop("farm");
    if (logica->has(".locations!fields"))
        Log("Found...");
    else
        Log("NOT Found...");

    //delete locations;
    locations->cleanup();
    if (logica->has(".locations.farm"))
        Log("Found...");
    else
        Log("NOT Found...");
    if (logica->has(".locations"))
        Log("Found...");
    else
        Log("NOT Found...");


    LogicRule* rule = new LogicRule();
    rule->addprecondition("actors.charles.at.livingroom");
    //rule->addprecondition("actors.lucy.at.hall");
    //rule->addprecondition("actors.revere.at.study");

    logica->add(".actors.charles.at.livingroom")->add(".in.the.park");
    //logica->add("actors")->add("charles")->add("at")->add("livingroom");
    //logica->has()
    logica->add(".actors.lucy.at.hall");
    logica->add(".actors.revere.at.study");
    logica->add(".locations.hall");
    logica->add(".locations.study");
    logica->add(".locations.livingroom");

    if (rule->evaluate(logica))
    {
        Log("Rule is fulfilled");
    }

    /*

    for(auto branch : logica->children){
        Log(branch.first.c_str());
    }
*/

    logica->outputTree("");

    logica->add("!simpleworld");
    logica->add(".simpleworld.netherworld.creatures.demon");
    logica->add(".simpleworld.netherworld.creatures.dragon");
    logica->add(".simpleworld.netherworld.locations.cave");
    logica->add(".simpleworld.netherworld.locations.temple");

    logica->outputTree("");

    for (auto prec : rule->preconditions)
    {
        Log(prec->c_str());
        *prec = "clean";
        /*
        or
        delete prec;
        prec = new std::string("clean");
        */
    }

    for (auto prec : rule->preconditions)
    {
        Log(prec->c_str());
    }
    
    
    Log("Finished!!");

    const char* pattern = "(set|del|print) [/(]?[\"]?([a-zA-Z/./!]+)[\"]?[/)]?";
    std::smatch* mytest = findPattern("set .ws.actors.charles.is.angry", pattern);
    if (mytest) {
        Log(mytest->str(1).c_str());
        Log(mytest->str(2).c_str());
    }
    delete mytest;
    mytest = findPattern("print (\"Hello world\")", pattern);
    if (mytest)
    {
        Log(mytest->str(1).c_str());
        Log(mytest->str(2).c_str());
    }

    rule->addimplication("print (\"Hello World says the rule\")");
    rule->addimplication("set .simpleworld.underworld");
    rule->addimplication("set .anotherworld.underworld");
    rule->addimplication("pop .simpleworld.netherworld.locations.cave");
    rule->addimplication("print (\"I am soo angry, says Lucelle\")");
    scenario scene;
    rule->implement(logica,scene);

    //logica->pop(".simpleworld.underworld");
    logica->outputTree("");

    rule->addrule("myrule \"Hello world\" : ws.actors.charles.in.lounge * asdsad -> {print(\"delte\"); \nprint(\"Hello\")}");
    rule->addrule("myrule : ws!actors.charles.in!lounge * asdsad -> {set aass.asdasd.asdsd}");
    //rule->addrule("myrule : ws.actors.charles.in.lounge * asdsad -> # print(\"delte\"), \nprint(\"Hello\") #");
    //rule->addrule("myrule : ws_actors_charles_in_lounge asdsad -> # askldaksdl #");
    //rule->addrule("myrule : ws.actors.charles.in.lounge * asdsad -> print(\"delte\"), \nprint(\"Hello\") ;");
    //rule->addrule("myrule : ws_actors_charles_in_lounge asdsad -> askldaksdl ;");
    delete rule;

    scenarioList scenes;
    
    logica->parameters(".X.netherworld.Y.V",scenes);
    Log(("\t"+std::to_string(scenes.size())).c_str());
    pscenarioList(scenes);

    scenarioList scenes2;
    //clearscenarioList(scenes);

    logica->parameters(".A.Y", scenes2);
    Log(("\t" + std::to_string(scenes.size())).c_str());
    pscenarioList(scenes2);

    //scenarioList *output = combineLists(scenes, scenes);

    logica->add(".actors.charles.gender!male");
    logica->add(".actors.charles.age!young");
    logica->add(".actors.lucy.gender!female");
    logica->add(".actors.lucy.age!middleaged");
    logica->add(".actors.manfred.gender!male");
    logica->add(".actors.manfred.age!middleaged");
    logica->add(".actors.mathilde.gender!female");
    logica->add(".actors.mathilde.age!young");

    logica->add(".at.charles!courtyard");
    logica->add(".at.lucy!courtyard");
    logica->add(".at.manfred!courtyard");
    logica->add(".at.mathilde!temple");

    logica->add(".colors.charles.red");
    logica->add(".colors.charles.blue");
    logica->add(".colors.charles.green");
    logica->add(".colors.lucy.green");
    logica->add(".colors.manfred.black");
    logica->add(".colors.mathilde.yellow");
    logica->add(".colors.mathilde.purple");
    logica->outputTree("");

    scenarioList all_actors;
    //logica->parameters(".actors.X.age.AGE", "", scene, all_actors);
    logica->parameters(".actors.X", all_actors);
    scenarioList all_locations;
    //logica->parameters(".actors.Y", "", scene, all_locations);
    logica->parameters(".colors.X!green", all_locations);
    scenarioList* output=combineLists(all_actors, all_locations);

    Log("output:");
    pscenarioList(*output);
    Log("All Actors:");
    pscenarioList(all_actors);
    Log("All Actors with color green:");
    pscenarioList(all_locations);

    //MyMatch("worldstate.actors.charles", "([A-Za-z_]+)[/.|/!]{0,1}");
    //Log("Please press <enter> to exit!");
    //std::cin.get();
    /*
    logica->add("t.aa.b");
    logica->add("t.a.b");
    logica->add("t!a.c.d");
    logica->outputTree("");
    logica->add("t!unik.c.d");
    logica->outputTree("");
    */
    LogicRule myrule;
    myrule.addprecondition(".actors.X");
    myrule.addprecondition(".colors.X!black");
    Log("Testing rule");
    myrule.combinations(logica);

    Log("--------");
    scenario rasmus;
    rasmus["X"]="Hello";
    rasmus["Y"] = "World";
    Log(fillInParms("X.Y.X", rasmus)->c_str());
}