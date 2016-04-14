#include "fl/Headers.h"
#include <string>

double getInputNumberInRange(std::string prompt, fl::InputVariable* inVar){
  double num = 0;
  std::string input = "";

  while (true) {
    std::cout << prompt << " (" << inVar->getMinimum() << " to " << inVar->getMaximum() << "): ";
    getline(std::cin, input);

    // Convert from string to number safely.
    std::stringstream myStream(input);
    if (myStream >> num){
      // make sure it's in the allowed range
      if (num >= inVar->getMinimum() && num <= inVar->getMaximum()){
        break;
      }
    }
    std::cout << "Invalid number, please try again" << std::endl;
  }

  return num;
}

int main(){
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
    incursionControl->addTerm(new Ramp(     "Low",  0.40, 0.00));
    incursionControl->addTerm(new Ramp(     "High", 0.30, 0.80));
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
    shipsDestroyed->addTerm(new Ramp(    "VeryLow", 1, 0));
    shipsDestroyed->addTerm(new Ramp(    "Low",     4, 0));
    shipsDestroyed->addTerm(new Triangle("Medium",  2, 10));
    shipsDestroyed->addTerm(new Ramp(    "High",    6, 10));
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
    ruleblock->addRule(Rule::parse("if SecurityRating is High then Safety is Safe", engine));
    ruleblock->addRule(Rule::parse("if SecurityRating is Low then Safety is Dangerous", engine));
    ruleblock->addRule(Rule::parse("if SecurityRating is VeryLow then Safety is VeryDangerous", engine));

    ruleblock->addRule(Rule::parse(
          std::string("if IncursionControl is Low and SecurityRating is not High")
          + " then Safety is SomewhatDangerous", engine));
    ruleblock->addRule(Rule::parse("if IncursionControl is High then Safety is Dangerous", engine));
    ruleblock->addRule(Rule::parse(
          std::string("if IncursionControl is High and SecurityRating is VeryLow")
          + " then Safety is VeryDangerous", engine));

    ruleblock->addRule(Rule::parse("if NonAlliedPilots is Low then Safety is Safe", engine));
    ruleblock->addRule(Rule::parse(
          std::string("if NonAlliedPilots is Medium and")
          + std::string(" (SecurityRating is Low or SecurityRating is VeryLow)")
          + " then Safety is Dangerous", engine));
    ruleblock->addRule(Rule::parse(
          std::string("if NonAlliedPilots is High and")
          + std::string(" (SecurityRating is Low or SecurityRating is VeryLow)")
          + " then Safety is VeryDangerous", engine));

    ruleblock->addRule(Rule::parse("if ShipsDestroyed is VeryLow then Safety is Safe", engine));
    ruleblock->addRule(Rule::parse("if ShipsDestroyed is Low then Safety is SomewhatSafe", engine));
    ruleblock->addRule(Rule::parse("if ShipsDestroyed is Medium then Safety is Dangerous", engine));
    ruleblock->addRule(Rule::parse("if ShipsDestroyed is High then Safety is VeryDangerous", engine));

    engine->addRuleBlock(ruleblock);

    engine->configure("Minimum", "Maximum", "Minimum", "Maximum", "Centroid");

    std::string status;
    if (not engine->isReady(&status)){
        throw Exception("Engine not ready. "
            "The following errors were encountered:\n" + status, FL_AT);
    }

    // get user input for a system
    std::cout << "Please enter the following information for the system you want evaluate" << std::endl;

    securityRating->setInputValue(getInputNumberInRange("Securty Rating", securityRating));
    incursionControl->setInputValue(getInputNumberInRange("Incursion Control Percent as a Decimal",
          incursionControl));

    int totalPilots = getInputNumberInRange("Average number of players in space in the last 30 minutes",
        nonAlliedPilots);
    int corpPilots = getInputNumberInRange("Number of corporation members in space", nonAlliedPilots);
    int nonAllied = totalPilots - corpPilots;
    if (nonAllied < 0)
      nonAllied = 0;
    nonAlliedPilots->setInputValue(nonAllied);

    shipsDestroyed->setInputValue(getInputNumberInRange("Ships destroyed in the last 30 minutes",
          shipsDestroyed));
    engine->process();

    std::cout << "Safety rating is " << safety->getOutputValue() << std::endl;
}
