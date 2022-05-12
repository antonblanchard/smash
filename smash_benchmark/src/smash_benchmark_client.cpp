/*
 * rCUDA: remote CUDA (www.rCUDA.net)
 * Copyright (C) 2016-2022
 * Grupo de Arquitecturas Paralelas
 * Departamento de Informática de Sistemas y Computadores
 * Universidad Politécnica de Valencia (Spain)
 */

#include <math.h>
#include <string.h>

#include <algorithm>
#include <chrono>  // NOLINT
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

// SMASH LIBRARIES
#include <compression_libraries.hpp>
#include <network_client.hpp>
#include <options.hpp>
#include <result.hpp>
#include <smash.hpp>
#include <smash_network.hpp>
#include <utils.hpp>

bool SetUnpackedMemory(std::string input_file_name, char **data, uint64_t *size,
                       NetworkClient *net) {
  bool result{true};
  std::ifstream src(input_file_name, std::ios::binary);
  if (!src.is_open()) {
    std::cout << "ERROR: The file does not exists" << std::endl;
    result = false;
  } else {
    src.seekg(0, std::ios::end);
    *size = src.tellg();
    src.seekg(0, std::ios::beg);

    *data = new char[*size];
    src.read(*data, *size);
    src.close();
    result = net->SendBuffer(reinterpret_cast<char *>(size), sizeof(*size));
    if (result) {
      result = net->SendBuffer(*data, *size);
    }
  }
  return result;
}

bool SetMemories(std::string input_file_name, char **uncompressed_data,
                 uint64_t *uncompressed_size, char **compressed_data,
                 uint64_t *compressed_size, NetworkClient *net) {
  bool result = SetUnpackedMemory(input_file_name, uncompressed_data,
                                  uncompressed_size, net);

  if (*uncompressed_size < 2500) {
    *compressed_size = 5000;
  } else {
    *compressed_size = *uncompressed_size * 2;
  }
  *compressed_data = new char[*compressed_size];
  memset(*compressed_data, '\0', sizeof(char) * (*compressed_size));
  return result;
}

void RemoveMemories(char *uncompressed_data, char *compressed_data) {
  if (uncompressed_data) {
    delete[] uncompressed_data;
    uncompressed_data = nullptr;
  }
  if (compressed_data) {
    delete[] compressed_data;
    compressed_data = nullptr;
  }
}

std::vector<std::string> GetLibraries(const std::string &library_name) {
  std::vector<std::string> libraries;
  if (!library_name.compare("all")) {
    libraries = CompressionLibraries().GetNameLibraries();
  } else {
    libraries.push_back(library_name);
  }
  return libraries;
}

std::vector<Options> GetOptions(const Options &option, const bool &all_options,
                                Smash *library) {
  std::vector<Options> options;
  if (all_options) {
    Utils::GetAllOptions(library, &options);
  } else {
    options.clear();
    options.push_back(option);
  }
  return options;
}

inline void InitializeTime(std::chrono::_V2::system_clock::time_point *start) {
  *start = std::chrono::system_clock::now();
}

inline void GetDuration(const std::chrono::_V2::system_clock::time_point &start,
                        std::chrono::duration<double> *time) {
  std::chrono::_V2::system_clock::time_point end =
      std::chrono::system_clock::now();
  *time = end - start;
}

