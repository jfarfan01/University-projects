#include "task2.h"

void createTransferfunctionLookupTable(std::vector<float4>& lut, const std::vector<ControlPointSet>& controlpoint_sets)
{
	// TODO: fill the Lookup Table 'lut'
	//
	// The lookup-table already has the right size.
	// Each entry consists of four float values:
	//  - xyz are the rgb values (range [0,1])
	//  - w is the alpha value (range [0,1])
	//
	// For each lookup-table entry determine the rgba values of the controlpoint-sets at and mix them together.
	// Ues TransferFunction::mixColors() to mix the values.
	//
	// Each controlpoint-set has a color and some controlpoints stored in a std::map (you can ignore the gui variables).
	// The map key corresponds to the density and the map value corresponds to the alpha value.
	// Since the values are stored in a map, the control-points are already sorted by their density values.
	// Each controlpoint-set contains at least two controlpoints for the min and max density (which corresponds to
	// the first and last entry of the lookup-table).
	// The color is the same for all density values!
	// The alpha value has to be linearly interpolated between control-points!
    for (size_t i = 0; i < lut.size(); ++i) 
    {
          float alpha = 0;
          float3 rgb ;

      for (const auto& cp_set : controlpoint_sets) 
      {
        auto it = cp_set.control_points.find(i);
        rgb = cp_set.color;		
        if (it == cp_set.control_points.end())
        {
          auto it_next = cp_set.control_points.lower_bound(i);
          auto it_prev = std::prev(it_next);
          float x0 = it_prev->first;
          float x1 = it_next->first;
          float y0 = it_prev->second;
          float y1 = it_next->second;

          if(y1 < y0)
          {
            alpha = y0 - (y0 - y1) * (i - x0) / (x1 - x0);
          }
          else
          {
            alpha = y0 + (i - x0) * (y1 - y0) / (x1 - x0);
          }

        } 
        else if (it != cp_set.control_points.end())
        {
          alpha = it->second;
        }
      }
        lut[i] = TransferFunction::mixColors(lut[i].xyz(), rgb, lut[i].w, alpha / 255.0f);
    }
}

float4 lookupTransferFunction(const float density, const float opacity_correction, const std::vector<float4>& lut)
{
	// TODO: implement the transfer function lookup
	//
	// The lookup-table vector contains the rgba values for all possible densities.
	// Linearly interpolate between the entries of the lookup-table.
	// The opacity correction is used for scaling of the opacity values --- multiply the a-value by this factor.
	// Return the computed rgba value.
  // Determine the indices of the lookup table entries that the density falls between

  size_t idx0 = static_cast<size_t>(std::floor(density));
  size_t idx1 = static_cast<size_t>(std::ceil(density));
  
  idx0 = std::max(size_t(0), std::min(idx0, lut.size() - 1));
  idx1 = std::max(size_t(0), std::min(idx1, lut.size() - 1));
  
  float t = density - static_cast<float>(idx0);
  
  float4 value0 = lut[idx0];
  float4 value1 = lut[idx1];
  
  float4 interpolatedValue(
      value0.x + t * (value1.x - value0.x),
      value0.y + t * (value1.y - value0.y),
      value0.z + t * (value1.z - value0.z),
      (value0.w + t * (value1.w - value0.w)) * opacity_correction
  );
  
  
  return interpolatedValue;
}

bool intersectRayWithAABB(const float3& ray_start, const float3& ray_dir, const float3& aabb_min, const float3& aabb_max, float& t_near, float& t_far)
{
	// TODO: intersect the ray with the axis-aligned bounding box.
	//
	//  Set t_near and t_far to the distance to the entry and exit points of the ray
	//  Return true if the intersection happens and false otherwise.

	float tx1 = (aabb_min.x - ray_start.x)/ ray_dir.x;
	float tx2 = (aabb_max.x - ray_start.x)/ ray_dir.x;

	float ty1 = (aabb_min.y - ray_start.y)/ ray_dir.y;
	float ty2 = (aabb_max.y - ray_start.y)/ ray_dir.y;

	float tz1 = (aabb_min.z - ray_start.z)/ ray_dir.z;
	float tz2 = (aabb_max.z - ray_start.z)/ ray_dir.z;

	float temp_near = std::max(std::min(tz1, tz2), std::max(std::min(ty1, ty2), std::min(tx1, tx2)));
	float temp_far = std::min(std::max(tz1, tz2), std::min(std::max(ty1, ty2), std::max(tx1, tx2)));

	if(temp_near <= temp_far)
	{
		t_near = temp_near;
		t_far = temp_far;
		return true;
	}


	return false;
}

