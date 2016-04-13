#include "fl/Headers.h"
#include <string>
int main(int argc, char* argv[]){
    using namespace fl;
    Engine* engine = new Engine("safety-rating");

    InputVariable* securityRating = new InputVariable;
    securityRating->setName("SecurityRating");
    securityRating->setRange(-1.00, 1.00);
    securityRating->addTerm(new Ramp(    "VeryLow",  0.00, -0.40));
    securityRating->addTerm(new Triangle("Low",     -0.10, 0.60));
    securityRating->addTerm(new Ramp(    "High",     0.50, 0.90));
    engine->addInputVariable(securityRating);

    InputVariable* incursionControl = new InputVariable;
    incursionControl->setName("IncursionControl");
    incursionControl->setRange(0.00, 1.00);
    incursionControl->addTerm(new Ramp("Low",  0.40, 0.00));
    incursionControl->addTerm(new Ramp("High", 0.30, 0.80));
    engine->addInputVariable(incursionControl);

    InputVariable* nonAlliedPilots = new InputVariable;
    nonAlliedPilots->setName("NonAlliedPilots");
    nonAlliedPilots->setRange(0, 10000);
    nonAlliedPilots->addTerm(new Ramp(    "Low",    5, 0));
    nonAlliedPilots->addTerm(new Triangle("Medium", 2, 20));
    nonAlliedPilots->addTerm(new Ramp(    "High",  10, 20));
    engine->addInputVariable(nonAlliedPilots);

    InputVariable* shipsDestroyed = new InputVariable;
    shipsDestroyed->setName("ShipsDestroyed");
    shipsDestroyed->setRange(0, 5000);
    shipsDestroyed->addTerm(new Ramp(    "Low",    4, 0));
    shipsDestroyed->addTerm(new Triangle("Medium", 2, 10));
    shipsDestroyed->addTerm(new Ramp(    "High",   6, 10));
    engine->addInputVariable(shipsDestroyed);

    OutputVariable* safety = new OutputVariable;
    safety->setName("Safety");
    safety->setRange(0.000, 1.000);
    safety->setDefaultValue(fl::nan);
    safety->addTerm(new Ramp(    "VeryDangerous",     0.25, 0.00));
    safety->addTerm(new Triangle("Dangerous",         0.00, 0.50));
    safety->addTerm(new Triangle("SomewhatDangerous", 0.25, 0.75));
    safety->addTerm(new Triangle("SomewhatSafe",      0.50, 1.00));
    safety->addTerm(new Ramp(    "Safe",              0.75, 1.00));
    engine->addOutputVariable(safety);

    RuleBlock* ruleblock = new RuleBlock;
    ruleblock->addRule(Rule::parse("if SecurityRating is Low then Safety is Dangerous", engine));
    ruleblock->addRule(Rule::parse("if SecurityRating is VeryLow then Safety is VeryDangerous", engine));
    engine->addRuleBlock(ruleblock);

    engine->configure("", "", "Minimum", "Maximum", "Centroid");

    //std::string status;
    //if (not engine->isReady(&status))
    //    throw Exception("Engine not ready. "
    //        "The following errors were encountered:\n" + status, FL_AT);
    //for (int i = 0; i < 50; ++i){
    //    scalar light = securityRating->getMinimum() + i * (securityRating->range() / 50);
    //    securityRating->setInputValue(light);
    //    engine->process();
    //    FL_LOG("securityRating.input = " << Op::str(light) << " -> " <<
    //        "Power.output = " << Op::str(power->getOutputValue()));
    //}
}
