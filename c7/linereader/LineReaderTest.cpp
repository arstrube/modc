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

#include "CppUTest/TestHarness.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "sys_stubs.h"
#include "LineReader.h"

static const char templ[] = "/tmp/line-reader-unittest-XXXXXX";

static int TemporaryFile() {
  const int fd = mkstemp(templ);

  return fd;
}

TEST_GROUP(LineReaderTest) {
    void teardown() override {
        unlink(templ);
    }
};

TEST(LineReaderTest, EmptyFile) {
  const int fd = TemporaryFile();
  LineReader reader(fd);

  const char *line;
  unsigned len;
  CHECK_FALSE(reader.GetNextLine(&line, &len));

  close(fd);
}

TEST(LineReaderTest, OneLineTerminated) {
  const int fd = TemporaryFile();
  write(fd, "a\n", 2);
  lseek(fd, 0, SEEK_SET);
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
  const int fd = TemporaryFile();
  write(fd, "a", 1);
  lseek(fd, 0, SEEK_SET);
  LineReader reader(fd);

  const char *line;
  unsigned len;
  CHECK_TRUE(reader.GetNextLine(&line, &len));
  LONGS_EQUAL(1U, len);
  STRCMP_EQUAL("a", line);
  reader.PopLine(len);

  CHECK_FALSE(reader.GetNextLine(&line, &len));

  close(fd);
}

TEST(LineReaderTest, TwoLinesTerminated) {
  const int fd = TemporaryFile();
  write(fd, "a\nb\n", 4);
  lseek(fd, 0, SEEK_SET);
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

  close(fd);
}

TEST(LineReaderTest, TwoLines) {
  const int fd = TemporaryFile();
  write(fd, "a\nb", 3);
  lseek(fd, 0, SEEK_SET);
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

  close(fd);
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

  close(fd);
}

TEST(LineReaderTest, TooLong) {
  const int fd = TemporaryFile();
  char l[LineReader::kMaxLineLen];
  memset(l, 'a', sizeof(l));
  write(fd, l, sizeof(l));
  lseek(fd, 0, SEEK_SET);
  LineReader reader(fd);

  const char *line;
  unsigned len;
  CHECK_FALSE(reader.GetNextLine(&line, &len));

  close(fd);
}
