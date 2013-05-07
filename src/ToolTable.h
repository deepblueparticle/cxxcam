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
 * ToolTable.h
 *
 *  Created on: 27/04/2012
 *      Author: nicholas
 */

#ifndef TOOLTABLE_H_
#define TOOLTABLE_H_
#include <map>
#include "Tool.h"

class ToolTable
{
private:
	std::map<int, Tool> m_Tools;
public:
	bool AddTool(int id, const Tool& tool);
	bool Get(int id, Tool* tool);
	bool RemoveTool(int id);
};

#endif /* TOOLTABLE_H_ */