void GetInformation(const uint32_t &repetitions,
                    std::vector<double> compression_results,
                    std::vector<double> transfer_results,
                    std::vector<double> decompression_results,
                    std::vector<double> total_results, double *mean_compression,
                    double *mean_decompression, double *mean_transfer,
                    double *mean_total, double *error_compression,
                    double *error_transfer, double *error_decompression,
                    double *error_total, const bool &pipeline) {
  if (!pipeline) {
    sort(compression_results.begin(), compression_results.end(),
         [](double &x, double &y) { return x > y; });
    sort(transfer_results.begin(), transfer_results.end(),
         [](double &x, double &y) { return x > y; });
    sort(decompression_results.begin(), decompression_results.end(),
         [](double &x, double &y) { return x > y; });
  }
  sort(total_results.begin(), total_results.end(),
       [](double &x, double &y) { return x > y; });
  if (repetitions > 1) {
    uint32_t i = 0;
    // All results are added up discarding 20%
    for (; (i < (total_results.size() / 1.2)) ||
           ((total_results.size() == 1) && (i == 0));
         ++i) {
      if (!pipeline) {
        *mean_compression += compression_results[i];
        *mean_transfer += transfer_results[i];
        *mean_decompression += decompression_results[i];
      }
      *mean_total += total_results[i];
    }
    // The means are obtained
    if (!pipeline) {
      *mean_compression /= i;
      *mean_transfer /= i;
      *mean_decompression /= i;
    }
    *mean_total /= i;
    // First step to obtain typical deviation
    for (uint32_t j = 0; j < i; ++j) {
      if (!pipeline) {
        *error_compression +=
            pow((compression_results[j] - *mean_compression), 2);
        *error_transfer += pow((transfer_results[j] - *mean_transfer), 2);
        *error_decompression +=
            pow((decompression_results[j] - *mean_decompression), 2);
      }
      *error_total += pow((total_results[j] - *mean_total), 2);
    }
    // Second step to obtain typical deviation
    --i;
    if (i > 0) {
      if (!pipeline) {
        *error_compression = sqrt(*error_compression / i);
        *error_transfer = sqrt(*error_transfer / i);
        *error_decompression = sqrt(*error_decompression / i);
      }
      *error_total = sqrt(*error_total / i);
    }
  } else {
    if (!pipeline) {
      *mean_compression += compression_results[0];
      *mean_transfer += transfer_results[0];
      *mean_decompression += decompression_results[0];
    }
    *mean_total += total_results[0];
  }
}

bool SendInformationToServer(
    const int &port, const std::string &address, const Options &options,
    const std::string &library_name, const bool &all_options,
    const uint8_t &best_options, const uint32_t &best_result_number,
    const uint32_t &repetitions, NetworkClient *net, const bool &pipeline) {
  bool status = true;
  status = net->CreateConnection(port, address);
  if (status) {
    status = net->SendBuffer(reinterpret_cast<const char *>(&options),
                             sizeof(options));
    if (status) {
      uint8_t len = library_name.length();
      status = net->SendBuffer(reinterpret_cast<char *>(&len), sizeof(len));
      if (status) {
        status = net->SendBuffer(library_name.data(), len);
        if (status) {
          status = net->SendBuffer(reinterpret_cast<const char *>(&all_options),
                                   sizeof(all_options));
          if (status) {
            status =
                net->SendBuffer(reinterpret_cast<const char *>(&best_options),
                                sizeof(best_options));
            if (status) {
              status = net->SendBuffer(
                  reinterpret_cast<const char *>(&best_result_number),
                  sizeof(best_result_number));
              if (status) {
                status = net->SendBuffer(
                    reinterpret_cast<const char *>(&repetitions),
                    sizeof(repetitions));
                if (status) {
                  status =
                      net->SendBuffer(reinterpret_cast<const char *>(&pipeline),
                                      sizeof(pipeline));
                }
              }
            }
          }
        }
      }
    }
  }
  return status;
}

