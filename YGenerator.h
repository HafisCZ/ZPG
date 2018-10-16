#pragma once

#include <random>

class YGenerator {
	private:
		const float ROUGHNESS = 0.3f;
		const int OCTAVES = 3;

		const float m_amplitude;
		unsigned int m_seed;

	public:
		YGenerator(float amplitude) : m_amplitude(amplitude), m_seed(std::random_device()()) {}

		float generate(int x, int z);

	private:
		float interpolateNoise(float x, float z);
		float interpolate(float a, float b, float blend);
		float smoothNoise(int x, int z);
		float noise(int x, int z);
};