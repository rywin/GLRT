#version 430

#define PI 3.1415926535
#define PI2 6.28318530717
#define LAMBERT 0
#define METAL 1
#define DIELECTRIC 2

layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform image2D img_output; // Color Buffer
layout(rgba32f, binding = 1) uniform image2D uid_output; // uID Buffer

uniform sampler2D texture_1_normal;
uniform sampler2D texture_1_diffuse;
uniform sampler2D texture_displacement;

uniform samplerCube cubemap;

vec2 ss_coords = vec2(0.0, 0.0);

struct Camera{
	vec3 position;
	vec3 front;
	vec3 right;
	mat4 view;
};

struct Material {
	int type;
	float rough;
	float refractive_index;
	vec3 color;
	int mat_id;
	int textureID;

};

struct Sphere{
	vec3 center;
	float radius;
	Material mat;
};

struct Plane {
	vec3 u;
	vec3 v;
	vec3 pos;
	vec3 normal;
	Material mat;
	int axis;
};

struct Scene {
	int n_spheres;
	int n_planes;
	int n_bounces;
	Sphere[4] spheres;
	Plane[3] planes;
} scene;


struct Ray{
	vec3 pos;
	vec3 direction;
};


uniform int iterations;
uniform float time;
uniform Camera camera;

uniform Material material_1;
uniform Material material_2;
uniform Material material_3;
uniform Material material_4;
uniform Material material_5;



uniform float refl;
uniform int bounces;
uniform float gamma;


int seed = 2101;

vec2 spherical_coords(in vec3 p) {
	vec2 uv;
	float phi = atan(p.z, p.x);
	float theta = asin(p.y);
	uv.x = 1 - (phi + phi) / (PI2);
	uv.y = (theta + PI / 2) / PI;
	return uv;
}

vec2 get_uv_sphere(inout Sphere s, in vec3 hit_point) {
	return spherical_coords((hit_point - s.center) / s.radius);
}


float hit_sphere(inout Sphere s, inout Ray r, out vec2 uv) {
	vec3 oc = r.pos - s.center;
	float a = dot(r.direction, r.direction);
	float b = 2.0 * dot(oc, r.direction);
	float c = dot(oc, oc) - s.radius * s.radius;
	float discriminant = b * b - 4 * a*c;
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		float t = (-b - sqrt(discriminant)) / (2.0 * a);
		vec3 hit_point = r.direction * t + r.pos;
		uv = get_uv_sphere(s, hit_point);
		return t;

	}
}

float hit_plane(inout Plane p, inout Ray r, out vec2 uv) {
	float t = (p.pos.z - r.pos.z) / r.direction.z;
	float x = r.pos.x + t * r.direction.x - p.pos.x;
	float y = r.pos.y + t * r.direction.y - p.pos.y;

	vec2 extents_x = vec2(-0.5, 0.5) * p.u.x;
	vec2 extents_y = vec2(-0.5, 0.5) * p.v.y;


	if (extents_x.x <= x && x <= extents_x.y && extents_y.x < y && y < extents_y.y) {
		uv = vec2(x, y)  + vec2(0.5);
		return t;
	}

	else {
		return -1.0;
	}

}




float random() {
	//Return pseudo random number
	return fract(sin(dot(ss_coords, vec2(23.21312 * sin(seed), 84.1023)) + seed++) * 42930.53);
}

vec3 random_vector() {
	// Return a vector from random spherical coords
	float theta = random() * PI2;
	float z = random() * 2.0 - 1.0;
	float a = sqrt(1.0 - z * z);
	vec3 vector = vec3(a * cos(theta), a * sin(theta), z);
	return vector * sqrt(random());
}


vec2 random_in_unit_disc(vec3 p) {
	//Return vector from random unit disc
	return vec2(random(), random());
}


bool ray_cast_hit(inout Ray ray, inout Scene scene, out vec3 hit_point, out vec3 normal, out Material material, out int uid, out vec2 uv, out mat3 tbn) {
	float closest = 99999;
	bool hit = false;
	bool hit_this_bounce = false;
	uid = -1;

	vec3 pos;
	vec3 direction;

	for (int i = 0; i < scene.n_spheres; i++) { // iterate over spheres
		vec2 g_uv;

		float t = hit_sphere(scene.spheres[i], ray, g_uv);

		if (t > 0.0) { // ray has hit primitive
			hit = true;
			if (closest < t) // not the closest hit so skip
			{
				continue;
			}
			else {

				uv = g_uv;
				closest = t;
				hit_point = ray.direction * t + ray.pos;

				//get_uv_sphere(scene.spheres[i], hit_point, uv);

				normal = normalize(hit_point - scene.spheres[i].center);
				vec3 _t = normalize(cross(normal, vec3(0.0, 1.0, 0.0)));
				vec3 _b = normalize(cross( normal, _t));
				tbn = mat3(_t, _b, normal);
				//normal = texture(texture_1_normal, uv).xyz;
				material = scene.spheres[i].mat;
				uid = scene.spheres[i].mat.mat_id;
			}
		}

	}

	for (int i = 0; i < scene.n_planes; i++) { // iterate over spheres
		Plane plane = scene.planes[i];
		vec2 g_uv;
		
		float t = hit_plane(scene.planes[i], ray, g_uv);

		if (t > 0.0) { // ray has hit primitive
			hit = true;
			if (closest < t) // not the closest hit so skip
			{
				continue;
			}
			else {
				uv = g_uv;
				closest = t;
				hit_point = ray.direction * t + ray.pos;
				normal = scene.planes[i].normal;
				//normal = cross(plane.u, plane.v);
				//normal = tbn * (texture(texture_1_normal, uv).xyz - 0.5) * 2.0;
				if (dot(ray.direction, normal) > 0) {
					normal = -normal;
				}
				material = scene.planes[i].mat;
				uid = scene.planes[i].mat.mat_id;
				tbn = mat3(1.0);
			}
		}

	}
	return hit;

}