bool CompressAndSend(char *uncompressed_data, const uint64_t &uncompressed_size,
                     char *compressed_data, uint64_t *compressed_size,
                     const std::string &library_name, Smash *lib,
                     NetworkClient *net, Options *option,
                     std::vector<double> *compression_results,
                     std::vector<double> *transfer_results,
                     std::vector<double> *decompression_results,
                     std::vector<double> *total_results) {
  bool status{true};
  std::chrono::_V2::system_clock::time_point start;
  std::chrono::duration<double> compression_time, transfer_time;
  double decompression_time{0};
  // Set Compression Options
  status = lib->SetOptionsCompressor(option);
  // Get compressed Data Size
  lib->GetCompressedDataSize(uncompressed_data, uncompressed_size,
                             compressed_size);
  if (status) {
    InitializeTime(&start);
    // Compress Data
    status = lib->Compress(uncompressed_data, uncompressed_size,
                           compressed_data, compressed_size);
    GetDuration(start, &compression_time);
    if (status) {
      // Transfers Data
      InitializeTime(&start);
      // First, data size is sent
      status = net->SendBuffer(reinterpret_cast<char *>(compressed_size),
                               sizeof(*compressed_size));
      // Second, the data is sent
      status = net->SendBuffer(compressed_data, *compressed_size, true);
      GetDuration(start, &transfer_time);
      if (status) {
        if (status = net->RecvACK()) {
          status =
              net->RecvBuffer(reinterpret_cast<char *>(&decompression_time),
                              sizeof(decompression_time));
          if (status) {
            compression_results->push_back(
                (static_cast<double>(uncompressed_size) / 1000000.0) /
                compression_time.count());
            transfer_results->push_back(
                (static_cast<double>(*compressed_size) / 1000000.0) /
                transfer_time.count());
            decompression_results->push_back(
                (static_cast<double>(*compressed_size) / 1000000.0) /
                decompression_time);
            total_results->push_back(
                (static_cast<double>(uncompressed_size) / 1000000.0) /
                (compression_time.count() + transfer_time.count() +
                 decompression_time));
          } else {
            std::cout << "ERROR: " << library_name
                      << " when decompressed data is transfered" << std::endl;
          }
        } else {
          std::cout << "ERROR: " << library_name
                    << " does not obtain the correct data" << std::endl;
        }
      } else {
        std::cout << "ERROR: " << library_name << " when data is transfered"
                  << std::endl;
      }
    } else {
      std::cout << "ERROR: " << library_name
                << " does not obtain the correct data" << std::endl;
    }
  } else {
    std::cout << "ERROR: " << library_name << " does not set options correctly"
              << std::endl;
  }
  return status;
}

bool CompressAndSendPipelined(
    char *uncompressed_data, const uint64_t &uncompressed_size,
    uint64_t *compressed_size, const std::string &library_name,
    const Options &option, NetworkClient *net, const uint8_t &threads,
    const uint32_t &chunk_size, const uint16_t &number_of_chunks,
    std::vector<double> *total_results) {
  bool status{true};
  SmashNetwork smash(library_name, option, threads, chunk_size,
                     number_of_chunks);
  smash.Connect(net->socket_id);
  std::chrono::_V2::system_clock::time_point start;
  std::chrono::duration<double> total_time;
  uint64_t remaining_data = uncompressed_size;
  InitializeTime(&start);
  smash.Write_v1(uncompressed_data, uncompressed_size, compressed_size, true);
  GetDuration(start, &total_time);
  if (status) {
    status = net->RecvACK();
    if (status) {
      total_results->push_back(
          (static_cast<double>(uncompressed_size) / 1000000.0) /
          total_time.count());
    } else {
      std::cout << "ERROR: " << library_name
                << " does not obtain the correct data" << std::endl;
    }
  } else {
    std::cout << "ERROR: " << library_name << " does not decompress correctly"
              << std::endl;
  }
  return status;
}

