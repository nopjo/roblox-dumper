#pragma once
#include "dumper/dumper.h"
#include <chrono>
#include <string>
#include <vector>

namespace dumper::writer {

    class IWriter {
      public:
        virtual ~IWriter() = default;

        auto write(const std::string& filename, std::chrono::milliseconds elapsed_time) -> bool;

      protected:
        virtual auto get_file_extension() -> std::string = 0;
        virtual auto generate_content() -> std::string = 0;
        virtual auto generate_header_comment(std::chrono::milliseconds elapsed_time) -> std::string;

        auto get_sorted_namespaces()
            -> std::vector<std::pair<std::string, std::vector<dumper::OffsetEntry>>>;
        auto get_total_offset_count() -> size_t;
    };

    class HeaderWriter : public IWriter {
      protected:
        auto get_file_extension() -> std::string override { return ".h"; }
        auto generate_content() -> std::string override;
    };

    class JsonWriter : public IWriter {
      protected:
        auto get_file_extension() -> std::string override { return ".json"; }
        auto generate_content() -> std::string override;
        auto generate_header_comment(std::chrono::milliseconds elapsed_time)
            -> std::string override;
    };

    class PythonWriter : public IWriter {
      protected:
        auto get_file_extension() -> std::string override { return ".py"; }
        auto generate_content() -> std::string override;
        auto generate_header_comment(std::chrono::milliseconds elapsed_time)
            -> std::string override;
    };

    class CSharpWriter : public IWriter {
      protected:
        auto get_file_extension() -> std::string override { return ".cs"; }
        auto generate_content() -> std::string override;
    };

    inline HeaderWriter g_header_writer;
    inline JsonWriter g_json_writer;
    inline PythonWriter g_python_writer;
    inline CSharpWriter g_csharp_writer;
} // namespace dumper::writer