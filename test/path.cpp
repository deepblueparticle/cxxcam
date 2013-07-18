#include "Path.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const cxxcam::math::vector_3& r)
{
	os << "(" << r.x << "," << r.y << "," << r.z << "," << r.a << ")";
	return os;
}

void simple()
{
	std::cout << "simple\n";
	using namespace cxxcam;
	using namespace cxxcam::path;
	using namespace cxxcam::units;
	using namespace cxxcam::math;
	
	Position start;
	Position end;
	end.X = length{50 * millimeters};
	end.Y = length{100 * millimeters};
	end.A = plane_angle{180 * degrees};
	
	limits::AvailableAxes geometry;
	
	std::cout << start << " -> " << end << '\n';
	
	auto steps = expand_linear(start, end, geometry, 1);
	
	for(const auto& step : steps)
	{
		std::cout << step << '\n';
		std::cout << normalise(vector_3(step.orientation)) << '\n';
	}
}

void check_start_end()
{
	std::cout << "check_start_end\n";
	using namespace cxxcam;
	using namespace cxxcam::path;
	using namespace cxxcam::units;
	using namespace cxxcam::math;
	
	Position start;
	start.Z = length{90 * millimeters};

	Position end;
	end.X = length{50 * millimeters};
	end.Z = length{90 * millimeters};
	
	limits::AvailableAxes geometry;
	
	std::cout << start << " -> " << end << '\n';
	
	auto steps = expand_linear(start, end, geometry, 1);
	
	step s0;
	s0.position.z = length{90 * millimeters};
	
	step s1;
	s1.position.x = length{50 * millimeters};
	s1.position.z = length{90 * millimeters};
	
	std::cout << "Expected: " << s0 << " Actual: " << steps.front() << '\n';
	std::cout << "Expected: " << s1 << " Actual: " << steps.back() << '\n';
	
	step as0 = steps.front();
	step as1 = steps.back();
	if(as0 != s0)
		throw std::logic_error("Start position is incorrect");
	if(as1 != s1)
		throw std::logic_error("End position is incorrect");
}

int main()
{
	simple();
	check_start_end();
	
	return 0;
}

