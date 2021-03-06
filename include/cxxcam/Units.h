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
 * Units.h
 *
 *  Created on: 2013-05-31
 *      Author: nicholas
 */

#ifndef UNITS_H_
#define UNITS_H_
#include <boost/units/quantity.hpp>
#include <boost/units/io.hpp>

#include <boost/units/systems/si.hpp>
#include <boost/units/systems/si/prefixes.hpp>
#include <boost/units/systems/si/io.hpp>

#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/torque.hpp>
#include <boost/units/systems/si/velocity.hpp>
#include <boost/units/systems/si/volume.hpp>
#include <boost/units/systems/si/time.hpp>
#include <boost/units/systems/si/angular_velocity.hpp>
#include <boost/units/systems/si/plane_angle.hpp>
#include <boost/units/systems/angle/degrees.hpp>

#include <boost/units/base_units/imperial/inch.hpp>

#include <boost/units/base_units/metric/minute.hpp>

namespace cxxcam
{
namespace units
{

typedef boost::units::quantity<boost::units::si::length> length;
typedef boost::units::quantity<boost::units::si::torque> torque;
typedef boost::units::quantity<boost::units::si::velocity> velocity;
typedef boost::units::quantity<boost::units::si::volume> volume;
// TODO is a static assert that time is represented as seconds necessary?
typedef boost::units::quantity<boost::units::si::time> time;
typedef boost::units::quantity<boost::units::si::angular_velocity> angular_velocity;
typedef boost::units::quantity<boost::units::si::plane_angle> plane_angle;

static const auto meter = boost::units::si::meter;
static const auto meters = meter;
static const auto millimeters = boost::units::si::milli * meter;
static const auto millimeter = millimeters;

static const auto cubic_millimeters = millimeter * millimeter * millimeter;

static const auto inches = boost::units::imperial::inch_base_unit::unit_type{};
static const auto inch = inches;

static const auto second = boost::units::si::seconds;
static const auto minute = boost::units::metric::minute_base_unit::unit_type{};

static const auto millimeters_per_minute = millimeters / minute;
static const auto inches_per_minute = inches / minute;

static const auto radians = boost::units::si::radians;
static const auto degrees = boost::units::degree::degrees;
static const auto degrees_per_second = degrees / second;

static const auto newton_meters = boost::units::si::newton_meters;

typedef boost::units::quantity<decltype(millimeter)> length_mm;
typedef boost::units::quantity<decltype(inch)> length_inch;
typedef boost::units::quantity<decltype(millimeters_per_minute)> velocity_mmpm;
typedef boost::units::quantity<decltype(degrees)> plane_angle_deg;
typedef boost::units::quantity<decltype(radians)> plane_angle_rads;
typedef boost::units::quantity<decltype(newton_meters)> torque_nm;

}
}

#endif /* UNITS_H_ */
