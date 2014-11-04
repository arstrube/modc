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
   
   int WriteTemporaryFile(const char* records) {
      fd = TemporaryFile();
      write(fd, records, strlen(records));
      lseek(fd, 0, SEEK_SET);
      return fd;
   }

   void teardown() override {
      close(fd);
   }
};

TEST(LineReaderTest, EmptyFile) {
  const int fd = WriteTemporaryFile("");
  LineReader reader(fd);

  const char *line;
  unsigned len;
  CHECK_FALSE(reader.GetNextLine(&line, &len));
}

TEST(LineReaderTest, OneLineTerminated) {
  const int fd = WriteTemporaryFile("a\n");
  LineReader reader(fd);

  const char *line;
  unsigned int len;
  CHECK_TRUE(reader.GetNextLine(&line, &len));
  LONGS_EQUAL(1, len);
  STRCMP_EQUAL("a", line);
  reader.PopLine(len);

  CHECK_FALSE(reader.GetNextLine(&line, &len));

  close(fd);
}

TEST(LineReaderTest, OneLine) {
  const int fd = WriteTemporaryFile("a");
  LineReader reader(fd);

  const char *line;
  unsigned len;
  CHECK_TRUE(reader.GetNextLine(&line, &len));
  LONGS_EQUAL(1U, len);
  STRCMP_EQUAL("a", line);
  reader.PopLine(len);

  CHECK_FALSE(reader.GetNextLine(&line, &len));
}

TEST(LineReaderTest, TwoLinesTerminated) {
  const int fd = WriteTemporaryFile("a\nb\n");
  LineReader reader(fd);

  const char *line;
  unsigned len;
  CHECK_TRUE(reader.GetNextLine(&line, &len));
  LONGS_EQUAL(1u, len);
  STRCMP_EQUAL("a", line);
  reader.PopLine(len);

  CHECK_TRUE(reader.GetNextLine(&line, &len));
  LONGS_EQUAL(1u, len);
  STRCMP_EQUAL("b", line);
  reader.PopLine(len);

  CHECK_FALSE(reader.GetNextLine(&line, &len));
}

TEST(LineReaderTest, TwoLines) {
  const int fd = WriteTemporaryFile("a\nb");
  LineReader reader(fd);

  const char *line;
  unsigned len;
  CHECK_TRUE(reader.GetNextLine(&line, &len));
  LONGS_EQUAL(1u, len);
  STRCMP_EQUAL("a", line);
  reader.PopLine(len);

  CHECK_TRUE(reader.GetNextLine(&line, &len));
  LONGS_EQUAL(1u, len);
  STRCMP_EQUAL("b", line);
  reader.PopLine(len);

  CHECK_FALSE(reader.GetNextLine(&line, &len));
}

TEST(LineReaderTest, MaxLength) {
  const int fd = TemporaryFile();
  char l[LineReader::kMaxLineLen - 1];
  memset(l, 'a', sizeof(l));
  write(fd, l, sizeof(l));
  lseek(fd, 0, SEEK_SET);
  LineReader reader(fd);

  const char *line;
  unsigned len;
  CHECK_TRUE(reader.GetNextLine(&line, &len));
  LONGS_EQUAL( sizeof(l), len);
  CHECK_TRUE(memcmp(l, line, sizeof(l)) == 0);
  CHECK_TRUE(0 == line[len]);
}

TEST(LineReaderTest, TooLong) {
  char l[LineReader::kMaxLineLen];
  memset(l, 'a', sizeof(l));
  const int fd = WriteTemporaryFile(l);
  LineReader reader(fd);

  const char *line;
  unsigned len;
  CHECK_FALSE(reader.GetNextLine(&line, &len));
}
