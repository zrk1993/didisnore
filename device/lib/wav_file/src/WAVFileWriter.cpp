#include <SD.h>
#include "esp_log.h"
#include "WAVFileWriter.h"

static const char *TAG = "WAV";

WAVFileWriter::WAVFileWriter(File fp, int sample_rate)
{
  m_fp = fp;
  m_header.sample_rate = sample_rate;
  // write out the header - we'll fill in some of the blanks later
  //fwrite(&m_header, sizeof(wav_header_t), 1, m_fp);
  fp.write((byte*)&m_header, sizeof(wav_header_t));
  m_file_size = sizeof(wav_header_t);
}

void WAVFileWriter::write(uint8_t *samples, int count)
{
  // write the samples and keep track of the file size so far
  m_fp.write(samples, count);
  //fwrite(samples, sizeof(int16_t), count, m_fp);
  m_file_size += sizeof(uint8_t) * count;
}

void WAVFileWriter::finish()
{
  Serial.printf("\nFinishing wav file size: %d", m_file_size);
  // now fill in the header with the correct information and write it again
  m_header.data_bytes = m_file_size - sizeof(wav_header_t);
  m_header.wav_size = m_file_size - 8;
  // fseek(m_fp, 0, SEEK_SET);
  // fwrite(&m_header, sizeof(wav_header_t), 1, m_fp);
  m_fp.seek(0);
  m_fp.write((byte*)&m_header, sizeof(wav_header_t));
}