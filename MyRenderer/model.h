#pragma once
#include <vector>
#include <string>
#include "geometry.h"
#include "tgaimage.h"

//class Model
//{
//	std::vector<vec3> verts{};				//arrays of vertices
//	std::vector<vec2> tex_coord{};			//per-vertex array of tex coords
//	std::vector<vec3> norms{};				//per-vertex array of normal vectors
//	std::vector<int> facet_vrt{};
//	std::vector<int> facet_tex{};			//per-triangle indices in the above arrays
//	std::vector<int> facet_nrm{};
//	TGAImage diffuseMap{};					//diffuse color texture
//	TGAImage normalMap{};					//normal map texture
//	TGAImage speculatMap{};					//specular map texture
//	void load_texture(const std::string filename, const std::string suffix, TGAImage& img);
//public:
//	Model(const std::string filename);
//	~Model() {}
//	int nverts() const;
//	int nfaces() const;
//	vec3 normal(const int iface, const int nthvert) const;		//per triangle corner normal vertex
//	vec3 normal(const vec2& uvf) const;							//fetch the normal vector from the normal map texture
//	vec3 vert(const int i) const;
//	vec3 vert(const int iface, const int nthvert) const;
//	vec2 uv(const int iface, const int nthvert) const;
//	const TGAImage& diffuse() const { return diffuseMap; }
//	const TGAImage& specular() const { return speculatMap; }
//};
class Model {
private:
	std::vector<Vec3f> verts_;
	std::vector<std::vector<int> > faces_;
public:
	Model(const char* filename);
	~Model();
	int nverts();
	int nfaces();
	Vec3f vert(int i);
	std::vector<int> face(int idx);
};