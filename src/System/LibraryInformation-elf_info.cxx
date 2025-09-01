/*!
 * \file   LibraryInformation-elf_info.cxx
 * \brief
 * \author Thomas Helfer
 * \date   28/03/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 *
 * \note This file contains code that has been extracted from the
 * boost/dll library version 1.63 and has been originally written by
 * Antony Polukhin, Renato Tegon Forti and Antony Polukhin.
 */

#include <cstdint>

namespace tfel::system::details {

  template <class AddressOffsetT>
  struct Elf_Ehdr_template {
    unsigned char e_ident[16]; /* Magic number and other info */
    std::uint16_t e_type;      /* Object file type */
    std::uint16_t e_machine;   /* Architecture */
    std::uint32_t e_version;   /* Object file version */
    AddressOffsetT e_entry;    /* Entry point virtual address */
    AddressOffsetT e_phoff;    /* Program header table file offset */
    AddressOffsetT e_shoff;    /* Section header table file offset */
    std::uint32_t e_flags;     /* Processor-specific flags */
    std::uint16_t e_ehsize;    /* ELF header size in bytes */
    std::uint16_t e_phentsize; /* Program header table entry size */
    std::uint16_t e_phnum;     /* Program header table entry count */
    std::uint16_t e_shentsize; /* Section header table entry size */
    std::uint16_t e_shnum;     /* Section header table entry count */
    std::uint16_t e_shstrndx;  /* Section header string table index */
  };

  typedef Elf_Ehdr_template<std::uint32_t> Elf32_Ehdr_;
  typedef Elf_Ehdr_template<std::uint64_t> Elf64_Ehdr_;

  template <class AddressOffsetT>
  struct Elf_Shdr_template {
    std::uint32_t sh_name;       /* Section name (string tbl index) */
    std::uint32_t sh_type;       /* Section type */
    AddressOffsetT sh_flags;     /* Section flags */
    AddressOffsetT sh_addr;      /* Section virtual addr at execution */
    AddressOffsetT sh_offset;    /* Section file offset */
    AddressOffsetT sh_size;      /* Section size in bytes */
    std::uint32_t sh_link;       /* Link to another section */
    std::uint32_t sh_info;       /* Additional section information */
    AddressOffsetT sh_addralign; /* Section alignment */
    AddressOffsetT sh_entsize;   /* Entry size if section holds table */
  };

  typedef Elf_Shdr_template<std::uint32_t> Elf32_Shdr_;
  typedef Elf_Shdr_template<std::uint64_t> Elf64_Shdr_;

  template <class AddressOffsetT>
  struct Elf_Sym_template;

  template <>
  struct Elf_Sym_template<std::uint32_t> {
    typedef std::uint32_t AddressOffsetT;

    std::uint32_t st_name;   /* Symbol name (string tbl index) */
    AddressOffsetT st_value; /* Symbol value */
    AddressOffsetT st_size;  /* Symbol size */
    unsigned char st_info;   /* Symbol type and binding */
    unsigned char st_other;  /* Symbol visibility */
    std::uint16_t st_shndx;  /* Section index */
  };

  template <>
  struct Elf_Sym_template<std::uint64_t> {
    typedef std::uint64_t AddressOffsetT;

    std::uint32_t st_name;   /* Symbol name (string tbl index) */
    unsigned char st_info;   /* Symbol type and binding */
    unsigned char st_other;  /* Symbol visibility */
    std::uint16_t st_shndx;  /* Section index */
    AddressOffsetT st_value; /* Symbol value */
    AddressOffsetT st_size;  /* Symbol size */
  };

  typedef Elf_Sym_template<std::uint32_t> Elf32_Sym_;
  typedef Elf_Sym_template<std::uint64_t> Elf64_Sym_;

  template <class AddressOffsetT>
  class elf_info : public tfel::system::LibraryInformation::Implementation {
    std::shared_ptr<std::ifstream> f_;

    typedef details::Elf_Ehdr_template<AddressOffsetT> header_t;
    typedef details::Elf_Shdr_template<AddressOffsetT> section_t;
    typedef details::Elf_Sym_template<AddressOffsetT> symbol_t;

    static constexpr std::uint32_t SHT_SYMTAB_ = 2;
    static constexpr std::uint32_t SHT_STRTAB_ = 3;

    static constexpr unsigned char STB_LOCAL_ = 0;  /* Local symbol */
    static constexpr unsigned char STB_GLOBAL_ = 1; /* Global symbol */
    static constexpr unsigned char STB_WEAK_ = 2;   /* Weak symbol */

    /* Symbol visibility specification encoded in the st_other field.  */
    static constexpr unsigned char STV_DEFAULT_ =
        0; /* Default symbol visibility rules */
    static constexpr unsigned char STV_INTERNAL_ =
        1; /* Processor specific hidden class */
    static constexpr unsigned char STV_HIDDEN_ =
        2; /* Sym unavailable in other modules */
    static constexpr unsigned char STV_PROTECTED_ =
        3; /* Not preemptible, not exported */

