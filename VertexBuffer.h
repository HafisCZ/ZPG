#pragma once

#include "Buffer.h"

class VertexLayout {
	private:
		struct Block {
			unsigned int type;
			unsigned int count;
			unsigned int size;
			bool normalized;
		};

		unsigned int _stride;
		std::vector<Block> _blocks;

	public:
		VertexLayout() : _stride(0) {}

		template<typename T> void addBlock(unsigned int count) {
			static_assert(false);
		}

		template<> void addBlock<float>(unsigned int count) {
			_blocks.push_back({ GL_FLOAT, count, count * 4, false });
			_stride += count * 4;
		}

		template<typename T> void addBlocks(unsigned int count) {
			addBlock<T>(count);
		}

		template<typename T, typename ... V> void addBlocks(unsigned int count, V ... counts) {
			addBlock<T>(count);
			addBlocks<T>(counts ...);
		}

		inline std::vector<Block>& getBlocks() { return _blocks; }
		inline unsigned int getStride() { return _stride; }

};

class VertexBuffer : public Buffer {
	private:
		unsigned int _count;
		VertexLayout _layout;

	public:
		VertexBuffer(void_cptr data, unsigned int count, VertexLayout layout);
		~VertexBuffer();

		virtual void bind() override;
		virtual void unbind() override;

		inline VertexLayout& getLayout() { return _layout; }
		inline unsigned int getCount() { return _count; }
};