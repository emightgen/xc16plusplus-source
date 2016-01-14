// merge.h -- handle section merging for gold  -*- C++ -*-

// Copyright 2006, 2007, 2008 Free Software Foundation, Inc.
// Written by Ian Lance Taylor <iant@google.com>.

// This file is part of gold.

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
// MA 02110-1301, USA.

#ifndef GOLD_MERGE_H
#define GOLD_MERGE_H

#include <climits>
#include <map>
#include <vector>

#include "stringpool.h"
#include "output.h"

namespace gold
{

class Merge_map;

// For each object with merge sections, we store an Object_merge_map.
// This is used to map locations in input sections to a merged output
// section.  The output section itself is not recorded here--it can be
// found in the output_sections_ field of the Object.

class Object_merge_map
{
 public:
  Object_merge_map()
    : first_shnum_(-1U), first_map_(),
      second_shnum_(-1U), second_map_(),
      section_merge_maps_()
  { }

  ~Object_merge_map();

  // Add a mapping for MERGE_MAP, for the bytes from OFFSET to OFFSET
  // + LENGTH in the input section SHNDX to OUTPUT_OFFSET in the
  // output section.  An OUTPUT_OFFSET of -1 means that the bytes are
  // discarded.  OUTPUT_OFFSET is relative to the start of the merged
  // data in the output section.
  void
  add_mapping(const Merge_map*, unsigned int shndx, section_offset_type offset,
	      section_size_type length, section_offset_type output_offset);

  // Get the output offset for an input address.  MERGE_MAP is the map
  // we are looking for, or NULL if we don't care.  The input address
  // is at offset OFFSET in section SHNDX.  This sets *OUTPUT_OFFSET
  // to the offset in the output section; this will be -1 if the bytes
  // are not being copied to the output.  This returns true if the
  // mapping is known, false otherwise.  *OUTPUT_OFFSET is relative to
  // the start of the merged data in the output section.
  bool
  get_output_offset(const Merge_map*, unsigned int shndx,
		    section_offset_type offset,
		    section_offset_type *output_offset);

  // Return whether this is the merge map for section SHNDX.
  bool
  is_merge_section_for(const Merge_map*, unsigned int shndx);

  // Initialize an mapping from input offsets to output addresses for
  // section SHNDX.  STARTING_ADDRESS is the output address of the
  // merged section.
  template<int size>
  void
  initialize_input_to_output_map(
      unsigned int shndx,
      typename elfcpp::Elf_types<size>::Elf_Addr starting_address,
      Unordered_map<section_offset_type,
		    typename elfcpp::Elf_types<size>::Elf_Addr>*);

 private:
  // Map input section offsets to a length and an output section
  // offset.  An output section offset of -1 means that this part of
  // the input section is being discarded.
  struct Input_merge_entry
  {
    // The offset in the input section.
    section_offset_type input_offset;
    // The length.
    section_size_type length;
    // The offset in the output section.
    section_offset_type output_offset;
  };

  // A less-than comparison routine for Input_merge_entry.
  struct Input_merge_compare
  {
    bool
    operator()(const Input_merge_entry& i1, const Input_merge_entry& i2) const
    { return i1.input_offset < i2.input_offset; }
  };

  // A list of entries for a particular input section.
  struct Input_merge_map
  {
    typedef std::vector<Input_merge_entry> Entries;

    // We store these with the Relobj, and we look them up by input
    // section.  It is possible to have two different merge maps
    // associated with a single output section.  For example, this
    // happens routinely with .rodata, when merged string constants
    // and merged fixed size constants are both put into .rodata.  The
    // output offset that we store is not the offset from the start of
    // the output section; it is the offset from the start of the
    // merged data in the output section.  That means that the caller
    // is going to add the offset of the merged data within the output
    // section, which means that the caller needs to know which set of
    // merged data it found the entry in.  So it's not enough to find
    // this data based on the input section and the output section; we
    // also have to find it based on a set of merged data in the
    // output section.  In order to verify that we are looking at the
    // right data, we store a pointer to the Merge_map here, and we
    // pass in a pointer when looking at the data.  If we are asked to
    // look up information for a different Merge_map, we report that
    // we don't have it, rather than trying a lookup and returning an
    // answer which will receive the wrong offset.
    const Merge_map* merge_map;
    // The list of mappings.
    Entries entries;
    // Whether the ENTRIES field is sorted by input_offset.
    bool sorted;

    Input_merge_map()
      : merge_map(NULL), entries(), sorted(true)
    { }
  };

  // Map input section indices to merge maps.
  typedef std::map<unsigned int, Input_merge_map*> Section_merge_maps;

  // Return a pointer to the Input_merge_map to use for the input
  // section SHNDX, or NULL.
  Input_merge_map*
  get_input_merge_map(unsigned int shndx);

