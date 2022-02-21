/*
 * rCUDA: remote CUDA (www.rCUDA.net)
 * Copyright (C) 2016-2022
 * Grupo de Arquitecturas Paralelas
 * Departamento de Informática de Sistemas y Computadores
 * Universidad Politécnica de Valencia (Spain)
 */

#include <lzo/lzo1.h>
#include <lzo/lzo1a.h>
#include <lzo/lzo1b.h>
#include <lzo/lzo1c.h>
#include <lzo/lzo1f.h>
#include <lzo/lzo1x.h>
#include <lzo/lzo1y.h>
#include <lzo/lzo1z.h>
#include <lzo/lzoconf.h>

// SMASH LIBRARIES
#include <lzo_library.hpp>
#include <options.hpp>

bool LzoLibrary::CheckOptions(Options options, const bool &compressor) {
  bool result{true};
  result = CompressionLibrary::CheckMode("lzo", options.GetMode(), 0, 7);
  if (compressor && result) {
    switch (options.GetMode()) {
      case 0:
        result = CompressionLibrary::CheckCompressionLevel(
            "mode value 0 in lzo", options.GetCompressionLevel(), 0, 1);
        break;
      case 1:
        result = CompressionLibrary::CheckCompressionLevel(
            "mode value 1 in lzo", options.GetCompressionLevel(), 0, 1);
        break;
      case 2:
        result = CompressionLibrary::CheckCompressionLevel(
            "mode value 2 in lzo", options.GetCompressionLevel(), 0, 10);
        break;
      case 3:
        result = CompressionLibrary::CheckCompressionLevel(
            "mode value 3 in lzo", options.GetCompressionLevel(), 0, 10);
        break;
      case 4:
        result = CompressionLibrary::CheckCompressionLevel(
            "mode value 4 in lzo", options.GetCompressionLevel(), 0, 1);
        break;
      case 5:
        result = CompressionLibrary::CheckCompressionLevel(
            "mode value 5 in lzo", options.GetCompressionLevel(), 0, 4);
        break;
      case 6:
        result = CompressionLibrary::CheckCompressionLevel(
            "mode value 6 in lzo", options.GetCompressionLevel(), 0, 1);
        break;
      default:
        break;
    }
  }
  return result;
}

