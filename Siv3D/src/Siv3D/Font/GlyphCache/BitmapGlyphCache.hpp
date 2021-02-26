﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2021 Ryo Suzuki
//	Copyright (c) 2016-2021 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# pragma once
# include <Siv3D/Font.hpp>
# include <Siv3D/HashTable.hpp>
# include <Siv3D/DynamicTexture.hpp>
# include "IGlyphCache.hpp"
# include "GlyphCacheCommon.hpp"

namespace s3d
{
	class BitmapGlyphCache : public IGlyphCache
	{
	public:

		BitmapGlyphCache() = default;

		RectF draw(const FontData & font, StringView s, const Array<GlyphCluster>&clusters, bool usebasePos, const Vec2 & pos, double size, const TextStyle & textStyle, const ColorF & color, double lineHeightScale) override;

		bool draw(const FontData & font, StringView s, const Array<GlyphCluster>&clusters, const RectF & area, double size, const TextStyle & textStyle, const ColorF & color, double lineHeightScale) override;

		[[nodiscard]]
		RectF drawFallback(const FontData & font, const GlyphCluster & cluster, bool usebasePos, const Vec2 & pos, double size, const ColorF & color, double lineHeightScale) override;

		[[nodiscard]]
		Array<double> getXAdvances(const FontData & font, StringView s, const Array<GlyphCluster>&clusters) override;

		[[nodiscard]]
		double xAdvanceFallback(const FontData & font, const GlyphCluster & cluster) override;

		[[nodiscard]]
		RectF region(const FontData & font, StringView s, const Array<GlyphCluster>&clusters, bool usebasePos, const Vec2 & pos, double size, double lineHeightScale) override;

		[[nodiscard]]
		RectF regionFallback(const FontData & font, const GlyphCluster & cluster, bool usebasePos, const Vec2 & pos, double size, double lineHeightScale) override;

		void setBufferWidth(int32 width) override;

		[[nodiscard]]
		int32 getBufferWidth() const noexcept override;

		bool preload(const FontData & font, StringView s) override;

		[[nodiscard]]
		const Texture& getTexture() const noexcept override;

	private:

		[[nodiscard]]
		bool prerender(const FontData& font, const Array<GlyphCluster>& clusters);

		HashTable<GlyphIndex, GlyphCache> m_glyphTable;

		DynamicTexture m_texture;

		BufferImage m_buffer = {};
	};
}