/*
 * HEIF codec.
 * Copyright (c) 2023 Dirk Farin <dirk.farin@gmail.com>
 *
 * This file is part of libheif.
 *
 * libheif is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * libheif is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libheif.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef LIBHEIF_UNCOMPRESSED_BOX_H
#define LIBHEIF_UNCOMPRESSED_BOX_H

#include "box.h"
#include "bitstream.h"
#include "uncompressed.h"

#include <cstdint>
#include <string>
#include <vector>
#include <memory>


/**
 * Component definition (cmpd) box.
 */
class Box_cmpd : public Box
{
public:
  Box_cmpd()
  {
    set_short_type(fourcc("cmpd"));
  }

  std::string dump(Indent&) const override;

  Error write(StreamWriter& writer) const override;

  struct Component
  {
    uint16_t component_type;
    std::string component_type_uri;

    std::string get_component_type_name() const { return get_component_type_name(component_type); }

    static std::string get_component_type_name(uint16_t type);
  };

  const std::vector<Component>& get_components() const { return m_components; }

  void add_component(const Component& component)
  {
    m_components.push_back(component);
  }

protected:
  Error parse(BitstreamRange& range) override;

  std::vector<Component> m_components;
};

/**
 * Uncompressed Frame Configuration Box
*/
class Box_uncC : public FullBox
{
public:
  Box_uncC() {
    set_short_type(fourcc("uncC"));
  }

  void derive_box_version() override {};

  std::string dump(Indent&) const override;

  Error write(StreamWriter& writer) const override;

  struct Component
  {
    uint16_t component_index;
    uint16_t component_bit_depth; // range [1..256]
    uint8_t component_format;
    uint8_t component_align_size;
  };

  const std::vector<Component>& get_components() const { return m_components; }

  void add_component(Component component)
  {
    m_components.push_back(component);
  }

  uint32_t get_profile() const { return m_profile; }

  void set_profile(const uint32_t profile)
  {
    m_profile = profile;
  }

  uint8_t get_sampling_type() const { return m_sampling_type; }

  void set_sampling_type(const uint8_t sampling_type)
  {
    m_sampling_type = sampling_type;
  }

  uint8_t get_interleave_type() const { return m_interleave_type; }

  void set_interleave_type(const uint8_t interleave_type)
  {
    m_interleave_type = interleave_type;
  }

  uint8_t get_block_size() const { return m_block_size; }

  void set_block_size(const uint8_t block_size)
  {
    m_block_size = block_size;
  }

  bool is_components_little_endian() const { return m_components_little_endian; }

  void set_components_little_endian (const bool components_little_endian)
  {
    m_components_little_endian = components_little_endian;
  }

  bool is_block_pad_lsb() const { return m_block_pad_lsb; }

  void set_block_pad_lsb(const bool block_pad_lsb)
  {
    m_block_pad_lsb = block_pad_lsb;
  }

  bool is_block_little_endian() const { return m_block_little_endian; }

  void set_block_little_endian(const bool block_little_endian)
  {
    m_block_little_endian = block_little_endian;
  }

  bool is_block_reversed() const { return m_block_reversed; }

  void set_block_reversed(const bool block_reversed)
  {
    m_block_reversed = block_reversed;
  }

  bool is_pad_unknown() const { return m_pad_unknown; }

  void set_pad_unknown(const bool pad_unknown)
  {
    m_pad_unknown = pad_unknown;
  }

  uint32_t get_pixel_size() const { return m_pixel_size; }

  void set_pixel_size(const uint32_t pixel_size)
  {
    m_pixel_size = pixel_size;
  }

  uint32_t get_row_align_size() const { return m_row_align_size; }

  void set_row_align_size(const uint32_t row_align_size)
  {
    m_row_align_size = row_align_size;
  }

  uint32_t get_tile_align_size() const { return m_tile_align_size; }

  void set_tile_align_size(const uint32_t tile_align_size)
  {
    m_tile_align_size = tile_align_size;
  }

  uint32_t get_number_of_tile_columns() const { return m_num_tile_cols; }

  void set_number_of_tile_columns(const uint32_t num_tile_cols)
  {
    m_num_tile_cols = num_tile_cols;
  }

  uint32_t get_number_of_tile_rows() const { return m_num_tile_rows; }

  void set_number_of_tile_rows(const uint32_t num_tile_rows)
  {
    m_num_tile_rows = num_tile_rows;
  }

protected:
  Error parse(BitstreamRange& range) override;

  uint32_t m_profile = 0; // not compliant to any profile

  std::vector<Component> m_components;
  uint8_t m_sampling_type = sampling_mode_no_subsampling; // no subsampling
  uint8_t m_interleave_type = interleave_mode_pixel; // component interleaving
  uint8_t m_block_size = 0;
  bool m_components_little_endian = false;
  bool m_block_pad_lsb = false;
  bool m_block_little_endian = false;
  bool m_block_reversed = false;
  bool m_pad_unknown = false;
  uint32_t m_pixel_size = 0;
  uint32_t m_row_align_size = 0;
  uint32_t m_tile_align_size = 0;
  uint32_t m_num_tile_cols = 1;
  uint32_t m_num_tile_rows = 1;
};

/**
 * Generic compression configuration box (cmpC).
 *
 * This is from ISO/IEC 23001-17 Amd 2.
 */
class Box_cmpC : public FullBox
{
public:
  Box_cmpC()
  {
    set_short_type(fourcc("cmpC"));
  }

  std::string dump(Indent&) const override;

  uint32_t get_compression_type() const { return compression_type; }
  uint8_t get_compressed_unit_type() const { return compressed_unit_type; }

  Error write(StreamWriter& writer) const override;

protected:
  Error parse(BitstreamRange& range) override;

  uint32_t compression_type;
  uint8_t compressed_unit_type;
};

/**
 * Generically compressed units item info (icef).
 *
 * This describes the units of compressed data for an item.
 *
 * The box is from ISO/IEC 23001-17 Amd 2.
 */
class Box_icef : public FullBox
{
public:
  Box_icef()
  {
    set_short_type(fourcc("icef"));
  }

  struct CompressedUnitInfo
  {
    uint64_t unit_offset;
    uint64_t unit_size;
  };

  const std::vector<CompressedUnitInfo>& get_units() const { return m_unit_infos; }

  void add_component(const CompressedUnitInfo& unit_info)
  {
    m_unit_infos.push_back(unit_info);
  }

  std::string dump(Indent&) const override;

  Error write(StreamWriter& writer) const override;

protected:
  Error parse(BitstreamRange& range) override;

  std::vector<CompressedUnitInfo> m_unit_infos;

private:
  const uint8_t get_required_offset_code(uint64_t offset) const;
  const uint8_t get_required_size_code(uint64_t size) const;
};

#endif //LIBHEIF_UNCOMPRESSED_BOX_H
