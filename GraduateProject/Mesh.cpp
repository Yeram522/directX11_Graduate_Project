#include "Mesh.h"



Mesh::Mesh()
{
	m_Texture = 0;
	m_model = 0;
	m_textureCount = 0;
	m_normalCount = 0;
	m_faceCount = 0;
}

Mesh::Mesh(vector<Vertex>, vector<unsigned int>, vector<Texture>)
{
}


ID3D11ShaderResourceView* Mesh::GetTexture()
{
	return m_Texture->GetTexture();
}


int Mesh::GetIndexCount()
{
	return m_indexCount;
}


bool Mesh::LoadTexture(ID3D11Device* device, const WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}


void Mesh::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}




bool Mesh::LoadModel(const WCHAR* filename)
{
	ReadFileCounts(filename);


	return true;
}


void Mesh::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

	return;
}


bool Mesh::InitializeBuffers(ID3D11Device* device)
{
	
	Vertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create the vertex array.
	vertices = new Vertex[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void Mesh::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void Mesh::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(Vertex);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool Mesh::ReadFileCounts(const WCHAR* filename)
{
	ifstream fin;
	char input;
	// Initialize the counts.
	int vertexCount = 0;
	int textureCount = 0;
	int normalCount = 0;
	int faceCount = 0;
	// Open the file.
	fin.open(filename);
	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}
	// Read from the file and continue to read until the end of the file is reached.
	fin.get(input);
	while (!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		// Otherwise read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
			if (fin.eof())
				break;
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}
	// Close the file.
	fin.close();

	LoadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount);

	return true;
}

bool Mesh::LoadDataStructures(const WCHAR* filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	//VertexType
	XMFLOAT3* m_vertices;
	XMFLOAT3* m_textureCoords;
	XMFLOAT3* m_normals;

	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;

	// Initialize the four data structures.
	m_vertices = new XMFLOAT3[vertexCount];
	if (!m_vertices)
	{
		return false;
	}

	m_textureCoords = new XMFLOAT3[textureCount];
	if (!m_textureCoords)
	{
		return false;
	}

	m_normals = new XMFLOAT3[normalCount];
	if (!m_normals)
	{
		return false;
	}

	m_faces = new Face[faceCount];
	if (!m_faces)
	{
		return false;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if (fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if (input == ' ')
			{
				fin >> m_vertices[vertexIndex].x >> m_vertices[vertexIndex].y >>
					m_vertices[vertexIndex].z;

				// Invert the Z vertex to change to left hand system.
				m_vertices[vertexIndex].z = m_vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			// Read in the texture uv coordinates.
			if (input == 't')
			{
				fin >> m_textureCoords[texcoordIndex].x >> m_textureCoords[texcoordIndex].y;
				// Invert the V texture coordinates to left hand system.
				m_textureCoords[texcoordIndex].y = 1.0f - m_textureCoords[texcoordIndex].y;
				texcoordIndex++;
			}

			// Read in the normals.
			if (input == 'n')
			{
				fin >> m_normals[normalIndex].x >> m_normals[normalIndex].y >>
					m_normals[normalIndex].z;

				// Invert the Z normal to change to left hand system.
				m_normals[normalIndex].z = m_normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		// Read in the faces.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				fin >> m_faces[faceIndex].vIndex3 >> input2 >> m_faces[faceIndex].tIndex3 >>
					input2 >> m_faces[faceIndex].nIndex3 >> m_faces[faceIndex].vIndex2 >> input2 >>
					m_faces[faceIndex].tIndex2 >> input2 >> m_faces[faceIndex].nIndex2 >>
					m_faces[faceIndex].vIndex1 >> input2 >> m_faces[faceIndex].tIndex1 >> input2 >>
					m_faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		// Read in the remainder of the line.
		while (input != '\n')
		{
			fin.get(input);
			if (fin.eof())
				break;
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	//// Close the file.
	//fin.close();
	//// Open the output file.
	//fout.open("model.txt");
	//// Write out the file header that our model format uses.
	//fout << "Vertex Count: " << (faceCount * 3) << endl;
	//fout << endl;
	//fout << "Data:" << endl;
	//fout << endl;

	m_vertexCount = faceCount * 3;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Now loop through all the faces and output the three vertices for each face.
	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = m_faces[i].vIndex1 - 1;
		tIndex = m_faces[i].tIndex1 - 1;
		nIndex = m_faces[i].nIndex1 - 1;
		//fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
		//	<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
		//	<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		m_model[i * 3].x = m_vertices[vIndex].x;
		m_model[i * 3].y = m_vertices[vIndex].y;
		m_model[i * 3].z = m_vertices[vIndex].z;

		m_model[i * 3].tu = m_textureCoords[tIndex].x;
		m_model[i * 3].tv = m_textureCoords[tIndex].y;

		m_model[i * 3].nx = m_normals[nIndex].x;
		m_model[i * 3].ny = m_normals[nIndex].y;
		m_model[i * 3].nz = m_normals[nIndex].z;

		vIndex = m_faces[i].vIndex2 - 1;
		tIndex = m_faces[i].tIndex2 - 1;
		nIndex = m_faces[i].nIndex2 - 1;
		//fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
		//	<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
		//	<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		m_model[i * 3 + 1].x = m_vertices[vIndex].x;
		m_model[i * 3 + 1].y = m_vertices[vIndex].y;
		m_model[i * 3 + 1].z = m_vertices[vIndex].z;

		m_model[i * 3 + 1].tu = m_textureCoords[tIndex].x;
		m_model[i * 3 + 1].tv = m_textureCoords[tIndex].y;

		m_model[i * 3 + 1].nx = m_normals[nIndex].x;
		m_model[i * 3 + 1].ny = m_normals[nIndex].y;
		m_model[i * 3 + 1].nz = m_normals[nIndex].z;

		vIndex = m_faces[i].vIndex3 - 1;
		tIndex = m_faces[i].tIndex3 - 1;
		nIndex = m_faces[i].nIndex3 - 1;
		//fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
		//	<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
		//	<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		m_model[i * 3 + 2].x = m_vertices[vIndex].x;
		m_model[i * 3 + 2].y = m_vertices[vIndex].y;
		m_model[i * 3 + 2].z = m_vertices[vIndex].z;

		m_model[i * 3 + 2].tu = m_textureCoords[tIndex].x;
		m_model[i * 3 + 2].tv = m_textureCoords[tIndex].y;

		m_model[i * 3 + 2].nx = m_normals[nIndex].x;
		m_model[i * 3 + 2].ny = m_normals[nIndex].y;
		m_model[i * 3 + 2].nz = m_normals[nIndex].z;
	}

	//// Close the output file.
	//fout.close();

	// Release the four data structures.
	if (m_vertices)
	{
		delete[] m_vertices;
		m_vertices = 0;
	}

	if (m_textureCoords)
	{
		delete[] m_textureCoords;
		m_textureCoords = 0;
	}

	if (m_normals)
	{
		delete[] m_normals;
		m_normals = 0;
	}

	if (m_faces)
	{
		delete[] m_faces;
		m_faces = 0;
	}

	return true;
}