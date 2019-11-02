/**

 <!-- By guoping.liu -->

 @page chapter05 第五章 zglEngine的高级应用开发


 @section chapter05_1 5.1 zglPrim的扩展开发



 @image html sample-prim.png "图 5-1 示例一"

 @code
	class zglPrim3DAppIcon : public zglPrim3D
	{
	public:
		void init(zglVertex* vertex,
				zglTexture * tex_1, zglTexture * tex_2);
	protected:
		virtual void render();

	public:
		zglTexture * m_texture_text;
	};
 @endcode

 @code
	void zglPrim3DAppIcon::init(zglVertex* vertex,
			zglTexture * tex_1, zglTexture * tex_2)
	{
		m_vertex = vertex;
		m_vertex_num = 12;

		m_texture = tex_1;
		m_texture_text = tex_2;

		m_indices = NULL;
		m_indices_num = 0;

		//!< reset the local coordinate system matrix
		m_world = 1.0f;

		//Add to the node list
		queue();
	}

	void zglPrim3DAppIcon::render()
	{
		const unsigned int CHECK_FLAG = FLAG_NO_FOG | FLAG_CULL_FACE;
		if (m_flag & CHECK_FLAG)
		{
			if (m_flag & FLAG_CULL_FACE)
			{
				zglCore::setCullFace(true);
			}
		}

		if (m_flag & FLAG_BILLBOARD)
		{
			zglMatrix mat = zglEngine::getEngine()->world();
			mat.T() = m_world.T();
			zglCore::setModelView(zglEngine::getEngine()->modelview() * mat);
		}
		else
		{
			zglCore::setModelView(zglEngine::getEngine()->modelview() * m_world);
		}

		if(m_material)
		{
			m_material->applyMaterial();
		}

		zglCore::setBlendType((zglBlendType) m_blend_type);

		if (m_hwbuffer_type != HWBUFFER_AVAILABLE)
		{
			zglCore::setVertexPointer3((float*) &m_vertex->x, sizeof(zglVertex));
			zglCore::setColorPointer4(&m_vertex->r, sizeof(zglVertex));
			zglCore::setTexCoordPointer2((float*) &m_vertex->u, sizeof(zglVertex));

			//!< Background
			zglCore::setTexture(NULL, false);
			zglCore::drawArrays(DRAW_TRI_STRIP, 0, 4);

			//!< Icon
			if (m_texture)
			{
				zglCore::setTexture(m_texture->getTexID(), true);
			}
			else
			{
				zglCore::setTexture(NULL, false);
			}
			zglCore::drawArrays(DRAW_TRI_STRIP, 4, 4);

			//!< Title
			if (m_texture_text)
			{
				zglCore::setTexture(m_texture_text->getTexID(), true);
			}
			else
			{
				zglCore::setTexture(NULL, false);
			}
			zglCore::drawArrays(DRAW_TRI_STRIP, 8, 4);
		}
		else
		{
			zglCore::setBuffer(INDEX_BUFFER, 0);
			zglCore::setBuffer(VERTEX_BUFFER, m_vbo_vertex);
			zglCore::setVertexPointer3(NULL, sizeof(zglVertex));
			zglCore::setColorPointer4( ((unsigned char*)NULL + 0x0c), sizeof(zglVertex));
			zglCore::setTexCoordPointer2((float*)((char*)NULL + 0x10), sizeof(zglVertex));

			//!< Background
			zglCore::setTexture(NULL, false);
			zglCore::drawArrays(DRAW_TRI_STRIP, 0, 4);

			//!< Icon
			if (m_texture)
			{
				zglCore::setTexture(m_texture->getTexID(), true);
			}
			else
			{
				zglCore::setTexture(NULL, false);
			}
			zglCore::drawArrays(DRAW_TRI_STRIP, 4, 4);

			//!< Title
			if (m_texture_text)
			{
				zglCore::setTexture(m_texture_text->getTexID(), true);
			}
			else
			{
				zglCore::setTexture(NULL, false);
			}
			zglCore::drawArrays(DRAW_TRI_STRIP, 8, 4);

			zglCore::setBuffer(VERTEX_BUFFER, 0);
		}

		//!< Restore environment.
		if(m_material)
		{
			zglCore::setDefaultMaterial();
		}

		if (m_flag & CHECK_FLAG)
		{
			if (m_flag & FLAG_CULL_FACE)
			{
				zglCore::setCullFace(false);
			}
		}
	}

 @endcode

 @section chapter05_2 5.2 zglWidget的扩展开发

 创建一个新的Widget。


 @section chapter05_3 5.3 zglAnimation的扩展开发

 创建一个新的Animation。


 @section chapter05_4 5.4 zglInterpolator的扩展开发
 增加一个新的插值器。


 @section chapter05_5 5.5 JNI的使用

 @subsection chapter05_5_1 5.5.1 JNI的使用

 @subsection chapter05_5_2 5.5.2 JNI的使用

 */
