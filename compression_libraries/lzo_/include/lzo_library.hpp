/*
 * rCUDA: remote CUDA (www.rCUDA.net)
 * Copyright (C) 2016-2022
 * Grupo de Arquitecturas Paralelas
 * Departamento de Informática de Sistemas y Computadores
 * Universidad Politécnica de Valencia (Spain)
 */

#pragma once

#include <lzo1.h>
#include <lzo1a.h>
#include <lzo1b.h>
#include <lzo1c.h>
#include <lzo1f.h>
#include <lzo1x.h>
#include <lzo1y.h>
#include <lzo1z.h>
#include <lzoconf.h>

#include <iostream>
#include <string>
#include <vector>

// SMASH LIBRARIES
#include <compression_library.hpp>
#include <options.hpp>
class LzoLibrary : public CompressionLibrary {
 private:
  uint8_t number_of_modes_;
  std::string *modes_;

 private:
  uint64_t compression_work_memory_size_;
  int (*LZO_compress_)(const lzo_bytep, lzo_uint, lzo_bytep, lzo_uintp,
                       lzo_voidp);

  uint64_t decompression_work_memory_size_;
  int (*LZO_decompress_)(const lzo_bytep, lzo_uint, lzo_bytep, lzo_uintp,
                         lzo_voidp);

  void GetFunctions(const uint8_t &mode, const uint8_t &compression_level);

 public:
  bool CheckOptions(Options options);

  bool SetOptions(Options options);

  void GetCompressedDataSize(uint64_t uncompressed_size,
                             uint64_t *compressed_size);

  bool Compress(char *uncompressed_data, uint64_t uncompressed_size,
                char *compressed_data, uint64_t *compressed_size);

  void GetDecompressedDataSize(char *compressed_data, uint64_t compressed_size,
                               uint64_t *decompressed_size);

  bool Decompress(char *compressed_data, uint64_t compressed_size,
                  char *decompressed_data, uint64_t *decompressed_size);

  void GetTitle();

  bool GetCompressionLevelInformation(
      uint8_t *minimum_level = nullptr, uint8_t *maximum_level = nullptr,
      std::vector<std::string> *compression_level_information = nullptr);

  bool GetWindowSizeInformation(
      uint32_t *minimum_size = nullptr, uint32_t *maximum_size = nullptr,
      std::vector<std::string> *window_size_information = nullptr);

  bool GetModeInformation(uint8_t *minimum_mode = nullptr,
                          uint8_t *maximum_mode = nullptr,
                          std::vector<std::string> *mode_information = nullptr);

  bool GetWorkFactorInformation(
      uint8_t *minimum_factor = nullptr, uint8_t *maximum_factor = nullptr,
      std::vector<std::string> *work_factor_information = nullptr);

  bool GetShuffleInformation(
      uint8_t *minimum_shuffle = nullptr, uint8_t *maximum_shuffle = nullptr,
      std::vector<std::string> *shuffle_information = nullptr);

  bool GetNumberThreadsInformation(
      uint8_t *minimum_threads = nullptr, uint8_t *maximum_threads = nullptr,
      std::vector<std::string> *number_threads_information = nullptr);

  bool GetBackReferenceBitsInformation(
      uint8_t *minimum_bits = nullptr, uint8_t *maximum_bits = nullptr,
      std::vector<std::string> *back_reference_information = nullptr);

  std::string GetModeName(const uint8_t &mode);

  std::string GetShuffleName(const uint8_t &shuffle);

  LzoLibrary();
  ~LzoLibrary();
};
