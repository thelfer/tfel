/*!
 * \file   LibraryInformation-pe_info.cxx
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

namespace tfel {

  namespace system {

    namespace details {

      // reference:
      // http://www.joachim-bauch.de/tutorials/loading-a-dll-from-memory/
      // http://msdn.microsoft.com/en-us/magazine/ms809762.aspx
      // http://msdn.microsoft.com/en-us/magazine/cc301808.aspx
      //

      // Basic Windows typedefs. We can not use
      // <boost/detail/winapi/basic_types.hpp> header because that header must
      // be included only on Windows platform
      typedef unsigned char BYTE_;
      typedef unsigned short WORD_;

      typedef std::uint32_t DWORD_;
      typedef std::int32_t LONG_;
      typedef std::uint32_t ULONG_;

      typedef std::int64_t LONGLONG_;
      typedef std::uint64_t ULONGLONG_;

      struct IMAGE_DOS_HEADER_ {    // 32/64 independent header
        details::WORD_ e_magic;     // Magic number
        details::WORD_ e_cblp;      // Bytes on last page of file
        details::WORD_ e_cp;        // Pages in file
        details::WORD_ e_crlc;      // Relocations
        details::WORD_ e_cparhdr;   // Size of header in paragraphs
        details::WORD_ e_minalloc;  // Minimum extra paragraphs needed
        details::WORD_ e_maxalloc;  // Maximum extra paragraphs needed
        details::WORD_ e_ss;        // Initial (relative) SS value
        details::WORD_ e_sp;        // Initial SP value
        details::WORD_ e_csum;      // Checksum
        details::WORD_ e_ip;        // Initial IP value
        details::WORD_ e_cs;        // Initial (relative) CS value
        details::WORD_ e_lfarlc;    // File address of relocation table
        details::WORD_ e_ovno;      // Overlay number
        details::WORD_ e_res[4];    // Reserved words
        details::WORD_ e_oemid;     // OEM identifier (for e_oeminfo)
        details::WORD_ e_oeminfo;   // OEM information; e_oemid specific
        details::WORD_ e_res2[10];  // Reserved words
        details::LONG_ e_lfanew;    // File address of new exe header
      };

      struct IMAGE_FILE_HEADER_ {  // 32/64 independent header
        details::WORD_ Machine;
        details::WORD_ NumberOfSections;
        details::DWORD_ TimeDateStamp;
        details::DWORD_ PointerToSymbolTable;
        details::DWORD_ NumberOfSymbols;
        details::WORD_ SizeOfOptionalHeader;
        details::WORD_ Characteristics;
      };

      struct IMAGE_DATA_DIRECTORY_ {  // 32/64 independent header
        details::DWORD_ VirtualAddress;
        details::DWORD_ Size;
      };

      struct IMAGE_EXPORT_DIRECTORY_ {  // 32/64 independent header
        details::DWORD_ Characteristics;
        details::DWORD_ TimeDateStamp;
        details::WORD_ MajorVersion;
        details::WORD_ MinorVersion;
        details::DWORD_ Name;
        details::DWORD_ Base;
        details::DWORD_ NumberOfFunctions;
        details::DWORD_ NumberOfNames;
        details::DWORD_ AddressOfFunctions;
        details::DWORD_ AddressOfNames;
        details::DWORD_ AddressOfNameOrdinals;
      };

      struct IMAGE_SECTION_HEADER_ {  // 32/64 independent header
        static const std::size_t IMAGE_SIZEOF_SHORT_NAME_ = 8;

        details::BYTE_ Name[IMAGE_SIZEOF_SHORT_NAME_];
        union {
          details::DWORD_ PhysicalAddress;
          details::DWORD_ VirtualSize;
        } Misc;
        details::DWORD_ VirtualAddress;
        details::DWORD_ SizeOfRawData;
        details::DWORD_ PointerToRawData;
        details::DWORD_ PointerToRelocations;
        details::DWORD_ PointerToLinenumbers;
        details::WORD_ NumberOfRelocations;
        details::WORD_ NumberOfLinenumbers;
        details::DWORD_ Characteristics;
      };

      template <class AddressOffsetT>
      struct IMAGE_OPTIONAL_HEADER_template {
        static const std::size_t IMAGE_NUMBEROF_DIRECTORY_ENTRIES_ = 16;

        details::WORD_ Magic;
        details::BYTE_ MajorLinkerVersion;
        details::BYTE_ MinorLinkerVersion;
        details::DWORD_ SizeOfCode;
        details::DWORD_ SizeOfInitializedData;
        details::DWORD_ SizeOfUninitializedData;
        details::DWORD_ AddressOfEntryPoint;
        union {
          details::DWORD_ BaseOfCode;
          unsigned char
              padding_[sizeof(AddressOffsetT) == 8
                           ? 4
                           : 8];  // in x64 version BaseOfData does not exist
        } BaseOfCode_and_BaseOfData;

        AddressOffsetT ImageBase;
        details::DWORD_ SectionAlignment;
        details::DWORD_ FileAlignment;
        details::WORD_ MajorOperatingSystemVersion;
        details::WORD_ MinorOperatingSystemVersion;
        details::WORD_ MajorImageVersion;
        details::WORD_ MinorImageVersion;
        details::WORD_ MajorSubsystemVersion;
        details::WORD_ MinorSubsystemVersion;
        details::DWORD_ Win32VersionValue;
        details::DWORD_ SizeOfImage;
        details::DWORD_ SizeOfHeaders;
        details::DWORD_ CheckSum;
        details::WORD_ Subsystem;
        details::WORD_ DllCharacteristics;
        AddressOffsetT SizeOfStackReserve;
        AddressOffsetT SizeOfStackCommit;
        AddressOffsetT SizeOfHeapReserve;
        AddressOffsetT SizeOfHeapCommit;
        details::DWORD_ LoaderFlags;
        details::DWORD_ NumberOfRvaAndSizes;
        IMAGE_DATA_DIRECTORY_ DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES_];
      };

      typedef IMAGE_OPTIONAL_HEADER_template<details::DWORD_>
          IMAGE_OPTIONAL_HEADER32_;
      typedef IMAGE_OPTIONAL_HEADER_template<details::ULONGLONG_>
          IMAGE_OPTIONAL_HEADER64_;

      template <class AddressOffsetT>
      struct IMAGE_NT_HEADERS_template {
        details::DWORD_ Signature;
        IMAGE_FILE_HEADER_ FileHeader;
        IMAGE_OPTIONAL_HEADER_template<AddressOffsetT> OptionalHeader;
      };

      typedef IMAGE_NT_HEADERS_template<details::DWORD_> IMAGE_NT_HEADERS32_;
      typedef IMAGE_NT_HEADERS_template<details::ULONGLONG_>
          IMAGE_NT_HEADERS64_;

      template <class AddressOffsetT>
      class pe_info : public tfel::system::LibraryInformation::Implementation {
        std::shared_ptr<std::ifstream> f_;

        typedef IMAGE_NT_HEADERS_template<AddressOffsetT> header_t;
        typedef IMAGE_EXPORT_DIRECTORY_ exports_t;
        typedef IMAGE_SECTION_HEADER_ section_t;
        typedef IMAGE_DOS_HEADER_ dos_t;

        template <class T>
        inline void read_raw(T& value, std::size_t size = sizeof(T)) const {
          this->f_->read(reinterpret_cast<char*>(&value),
                         static_cast<std::streamsize>(size));
        }

       public:
        static bool parsing_supported(std::ifstream& f) {
          dos_t dos;
          f.seekg(0);
          f.read(reinterpret_cast<char*>(&dos), sizeof(dos));

          // 'MZ' and 'ZM' according to Wikipedia
          if (dos.e_magic != 0x4D5A && dos.e_magic != 0x5A4D) {
            return false;
          }

          header_t h;
          f.seekg(dos.e_lfanew);
          f.read(reinterpret_cast<char*>(&h), sizeof(h));

          return h.Signature == 0x00004550  // 'PE00'
                 &&
                 h.OptionalHeader.Magic ==
                     (sizeof(std::uint32_t) == sizeof(AddressOffsetT) ? 0x10B
                                                                      : 0x20B);
        }

        explicit pe_info(std::shared_ptr<std::ifstream>& f) noexcept : f_(f) {}

       private:
        inline header_t header() {
          header_t h;

          dos_t dos;
          this->f_->seekg(0);
          read_raw(dos);

          this->f_->seekg(dos.e_lfanew);
          read_raw(h);

          return h;
        }

        inline exports_t exports(const header_t& h) {
          exports_t e;

          static const unsigned int IMAGE_DIRECTORY_ENTRY_EXPORT_ = 0;
          const std::size_t exp_virtual_address =
              h.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT_]
                  .VirtualAddress;

          const std::size_t real_offset =
              get_file_offset(exp_virtual_address, h);
          assert(real_offset);

          this->f_->seekg(static_cast<std::streamoff>(real_offset));
          read_raw(e);

          return e;
        }

        std::size_t get_file_offset(std::size_t virtual_address,
                                    const header_t& h) {
          section_t image_section_header;

          {  // this->f_->seekg to the beginning on section headers
            dos_t dos;
            this->f_->seekg(0);
            read_raw(dos);
            this->f_->seekg(static_cast<std::streamoff>(
                static_cast<std::size_t>(dos.e_lfanew) + sizeof(header_t)));
          }

          for (std::size_t i = 0; i < h.FileHeader.NumberOfSections; ++i) {
            read_raw(image_section_header);
            if (virtual_address >= image_section_header.VirtualAddress &&
                virtual_address < image_section_header.VirtualAddress +
                                      image_section_header.SizeOfRawData) {
              return image_section_header.PointerToRawData + virtual_address -
                     image_section_header.VirtualAddress;
            }
          }

          return 0;
        }

       public:
        std::vector<std::string> sections() override {
          std::vector<std::string> ret;

          const header_t h = header();
          ret.reserve(h.FileHeader.NumberOfSections);

          // get names, e.g: .text .rdata .data .rsrc .reloc
          section_t image_section_header;
          char name_helper[section_t::IMAGE_SIZEOF_SHORT_NAME_ + 1];
          std::memset(name_helper, 0, sizeof(name_helper));
          for (std::size_t i = 0; i < h.FileHeader.NumberOfSections; ++i) {
            // There is no terminating null character if the string is exactly
            // eight characters long
            read_raw(image_section_header);
            std::memcpy(name_helper, image_section_header.Name,
                        section_t::IMAGE_SIZEOF_SHORT_NAME_);

            if (name_helper[0] != '/') {
              ret.push_back(name_helper);
            } else {
              // For longer names, image_section_header.Name contains a slash
              // (/) followed by ASCII representation of a decimal number. this
              // number is an offset into the string table.
              // TODO: fixme
              ret.push_back(name_helper);
            }
          }

          return ret;
        }

        std::vector<std::string> symbols() override {
          std::vector<std::string> ret;

          const header_t h = header();
          const exports_t exprt = exports(h);
          const std::size_t exported_symbols = exprt.NumberOfNames;
          const std::size_t fixed_names_addr =
              get_file_offset(exprt.AddressOfNames, h);

          ret.reserve(exported_symbols);
          details::DWORD_ name_offset;
          std::string symbol_name;
          for (std::size_t i = 0; i < exported_symbols; ++i) {
            this->f_->seekg(static_cast<std::streamoff>(
                fixed_names_addr + i * sizeof(name_offset)));
            read_raw(name_offset);
            this->f_->seekg(
                static_cast<std::streamoff>(get_file_offset(name_offset, h)));
            std::getline(*(this->f_), symbol_name, '\0');
            ret.push_back(symbol_name);
          }

          return ret;
        }

        std::vector<std::string> symbols(const char* section_name) override {
          std::vector<std::string> ret;

          const header_t h = header();

          std::size_t section_begin_addr = 0;
          std::size_t section_end_addr = 0;

          {  // getting address range for the section
            section_t image_section_header;
            char name_helper[section_t::IMAGE_SIZEOF_SHORT_NAME_ + 1];
            std::memset(name_helper, 0, sizeof(name_helper));
            for (std::size_t i = 0; i < h.FileHeader.NumberOfSections; ++i) {
              // There is no terminating null character if the string is exactly
              // eight characters long
              read_raw(image_section_header);
              std::memcpy(name_helper, image_section_header.Name,
                          section_t::IMAGE_SIZEOF_SHORT_NAME_);
              if (!std::strcmp(section_name, name_helper)) {
                section_begin_addr = image_section_header.PointerToRawData;
                section_end_addr =
                    section_begin_addr + image_section_header.SizeOfRawData;
              }
            }

            // returning empty result if section was not found
            if (section_begin_addr == 0 || section_end_addr == 0) return ret;
          }

          const exports_t exprt = exports(h);
          const std::size_t exported_symbols = exprt.NumberOfFunctions;
          const std::size_t fixed_names_addr =
              get_file_offset(exprt.AddressOfNames, h);
          const std::size_t fixed_ordinals_addr =
              get_file_offset(exprt.AddressOfNameOrdinals, h);
          const std::size_t fixed_functions_addr =
              get_file_offset(exprt.AddressOfFunctions, h);

          ret.reserve(exported_symbols);
          details::DWORD_ ptr;
          details::WORD_ ordinal;
          std::string symbol_name;
          for (std::size_t i = 0; i < exported_symbols; ++i) {
            // getting ordinal
            this->f_->seekg(static_cast<std::streamoff>(fixed_ordinals_addr +
                                                        i * sizeof(ordinal)));
            read_raw(ordinal);

            // getting function addr
            this->f_->seekg(static_cast<std::streamoff>(fixed_functions_addr +
                                                        ordinal * sizeof(ptr)));
            read_raw(ptr);
            ptr = static_cast<details::DWORD_>(get_file_offset(ptr, h));

            if (ptr >= section_end_addr || ptr < section_begin_addr) {
              continue;
            }

            this->f_->seekg(static_cast<std::streamoff>(fixed_names_addr +
                                                        i * sizeof(ptr)));
            read_raw(ptr);
            this->f_->seekg(
                static_cast<std::streamoff>(get_file_offset(ptr, h)));
            std::getline(*(this->f_), symbol_name, '\0');
            ret.push_back(symbol_name);
          }

          return ret;
        }

        // a test method to get dependents modules,
        // who my plugin imports (1st level only)
        /*
          e.g. for myself I get:
          KERNEL32.dll
          MSVCP110D.dll
          boost_system-vc-mt-gd-1_56.dll
          MSVCR110D.dll
        */
        /*
          std::vector<std::string> depend_of(std::error_code &ec) BOOST_NOEXCEPT
          { std::vector<std::string> ret;

          IMAGE_DOS_HEADER* image_dos_header = (IMAGE_DOS_HEADER*)native();
          if(!image_dos_header) {
          // ERROR_BAD_EXE_FORMAT
          ec = std::error_code(
          std::errc::executable_format_error,
          std::generic_category()
          );

          return ret;
          }

          IMAGE_OPTIONAL_HEADER* image_optional_header =
          (IMAGE_OPTIONAL_HEADER*)((details::BYTE_*)native() +
          image_dos_header->e_lfanew + 24); if(!image_optional_header) {
          // ERROR_BAD_EXE_FORMAT
          ec = std::error_code(
          std::errc::executable_format_error,
          std::generic_category()
          );

          return ret;
          }

          IMAGE_IMPORT_DESCRIPTOR* image_import_descriptor =
          (IMAGE_IMPORT_DESCRIPTOR*)((details::BYTE_*)native() +
          image_optional_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
          if(!image_import_descriptor) {
          // ERROR_BAD_EXE_FORMAT
          ec = std::error_code(
          std::errc::executable_format_error,
          std::generic_category()
          );

          return ret;
          }

          while(image_import_descriptor->FirstThunk) {
          std::string module_name =
          reinterpret_cast<char*>((details::BYTE_*)native() +
          image_import_descriptor->Name);

          if(module_name.size()) {
          ret.push_back(module_name);
          }

          image_import_descriptor++;
          }

          return ret;
          }
        */
      };

      typedef pe_info<details::DWORD_> pe_info32;
      typedef pe_info<details::ULONGLONG_> pe_info64;

    }  // namespace details

  }  // namespace system

}  // namespace tfel