void LzoLibrary::GetFunctions(const uint8_t &mode,
                              const uint8_t &compression_level) {
  switch (mode) {
    case 0: {
      decompression_work_memory_size_ = LZO1_MEM_DECOMPRESS;
      LZO_decompress_ = lzo1_decompress;
      switch (compression_level) {
        case 0: {
          compression_work_memory_size_ = LZO1_MEM_COMPRESS;
          LZO_compress_ = lzo1_compress;
          break;
        }
        case 1: {
          compression_work_memory_size_ = LZO1_99_MEM_COMPRESS;
          LZO_compress_ = lzo1_99_compress;
          break;
        }
        default:
          break;
      }
      break;
    }
    case 1: {
      decompression_work_memory_size_ = LZO1A_MEM_DECOMPRESS;
      LZO_decompress_ = lzo1a_decompress;
      switch (compression_level) {
        case 0: {
          compression_work_memory_size_ = LZO1A_MEM_COMPRESS;
          LZO_compress_ = lzo1a_compress;
          break;
        }
        case 1: {
          compression_work_memory_size_ = LZO1A_99_MEM_COMPRESS;
          LZO_compress_ = lzo1a_99_compress;
          break;
        }
        default:
          break;
      }
      break;
    }
    case 2: {
      decompression_work_memory_size_ = LZO1B_MEM_DECOMPRESS;
      LZO_decompress_ = lzo1b_decompress_safe;
      switch (compression_level) {
        case 0: {
          compression_work_memory_size_ = LZO1B_MEM_COMPRESS;
          LZO_compress_ = lzo1b_1_compress;
          break;
        }
        case 1: {
          compression_work_memory_size_ = LZO1B_MEM_COMPRESS;
          LZO_compress_ = lzo1b_2_compress;
          break;
        }
        case 2: {
          compression_work_memory_size_ = LZO1B_MEM_COMPRESS;
          LZO_compress_ = lzo1b_3_compress;
          break;
        }
        case 3: {
          compression_work_memory_size_ = LZO1B_MEM_COMPRESS;
          LZO_compress_ = lzo1b_4_compress;
          break;
        }
        case 4: {
          compression_work_memory_size_ = LZO1B_MEM_COMPRESS;
          LZO_compress_ = lzo1b_5_compress;
          break;
        }
        case 5: {
          compression_work_memory_size_ = LZO1B_MEM_COMPRESS;
          LZO_compress_ = lzo1b_6_compress;
          break;
        }
        case 6: {
          compression_work_memory_size_ = LZO1B_MEM_COMPRESS;
          LZO_compress_ = lzo1b_7_compress;
          break;
        }
        case 7: {
          compression_work_memory_size_ = LZO1B_MEM_COMPRESS;
          LZO_compress_ = lzo1b_8_compress;
          break;
        }
        case 8: {
          compression_work_memory_size_ = LZO1B_MEM_COMPRESS;
          LZO_compress_ = lzo1b_9_compress;
          break;
        }
        case 9: {
          compression_work_memory_size_ = LZO1B_99_MEM_COMPRESS;
          LZO_compress_ = lzo1b_99_compress;
          break;
        }
        case 10: {
          compression_work_memory_size_ = LZO1B_999_MEM_COMPRESS;
          LZO_compress_ = lzo1b_999_compress;
          break;
        }
        default:
          break;
      }
      break;
    }
    case 3: {
      decompression_work_memory_size_ = LZO1C_MEM_DECOMPRESS;
      LZO_decompress_ = lzo1c_decompress_safe;
      switch (compression_level) {
        case 0: {
          compression_work_memory_size_ = LZO1C_MEM_COMPRESS;
          LZO_compress_ = lzo1c_1_compress;
          break;
        }
        case 1: {
          compression_work_memory_size_ = LZO1C_MEM_COMPRESS;
          LZO_compress_ = lzo1c_2_compress;
          break;
        }
        case 2: {
          compression_work_memory_size_ = LZO1C_MEM_COMPRESS;
          LZO_compress_ = lzo1c_3_compress;
          break;
        }
        case 3: {
          compression_work_memory_size_ = LZO1C_MEM_COMPRESS;
          LZO_compress_ = lzo1c_4_compress;
          break;
        }
        case 4: {
          compression_work_memory_size_ = LZO1C_MEM_COMPRESS;
          LZO_compress_ = lzo1c_5_compress;
          break;
        }
        case 5: {
          compression_work_memory_size_ = LZO1C_MEM_COMPRESS;
          LZO_compress_ = lzo1c_6_compress;
          break;
        }
        case 6: {
          compression_work_memory_size_ = LZO1C_MEM_COMPRESS;
          LZO_compress_ = lzo1c_7_compress;
          break;
        }
        case 7: {
          compression_work_memory_size_ = LZO1C_MEM_COMPRESS;
          LZO_compress_ = lzo1c_8_compress;
          break;
        }
        case 8: {
          compression_work_memory_size_ = LZO1C_MEM_COMPRESS;
          LZO_compress_ = lzo1c_9_compress;
          break;
        }
        case 9: {
          compression_work_memory_size_ = LZO1C_99_MEM_COMPRESS;
          LZO_compress_ = lzo1c_99_compress;
          break;
        }
        case 10: {
          compression_work_memory_size_ = LZO1C_999_MEM_COMPRESS;
          LZO_compress_ = lzo1c_999_compress;
          break;
        }
        default:
          break;
      }
      break;
    }
    case 4: {
      decompression_work_memory_size_ = LZO1F_MEM_DECOMPRESS;
      LZO_decompress_ = lzo1f_decompress_safe;
      switch (compression_level) {
        case 0: {
          compression_work_memory_size_ = LZO1F_MEM_COMPRESS;
          LZO_compress_ = lzo1f_1_compress;
          break;
        }
        case 1: {
          compression_work_memory_size_ = LZO1F_999_MEM_COMPRESS;
          LZO_compress_ = lzo1f_999_compress;
          break;
        }
        default:
          break;
      }
      break;
    }
    case 5: {
      decompression_work_memory_size_ = LZO1X_MEM_DECOMPRESS;
      LZO_decompress_ = lzo1x_decompress_safe;
      switch (compression_level) {
        case 0: {
          compression_work_memory_size_ = LZO1X_1_MEM_COMPRESS;
          LZO_compress_ = lzo1x_1_compress;
          break;
        }
        case 1: {
          compression_work_memory_size_ = LZO1X_1_11_MEM_COMPRESS;
          LZO_compress_ = lzo1x_1_11_compress;
          break;
        }
        case 2: {
          compression_work_memory_size_ = LZO1X_1_12_MEM_COMPRESS;
          LZO_compress_ = lzo1x_1_12_compress;
          break;
        }
        case 3: {
          compression_work_memory_size_ = LZO1X_1_15_MEM_COMPRESS;
          LZO_compress_ = lzo1x_1_15_compress;
          break;
        }
        case 4: {
          compression_work_memory_size_ = LZO1X_999_MEM_COMPRESS;
          LZO_compress_ = lzo1x_999_compress;
          break;
        }
        default:
          break;
      }
      break;
    }
    case 6: {
      decompression_work_memory_size_ = LZO1Y_MEM_DECOMPRESS;
      LZO_decompress_ = lzo1y_decompress_safe;
      switch (compression_level) {
        case 0: {
          compression_work_memory_size_ = LZO1Y_MEM_COMPRESS;
          LZO_compress_ = lzo1y_1_compress;
          break;
        }
        case 1: {
          compression_work_memory_size_ = LZO1Y_999_MEM_COMPRESS;
          LZO_compress_ = lzo1y_999_compress;
          break;
        }
        default:
          break;
      }
      break;
    }
    case 7: {
      decompression_work_memory_size_ = LZO1Z_MEM_DECOMPRESS;
      LZO_decompress_ = lzo1z_decompress_safe;
      compression_work_memory_size_ = LZO1Z_999_MEM_COMPRESS;
      LZO_compress_ = lzo1z_999_compress;
      break;
    }
    default:
      break;
  }
}

