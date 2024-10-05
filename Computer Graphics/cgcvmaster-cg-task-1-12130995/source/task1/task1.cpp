

#include <limits>

#include "Scene.h"

#include "task1.h"

constexpr float epsilon = 0.001f;
float3 recursiveRay(const float3 &origin, const float3 &direction, const Scene &scene,
                const Pointlight *lights, std::size_t num_lights,
                const float3 &background_color, int max_bounces);

bool intersectRayPlane(const float3 &p, const float3 &d, const float4 &plane, float &t)
{
  // TODO implement the intersection test between a ray and a plane.
  //
  // A plane is defined by the plane normal n and the offset w along the normal.
  // The plane is given as parameter plane where (plane.x, plane.y, plane.z) represents the plane normal
  // and plane.w is the offset w.
  //
  // If there is no intersection (Hint: or one we do not care about), return false.
  // Otherwise, compute and set the parameter t such that p + t * d yields the intersection point and return true.

  float3 n = {plane.x, plane.y, plane.z};
  float denominator = math::dot(d, n);

  if (std::abs(denominator) == 0) // Check if the ray is parallel to the plane
  {
    return false;
  }

  float temp_t = (plane.w - math::dot(p, n)) / denominator;

  if(temp_t <= 0)
  {
    return false;
  }

  t = temp_t;
  return true;
}

bool intersectsRayPlane(const float3 &p, const float3 &d, const Plane *planes, std::size_t num_planes, float t_min, float t_max)
{
  // TODO: implement intersection test between a ray and a set of planes.
  // This method only has to detect whether there is an intersection with ANY
  // plane along the given subset of the ray. The ray is given by its start
  // point p and direction d.
  // A plane is defined by the plane normal n and the offset w along the normal.
  // Each plane in planes contains a float4 member p where the plane normal n is
  // (p.x, p.y, p.z) and w is p.w.
  // If an intersection is found that falls on a point on the ray
  // between t_min and t_max, return true. Otherwise, return fals


  for (size_t i = 0; i < num_planes; i++)
  {
    float t = 0;
    if (intersectRayPlane(p, d, (float4 &) planes[i], t) && t >= t_min && t <= t_max)
    {
      return true;
    }
  }

  return false;
}




const Plane *findClosestHitPlanes(const float3 &p, const float3 &d, const Plane *planes, std::size_t num_planes, float &t)
{
  // TODO: implement intersection test between a ray and a set of planes.
  // This function should find the CLOSEST intersection with a plane along
  // the ray. The ray is given by its start point p and direction d.
  // A plane is defined by the plane normal n and the offset w along the normal.
  // Each plane in planes contains a float4 member p where the plane normal n is
  // (p.x, p.y, p.z) and w is p.w.
  // If an intersection is found, set t to the ray parameter and
  // return a pointer to the hit plane.
  // If no intersection is found, return nullptr.


  const Plane *closest_plane = nullptr;
  float closest_t = std::numeric_limits<float>::infinity();

  for (size_t i = 0; i < num_planes; i++)
  {
    float temp_t = 0;
    if (intersectRayPlane(p, d, (float4 &) planes[i], temp_t) && temp_t < closest_t && temp_t > 0)
    {
      closest_t = temp_t;
      closest_plane = planes + i;
    }
  }

  if(!closest_plane)
  {
    return nullptr;
  }

  t = closest_t;
  return closest_plane;
}


bool intersectRayTriangle(const float3 &p, const float3 &d, const float3 &p1,
                          const float3 &p2, const float3 &p3, float &t,
                          float &lambda_1, float &lambda_2)
{
  // TODO implement the intersection test between a ray and a triangle.
  //
  // The triangle is defined by the three vertices p1, p2 and p3
  //
  // If there is no intersection return false.
  // Otherwise, compute and set the parameters lambda_1 and lambda_2
  // to the barycentric coordinates corresponding to the
  // closest point of intersection
  float3 e1 = p2 - p1;
  float3 e2 = p3 - p1;
  float3 q = p - p1;

  float denominator = math::dot(math::cross(d, e2), e1);
  if(denominator == 0) 
  {
    return false;
  }

  float temp_t = math::dot(math::cross(q, e1), e2) /  denominator;      //tempss
  float t_lambda_1 = math::dot(math::cross(d, e2), q) /  denominator;
  float t_lambda_2 = math::dot(math::cross(q, e1), d) /  denominator;

  if (t_lambda_1 >= 0 && t_lambda_2 >= 0 && t_lambda_1 + t_lambda_2 <= 1)
  {
    t = temp_t;
    lambda_1 = t_lambda_1;
    lambda_2 = t_lambda_2;
    return true;
  }  

  return false;
}