  // Get or make the the Input_merge_map to use for the section SHNDX
  // with MERGE_MAP.
  Input_merge_map*
  get_or_make_input_merge_map(const Merge_map* merge_map, unsigned int shndx);

  // Any given object file will normally only have a couple of input
  // sections with mergeable contents.  So we keep the first two input
  // section numbers inline, and push any further ones into a map.  A
  // value of -1U in first_shnum_ or second_shnum_ means that we don't
  // have a corresponding entry.
  unsigned int first_shnum_;
  Input_merge_map first_map_;
  unsigned int second_shnum_;
  Input_merge_map second_map_;
  Section_merge_maps section_merge_maps_;
};

// This class manages mappings from input sections to offsets in an
// output section.  This is used where input sections are merged.  The
// actual data is stored in fields in Object.

class Merge_map
{
 public:
  Merge_map()
  { }

  // Add a mapping for the bytes from OFFSET to OFFSET + LENGTH in the
  // input section SHNDX in object OBJECT to OUTPUT_OFFSET in the
  // output section.  An OUTPUT_OFFSET of -1 means that the bytes are
  // discarded.  OUTPUT_OFFSET is not the offset from the start of the
  // output section, it is the offset from the start of the merged
  // data within the output section.
  void
  add_mapping(Relobj* object, unsigned int shndx,
	      section_offset_type offset, section_size_type length,
	      section_offset_type output_offset);

  // Return the output offset for an input address.  The input address
  // is at offset OFFSET in section SHNDX in OBJECT.  This sets
  // *OUTPUT_OFFSET to the offset in the output section; this will be
  // -1 if the bytes are not being copied to the output.  This returns
  // true if the mapping is known, false otherwise.  This returns the
  // value stored by add_mapping, namely the offset from the start of
  // the merged data within the output section.
  bool
  get_output_offset(const Relobj* object, unsigned int shndx,
		    section_offset_type offset,
		    section_offset_type *output_offset) const;

  // Return whether this is the merge mapping for section SHNDX in
  // OBJECT.  This should return true when get_output_offset would
  // return true for some input offset.
  bool
  is_merge_section_for(const Relobj* object, unsigned int shndx) const;
};

// A general class for SHF_MERGE data, to hold functions shared by
// fixed-size constant data and string data.

class Output_merge_base : public Output_section_data
{
 public:
  Output_merge_base(uint64_t entsize, uint64_t addralign)
    : Output_section_data(addralign), merge_map_(), entsize_(entsize)
  { }

  // Return the entry size.
  uint64_t
  entsize() const
  { return this->entsize_; }

  // Whether this is a merge string section.  This is only true of
  // Output_merge_string.
  bool
  is_string()
  { return this->do_is_string(); }

 protected:
  // Return the output offset for an input offset.
  bool
  do_output_offset(const Relobj* object, unsigned int shndx,
		   section_offset_type offset,
		   section_offset_type* poutput) const;

  // Return whether this is the merge section for an input section.
  bool
  do_is_merge_section_for(const Relobj*, unsigned int shndx) const;

  // Add a mapping from an OFFSET in input section SHNDX in object
  // OBJECT to an OUTPUT_OFFSET in the output section.  OUTPUT_OFFSET
  // is the offset from the start of the merged data in the output
  // section.
  void
  add_mapping(Relobj* object, unsigned int shndx, section_offset_type offset,
	      section_size_type length, section_offset_type output_offset)
  {
    this->merge_map_.add_mapping(object, shndx, offset, length, output_offset);
  }

  // This may be overriden by the child class.
  virtual bool
  do_is_string()
  { return false; }

 private:
  // A mapping from input object/section/offset to offset in output
  // section.
  Merge_map merge_map_;
  // The entry size.  For fixed-size constants, this is the size of
  // the constants.  For strings, this is the size of a character.
  uint64_t entsize_;
};

// Handle SHF_MERGE sections with fixed-size constant data.

class Output_merge_data : public Output_merge_base
{
 public:
  Output_merge_data(uint64_t entsize, uint64_t addralign)
    : Output_merge_base(entsize, addralign), p_(NULL), len_(0), alc_(0),
      input_count_(0),
      hashtable_(128, Merge_data_hash(this), Merge_data_eq(this))
  { }

 protected:
  // Add an input section.
  bool
  do_add_input_section(Relobj* object, unsigned int shndx);

  // Set the final data size.
  void
  set_final_data_size();

  // Write the data to the file.
  void
  do_write(Output_file*);

  // Write the data to a buffer.
  void
  do_write_to_buffer(unsigned char*);

  // Write to a map file.
  void
  do_print_to_mapfile(Mapfile* mapfile) const
  { mapfile->print_output_data(this, _("** merge constants")); }

  // Print merge stats to stderr.
  void
  do_print_merge_stats(const char* section_name);

 private:
  // We build a hash table of the fixed-size constants.  Each constant
  // is stored as a pointer into the section data we are accumulating.