bool LzoLibrary::SetOptionsCompressor(Options options) {
  if (initialized_decompressor_) initialized_decompressor_ = false;
  initialized_compressor_ = CheckOptions(options, true);
  if (initialized_compressor_) {
    options_ = options;
    GetFunctions(options_.GetMode(), options_.GetCompressionLevel());
  }
  return initialized_compressor_;
}

bool LzoLibrary::SetOptionsDecompressor(Options options) {
  if (initialized_compressor_) initialized_compressor_ = false;
  initialized_decompressor_ = CheckOptions(options, false);
  if (initialized_decompressor_) {
    options_ = options;
    GetFunctions(options_.GetMode(), options_.GetCompressionLevel());
  }
  return initialized_decompressor_;
}

void LzoLibrary::GetCompressedDataSize(uint64_t uncompressed_size,
                                       uint64_t *compressed_size) {
  // There is no way to obtain with Lzo
  *compressed_size = ((uncompressed_size / 5000) + 1) * 5000;
}

bool LzoLibrary::Compress(char *uncompressed_data, uint64_t uncompressed_size,
                          char *compressed_data, uint64_t *compressed_size) {
  bool result{initialized_compressor_};
  if (result) {
    char *work_memory{nullptr};
    int error{0};
    if (compression_work_memory_size_) {
      work_memory = new char[compression_work_memory_size_];
    }
    error = LZO_compress_(reinterpret_cast<unsigned char *>(uncompressed_data),
                          uncompressed_size,
                          reinterpret_cast<unsigned char *>(compressed_data),
                          compressed_size, work_memory);
    if (error != LZO_E_OK) {
      std::cout << "ERROR: lzo error when compress data" << std::endl;
      result = false;
    }
    if (work_memory) {
      delete[] work_memory;
    }
  }
  return result;
}

void LzoLibrary::GetDecompressedDataSize(char *compressed_data,
                                         uint64_t compressed_size,
                                         uint64_t *decompressed_size) {
  // There is no way to obtain with Lzo
}

bool LzoLibrary::Decompress(char *compressed_data, uint64_t compressed_size,
                            char *decompressed_data,
                            uint64_t *decompressed_size) {
  bool result{initialized_decompressor_};
  if (result) {
    char *work_memory{nullptr};
    int error{0};
    if (decompression_work_memory_size_) {
      work_memory = new char[decompression_work_memory_size_];
    }
    error = LZO_decompress_(
        reinterpret_cast<unsigned char *>(compressed_data), compressed_size,
        reinterpret_cast<unsigned char *>(decompressed_data), decompressed_size,
        work_memory);
    if (error != LZO_E_OK) {
      result = false;
      std::cout << "ERROR: lzo error when decompress data" << std::endl;
    }
    if (work_memory) {
      delete[] work_memory;
    }
  }
  return result;
}

void LzoLibrary::GetTitle() {
  CompressionLibrary::GetTitle("lzo", "A real-time data compression library");
}