int main(int argc, char *argv[]) {
  Options opt;
  Smash *lib;
  std::string input_file_name;
  std::string compression_library_name;
  uint64_t uncompressed_size{0};
  std::vector<Options> options;
  std::vector<Result> results;
  uint8_t best_options{0};
  uint32_t best_result_number{0};
  uint32_t repetitions{1};
  bool all_options{false};
  int result{EXIT_FAILURE};
  int port{-1};
  std::string address;
  NetworkClient *net{nullptr};
  bool pipeline{false};
  uint8_t pipeline_threads{1};
  uint32_t pipeline_chunk_size{1024};
  uint16_t pipeline_number_of_chunks{2};
  if (Utils::GetParamsClient(
          argc, argv, &opt, &input_file_name, &compression_library_name,
          &all_options, &best_options, &best_result_number, &repetitions, &port,
          &address, &pipeline, &pipeline_threads, &pipeline_chunk_size,
          &pipeline_number_of_chunks)) {
    net = new NetworkClient();
    if (SendInformationToServer(port, address, opt, compression_library_name,
                                all_options, best_options, best_result_number,
                                repetitions, net, pipeline)) {
      std::vector<std::string> libraries =
          GetLibraries(compression_library_name);
      for (auto &library_name : libraries) {
        lib = new Smash(library_name);
        options = GetOptions(opt, all_options, lib);
        for (auto &option : options) {
          char *uncompressed_data{nullptr}, *compressed_data{nullptr};
          uint64_t compressed_size{0};
          if (SetMemories(input_file_name, &uncompressed_data,
                          &uncompressed_size, &compressed_data,
                          &compressed_size, net)) {
            result = EXIT_SUCCESS;
            std::vector<double> compression_results, transfer_results,
                decompression_results, total_results;
            for (uint32_t r = 0; r < repetitions && result == EXIT_SUCCESS;
                 ++r) {
              if (result == EXIT_SUCCESS) {
                if (!pipeline) {
                  result =
                      CompressAndSend(uncompressed_data, uncompressed_size,
                                      compressed_data, &compressed_size,
                                      library_name, lib, net, &option,
                                      &compression_results, &transfer_results,
                                      &decompression_results, &total_results)
                          ? EXIT_SUCCESS
                          : EXIT_FAILURE;
                } else {
                  result = CompressAndSendPipelined(
                               uncompressed_data, uncompressed_size,
                               &compressed_size, library_name, option, net,
                               pipeline_threads, pipeline_chunk_size,
                               pipeline_number_of_chunks, &total_results)
                               ? EXIT_SUCCESS
                               : EXIT_FAILURE;
                }
              }
            }
            if (result == EXIT_SUCCESS) {
              if (pipeline) {
                lib->SetOptionsCompressor(&option) ? EXIT_SUCCESS
                                                   : EXIT_FAILURE;
              }
              double mean_compression{0}, mean_transfer{0},
                  mean_decompression{0}, mean_total{0}, error_compression{0},
                  error_transfer{0}, error_decompression{0}, error_total{0};

              GetInformation(repetitions, compression_results, transfer_results,
                             decompression_results, total_results,
                             &mean_compression, &mean_transfer,
                             &mean_decompression, &mean_total,
                             &error_compression, &error_transfer,
                             &error_decompression, &error_total, pipeline);
              std::string message = Utils::ShowResult(
                  lib, library_name, uncompressed_size, compressed_size,
                  mean_compression, error_compression, mean_transfer,
                  error_transfer, mean_decompression, error_decompression,
                  mean_total, error_total, pipeline);
              results.push_back(Result(
                  message, uncompressed_size, compressed_size, mean_compression,
                  mean_transfer, mean_decompression, mean_total, best_options));
              result = EXIT_SUCCESS;
            }
          }
          RemoveMemories(uncompressed_data, compressed_data);
        }
        delete lib;
      }
      if (!results.empty()) {
        Utils::ShowTitle(uncompressed_size, repetitions, true, pipeline);
        if (!best_options) {
          best_result_number = results.size();
        } else {
          std::sort(results.begin(), results.end());
        }
        while (best_result_number && !results.empty()) {
          std::cout << results.back().message_;
          results.pop_back();
          --best_result_number;
        }
      }
      delete net;
    } else {
      std::cout << "Error sending options to server" << std::endl;
      result = EXIT_FAILURE;
    }
  }
  return result;
}