vec3 trace(inout Ray ray, inout Scene scene) {
	Material material;
	vec3 normal = vec3(0.0);
	vec3 hit_point = vec3(0.0);

	vec3 pixel = vec3(0.0);
	vec3 frac = vec3(1.0);

	float at = 0.8;

	int uid;
	vec2 uv;
	mat3 tbn;
	
	for (int n = 0; n < scene.n_bounces; n++) {
		if (ray_cast_hit(ray, scene, hit_point, normal, material, uid, uv, tbn)) {
			if (n == 0) {
				if (material.textureID == 0) {
					pixel =  texture(texture_1_diffuse, uv).xyz * material.color;
				}
				else {
					pixel = material.color;
				}
			}
			if (material.textureID == 0){
				//normal =  (texture(texture_1_normal, uv).xyz - 0.5) * 2.0;
				normal = tbn * (texture(texture_1_normal, uv).xyz - 0.5) * 2.0 ;
			}

			vec3 reflected = reflect(ray.direction, normal);
			if (material.textureID == 0){
				pixel *= frac * texture(texture_1_diffuse, uv).xyz * material.color;
			}
			else{
				pixel *= frac * material.color;
			}

			//ray.direction = reflected;
			if (material.type == LAMBERT) {
				ray.direction =  random_vector() + (reflected + normal) / 2.0;
				frac *= refl;
			}else if (material.type == METAL) {
				ray.direction = material.rough * random_vector() + reflected;
				frac *= refl;
			}
			else {
				ray.direction = refract(ray.direction, normal, material.refractive_index);
			}
			//pixel = (normal + vec3(1.0)) / 2.0;
			//pixel = vec3(uv, 1.0);
			//break;

			ray.pos = hit_point;
		}
		else {
			if (n == 0) {
				pixel = texture(cubemap, ray.direction).rgb;
			}
			else {
				pixel *= frac  * texture(cubemap, ray.direction).rgb;

			}
			break;
		}
	}
	return pixel;
}


int get_uid(inout Ray ray, inout Scene scene) {
	int uid;
	uid = -1;
	vec3 hit_point;
	vec3 normal;
	Material material;
	vec2 uv;
	mat3 tbn;

	ray_cast_hit(ray, scene, hit_point, normal, material, uid, uv, tbn);
	return uid;
}

void main() {
	// base pixel colour for image
	vec4 pixel = vec4(0.0, 0.0, 0.0, 1.0);
	int uid;

	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	ivec2 dims = imageSize(img_output);
	float aspect = float(dims.x) / float(dims.y);

	float x = (float(pixel_coords.x * 2 - dims.x) / dims.x);
	float y = (float(pixel_coords.y * 2 - dims.y) / dims.y);


	ss_coords = vec2(x, y) * time;

	Ray ray;
	ray.pos =  camera.position;

	// Do random subpixel sampling for AA.
	vec2 pixel_size = vec2(1.0/ dims.x, 1.0/ dims.y) * 2;
	vec2 noise =  random_in_unit_disc(vec3(ss_coords,1.0) *300.0) * pixel_size;

	ray.direction = camera.front + (-x * aspect + noise.x) * normalize(camera.right) + (y + noise.y) * normalize(cross(camera.front, camera.right));

	Sphere 
	s_1;
	s_1.center = vec3(0.0, 0.0, -3.0);
	s_1.radius = 0.4;
	s_1.mat = material_1;

	Sphere s_2;
	s_2.center = vec3(-1.0, 0.0, -3.0);
	s_2.radius = 0.4;
	s_2.mat = material_2;

	Sphere s_3;
	s_3.center = vec3(1.0, 0.0, -3.0);
	s_3.radius = 0.4;
	s_3.mat = material_3;

	Sphere s_4;
	s_4.center = vec3(1.0, -100.0 -0.5, -3.0);
	s_4.radius = 100.0;
	s_4.mat = material_4;

	scene.n_bounces = bounces;
	scene.n_spheres = 4;
	scene.spheres[0] = s_1;
	scene.spheres[1] = s_2;
	scene.spheres[2] = s_3;
	scene.spheres[3] = s_4;

	Plane p_1;

	p_1.u = vec3(10.0, 0.0, 0.0);
	p_1.v = vec3(0.0, 5.0, 0.0);
	p_1.pos = vec3(0.0, 0.0,-4.0);
	p_1.normal = vec3(0.0, 0.0, 1.0);
	p_1.mat = material_5;

	Plane p_2;
	p_2.pos = vec3(0.0, 0.0,-2.0);
	p_2.normal = vec3(0.0, 0.0, 1.0);
	p_2.mat = material_5;
	
	scene.n_planes = 1;
	scene.planes[0] = p_1;
	scene.planes[1] = p_2;

	uid = get_uid(ray, scene);
	
	if (iterations == 0) {
		vec3 traced_pixel = trace(ray, scene);
		pixel.rgb = pow(traced_pixel, vec3(1.0 / gamma));
	}
	else {
		pixel = imageLoad(img_output, pixel_coords);
		vec3 traced_pixel = trace(ray, scene);
		pixel.rgb += (pow(traced_pixel, vec3(1.0/gamma)) - pixel.xyz) / iterations;

	}

	imageStore(img_output, pixel_coords, pixel);
	imageStore(uid_output, pixel_coords, vec4(uid, vec2((uid + 2.0) * 20 / 255.0), 1.0));

}