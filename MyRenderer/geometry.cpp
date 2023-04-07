#include "geometry.h"

//vec3 cross(const vec3& v1, const vec3& v2)
//{
//	return vec<3>{v1.y* v2.z - v1.z * v2.y, v1.z* v2.x - v1.x * v2.z, v1.x* v2.y - v1.y * v2.x};
//}

double inverseSqrt(double x) {
	double xhalf = 0.5 * x;
	uint64_t i = *(uint64_t*)&x; // 将双精度浮点数转化为整型
	i = 0x5fe6ec85e7de30da - (i >> 1); // 使用魔数估算初始值
	x = *(double*)&i; // 将整型转化为双精度浮点数
	x *= (1.5 - xhalf * x * x); // 牛顿迭代法进行精细调整
	return x;
}