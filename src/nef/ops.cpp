/*
 * ops.cpp
 *
 *  Created on: 06/05/2013
 *      Author: nicholas
 */
#include "ops.h"
#include "private.h"
#include "copy_to.h"
#include <utility>
#include <cassert>

#include "cgal.h"

// Meshing (for volume)
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Mesh_triangulation_3.h>
#include <CGAL/Mesh_complex_3_in_triangulation_3.h>
#include <CGAL/Mesh_criteria_3.h>
#include <CGAL/Polyhedral_mesh_domain_3.h>
#include <CGAL/Polyhedral_mesh_domain_with_features_3.h>
#include <CGAL/make_mesh_3.h>
#include <CGAL/refine_mesh_3.h>

// glide
#include <CGAL/minkowski_sum_3.h>

// Meshing
typedef CGAL::Exact_predicates_inexact_constructions_kernel Inexact_Kernel;
typedef CGAL::Mesh_polyhedron_3<Inexact_Kernel>::type Mesh_polyhedron_3;

typedef CGAL::Polyhedral_mesh_domain_with_features_3<Inexact_Kernel, Mesh_polyhedron_3> Mesh_domain;
typedef CGAL::Mesh_triangulation_3<Mesh_domain>::type Tr;
typedef CGAL::Mesh_complex_3_in_triangulation_3<Tr> C3t3;
typedef CGAL::Mesh_criteria_3<Tr> Mesh_criteria;

namespace nef
{

polyhedron_t glide(const polyhedron_t& polyhedron, const polyline_t& path)
{
	typedef std::vector<Point_3>::const_iterator point_iterator;
	typedef std::pair<point_iterator, point_iterator>  point_range;
	typedef std::vector<point_range> polyline;

	std::vector<Point_3> point_line;
	for(auto& point : path.line)
		point_line.emplace_back(point.x, point.y, point.z);

	polyline poly = { point_range(begin(point_line), end(point_line)) };
	Nef_polyhedron_3 N1(poly.begin(), poly.end(), Nef_polyhedron_3::Polylines_tag());

	// Need to make a copy because minkowski_sum_3 can modify its arguments
	auto nef = polyhedron.priv->nef;
	auto priv = std::make_shared<polyhedron_t::private_t>(CGAL::minkowski_sum_3(nef, N1));

	if (priv->nef.is_simple())
		return { priv };

	throw std::runtime_error("glide result is not 2-manifold");
}

double volume(const polyhedron_t& polyhedron)
{
	Mesh_polyhedron_3 PK;
	{
		typedef CGAL::Mesh_polyhedron_3<Exact_Kernel>::type Exact_Mesh_Polyhedron_3;
	
		Exact_Mesh_Polyhedron_3 EP;
		polyhedron.priv->nef.convert_to_polyhedron(EP);
		copy_to(EP, PK);
	}
	assert(PK.is_valid());

	Mesh_domain domain(PK);
	domain.detect_features();

	using CGAL::parameters::facet_angle;
	using CGAL::parameters::facet_size;
	using CGAL::parameters::facet_distance;
	using CGAL::parameters::cell_radius_edge_ratio;
	Mesh_criteria criteria(facet_angle = 25, facet_size = 0.15, facet_distance = 0.008, cell_radius_edge_ratio = 3);

	using CGAL::parameters::no_perturb;
	using CGAL::parameters::no_exude;
	auto c3t3 = CGAL::make_mesh_3<C3t3>(domain, criteria, no_perturb(), no_exude());

	auto triangulation = c3t3.triangulation();

	double volume(0);
//	std::vector<double> volumes;
//	volumes.reserve(triangulation.number_of_finite_cells());
	for(auto it = triangulation.finite_cells_begin(); it != triangulation.finite_cells_end(); ++it)
	{
		auto tetr = triangulation.tetrahedron(it);
		volume += tetr.volume();
	}
//	std::sort(volumes.begin(), volumes.end());
//	return std::accumulate(volumes.begin(), volumes.end(), 0.0);

	return volume;
}

}
