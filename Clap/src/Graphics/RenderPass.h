/*

m_QuadVA = VertexArray::Create();
float vertices[4 * 5] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f 
};

Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
BufferLayout layout = {
    { ShaderDataType::Float3, "a_Position" },
    { ShaderDataType::Float2, "a_TexCoords" }
};
vertexBuffer->SetLayout(layout);
m_QuadVA->AddVertexBuffer(vertexBuffer);

uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
m_QuadVA->SetIndexBuffer(indexBuffer);
m_QuadVA->Unbind();

m_QuadShader = Shader::Create("../../../Game2D/res/QuadShader.glsl");
m_QuadShader->SetUniformBufferBinding(s_Data.CameraUniformBuffer, "Camera");