float interpolate(float v0, float v1, float t) {
    return v0 * (1.0f - t) + v1 * t;
}

float interpolatedDensity(const Volume& volume, const float3& sample_position)
{
	// TODO: use trilinear interpolation to calculate the density value for the sample position
	//
	// Useful functions:
	//  - AABB minimum corner position:  Volume::getAABBMin()
	//  - AABB maximum corner position:  Volume::getAABBMax()
	//  - Volume size in voxels:         Volume::getVolumeSize()
	//  - Volume voxel grid ratio:       Volume::getRatio() , Volume::getRatioInv()
	//
	// The voxel density values can be accessed via volume[uint3(x,y,z)].
	// The voxel grid ratio defines the spacing between the voxels in x,y,z direction.
	// e.g. ratio.x is the distance between two voxels in x-direction
	// Return the computed density.
  float3 aabb_min = volume.getAABBMin();
  float3 aabb_max = volume.getAABBMax();
  uint3 volume_size = volume.getVolumeSize();
  float3 ratio = volume.getRatio();
  
  float3 local_pos = (sample_position - aabb_min) / ratio;
  
  uint3 min_idx = { static_cast<unsigned int>(floor(local_pos.x)),
                    static_cast<unsigned int>(floor(local_pos.y)),
                    static_cast<unsigned int>(floor(local_pos.z)) };
  uint3 max_idx = { min_idx.x + 1, min_idx.y + 1, min_idx.z + 1 };
  
  min_idx.x = std::max(0u, std::min(min_idx.x, volume_size.x - 1));
  min_idx.y = std::max(0u, std::min(min_idx.y, volume_size.y - 1));
  min_idx.z = std::max(0u, std::min(min_idx.z, volume_size.z - 1));
  
  max_idx.x = std::max(0u, std::min(max_idx.x, volume_size.x - 1));
  max_idx.y = std::max(0u, std::min(max_idx.y, volume_size.y - 1));
  max_idx.z = std::max(0u, std::min(max_idx.z, volume_size.z - 1));
  
  if (local_pos.x < 0 || local_pos.x >= volume_size.x - 1 ||
      local_pos.y < 0 || local_pos.y >= volume_size.y - 1 ||
      local_pos.z < 0 || local_pos.z >= volume_size.z - 1) 
  {
      return 0.0f;
  }
  
  float density_v000 = volume[{ min_idx.x, min_idx.y, min_idx.z }];
  float density_v100 = volume[{ max_idx.x, min_idx.y, min_idx.z }];
  float density_v010 = volume[{ min_idx.x, max_idx.y, min_idx.z }];
  float density_v110 = volume[{ max_idx.x, max_idx.y, min_idx.z }];
  float density_v001 = volume[{ min_idx.x, min_idx.y, max_idx.z }];
  float density_v101 = volume[{ max_idx.x, min_idx.y, max_idx.z }];
  float density_v011 = volume[{ min_idx.x, max_idx.y, max_idx.z }];
  float density_v111 = volume[{ max_idx.x, max_idx.y, max_idx.z }];
  
  float tX = local_pos.x - min_idx.x;
  float tY = local_pos.y - min_idx.y;
  float tZ = local_pos.z - min_idx.z;
  
  float c00 = interpolate(density_v000, density_v100, tX);
  float c10 = interpolate(density_v010, density_v110, tX);
  float c01 = interpolate(density_v001, density_v101, tX);
  float c11 = interpolate(density_v011, density_v111, tX);
  
  float c0 = interpolate(c00, c10, tY);
  float c1 = interpolate(c01, c11, tY);
  
  float interpolated_value = interpolate(c0, c1, tZ);
  
  return interpolated_value;
}

