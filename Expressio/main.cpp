#include <iostream>
#include <ctype.h>
#include <cmath>
#include <algorithm>
#include <boost/math/constants/constants.hpp>

#include "Expressio.hpp"

class Calculator
{
	Expressio::Evaluator<double> evaluator;
	
public:
	Calculator()
	{
		const double pi = boost::math::constants::pi<double>();
		const double e =  boost::math::constants::e<double>();
		
		evaluator.addConstant("pi", pi);
		evaluator.addConstant("e", e);
		
		evaluator.addFunction("abs"  , [](double x) { return std::abs(x);    });
		evaluator.addFunction("cbrt" , [](double x) { return std::cbrt(x);   });
		evaluator.addFunction("ceil" , [](double x) { return std::ceil(x);   });
		evaluator.addFunction("exp2" , [](double x) { return std::exp2(x);   });
		evaluator.addFunction("exp"  , [](double x) { return std::exp(x);    });
		evaluator.addFunction("fact" , [](double x) { return std::tgamma(x); });
		evaluator.addFunction("floor", [](double x) { return std::floor(x);  });
		evaluator.addFunction("log10", [](double x) { return std::log10(x);  });
		evaluator.addFunction("log2" , [](double x) { return std::log2(x);   });
		evaluator.addFunction("log"  , [](double x) { return std::log(x);    });
		evaluator.addFunction("round", [](double x) { return std::round(x);  });
		evaluator.addFunction("sqrt" , [](double x) { return std::sqrt(x);   });
		evaluator.addFunction("trunc", [](double x) { return std::trunc(x);  });

		auto sin = [=](double x)
		{	
			// zero for 180°n, n ∈ ℤ (make it explicit to fix rounding issues)
			if (std::fmod(x, 180) == 0)
				return 0.0;
			
			return std::sin(x * pi / 180);
		};
		
		auto cos = [=](double x)
		{
			// zero for ±90° + 180°n, n ∈ ℤ (make it explicit to fix rounding issues)
			if (std::fmod(x - 90, 180) == 0)
				return 0.0;
			
			return std::cos(x * pi / 180);
		};
		
		auto tan = [=](double x)
		{
			// undefined for ±90° + 180°n, n ∈ ℤ
			if (std::fmod(x - 90, 180) == 0)
				throw std::invalid_argument("Math error: Invalid angle");
			
			// zero for 180°n, n ∈ ℤ (make it explicit to fix rounding issues)
			if (std::fmod(x, 180) == 0)
				return 0.0;
			
			return std::tan(x * pi / 180);
		};
		
		evaluator.addFunction("sin"  , sin                                                   );
		evaluator.addFunction("cos"  , cos                                                   );
		evaluator.addFunction("tan"  , tan                                                   );
		evaluator.addFunction("cot"  , [=](double x) { return tan(90 - x);                  });
		evaluator.addFunction("asin" , [=](double x) { return std::asin(x) * 180 / pi;      });
		evaluator.addFunction("acos" , [=](double x) { return std::acos(x) * 180 / pi;      });
		evaluator.addFunction("atan" , [=](double x) { return std::atan(x) * 180 / pi;      });
		evaluator.addFunction("acot" , [=](double x) { return 90 - std::atan(x) * 180 / pi; });
		
		evaluator.addFunction("acosh", [](double x) { return std::acosh(x); });
		evaluator.addFunction("asinh", [](double x) { return std::asinh(x); });
		evaluator.addFunction("atanh", [](double x) { return std::atanh(x); });
		evaluator.addFunction("cosh" , [](double x) { return std::cosh(x);  });
		evaluator.addFunction("sinh" , [](double x) { return std::sinh(x);  });
		evaluator.addFunction("tanh" , [](double x) { return std::tanh(x);  });
		
		evaluator.addFunction("hypot", [](double x, double y) { return std::hypot(x, y); });
		evaluator.addFunction("max"  , [](double x, double y) { return std::fmax(x, y);  });
		evaluator.addFunction("min"  , [](double x, double y) { return std::fmin(x, y);  });
		evaluator.addFunction("mod"  , [](double x, double y) { return std::fmod(x, y);  });
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

int main(int argc, char* argv[])
{
	Calculator c;
	
	if (argc > 1)
	{
		std::cout << c.calculate(argv[1]) << '\n';
		return 0;
	}
	
	for (std::string expr; getline(std::cin, expr) && expr != "exit"; )
		std::cout << c.calculate(expr) << '\n';
}
