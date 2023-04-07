#include <iostream>
#include <sstream>
#include "model.h"

//Model::Model(const std::string filename)
//{
//	std::ifstream in;
//	in.open(filename, std::ifstream::in);
//	if (in.fail()) return;
//	std::string line;
//	while (!in.eof())
//	{
//		std::getline(in, line);
//		std::istringstream iss(line.c_str());
//		char trash;
//		if (!line.compare(0, 2, "v "))
//		{
//			iss >> trash;
//			vec3 v;
//			for (int i = 0; i < 3; i++) iss >> v[i];
//			verts.emplace_back(v);
//		}
//		else if (!line.compare(0, 3, "vn "))
//		{
//			iss >> trash >> trash;
//			vec3 n;
//			for (int i = 0; i < 3; i++) iss >> n[i];
//			norms.emplace_back(n.normalize());
//		}
//		else if (!line.compare(0, 3, "vt "))
//		{
//			iss >> trash >> trash;
//			vec2 uv;
//			for (int i = 0; i < 2; i++) iss >> uv[i];
//			tex_coord.push_back({ uv.x,1 - uv.y });
//		}
//		else if (!line.compare(0, 2, "f"))
//		{
//			int f, t, n;
//			iss >> trash;
//			int cnt = 0;
//			while (iss>>f>>trash>>t>>trash>>n)
//			{
//				facet_vrt.emplace_back(--f);
//				facet_tex.emplace_back(--t);
//				facet_nrm.emplace_back(--n);
//			}
//			if (3 != cnt)
//			{
//				std::cerr << "Error: the obj file is supposed to be triangulated" << std::endl;
//				in.close();
//				return;
//			}
//		}
//	}
//	in.close();
//	std::cerr << "# v# " << nverts() << " f# " << nfaces() << " vt# " << tex_coord.size() << " vn# " << norms.size() << std::endl;
//	load_texture(filename, "_diffuse.tga", diffuseMap);
//	load_texture(filename, "_nm_tangent.tga", normalMap);
//	load_texture(filename, "_spec.tga", speculatMap);
//}
//
//int Model::nverts() const
//{
//	return verts.size();
//}
//
//int Model::nfaces() const
//{
//	return facet_vrt.size() / 3;
//}
//
//vec3 Model::vert(const int i) const
//{
//	return verts[i];
//}
//
//vec3 Model::vert(const int iface, const int nthvert) const
//{
//	return verts[facet_vrt[iface * 3 + nthvert]];
//}
//
//void Model::load_texture(const std::string filename, const std::string suffix, TGAImage& img)
//{
//	size_t dot = filename.find_last_of(".");
//	if(dot == std::string::npos) return;
//	std::string texFile = filename.substr(0, dot) + suffix;
//	std::cerr << "texture file " << texFile << "loading" << (img.read_tga_file(texFile.c_str()) ? "ok" : "failed") << std::endl;
//}
//
//vec3 Model::normal(const vec2& uvf) const
//{
//	TGAColor c = normalMap.get(uvf[0] * normalMap.width(), uvf[1] * normalMap.height());
//	return vec3{ (double)c[2],(double)c[1],(double)c[0] }*2. / 255. - vec3{ 1,1,1 };
//}
//
//vec2 Model::uv(const int iface, const int nthvert) const
//{
//	return tex_coord[facet_tex[iface * 3 + nthvert]];
//}
//
//vec3 Model::normal(const int iface, const int nthvert) const
//{
//	return norms[facet_nrm[iface * 3 + nthvert]];
//}

Model::Model(const char* filename) : verts_(), faces_() {
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail()) return;
	std::string line;
	while (!in.eof()) {
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;
			Vec3f v;
			for (int i = 0; i < 3; i++) iss >> v.raw[i];
			verts_.push_back(v);
		}
		else if (!line.compare(0, 2, "f ")) {
			std::vector<int> f;
			int itrash, idx;
			iss >> trash;
			while (iss >> idx >> trash >> itrash >> trash >> itrash) {
				idx--; // in wavefront obj all indices start at 1, not zero
				f.push_back(idx);
			}
			faces_.push_back(f);
		}
	}
	std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
	return (int)verts_.size();
}

int Model::nfaces() {
	return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
	return faces_[idx];
}

Vec3f Model::vert(int i) {
	return verts_[i];
}