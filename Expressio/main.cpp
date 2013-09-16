#include <iostream>
#include <complex>
#include <cmath>
#include "Expressio.hpp"

class Calculator
{
	Expressio::Evaluator<double> evaluator;
	
public:
	Calculator()
	{
		evaluator.addFunction("abs"  , [](double x) { return std::abs(x);   });
		evaluator.addFunction("acos" , [](double x) { return std::acos(x);  });
		evaluator.addFunction("acosh", [](double x) { return std::acosh(x); });
		evaluator.addFunction("asin" , [](double x) { return std::asin(x);  });
		evaluator.addFunction("asinh", [](double x) { return std::asinh(x); });
		evaluator.addFunction("atan" , [](double x) { return std::atan(x);  });
		evaluator.addFunction("atanh", [](double x) { return std::atanh(x); });
		evaluator.addFunction("cos"  , [](double x) { return std::cos(x);   });
		evaluator.addFunction("cosh" , [](double x) { return std::cosh(x);  });
		evaluator.addFunction("exp"  , [](double x) { return std::exp(x);   });
		evaluator.addFunction("log"  , [](double x) { return std::log(x);   });
		evaluator.addFunction("log10", [](double x) { return std::log10(x); });
		evaluator.addFunction("sin"  , [](double x) { return std::sin(x);   });
		evaluator.addFunction("sinh" , [](double x) { return std::sinh(x);  });
		evaluator.addFunction("sqrt" , [](double x) { return std::sqrt(x);  });
		evaluator.addFunction("tan"  , [](double x) { return std::tan(x);   });
		evaluator.addFunction("tanh" , [](double x) { return std::tanh(x);  });	
	}

	std::string calculate(const std::string& expression)
	{
		std::stringstream result;
		
		try
		{
			result << evaluator.evaluate(expression);
		}
		catch (const std::exception& e)
		{
			result << e.what();
		}
		
		return result.str();
	}
};

int main()
{
	Calculator c;

	for (std::string expr; getline(std::cin, expr) && expr != "exit"; )
		std::cout << c.calculate(expr) << '\n';
}