const Triangle *findClosestHitTriangles(const float3 &p, const float3 &d,
                               const Triangle *triangles,
                               std::size_t num_triangles,
                               const float3 *vertices, float &t,
                               float &lambda_1, float &lambda_2)
{
  // TODO: implement intersection test between a ray and a set of triangles.
  // This function should find the CLOSEST intersection with a triangle along
  // the ray. The ray is given by its start point p and direction d. A triangle
  // is represented as an array of three vertex indices. The position of each
  // vertex can be looked up from the vertex array via the vertex index.
  // triangles points to the first element of an array of num_triangles
  // triangles. If an intersection is found, set t to the ray parameter and
  // lambda_1 and lambda_2 to the barycentric coordinates corresponding to the
  // closest point of intersection, and return a pointer to the hit triangle.
  // If no intersection is found, return nullptr.

  float closest_t = std::numeric_limits<float>::infinity();    
  const Triangle* closest_triangle =  nullptr;
  float f_temp_t, f_lambda1, f_lambda2 = 0; 

  for(size_t i = 0; i < num_triangles; i++)
  {
    float t_temp, t_lambda1, t_lambda2 = 0;
    const Triangle& triangle =  triangles[i];
    if(intersectRayTriangle(p, d, vertices[triangle[0]],vertices[triangle[1]], vertices[triangle[2]],
    t_temp, t_lambda1, t_lambda2)&& t_temp < closest_t && t_temp > 0)
    {
      f_temp_t = t_temp;
      f_lambda1 = t_lambda1;
      f_lambda2 = t_lambda2;
      closest_triangle = &triangles[i];
      closest_t = t_temp;
    }
  }

  if(closest_triangle)
  {
    t = f_temp_t;
    lambda_1 = f_lambda1;
    lambda_2 = f_lambda2;
    return  closest_triangle;
  }

  return nullptr;
}

bool intersectsRayTriangle(const float3 &p, const float3 &d,
                           const Triangle *triangles, std::size_t num_triangles,
                           const float3 *vertices, float t_min, float t_max)
{
  // TODO: implement intersection test between a ray and a set of triangles.
  // This method only has to detect whether there is an intersection with ANY
  // triangle along the given subset of the ray. The ray is given by its start
  // point p and direction d. A triangle is represented as an array of three
  // vertex indices. The position of each vertex can be looked up from the
  // vertex array via the vertex index. triangles points to an array of
  // num_triangles. If an intersection is found that falls on a point on the ray
  // between t_min and t_max, return true. Otherwise, return false.

  for (std::size_t i = 0; i < num_triangles; ++i) 
  {
    const Triangle& triangle = triangles[i];
    const float3& p1 = vertices[triangle[0]];
    const float3& p2 = vertices[triangle[1]];
    const float3& p3 = vertices[triangle[2]];

    float t, lambda_1, lambda_2 = 0;
    if (intersectRayTriangle(p, d, p1, p2, p3, t, lambda_1, lambda_2)) 
    {
      if (t >= t_min && t <= t_max) 
      {
        return true;
      }
    }
  }

  return false;
}

float3 shade(const float3 &p, const float3 &d, const HitPoint &hit,
             const Scene &scene, const Pointlight *lights,
             std::size_t num_lights)
{
  // TODO: implement phong shading.
  // hit represents the surface point to be shaded.
  // hit.position, hit.normal, and hit.k_d and hit.k_s contain the position,
  // surface normal, and diffuse and specular reflection coefficients,
  // hit.m the specular power.
  // lights is a pointer to the first element of an array of num_lights
  // point light sources to consider.
  // Each light contains a member to give its position and color.
  // Return the shaded color.

  // To implement shadows, use scene.intersectsRay(p, d, t_min, t_max) to test
  // whether a ray given by start point p and direction d intersects any
  // object on the section between t_min and t_max.

  float3 zero_vector = {0.0f, 0.0f, 0.0f};
  float3 final_color = zero_vector;

  for (int light_index = 0; light_index < num_lights; light_index++)
  {
    float3 light_direction = normalize(lights[light_index].position - hit.position); //l
    float3 surface_normal = (hit.normal); //n
    float cos_theta = fmax(dot(surface_normal, light_direction), 0.0f); 
    
    float3 reflected_direction = normalize(2.0f * surface_normal * dot(surface_normal, light_direction) - light_direction); //r 
    float3 view_direction = normalize(-d); //v
    float cos_alpha = fmax(dot(reflected_direction, view_direction), 0.0f);

    float3 diffuse_component = {hit.k_d.x * cos_theta, hit.k_d.y * cos_theta, hit.k_d.z * cos_theta};

    float3 specular_component = zero_vector;
    if(cos_theta > 0)
    {
      specular_component = {hit.k_s.x * powf(cos_alpha, hit.m), hit.k_s.y * powf(cos_alpha, hit.m), hit.k_s.z * powf(cos_alpha, hit.m)};
    }

    float3 epsilon_point = hit.position + epsilon * surface_normal;
    bool is_shadowed = scene.intersectsRay(epsilon_point, light_direction, 0.0f, length(lights[light_index].position - hit.position));

    if (!is_shadowed)
    {
      final_color += {(specular_component.x + diffuse_component.x) * lights[light_index].color.x,
      (specular_component.y + diffuse_component.y) * lights[light_index].color.y, 
      (specular_component.z + diffuse_component.z) * lights[light_index].color.z};
    }
  }   
  return final_color;
}

