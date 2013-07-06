/* cxxcam - C++ CAD/CAM driver library.
 * Copyright (C) 2013  Nicholas Gill
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Math.cpp
 *
 *  Created on: 2013-07-06
 *      Author: nicholas
 */

#include "Math.h"
#include <boost/units/cmath.hpp>

namespace cxxcam
{
namespace math
{

vector_3::vector_3(const quaternion_t& q)
{
	auto scale = sqrt(q.R_component_1() * q.R_component_1() + q.R_component_2() * q.R_component_2() + q.R_component_3() * q.R_component_3());
	if(scale == 0.0)
	{
		x = q.R_component_2();
		y = q.R_component_3();
		z = q.R_component_4();
	}
	else
	{
		x = q.R_component_2() / scale;
		y = q.R_component_3() / scale;
		z = q.R_component_4() / scale;
	}
	a = (acos(q.R_component_1()) * 2.0) * 57.2957795;
}

units::length distance(const point_3& p0, const point_3& p1)
{
	return units::length{sqrt((p0.x-p1.x)*(p0.x-p1.x) + (p0.y-p1.y)*(p0.y-p1.y) + (p0.z-p1.z)*(p0.z-p1.z))};
}

quaternion_t::value_type dot(const quaternion_t& q1, const quaternion_t& q2)
{
	return
		q1.R_component_1() * q2.R_component_1() +
		q1.R_component_2() * q2.R_component_2() +
		q1.R_component_3() * q2.R_component_3() +
		q1.R_component_4() * q2.R_component_4();
}
quaternion_t normalise(const quaternion_t& q)
{
	return q / dot(q, q);
}

quaternion_t axis2quat(double x, double y, double z, units::plane_angle theta)
{
	theta /= 2;
    auto sint = sin(theta);
	return quaternion_t{sint*x, sint*y, sint*z, cos(theta)};
}

}
}

