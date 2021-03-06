// Filename: datagramInputFile.h
// Created by:  drose (30Oct00)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
////////////////////////////////////////////////////////////////////

#ifndef DATAGRAMINPUTFILE_H
#define DATAGRAMINPUTFILE_H

#include "pandabase.h"

#include "datagramGenerator.h"
#include "filename.h"
#include "fileReference.h"
#include "virtualFile.h"

////////////////////////////////////////////////////////////////////
//       Class : DatagramInputFile
// Description : This class can be used to read a binary file that
//               consists of an arbitrary header followed by a number
//               of datagrams.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDA_PUTIL DatagramInputFile : public DatagramGenerator {
PUBLISHED:
  INLINE DatagramInputFile();
  INLINE ~DatagramInputFile();

  bool open(const FileReference *file);
  INLINE bool open(const Filename &filename);
  bool open(istream &in, const Filename &filename = Filename());
  INLINE istream &get_stream();

  void close();

  bool read_header(string &header, size_t num_bytes);
  virtual bool get_datagram(Datagram &data);
  virtual bool save_datagram(SubfileInfo &info);
  virtual bool is_eof();
  virtual bool is_error();

  virtual const Filename &get_filename();
  virtual time_t get_timestamp() const;
  virtual const FileReference *get_file();
  virtual VirtualFile *get_vfile();
  virtual streampos get_file_pos();

private:
  bool _read_first_datagram;
  bool _error;
  CPT(FileReference) _file;
  PT(VirtualFile) _vfile;
  istream *_in;
  bool _owns_in;
  Filename _filename;
  time_t _timestamp;
};

#include "datagramInputFile.I"

#endif
