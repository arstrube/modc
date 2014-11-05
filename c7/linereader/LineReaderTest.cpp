// Copyright (c) 2009, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// strict ansi shouldn't be defined, but under Cygwin G++, it is defined
// even when not commanded by -ansi

#undef __STRICT_ANSI__

#include "CppUTest/TestHarness.h"
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>

#include "sys_stubs.h"
#include "LineReader.h"

#define ASSERT_EQ_WITH_LENGTH(expected,actual,length,last) {\
  LONGS_EQUAL(length, strlen(actual));\
  STRCMP_EQUAL(expected, actual);\
  reader.PopLine(length);\
  if(!last) {\
    CHECK_TRUE(reader.GetNextLine(&line, &length));\
  } else {\
    CHECK_FALSE(reader.GetNextLine(&line, &length));\
  }\
}
#define last_line true
#define not_last_line false

static const char templ[] = "/tmp/line-reader-unittest-XXXXXX";

static int TemporaryFile() {
   char templ_copy[sizeof(templ)];
   memcpy(templ_copy, templ, sizeof(templ));
   const int fd = mkstemp(templ_copy);
   if (fd >= 0)
      unlink(templ_copy);

   return fd;
}

TEST_GROUP(LineReaderTest) {
   int fd;
   const char *line;
   unsigned len;

   int WriteTemporaryFile(const char* records) {
      fd = TemporaryFile();
      write(fd, records, strlen(records));
      lseek(fd, 0, SEEK_SET);
      return fd;
   }

   void teardown() override {
   }
};

TEST(LineReaderTest, EmptyFile) {
  LineReader reader(WriteTemporaryFile(""));

  CHECK_FALSE(reader.GetNextLine(&line, &len));
}

TEST(LineReaderTest, OneLineTerminated) {
  LineReader reader(WriteTemporaryFile("a\n"));

  CHECK_TRUE(reader.GetNextLine(&line, &len));
  ASSERT_EQ_WITH_LENGTH("a", line, len, last_line);
}

TEST(LineReaderTest, OneLine) {
  LineReader reader(WriteTemporaryFile("a"));

  CHECK_TRUE(reader.GetNextLine(&line, &len));
  ASSERT_EQ_WITH_LENGTH("a", line, len, last_line);
}

TEST(LineReaderTest, TwoLinesTerminated) {
  LineReader reader(WriteTemporaryFile("a\nb\n"));

  CHECK_TRUE(reader.GetNextLine(&line, &len));
  ASSERT_EQ_WITH_LENGTH("a", line, len, not_last_line);

  CHECK_TRUE(reader.GetNextLine(&line, &len));
  ASSERT_EQ_WITH_LENGTH("b", line, len, last_line);
}

TEST(LineReaderTest, TwoLines) {
  LineReader reader(WriteTemporaryFile("a\nb"));

  CHECK_TRUE(reader.GetNextLine(&line, &len));
  ASSERT_EQ_WITH_LENGTH("a", line, len, not_last_line);

  CHECK_TRUE(reader.GetNextLine(&line, &len));
  ASSERT_EQ_WITH_LENGTH("b", line, len, last_line);
}

TEST(LineReaderTest, MaxLength) {
  char l[LineReader::kMaxLineLen];
  memset(l, 'a', sizeof(l)-1);
  l[sizeof(l)] = '\0';
  LineReader reader(WriteTemporaryFile(l));

  const char *line; // TODO: If I refactor these by removing them and using
  unsigned len;     // instance variables instead, the test will fail.
  CHECK_TRUE(reader.GetNextLine(&line, &len));
  LONGS_EQUAL(strlen(l), len);
  CHECK_TRUE(memcmp(l, line, sizeof(l)) == 0);
  CHECK_TRUE(0 == line[len]);
}

TEST(LineReaderTest, TooLong) {
  char l[LineReader::kMaxLineLen+1];
  memset(l, 'a', sizeof(l)-1);
  l[sizeof(l)] = '\0';
  LineReader reader(WriteTemporaryFile(l));

  CHECK_FALSE(reader.GetNextLine(&line, &len));
}