void volumeRaycasting(std::vector<float3> &ray_directions,
                      const int left, const int top, const int right, const int bottom,
                      const Camera& camera)
{
	// TODO: implement the volume raycasting algorithm
	//
	// Cast a ray for every pixel and save it in ray_directions (row-major order).
	// Make sure to normalize ray directions.
	//
	// The cameras's u,v,w vectors can be retrieved with camera.getU(), camera.getV(), camera.getW()
	//

	const unsigned int r_x = camera.getViewport().x;
	const unsigned int r_y = camera.getViewport().y;
	const float f = camera.getFocalLength();
	const float fov = camera.getFOV();

  float h_s = 2*f* tan(fov / 2) ;
  float w_s = h_s * r_x / r_y;

  float3 w = camera.getW();
  float3 u = camera.getU();
  float3 v = camera.getV();


  for (int y = top; y < bottom; ++y) 
  {
      for (int x = left; x < right; ++x) 
      {
        float translated_x = (2 * (x + 0.5f) / r_x - 1) * w_s * 0.5f;
        float translated_y = (1 - 2 * (y + 0.5f) / r_y) * h_s * 0.5f;
        ray_directions[y * r_x + x] = normalize(translated_x * u + translated_y * v - f * w);
      }
  }

}


uchar4 colorPixel(const float3& camera_position, const float3& ray_direction,
 				  const Volume& volume, const float step_size, const float opacity_correction, 
				  const float opacity_threshold, const float3& background_color, const float3& light_color,
				  const float k_ambient, const float k_diffuse, const float k_specular, const float shininess, 
				  const bool shading, const std::vector<float4>& tf_lut)
{
	// TODO: implement color accumulation and (bonus) phong shading
	//
	//  1. Intersect ray with the AABB of volume to determine entry and exit point of the ray.
	//      If the ray does not intersect the AABB: pixel color = background color
	//  2. Front-to-back composition along the ray within the volume with the given stepsize + phong model if shading = true
	//      Test for early-ray-termination.
	//  3. Blend the accumulated color with background color, depending on the used opacity and the opacity threshold
	//	4. Map the colors from float [0,1] to discretized color values {0, ..., 255}
	//  5. Return result color
	//
	// Volume:
	//    - AABB minimum corner position: volume.getAABBMin()
	//    - AABB maximum corner position: volume.getAABBMax()
	//
	float t_near, t_far = 0;

  float3 volume_aabb_min = volume.getAABBMin();
  float3 volume_aabb_max = volume.getAABBMax();

  if (intersectRayWithAABB(camera_position, ray_direction, volume_aabb_min, volume_aabb_max, t_near, t_far)) 
  {
    float3 accumulated_color = {0, 0, 0};
    float accumulated_opacity = 0;

    float3 current_ray_position = camera_position + ray_direction * t_near;

    for (float t = t_near; t < t_far; t += step_size)
    {
      float current_density = interpolatedDensity(volume, current_ray_position);
      current_ray_position += ray_direction * step_size;

      float4 transfer_function_value = lookupTransferFunction(current_density, opacity_correction, tf_lut);
      float3 voxel_lighting = transfer_function_value.xyz();

      accumulated_color += (1 - accumulated_opacity) * transfer_function_value.w * voxel_lighting * light_color;
      accumulated_opacity += (1 - accumulated_opacity) * transfer_function_value.w;

      if (accumulated_opacity >= opacity_threshold) 
      {
        break;
      }
    }

    accumulated_color = math::clamp(accumulated_color, 0.0f, 1.0f);
    accumulated_opacity = math::clamp(accumulated_opacity, 0.0f, 1.0f);

    float4 final_color = {0, 0, 0, 0};

    if (accumulated_opacity < opacity_threshold)
    {
      final_color = TransferFunction::mixColors(accumulated_color, background_color, accumulated_opacity, opacity_threshold - accumulated_opacity);
      accumulated_color = final_color.xyz();
    }

    return uchar4(static_cast<unsigned char>(accumulated_color.x * 255),
                  static_cast<unsigned char>(accumulated_color.y * 255),
                  static_cast<unsigned char>(accumulated_color.z * 255),
                  255);
  }

  // No intersection with the volume
  return uchar4(static_cast<unsigned char>(background_color.x * 255),
                static_cast<unsigned char>(background_color.y * 255),
                static_cast<unsigned char>(background_color.z * 255),
                255);
}


float3 calcGradient(const Volume& volume, const float3& sample_position)
{
	// TODO (Bonus): use the central difference method to calculate the gradient for the sample position
	//
	// Useful functions:
	//  - Volume voxel grid ratio: Volume::getRatio()
	//
	// The Voxel grid ratio defines the spacing between the voxels in x,y,z direction.
	// e.g. ratio.x is the distance between two voxels in x-direction
	//
	// Hint: make use of your implementation of the interpolatedDensity() function.
	// Return the computed gradient.

	return float3(0.f, 0.f, 0.f);
}