#ifndef TEST_CONVERTINGREADERWRAPPERTEST_HPP_
#define TEST_CONVERTINGREADERWRAPPERTEST_HPP_

#include <utility>
#include <string>
#include "cute/cute.h"
#include "common/io/single_read.hpp"
#include "common/io/paired_read.hpp"
#include "common/io/reader.hpp"
#include "common/io/converting_reader_wrapper.hpp"

void TestConvertingReaderWrapperNoFile() {
  Reader<PairedRead> internal_reader(
      std::pair<std::string, std::string>("./no-file", "./no_file"));
  ConvertingReaderWrapper reader(&internal_reader);
  ASSERT(!reader.is_open());
}

void TestConvertingReaderWrapperReading() {
  Reader<PairedRead> internal_reader(
      std::pair<std::string, std::string>(
          "./test/data/s_test.fastq.gz",
          "./test/data/s_test_2.fastq.gz"));
  ConvertingReaderWrapper reader(&internal_reader);
  ASSERT(reader.is_open());
  ASSERT(!reader.eof());
  SingleRead read;
  reader >> read;
  ASSERT_EQUAL("EAS20_8_6_1_2_768/1", read.name());
  ASSERT_EQUAL("ATGCATGCATGC", read.GetSequenceString());
  ASSERT_EQUAL("HGHIHHHGHECH", read.GetPhredQualityString());
  reader >> read;
  ASSERT_EQUAL("!EAS20_8_6_1_2_1700/1", read.name());
  ASSERT_EQUAL("GTTTTTTTTTTT", read.GetSequenceString());
  ASSERT_EQUAL("GGEGGGGCGGGG", read.GetPhredQualityString());
  reader >> read;
  ASSERT_EQUAL("EAS20_8_6_1_2_1700/1", read.name());
  ASSERT_EQUAL("AAAAAAAAAAAC", read.GetSequenceString());
  ASSERT_EQUAL("GGGGCGGGGEGG", read.GetPhredQualityString());
  reader >> read;
  ASSERT_EQUAL("!EAS20_8_6_1_2_468/1", read.name());
  ASSERT_EQUAL("CACACACACACA", read.GetSequenceString());
  ASSERT_EQUAL("HH@>?<8ADDAD", read.GetPhredQualityString());
  ASSERT(reader.eof());
}

cute::suite ConvertingReaderWrapperSuite() {
  cute::suite s;
  s.push_back(CUTE(TestConvertingReaderWrapperNoFile));
  s.push_back(CUTE(TestConvertingReaderWrapperReading));
  return s;
}

#endif /* TEST_CONVERTINGREADERWRAPPERTEST_HPP_ */
