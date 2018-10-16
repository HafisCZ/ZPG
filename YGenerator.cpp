#include "YGenerator.h"

float YGenerator::generate(int x, int z) {
	float result = 0.0f;
	float d = pow(2.0f, OCTAVES - 1);

	for (int i = 0; i < OCTAVES; i++) {
		float freq = pow(2.0f, i) / d;
		float ampl = pow(ROUGHNESS, i) * m_amplitude;

		result += interpolateNoise(x * freq, z * freq) * ampl;
	}

	return result;
}

float YGenerator::interpolateNoise(float x, float z) {
	int ix = (int)x;
	int iz = (int)z;
	float fx = x - ix;
	float fz = z - iz;

	float a = smoothNoise(ix, iz);
	float b = smoothNoise(ix + 1, iz);
	float c = smoothNoise(ix, iz + 1);
	float d = smoothNoise(ix + 1, iz + 1);

	float i = interpolate(a, b, fx);
	float j = interpolate(c, d, fx);

	return interpolate(i, j, fz);
}

float YGenerator::interpolate(float a, float b, float blend) {
	float f = (1.0f - cos(blend * (float)std::_Pi)) * 0.5f;
	return a * (1.0f - f) + b * f;
}

float YGenerator::smoothNoise(int x, int z) {
	float a = (noise(x - 1, z - 1) + noise(x + 1, z + 1) + noise(x - 1, z + 1) + noise(x + 1, z - 1)) / 16.0f;
	float b = (noise(x - 1, z) + noise(x + 1, z) + noise(x, z + 1) + noise(x, z - 1)) / 8.0f;
	float c = (noise(x, z)) / 4.0f;

	return a + b + c;
}

float YGenerator::noise(int x, int z) {
	static std::mt19937 mersenne;
	static std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

	mersenne.seed(x * 123 + z * 543 + m_seed);
	return distribution(mersenne);
}