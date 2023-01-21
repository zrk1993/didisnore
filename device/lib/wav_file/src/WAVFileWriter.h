#pragma once

#include <SD.h>
#include <stdio.h>
#include "WAVFile.h"

class WAVFileWriter
{
private:
  int m_file_size;

  File m_fp;
  wav_header_t m_header;

public:
  WAVFileWriter(File fp, int sample_rate);
  void start();
  void write(uint8_t *samples, int count);
  void finish();
};
