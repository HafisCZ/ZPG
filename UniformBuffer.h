#pragma once

#include "Buffer.h"

class Alignment {
	public:
		static inline const unsigned int alignto(unsigned int size, unsigned int align = 16) {
			return size % align == 0 ? size : size + align - size % align;
		}
};

class UniformLayout {
	private:
		struct BlockItem {
			unsigned int index;
			unsigned int stride;
			unsigned int size;
		};

		struct Block : public BlockItem {
			std::vector<BlockItem> items;

			Block(unsigned int index, unsigned int stride, unsigned int size) : BlockItem{ index, stride, size } {}

			inline unsigned int blockSize() { return size; }
			inline unsigned int blockStride(unsigned int offset = 0) { return stride + items[offset].stride; }
			inline unsigned int itemSize(unsigned int index) { return items[index].size; }
		};

		std::vector<BlockItem> _items;
		std::vector<Block> _blocks;

		unsigned int _size;
		unsigned int _istr;
		int _step;

	public:
		UniformLayout();

		template <typename T> void addBlock(unsigned int count) {
			unsigned int bytes = sizeof(T);

			for (unsigned int i = 0; i < count; i++) {
				_items.push_back({ _items.size(), _istr, bytes });
				_istr += Alignment::alignto(bytes);
			}
		}

		void pack();

		template <typename T> void pack() {
			addBlock<T>(1);
			pack();
		}

		inline const std::vector<Block>& getBlocks() { return _blocks; }
		inline unsigned int getSize() { return _size; }
};

class UniformBuffer : public Buffer {
	private:
		UniformLayout _layout;

	public:
		UniformBuffer(UniformLayout& layout);
		~UniformBuffer();

		template <typename T> void setUniformBlock(unsigned int index, const T& a) {
			glBufferSubData(GL_UNIFORM_BUFFER, _layout.getBlocks()[index].blockStride(), _layout.getBlocks()[index].blockSize(), &a);
		}

		template <typename T> void setUniformBlock(unsigned int index, unsigned int position, const T& a) {
			glBufferSubData(GL_UNIFORM_BUFFER, _layout.getBlocks()[index].blockStride(position), _layout.getBlocks()[index].itemSize(position), &a);
		}

		virtual void bind() override;
		virtual void unbind() override;
};


