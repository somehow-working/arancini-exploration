#pragma once

#include <cstdint>
#include <fstream>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdio>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>


enum class bin_file_type : uint8_t { unk = 0, elf, pe };    //maybe later also mach-o?

struct unmanaged_file {
    unmanaged_file(const unmanaged_file &) = delete;
    unmanaged_file(unmanaged_file &&) = delete;
    unmanaged_file &operator=(unmanaged_file) = delete;

    unmanaged_file(const std::string &filename)
        : fd_(-1), size_(0), has_size_(false) {
        fd_ = open(filename.c_str(), O_RDONLY);

        if (fd_ < 0) {
            throw std::runtime_error("unable to open file for reading '" +
                                     filename + "'");
        }
    }

    ~unmanaged_file() { close(fd_); }

    off_t size() const {
        if (!has_size_) {
            struct stat64 st;
            if (fstat64(fd_, &st) < 0) {
                throw std::runtime_error("unable to stat file");
            }

            size_ = st.st_size;
            has_size_ = true;
        }

        return size_;
    }

    void *map() {
        void *ptr = mmap(nullptr, size(), PROT_READ, MAP_PRIVATE, fd_, 0);

        if (ptr == MAP_FAILED) {
            throw std::runtime_error("unable to map file");
        }

        return ptr;
    }

  private:
    int fd_;
    mutable off_t size_;
    mutable bool has_size_;
};

class bin_reader {
  public:
    bin_reader(const std::string &filename)
    : data_(nullptr), data_size_(0), entrypoint_(0) {
        unmanaged_file f(filename);

        data_size_ = f.size();
        data_ = f.map();
    }
    ~bin_reader() { munmap(data_, data_size_); }

    virtual void parse() =0;

    virtual off_t get_entrypoint() const { return entrypoint_; }

    static bin_file_type get_bin_type(const std::string& filename) {
        std::ifstream file(filename, std::ios_base::in | std::ios_base::binary);
        if(file.is_open()) {
            uint32_t magic;
            if(file.read(reinterpret_cast<char*>(&magic), sizeof(magic))) {
                if(magic == 0x464c457f) {
                    return bin_file_type::elf;
                }   else if ((magic & 65535) == 0x5A4D) {  //only look at first 2 bytes read (0 out high 2 bytes)
                    //read msdos magic number
                    return bin_file_type::pe;
                }
            }
        }

        //cannot open/read file or unknown magic number...
        return bin_file_type::unk;
    }

  private:
    void *data_;
    size_t data_size_;

  protected:
    off_t entrypoint_;

#if __cplusplus > 202002L
    constexpr const void *get_data_ptr(off_t offset) const {
        return (const void *)((uintptr_t)data_ + offset);
    }
#else
    const void *get_data_ptr(off_t offset) const {
        return (const void *)((uintptr_t)data_ + offset);
    }
#endif

    template <typename T> constexpr T read(off_t offset) const {
        return *(const T *)get_data_ptr(offset);
    }

    uint8_t read8(off_t offset) const { return read<uint8_t>(offset); }
    uint16_t read16(off_t offset) const { return read<uint16_t>(offset); }
    uint32_t read32(off_t offset) const { return read<uint32_t>(offset); }
    uint64_t read64(off_t offset) const { return read<uint64_t>(offset); }

    std::string readstr(off_t offset) const {
        return std::string((const char *)get_data_ptr(offset));
    }
};