bool LzoLibrary::GetCompressionLevelInformation(
    std::vector<std::string> *compression_level_information,
    uint8_t *minimum_level, uint8_t *maximum_level) {
  if (minimum_level) *minimum_level = 0;
  if (maximum_level) *maximum_level = 10;
  if (compression_level_information) {
    compression_level_information->clear();
    compression_level_information->push_back(
        "Available values depend on the mode:");
    compression_level_information->push_back("Mode " + modes_[0] + ": [0-1]");
    compression_level_information->push_back("Mode " + modes_[1] + ": [0-1]");
    compression_level_information->push_back("Mode " + modes_[2] + ": [0-10]");
    compression_level_information->push_back("Mode " + modes_[3] + ": [0-10]");
    compression_level_information->push_back("Mode " + modes_[4] + ": [0-1]");
    compression_level_information->push_back("Mode " + modes_[5] + ": [0-4]");
    compression_level_information->push_back("Mode " + modes_[6] + ": [0-1]");
    compression_level_information->push_back("Mode " + modes_[7] + ": [0]");
    compression_level_information->push_back("[compression]");
  }
  return true;
}

bool LzoLibrary::GetWindowSizeInformation(
    std::vector<std::string> *window_size_information, uint32_t *minimum_size,
    uint32_t *maximum_size) {
  if (minimum_size) *minimum_size = 0;
  if (maximum_size) *maximum_size = 0;
  if (window_size_information) window_size_information->clear();
  return false;
}

bool LzoLibrary::GetModeInformation(std::vector<std::string> *mode_information,
                                    uint8_t *minimum_mode,
                                    uint8_t *maximum_mode) {
  if (minimum_mode) *minimum_mode = 0;
  if (maximum_mode) *maximum_mode = 7;
  if (mode_information) {
    mode_information->clear();
    mode_information->push_back("Available values [0-7]");
    mode_information->push_back("0: " + modes_[0]);
    mode_information->push_back("1: " + modes_[1]);
    mode_information->push_back("2: " + modes_[2]);
    mode_information->push_back("3: " + modes_[3]);
    mode_information->push_back("4: " + modes_[4]);
    mode_information->push_back("5: " + modes_[5]);
    mode_information->push_back("6: " + modes_[6]);
    mode_information->push_back("7: " + modes_[7]);
    mode_information->push_back("[compression/decompression]");
  }
  return true;
}

bool LzoLibrary::GetWorkFactorInformation(
    std::vector<std::string> *work_factor_information, uint8_t *minimum_factor,
    uint8_t *maximum_factor) {
  if (minimum_factor) *minimum_factor = 0;
  if (maximum_factor) *maximum_factor = 0;
  if (work_factor_information) work_factor_information->clear();
  return false;
}

bool LzoLibrary::GetShuffleInformation(
    std::vector<std::string> *shuffle_information, uint8_t *minimum_shuffle,
    uint8_t *maximum_shuffle) {
  if (minimum_shuffle) *minimum_shuffle = 0;
  if (maximum_shuffle) *maximum_shuffle = 0;
  if (shuffle_information) shuffle_information->clear();
  return false;
}

bool LzoLibrary::GetNumberThreadsInformation(
    std::vector<std::string> *number_threads_information,
    uint8_t *minimum_threads, uint8_t *maximum_threads) {
  if (minimum_threads) *minimum_threads = 0;
  if (maximum_threads) *maximum_threads = 0;
  if (number_threads_information) number_threads_information->clear();
  return false;
}

bool LzoLibrary::GetBackReferenceBitsInformation(
    std::vector<std::string> *back_reference_information, uint8_t *minimum_bits,
    uint8_t *maximum_bits) {
  if (minimum_bits) *minimum_bits = 0;
  if (maximum_bits) *maximum_bits = 0;
  if (back_reference_information) back_reference_information->clear();
  return false;
}

std::string LzoLibrary::GetModeName(const uint8_t &mode) {
  std::string result = CompressionLibrary::GetDefaultModeName();
  if (mode < number_of_modes_) {
    result = modes_[mode];
  }
  return result;
}

std::string LzoLibrary::GetShuffleName(const uint8_t &shuffle) {
  return CompressionLibrary::GetDefaultShuffleName();
}

LzoLibrary::LzoLibrary() {
  compression_work_memory_size_ = 0;
  decompression_work_memory_size_ = 0;
  number_of_modes_ = 8;
  modes_ = new std::string[number_of_modes_];
  modes_[0] = "LZO1";
  modes_[1] = "LZO1a";
  modes_[2] = "LZO1b";
  modes_[3] = "LZO1c";
  modes_[4] = "LZO1f";
  modes_[5] = "LZO1x";
  modes_[6] = "LZO1y";
  modes_[7] = "LZO1z";
}

LzoLibrary::~LzoLibrary() { delete[] modes_; }