void render(image2D<float3> &framebuffer, int left, int top, int right,
            int bottom, const Scene &scene, const Camera &camera,
            const Pointlight *lights, std::size_t num_lights,
            const float3 &background_color, int max_bounces)
{
  // TODO: implement raytracing, render the given portion of the framebuffer.
  // left, top, right, and bottom specify the part of the image to compute
  // (inclusive left, top and exclusive right and bottom).
  // camera.eye, camera.lookat, and camera.up specify the position and
  // orientation of the camera, camera.w_s the width of the image plane,
  // and camera.f the focal length to use.
  // Use scene.findClosestHit(p, d) to find the closest point where the ray
  // hits an object.
  // The method returns an std::optional<HitPoint>.
  // If an object is hit, call the function shade to compute the color value
  // of the HitPoint illuminated by the given array of lights.
  // If the ray does not hit an object, use background_color.

  // BONUS: extend your implementation to recursive ray tracing.
  // max_bounces specifies the maximum number of secondary rays to trace.
  //influenced by my attempt from last year


  // Compute camera directions
  float3 w = math::normalize(camera.lookat - camera.eye);
  float3 u = math::normalize(math::cross(camera.up, w));
  float3 v = math::cross(w, u);

  float aspect_ratio = static_cast<float>(width(framebuffer)) / height(framebuffer);

  for (int y = top; y < bottom; ++y)
  {
    for (int x = left; x < right; ++x)
    {
      float u_dis = camera.w_s * ((((width(framebuffer) - x ) - 0.5) / width(framebuffer)) - 0.5); 
      float v_dis = camera.w_s * (((height(framebuffer) - (y + 0.5)) / height(framebuffer)) - 0.5) / aspect_ratio;

      float3 d = math::normalize(camera.f * w + u * u_dis + v * v_dis);

      auto hit = scene.findClosestHit(camera.eye, d);

      if (hit.has_value())
      {
        framebuffer(x, y) = shade(hit->position, d, *hit, scene, lights, num_lights);

        if(std::isinf(hit->m))
        {
          framebuffer(x, y) = recursiveRay(camera.eye, d, scene, lights, num_lights, background_color, max_bounces);
        }
      }
      else
      {
        framebuffer(x, y) = background_color;
      }
    }
  }
}



float3 recursiveRay(const float3 &p, const float3 &d, const Scene &scene, const Pointlight *lights, std::size_t num_lights,
                    const float3 &background_color, int max_bounces)
{
  if (max_bounces <= 0)
  {
    return background_color;
  }

  auto hit = scene.findClosestHit(p, d);
  
  if (hit.has_value())
  {
    if (hit->m == std::numeric_limits<float>::infinity())
    {
      float3 n = hit->normal;
      //https://paulbourke.net/geometry/reflected/
      float3 reflected_direction = math::normalize(d - 2.0f * dot(d, hit->normal) * hit->normal);

      float3 offset_origin = hit->position + epsilon * hit->normal;

      float3 reflected_color = recursiveRay(offset_origin, reflected_direction, scene, lights, num_lights, 
                                            background_color, --max_bounces);

      return { reflected_color.x * hit->k_s.x, reflected_color.y * hit->k_s.y, reflected_color.z * hit->k_s.z};
    }

    else
    {
      return shade(hit->position, d, *hit, scene, lights, num_lights);
    }
  }

  else
  {
    return background_color;
  }
}



//docker run --init -v $PWD:/cg1 -it cg1-runner ./scripts/run_all_tests.sh