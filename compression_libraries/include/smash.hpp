/*
 * rCUDA: remote CUDA (www.rCUDA.net)
 * Copyright (C) 2016-2022
 * Grupo de Arquitecturas Paralelas
 * Departamento de Informática de Sistemas y Computadores
 * Universidad Politécnica de Valencia (Spain)
 */

#pragma once

#include <iostream>
#include <string>

// SMASH LIBRARIES
#include <compression_library.hpp>
#include <options.hpp>

class Smash {
 private:
  CompressionLibrary *lib;

 public:
  void getCompressDataSize(uint64_t uncompress_size, uint64_t *compress_size);

  void compress(const Options &opt, char *uncompress_data,
                uint64_t uncompress_size, char *compress_data,
                uint64_t *compress_size);

  void decompress(char *compress_data, uint64_t compress_size,
                  char *decompress_data, uint64_t *decompress_size);

  bool compareData(char *uncompress_data, const uint64_t &uncompress_size,
                   char *decompress_data, const uint64_t &decompress_size);

  explicit Smash(const std::string &compression_library_name);

  ~Smash();
};
