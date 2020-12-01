#pragma once
#include "Glados/Renderer/Buffer.h"

namespace Glados {

	class OpenGLIndexBuffer : public IndexBuffer
	{
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override;
	};


	class OpenGLVertexBuffer : public VertexBuffer
	{
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
		Ref<IndexBuffer> m_IndexBuffer;
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t offset, uint32_t size) override;
		virtual float GetData(uint32_t offset, uint32_t size) const override;

		virtual void SetLayout(const BufferLayout& layout) override;
		virtual BufferLayout GetLayout() const override;
	};


	class OpenGLUniformBuffer : public UniformBuffer
	{
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	public:
		OpenGLUniformBuffer(uint32_t size);
		OpenGLUniformBuffer(float* vertices, uint32_t size);

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t offset, unsigned int size) override;
		virtual float GetData(uint32_t offset, uint32_t size) const override;

		virtual void SetLayout(const BufferLayout& layout) override;
		virtual BufferLayout GetLayout() const override;

		virtual void BindUniformBlock(const std::string& name, Ref<Shader> shader) override;
	};

}