#include "dramasim.h"

void testLogica(Logica* l, std::string name){
    if (l->has(name))
        Log(("\033[0:32m" + name + "\033[0m").c_str());
    else
        Log(("\033[0:31m" + name + "\033[0m").c_str());
}

int main(int argc, char **argv)
{
    Log("Logica 2018 running ... ");
    //getSettings(argc, argv);
    DramaSimulator drama = DramaSimulator();
    /*
    drama.addContext(".actors.charles.gender!male");
    drama.addContext(".actors.peter.gender!male");
    drama.addContext(".actors.lucy.gender!female");
    drama.addContext(".actors.melinda.gender!female");
    drama.addContext(".actors.rose.gender!female");

    drama.addContext(".at.rose!livingroom");
    drama.addContext(".at.melinda!livingroom");
    drama.addContext(".at.lucy!livingroom");
    drama.addContext(".at.charles!livingroom");
    drama.addContext(".at.peter!livingroom");
    drama.addContext(".makingout");

    drama.addrule("basic", "greet \"Moving to the Barn\" : .at.lucy!livingroom * .at.peter!livingroom -> { print (\"Lucy and Peter is in the livingroom. Peter makes his excuses and leaves for the barn...\"); set .at.peter!barn }");
    drama.addrule("basic", "arrival_melinda \"Melinda and Peter meets up in the barn\" : .at.peter!barn * ~.at.melinda!barn -> {print (\"Peter is at the barn. Melinda enters the barn.\") ; set .actors.melinda.gender!female; set .at.melinda!barn }");
//    drama.addrule("basic", "arrival_rose \"Rose arrives to the barn\" : .at.peter!barn * ~.at.rose!barn -> {print (\"Peter is at the barn. Rose enters the barn.\") ; set .actors.rose.gender!female; set .at.rose.barn }");
    drama.addrule("basic", "arrival_rose \"Rose arrives to the barn\" : .at.peter!barn * ~.at.rose!barn -> {print (\"Peter is at the barn. Rose enters the barn.\") ; set .actors.rose.gender!female; set .at.rose!barn }");
    //    drama.addrule("basic", "flirt1 \"Making out in the barn\" : .at.peter!barn * .at.melinda!barn -> {print (\"Peter and Melinda are both in the barn and makes out ... \") ; set .actors.melinda.gender!female; set .at.melinda.barn }");
    drama.addrule("basic", "flirt1 \"Making out in the [LOCATION]\" : .at.[MAN]![LOCATION] * .at.[WOMAN]![LOCATION] * .actors.[MAN].gender!male * .actors.[WOMAN].gender!female -> {print (\"[MAN] and [WOMAN] are both in the [LOCATION] and makes out ... \") ; set .makingout.[MAN].[WOMAN].[LOCATION]}");
    //drama.addrule("basic", "generic \"Many hitters here [ACTOR]\" : .at.ACTOR!barn * .actors.rasmus -> {print (\"[ACTOR] is in the barn ... \")}");
    //drama.addrule("basic", "generic \"[ACTOR] moves from barn to castle\" : .at.ACTOR!barn -> {print (\"[ACTOR] goes to the castle ... \"); set .at.ACTOR!castle}");
    */
    //drama.execute(13);

    Logica logica = Logica();
    if (false)
    {
        Log("------------------------");
        logica.add(".turntaking!lucy!rose");
        logica.outputTree("");
        logica.log();
        Log("------------------------");
        logica.add(".turntaking!rose!lucy");
        logica.outputTree("");
        Log("------------------------");
        logica.add(".turntaking!lucy!rose");
        logica.outputTree("");
        Log("------------------------");
        logica.add(".turntaking!rose!lucy");
        logica.outputTree("");
        Log("------------------------");
        logica.add(".turntaking!lucy!rose");
        logica.outputTree("");
        Log("------------------------");
        logica.add(".turntaking!rose!lucy");
        logica.outputTree("");
        Log("------------------------");
        logica.revert();
        logica.outputTree("");
        Log("------------------------");
        testLogica(&logica, ".hello.thirdworld");
    }
    if (false)
    {
        Log("------------------------");
        logica.add(".turntaking!lucy!rose");
        logica.outputTree("");
        logica.log();
        Log("------------------------");
        logica.add(".turntaking!rose!lucy");
        logica.outputTree("");
        Log("------------------------");
        logica.add(".turntaking!lucy!rose");
        logica.outputTree("");
        Log("------------------------");
        logica.add(".turntaking!rose!lucy");
        logica.outputTree("");
        Log("------------------------");
        logica.add(".turntaking!lucy!rose");
        logica.outputTree("");
        Log("------------------------");
        logica.add(".turntaking!rose!lucy");
        logica.outputTree("");
        Log("------------------------");
        logica.add(".turntaking!rose!lucy");
        logica.outputTree("");
        Log("------------------------");
        logica.add(".turntaking!lucy!rose");
        logica.add(".turntaking!rose!rose");
        logica.add(".turntaking!lucy!lucy");
        logica.add(".turntaking!rose!rose");
        logica.pop("!turntaking");
        logica.outputTree("");
        Log("------------------------");
        logica.add(".turntaking!rose!lucy");
        logica.outputTree("");
        Log("------------------------");
        logica.add(".turntaking!lucy!rose");
        logica.outputTree("");
        Log("------------------------");
        logica.add(".turntaking!rose!lucy");
        logica.outputTree("");
        Log("------------------------");
        logica.add(".turntaking!lucy!rose");
        logica.outputTree("");
        Log("------------------------");
        logica.revert();
        logica.outputTree("");
        Log("------------------------");
        testLogica(&logica, ".hello.thirdworld");
    }

    if (true){
        bool isLogging = false;

        logAffordances = false;
        if (argc > 3) {
            if (strncmp(argv[3], "debug", 5) == 0) {
                logAffordances = true;
            } 
        } else
            Log ("Using 'debug' as third argument will enable logging of all rule evaluations");

        if (argc > 1) 
            drama.loadfile(argv[1]);
        else
            Log ("You need to specify a LCA file as first argument");
        
        if (argc > 4) {
            if (strncmp(argv[4], "log", 3) == 0) {
                drama.worldstate.log();
                isLogging = true;
            }    
        } else {
            Log ("You can specify if we want to test for reversibility by inserting 'log' as fourth argument");
        }

        if (argc >2) {
            if (isNumber(argv[2])) {
                Log("--------------- executing ----------------");
                drama.execute(std::stoi(argv[2]));
                drama.worldstate.outputTree("");
            }
        } else {
            Log ("No Second argument specifying number of executing runs");
        }
        if (isLogging) {
            drama.worldstate.revert();
            Log("--------------------------- reverted ----------------------------------");
            drama.worldstate.outputTree("");
        }    
    }
    if (false)
    {
        logica.add("anewstree")->populate(&drama.worldstate);
        logica.outputTree("");
        Log("------------------------");
        logica.revert();
        logica.outputTree("");
        Log("------------------------");
    }
    /*

    LogicRule myrule = LogicRule();
    myrule.addrule("flirt1 \"Making out in the barn\" : .at.[MAN]![LOCATION] * .at.[WOMAN]![LOCATION] * .actors.[MAN].gender!male * .actors.[WOMAN].gender!female -> {print (\"[MAN] and [WOMAN] are both in the [LOCATION] and makes out ... \") ; set .makingout.[MAN].[WOMAN].[LOCATION]}");
//    myrule.addrule("flirt1 \"Making out in the barn\" : .at.peter!barn * .at.rose!barn * .actors.peter.gender!male * .actors.rose.gender!female -> {print (\"[MAN] and [WOMAN] are both in the [LOCATION] and makes out ... \") ; set .makingout.MAN.WOMAN.LOCATION}");
    if (myrule.evaluate(&drama.worldstate))
    {
        Log("OK");
    }
    else
    {
        Log("FAIL");
    }

    myrule.combinations(&drama.worldstate);
    pscenarioList(myrule.parms);
    for (auto scene : myrule.parms){
        pscenario(scene);
        if (myrule.evaluate(&drama.worldstate, scene))
        {
            Log("OK");
        } else {
            Log("FAIL");
        }
    }
    */
    //drama.worldstate.get("makingout")->outputTree("");
}