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
 * Limits.cpp
 *
 *  Created on: 2013-05-31
 *      Author: nicholas
 */

#include "cxxcam/Limits.h"
#include "cxxcam/Error.h"
#include "cxxcam/Position.h"
#include <boost/units/cmath.hpp>
#include <algorithm>

namespace cxxcam
{
namespace limits
{

void Travel::SetLimit(Axis::Type axis, units::length limit)
{
	m_Limits[axis] = limit;
}
void Travel::Validate(Axis::Type axis, units::length travel) const
{
	auto it = m_Limits.find(axis);
	if(it != m_Limits.end())
	{
		if(travel > it->second)
			throw error("Travel outside specified limit for axis");
	}
}
units::length Travel::MaxTravel(Axis::Type axis) const
{
	auto it = m_Limits.find(axis);
	if(it != m_Limits.end())
		return it->second;
	
	return {};
}

void FeedRate::SetGlobal(units::velocity limit)
{
	m_Global = limit;
}
void FeedRate::Set(Axis::Type axis, units::velocity limit)
{
	if(!is_linear(axis))
		throw error("Cannot set linear velocity on angular axis.");
	m_Linear[axis] = limit;
}
void FeedRate::Set(Axis::Type axis, units::angular_velocity limit)
{
	if(is_linear(axis))
		throw error("Cannot set angular velocity on linear axis.");
	m_Angular[axis] = limit;
}
void FeedRate::Validate(Axis::Type axis, units::velocity rate) const
{
	if(!is_linear(axis))
		throw error("Attempt to validate linear velocity on angular axis.");
	
	auto it = m_Linear.find(axis);
	if(it != m_Linear.end())
	{
		if(rate > it->second)
			throw error("FeedRate outside specified limit for axis");
	}
	
	if(rate > m_Global)
		throw error("FeedRate outside specified global limit");
}
void FeedRate::Validate(Axis::Type axis, units::angular_velocity rate) const
{
	if(is_linear(axis))
		throw error("Attempt to validate angular velocity on linear axis.");
	
	auto it = m_Angular.find(axis);
	if(it != m_Angular.end())
	{
		if(rate > it->second)
			throw error("FeedRate outside specified limit for axis");
	}
}
units::velocity FeedRate::MaxLinear(Axis::Type axis) const
{
	if(!is_linear(axis))
		throw error("Attempt to get max linear velocity on angular axis.");
		
	auto it = m_Linear.find(axis);
	if(it != m_Linear.end())
		return it->second;
	
	return m_Global;
}
units::angular_velocity FeedRate::MaxAngular(Axis::Type axis) const
{
	if(is_linear(axis))
		throw error("Attempt to get max angular velocity on linear axis.");
		
	auto it = m_Angular.find(axis);
	if(it != m_Angular.end())
		return it->second;
	
	return {};
}

void Rapids::SetGlobal(units::velocity limit)
{
	m_Global = limit;
}
void Rapids::Set(Axis::Type axis, units::velocity limit)
{
	if(!is_linear(axis))
		throw error("Cannot set linear velocity on angular axis.");
	m_Linear[axis] = limit;
}
void Rapids::Set(Axis::Type axis, units::angular_velocity limit)
{
	if(is_linear(axis))
		throw error("Cannot set angular velocity on linear axis.");
	m_Angular[axis] = limit;
}
units::time Rapids::Duration(const Position& begin, const Position& end) const
{
	static const units::length zero;
	static const units::velocity velocity_zero;
	static const units::plane_angle angular_zero;
	static const units::angular_velocity angular_velocity_zero;

	auto linear_axis_time = [this](units::length begin, units::length end, Axis::Type axis) -> units::time
	{
		if(!is_linear(axis))
			throw std::logic_error("Attempt to calculate linear time on angular axis.");
		
		auto distance = abs(end - begin);
		auto velocity = LinearVelocity(axis);
		
		if(velocity == velocity_zero)
		{
			if(distance != zero)
				throw error("Linear movement on axis with zero velocity will take infinite time.");
			
			return {};
		}
		return distance / velocity;
	};
	auto angular_axis_time = [this](units::plane_angle begin, units::plane_angle end, Axis::Type axis) -> units::time
	{
		if(is_linear(axis))
			throw std::logic_error("Attempt to calculate angular time on linear axis.");
		
		auto distance = abs(end - begin);
		auto velocity = AngularVelocity(axis);
		if(velocity == angular_velocity_zero)
		{
			if(distance != angular_zero)
				throw error("Angular movement on axis with zero velocity will take infinite time.");
			
			return {};
		}
		return distance / velocity;
	};
	
	return std::max(
	{
		linear_axis_time(begin.X, end.X, Axis::Type::X),
		linear_axis_time(begin.Y, end.Y, Axis::Type::Y),
		linear_axis_time(begin.Z, end.Z, Axis::Type::Z),
		angular_axis_time(begin.A, end.A, Axis::Type::A),
		angular_axis_time(begin.B, end.B, Axis::Type::B),
		angular_axis_time(begin.C, end.C, Axis::Type::C),
		linear_axis_time(begin.U, end.U, Axis::Type::U),
		linear_axis_time(begin.V, end.V, Axis::Type::V),
		linear_axis_time(begin.W, end.W, Axis::Type::W)
	});
}
units::velocity Rapids::LinearVelocity(Axis::Type axis) const
{
	if(!is_linear(axis))
		throw error("Attempt to get max linear velocity on angular axis.");
	
	auto it = m_Linear.find(axis);
	if(it != m_Linear.end())
		return it->second;
	
	return m_Global;
}
units::angular_velocity Rapids::AngularVelocity(Axis::Type axis) const
{
	if(is_linear(axis))
		throw error("Attempt to get max angular velocity on linear axis.");
	
	auto it = m_Angular.find(axis);
	if(it != m_Angular.end())
		return it->second;
	
	return {};
}

auto AvailableAxes::begin() const -> const_iterator
{
	return axes.begin();
}
auto AvailableAxes::end() const -> const_iterator
{
	return axes.end();
}

AvailableAxes::AvailableAxes()
 : axes({
   Axis::Type::X,
   Axis::Type::Y,
   Axis::Type::Z,
   
   Axis::Type::A,
   Axis::Type::B,
   Axis::Type::C,
   
   Axis::Type::U,
   Axis::Type::V,
   Axis::Type::W
   })
{
}
AvailableAxes::AvailableAxes(std::vector<Axis::Type> axes)
 : axes(axes)
{
}
void AvailableAxes::Validate(Axis::Type axis) const
{
	if(std::find(begin(), end(), axis) == end())
		throw error("Invalid Axis.");
}

}
}