  // A key in the hash table.  This is an offset in the section
  // contents we are building.
  typedef section_offset_type Merge_data_key;

  // Compute the hash code.  To do this we need a pointer back to the
  // object holding the data.
  class Merge_data_hash
  {
   public:
    Merge_data_hash(const Output_merge_data* pomd)
      : pomd_(pomd)
    { }

    size_t
    operator()(Merge_data_key) const;

   private:
    const Output_merge_data* pomd_;
  };

  friend class Merge_data_hash;

  // Compare two entries in the hash table for equality.  To do this
  // we need a pointer back to the object holding the data.  Note that
  // we now have a pointer to the object stored in two places in the
  // hash table.  Fixing this would require specializing the hash
  // table, which would be hard to do portably.
  class Merge_data_eq
  {
   public:
    Merge_data_eq(const Output_merge_data* pomd)
      : pomd_(pomd)
    { }

    bool
    operator()(Merge_data_key k1, Merge_data_key k2) const;

   private:
    const Output_merge_data* pomd_;
  };

  friend class Merge_data_eq;

  // The type of the hash table.
  typedef Unordered_set<Merge_data_key, Merge_data_hash, Merge_data_eq>
    Merge_data_hashtable;

  // Given a hash table key, which is just an offset into the section
  // data, return a pointer to the corresponding constant.
  const unsigned char*
  constant(Merge_data_key k) const
  {
    gold_assert(k >= 0 && k < static_cast<section_offset_type>(this->len_));
    return this->p_ + k;
  }

  // Add a constant to the output.
  void
  add_constant(const unsigned char*);

  // The accumulated data.
  unsigned char* p_;
  // The length of the accumulated data.
  section_size_type len_;
  // The size of the allocated buffer.
  section_size_type alc_;
  // The number of entries seen in input files.
  size_t input_count_;
  // The hash table.
  Merge_data_hashtable hashtable_;
};

// Handle SHF_MERGE sections with string data.  This is a template
// based on the type of the characters in the string.

template<typename Char_type>
class Output_merge_string : public Output_merge_base
{
 public:
  Output_merge_string(uint64_t addralign)
    : Output_merge_base(sizeof(Char_type), addralign), stringpool_(),
      merged_strings_lists_(), input_count_(0), input_size_(0)
  {
    gold_assert(addralign <= sizeof(Char_type));
    this->stringpool_.set_no_zero_null();
  }

 protected:
  // Add an input section.
  bool
  do_add_input_section(Relobj* object, unsigned int shndx);

  // Do all the final processing after the input sections are read in.
  // Returns the final data size.
  section_size_type
  finalize_merged_data();

  // Set the final data size.
  void
  set_final_data_size();

  // Write the data to the file.
  void
  do_write(Output_file*);

  // Write the data to a buffer.
  void
  do_write_to_buffer(unsigned char*);

  // Write to a map file.
  void
  do_print_to_mapfile(Mapfile* mapfile) const
  { mapfile->print_output_data(this, _("** merge strings")); }

  // Print merge stats to stderr.
  void
  do_print_merge_stats(const char* section_name);

  // Writes the stringpool to a buffer.
  void
  stringpool_to_buffer(unsigned char* buffer, section_size_type buffer_size)
  { this->stringpool_.write_to_buffer(buffer, buffer_size); }

  // Clears all the data in the stringpool, to save on memory.
  void
  clear_stringpool()
  { this->stringpool_.clear(); }

  // Whether this is a merge string section.
  virtual bool
  do_is_string()
  { return true; }

 private:
  // The name of the string type, for stats.
  const char*
  string_name();

  // As we see input sections, we build a mapping from object, section
  // index and offset to strings.
  struct Merged_string
  {
    // The offset in the input section.
    section_offset_type offset;
    // The key in the Stringpool.
    Stringpool::Key stringpool_key;

    Merged_string(section_offset_type offseta, Stringpool::Key stringpool_keya)
      : offset(offseta), stringpool_key(stringpool_keya)
    { }
  };

  typedef std::vector<Merged_string> Merged_strings;

  struct Merged_strings_list
  {
    // The input object where the strings were found.
    Relobj* object;
    // The input section in the input object.
    unsigned int shndx;
    // The list of merged strings.
    Merged_strings merged_strings;

    Merged_strings_list(Relobj* objecta, unsigned int shndxa)
      : object(objecta), shndx(shndxa), merged_strings()
    { }
  };

  typedef std::vector<Merged_strings_list*> Merged_strings_lists;

  // As we see the strings, we add them to a Stringpool.
  Stringpool_template<Char_type> stringpool_;
  // Map from a location in an input object to an entry in the
  // Stringpool.
  Merged_strings_lists merged_strings_lists_;
  // The number of entries seen in input files.
  size_t input_count_;
  // The total size of input sections.
  size_t input_size_;
};

} // End namespace gold.

#endif // !defined(GOLD_MERGE_H)