   public:
    static bool parsing_supported(std::ifstream& f) {
      const unsigned char magic_bytes[5] = {
          0x7f, 'E', 'L', 'F',
          sizeof(std::uint32_t) == sizeof(AddressOffsetT) ? 1 : 2};

      unsigned char ch;
      f.seekg(0);
      for (std::size_t i = 0; i < sizeof(magic_bytes); ++i) {
        f >> ch;
        if (ch != magic_bytes[i]) {
          return false;
        }
      }

      return true;
    }

    explicit elf_info(std::shared_ptr<std::ifstream>& f) noexcept : f_(f) {}

    std::vector<std::string> sections() override {
      std::vector<std::string> ret;
      std::vector<char> names;
      sections_names_raw(names);

      const char* name_begin = &names[0];
      const char* const name_end = name_begin + names.size();
      ret.reserve(header().e_shnum);
      do {
        ret.push_back(name_begin);
        name_begin += ret.back().size() + 1;
      } while (name_begin != name_end);

      return ret;
    }

   private:
    template <class T>
    inline void read_raw(T& value, std::size_t size = sizeof(T)) const {
      this->f_->read(reinterpret_cast<char*>(&value),
                     static_cast<std::streamsize>(size));
    }

    inline header_t header() {
      header_t elf;

      this->f_->seekg(0);
      read_raw(elf);

      return elf;
    }

    void sections_names_raw(std::vector<char>& snames) {
      const header_t elf = header();

      section_t section_names_section;
      this->f_->seekg(static_cast<std::streamoff>(
          elf.e_shoff + elf.e_shstrndx * sizeof(section_t)));
      read_raw(section_names_section);

      snames.resize(static_cast<std::size_t>(section_names_section.sh_size));
      this->f_->seekg(
          static_cast<std::streamoff>(section_names_section.sh_offset));
      read_raw(snames[0],
               static_cast<std::size_t>(section_names_section.sh_size));
    }

    void symbols_text(std::vector<symbol_t>& s, std::vector<char>& t) {
      const header_t elf = header();
      this->f_->seekg(static_cast<std::streamoff>(elf.e_shoff));

      for (std::size_t i = 0; i < elf.e_shnum; ++i) {
        section_t section;
        read_raw(section);

        if (section.sh_type == SHT_SYMTAB_) {
          s.resize(
              static_cast<std::size_t>(section.sh_size / sizeof(symbol_t)));

          const std::ifstream::pos_type pos = this->f_->tellg();
          this->f_->seekg(static_cast<std::streamoff>(section.sh_offset));
          read_raw(s[0],
                   static_cast<std::size_t>(
                       section.sh_size - (section.sh_size % sizeof(symbol_t))));
          this->f_->seekg(pos);
        } else if (section.sh_type == SHT_STRTAB_) {
          t.resize(static_cast<std::size_t>(section.sh_size));

          const std::ifstream::pos_type pos = this->f_->tellg();
          this->f_->seekg(static_cast<std::streamoff>(section.sh_offset));
          read_raw(t[0], static_cast<std::size_t>(section.sh_size));
          this->f_->seekg(pos);
        }
      }
    }

    static bool is_visible(const symbol_t& sym) noexcept {
      // `(sym.st_info >> 4) != STB_LOCAL_ && !!sym.st_size` check also
      // workarounds the GCC's issue
      // https://sourceware.org/bugzilla/show_bug.cgi?id=13621
      return (sym.st_other & 0x03) == STV_DEFAULT_ &&
             (sym.st_info >> 4) != STB_LOCAL_ && !!sym.st_size;
    }

   public:
    std::vector<std::string> symbols() override {
      std::vector<std::string> ret;

      std::vector<symbol_t> s;
      std::vector<char> t;
      symbols_text(s, t);

      ret.reserve(s.size());
      for (std::size_t i = 0; i < s.size(); ++i) {
        if (is_visible(s[i])) {
          ret.push_back(&t[0] + s[i].st_name);
          if (ret.back().empty()) {
            ret.pop_back();  // Do not show empty names
          }
        }
      }

      return ret;
    }

    std::vector<std::string> symbols(const char* section_name) override {
      std::vector<std::string> ret;

      std::size_t index = 0;
      std::size_t ptrs_in_section_count = 0;
      {
        std::vector<char> names;
        sections_names_raw(names);

        const header_t elf = header();

        for (; index < elf.e_shnum; ++index) {
          section_t section;
          this->f_->seekg(static_cast<std::streamsize>(
              elf.e_shoff + index * sizeof(section_t)));
          read_raw(section);

          if (!std::strcmp(&names[0] + section.sh_name, section_name)) {
            if (!section.sh_entsize) {
              section.sh_entsize = 1;
            }
            ptrs_in_section_count =
                static_cast<std::size_t>(section.sh_size / section.sh_entsize);
            break;
          }
        }
      }

      std::vector<symbol_t> s;
      std::vector<char> t;
      symbols_text(s, t);

      if (ptrs_in_section_count < s.size()) {
        ret.reserve(ptrs_in_section_count);
      } else {
        ret.reserve(s.size());
      }

      for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i].st_shndx == index && is_visible(s[i])) {
          ret.push_back(&t[0] + s[i].st_name);
          if (ret.back().empty()) {
            ret.pop_back();  // Do not show empty names
          }
        }
      }

      return ret;
    }
  };

  typedef elf_info<std::uint32_t> elf_info32;
  typedef elf_info<std::uint64_t> elf_info64;

}  // namespace tfel::system::details
