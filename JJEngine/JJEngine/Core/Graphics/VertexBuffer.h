/* Start Header -------------------------------------------------------
Project: JJEngine
Members: Junwoo Seo, Junsu Jang, Dahyeon Kim
Platform: x64
Date: 12/20/2022
End Header-------------------------------------------------------- */
#pragma once
#include <memory>
#include <vector>

enum class DataType
{
	None = 0, Bool, Int, Int2, Int3, Int4, Float, Float2, Float3, Float4, Mat3, Mat4,
};

struct DataAndLayoutLocation
{
	DataType m_DataType;
	unsigned int m_LayoutLocation;
	unsigned int m_Size;
	unsigned int m_ElementCount;
	unsigned int m_Offset;
	bool m_Normalize;

	DataAndLayoutLocation() = delete;
	DataAndLayoutLocation(unsigned int layout_location, DataType data, bool normalize = false);
	unsigned ShaderDataTypeToOpenGLBaseType() const;
};

class DescribedData
{
public:
	DescribedData() :m_Stride(0) {}
	DescribedData(const std::initializer_list<DataAndLayoutLocation>& described_data);

	unsigned int GetStride() const { return m_Stride; }

	std::vector<DataAndLayoutLocation>::iterator begin() { return m_DescribedData.begin(); }
	std::vector<DataAndLayoutLocation>::iterator end() { return m_DescribedData.end(); }
	std::vector<DataAndLayoutLocation>::const_iterator begin() const { return m_DescribedData.cbegin(); }
	std::vector<DataAndLayoutLocation>::const_iterator end() const { return m_DescribedData.cend(); }
private:
	std::vector<DataAndLayoutLocation> m_DescribedData;
	unsigned int m_Stride;
};

class VertexBuffer
{
public:
	VertexBuffer() = delete;
	~VertexBuffer();
	static std::shared_ptr<VertexBuffer> CreateVertexBuffer(int byte_size);
	void Bind() const;
	void UnBind() const;
	void SetData(int size, const void* data, unsigned offset=0);
private:
	
	VertexBuffer(int byte_size);
	void CreateBuffer(int size, const void* data);
	unsigned m_Buffer;
};
